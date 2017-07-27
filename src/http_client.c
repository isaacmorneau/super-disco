#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>


int read_all(int fd, void * buffer, int size) {
    int bytes_read = 0, total_read = 0;
    while ((bytes_read = read(fd, buffer+total_read, size-total_read)) > 0) {
        total_read += bytes_read;
    }
    return total_read ? total_read : -1;
}

int write_all(int fd, void * buffer, int size) {
    int bytes_wrote = 0, total_wrote = 0;
    while ((bytes_wrote = write(fd, buffer+total_wrote, size-total_wrote)) > 0) {
        total_wrote += bytes_wrote;
    }
    return total_wrote ? total_wrote : -1;
}

int send_get(int fd, char * location, char * server, int port) {
    char buffer[4096];
    sprintf(buffer, 
            "GET %s HTTP/1.1\r\n"
            "Host: %s:%d\r\n"
            "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.78 Safari/537.36\r\n"
            "Connection: Close\r\n"
            "\r\n",
            location, server, port);
    return write_all(fd, buffer, strlen(buffer));
}

int read_response(int fd) {
    int allocated = 2046;
    int current_pos = 0;
    void * buffer = malloc(allocated);
    int resp_read = 0;
    while ((resp_read = read_all(fd, buffer+current_pos, allocated-current_pos))) {
        if (resp_read == -1) {
            //something broke /shrug
            free(buffer);
            return -1;
        } else if (resp_read < allocated) {
            //we hit the end
            break;
        } else if (resp_read == allocated) {
            //we need more room
            allocated *= 2;
            void * temp;
            if (!(temp = realloc(buffer, allocated))) {
                //probably ran out of memory
                free(buffer);
                return -1;
            }
            buffer = temp;
            current_pos += resp_read;
        }
    }
    printf("%s", buffer);
    free(buffer);
    return 0;
}

int main(int argc, char ** argv) {
    int sock;
    int port = 80;
    struct sockaddr_in addr;
    
    if (argc < 2) {
        printf("Please Specify Url\n format: www.example.com or www.example.com /location/requested");
        return 1;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return 2;
    }

    //TODO this has been depricated a while but i dont feel like implementing getaddrinfo yet
    struct hostent *host_entity = gethostbyname(argv[1]);
    addr.sin_family = AF_INET;
    memcpy(&addr.sin_addr, host_entity->h_addr_list[0], host_entity->h_length);

    addr.sin_port = htons(port);

    if (connect(sock, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
        close(sock);
        return 3;
    }

    if (argc == 2 ) {
        send_get(sock, "/", argv[1], port);
    } else {
        send_get(sock, argv[2], argv[1], port);
    }

    read_response(sock);

    //shutdown happens within the close so dont bother with shutdown
    close(sock);
    return 0;
}

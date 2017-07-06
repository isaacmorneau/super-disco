SHELL := /bin/bash
#this make was for cpp but this project is c
CXX := gcc
CXXFLAGS := -Wall -pedantic -std=c11 -lrt -D_POSIX_C_SOURCE=200809L

CLIBS := 
APPNAME := super-disco
ODIR := bin
SRC := super-disco
HEAD := $(SRC)
OBJS := $(ODIR)/%.o
SRCOBJS := $(SRC)/%.c
SRCWILD := $(wildcard $(SRC)/*.c)
HEADWILD := $(wildcard $(HEAD)/*.h)
EXEC := $(ODIR)/$(APPNAME)
DEPS := $(EXEC).d


# Add flags for release mode binary
release: CXXFLAGS += -O3 -march=native -flto -DNDEBUG
release: alldem

# Do note that if optimizate causes the error it wont happen in debug
debug: CXXFLAGS += -g -O0
debug: alldem

alldem: $(patsubst $(SRCOBJS), $(OBJS), $(SRCWILD))
# create exe out of .o files
	$(CXX) $^ $(CFLAGS) $(CXXFLAGS) $(CLIBS) -o $(EXEC)

$(ODIR):
	@mkdir -p $(ODIR)

# Create dependency file for make and manually adjust it silently to work with other directories
$(DEPS): $(SRCWILD) $(HEADWILD) | $(ODIR) 
# Compile the non-system dependencies and store it in outputdir/execname.d
	@$(CXX) -MM $(CFLAGS) $(CXXFLAGS) $(SRCWILD) > $(DEPS)
# Copy the contents to a temp file
	@cp $(DEPS) $(DEPS).tmp
# Take the temp file contents, do a regex text replace to change all .o strings into
# outputdir/.o strings, and store the result in outputdir/execname.d
	@sed -e 's/\w\+\.o/$(ODIR)\/&/g' < $(DEPS).tmp > $(DEPS)
# Delete the temp file
	@$(RM) $(DEPS).tmp

# Add the dependencies into make and don't throw an error if it doesn't exist
# Also don't generate dependency file during a clean
ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif

# Target is any bin .o file, prereq is the equivalent src .cpp file
$(OBJS): $(SRCOBJS)
# Command compiles the src .cpp file with the listed flags and turns it into a bin .o file
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $< -o $(patsubst $(SRCOBJS), $(OBJS), $<) $(CLIBS)

# Prevent clean from trying to do anything with a file called clean
.PHONY: clean

# Deletes the executable and all .o and .d files in the bin folder
clean: |$(ODIR)
	$(RM) $(EXEC) $(wildcard $(EXEC).*) $(wildcard $(ODIR)/*.d*) $(wildcard $(ODIR)/*.o)


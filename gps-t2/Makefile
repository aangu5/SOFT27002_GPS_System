PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

#ifeq ($(BUILD_MODE),debug)
#	CFLAGS += -g
#else ifeq ($(BUILD_MODE),run)
#	CFLAGS += -O2 -std=c++11 -Wall -Wfatal-errors 
#else ifeq ($(BUILD_MODE),linuxtools)
#	CFLAGS += -g -pg -fprofile-arcs -ftest-coverage
#	LDFLAGS += -pg -fprofile-arcs -ftest-coverage 
#else
#    $(error Build mode $(BUILD_MODE) not supported by this Makefile)
#endif

CFLAGS += -O2 -std=c++11 -Wall -Wfatal-errors 

EXEC = nmea-tests

LDFLAGS += -lboost_unit_test_framework

INCDIR = $(mkfile_dir)headers
OBJDIR = $(mkfile_dir)obj
SRCDIR = $(mkfile_dir)src
BINDIR = $(mkfile_dir)bin

$(shell mkdir -p $(OBJDIR))
$(shell mkdir -p $(BINDIR))

SOURCES += \
    earth.cpp \
    geometry.cpp \
    logs.cpp \
    parseNMEA.cpp \
    position.cpp \
    nmea-tests.cpp

_OBJ = $(SOURCES:.cpp=.o)
OBJ = $(patsubst %,$(OBJDIR)/%,$(_OBJ))

all: $(BINDIR)/$(EXEC)

CFLAGS += -I$(INCDIR) -I$(SRCDIR) 


all: $(BINDIR)/$(EXEC)
	

$(BINDIR)/$(EXEC): $(OBJ)
	$(CXX) -o $(BINDIR)/$(EXEC) $(OBJ) $(LDFLAGS)  
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(CXXFLAGS) $(EXTRA_FLAGS) $(LDFLAGS)

	
clean:
	rm -rf $(OBJDIR) $(BINDIR)

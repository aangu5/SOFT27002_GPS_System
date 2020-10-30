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

EXEC = generate-gpx

LDFLAGS += -lboost_unit_test_framework

INCDIR = $(mkfile_dir)headers 
INCDIR+=-I$(mkfile_dir)headers/gridworld
INCDIR+=-I$(mkfile_dir)headers/xml
OBJDIR = $(mkfile_dir)obj
OBJDIR1 = $(mkfile_dir)obj/gridworld
OBJDIR2 = $(mkfile_dir)obj/xml
SRCDIR = $(mkfile_dir)src
BINDIR = $(mkfile_dir)bin

$(shell mkdir -p $(OBJDIR))
$(shell mkdir -p $(OBJDIR1))
$(shell mkdir -p $(OBJDIR2))
$(shell mkdir -p $(BINDIR))

SOURCES += \
    earth.cpp \
    geometry.cpp \
    logs.cpp \
    position.cpp \
    gridworld/gridworld_model.cpp \
    gridworld/gridworld_route.cpp \
    gridworld/gridworld_track.cpp \
    gridworld/generate-gpx.cpp \
    xml/generator.cpp

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

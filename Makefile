######################################################
## Makefile for the DT (Duke Single Top) library   ## 
## Author: Douglas Davis < ddavis@phy.duke.edu >    ##
##                       < douglas.davis@cern.ch >  ##
######################################################

ifndef DT_BASE
$(error DT_BASE not set, source config/setup)
endif

CXX      := clang++
CXXFLAGS := -fPIC -Wall -O2 $(shell root-config --cflags) -I$(DT_BASE)/include
LDFLAGS  := -L$(DT_BASE)/lib $(shell root-config --glibs) -lTreePlayer -lboost_filesystem

ARCH      = $(shell uname)
ifeq ($(ARCH),Darwin)
	LIBSUFFIX = dylib
else
	LIBSUFFIX = so
endif

TARGET  := lib/libDT.$(LIBSUFFIX)

HEADERS = include/DukeTop/AnaBase.h include/DukeTop/Job.h \
		include/DukeTop/FileManager.h

all: $(TARGET)

$(TARGET): src/AnaBase.o src/Job.o src/FileManager.o
	-@echo "[DT] Building $@";$(CXX) -shared -o $@ $^ $(LDFLAGS)
	-@echo "[DT] Done"

src/AnaBase.o: src/AnaBase.cxx $(HEADERS)
	-@echo "[DT] Building $@";$(CXX) $(CXXFLAGS) -o $@ -c $<

src/Job.o: src/Job.cxx $(HEADERS)
	-@echo "[DT] Building $@";$(CXX) $(CXXFLAGS) -o $@ -c $<

src/FileManager.o: src/FileManager.cxx $(HEADERS)
	-@echo "[DT] Building $@";$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	-@$(RM) src/*.o lib/*.$(LIBSUFFIX)

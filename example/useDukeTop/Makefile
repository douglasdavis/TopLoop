ifndef DT_BASE
$(error DT_BASE not set, run source config/setup from the DukeTop directory)
endif

CXX      := g++
CXXFLAGS := -fPIC -Wall -I$(DT_BASE)/include -O2 $(shell root-config --cflags) -Iinclude
LDFLAGS  := -L$(DT_BASE)/lib $(shell root-config --glibs) -lTreePlayer -lDT -lboost_filesystem
MAKEDIR  := $(shell mkdir -p lib)

ARCH      = $(shell uname)
ifeq ($(ARCH),Darwin)
	LIBSUFFIX = dylib
else
	LIBSUFFIX = so
endif

LIBTARGET  := lib/libmyDT.$(LIBSUFFIX)
EXETARGET  := use

all: $(LIBTARGET) $(EXETARGET)

$(LIBTARGET): src/MyDukeTopAna.o
	$(CXX) -shared -o $@ $^ $(LDFLAGS)

src/MyDukeTopAna.o: src/MyDukeTopAna.cxx include/MyDukeTopAna/MyDukeTopAna.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(EXETARGET): util/use.o
	$(CXX) -o $@ $< $(LDFLAGS) -Llib -lmyDT

util/use.o: util/use.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	-@$(RM) src/*.o lib/*.$(LIBSUFFIX) $(EXETARGET) util/*.o

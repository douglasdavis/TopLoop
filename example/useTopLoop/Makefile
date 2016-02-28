include ../../Makefile.common

CXXFLAGS += -Iinclude
LDFLAGS  += -lTopLoop

MAKELIBDIR := $(shell mkdir -p lib)

LIBTARGET := lib/libmyTopLoop.$(LIBSUFFIX)
EXETARGET := use

all: $(LIBTARGET) $(EXETARGET)

$(LIBTARGET): src/MyTopLoopAna.o
	-@echo "[Example] Building $@";$(CXX) -shared -o $@ $^ $(LDFLAGS)

src/MyTopLoopAna.o: src/MyTopLoopAna.cxx include/MyTopLoopAna/MyTopLoopAna.h
	-@echo "[Example] Building $@";$(CXX) $(CXXFLAGS) -o $@ -c $<

$(EXETARGET): util/use.o
	-@echo "[Example] Building $@";$(CXX) -o $@ $< $(LDFLAGS) -Llib -lmyTopLoop

util/use.o: util/use.cpp
	-@echo "[Example] Building $@";$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	-@$(RM) src/*.o lib/*.$(LIBSUFFIX) $(EXETARGET) util/*.o

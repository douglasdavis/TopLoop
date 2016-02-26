include $(DT_BASE)/config/Common.mk

CXXFLAGS += -Iinclude
LDFLAGS  += -lDT

MAKELIBDIR := $(shell mkdir -p lib)

LIBTARGET := lib/libmyDT.$(LIBSUFFIX)
EXETARGET := use

all: $(LIBTARGET) $(EXETARGET)

$(LIBTARGET): src/MyDukeTopAna.o
	-@echo "[Example] Building $@";$(CXX) -shared -o $@ $^ $(LDFLAGS)

src/MyDukeTopAna.o: src/MyDukeTopAna.cxx include/MyDukeTopAna/MyDukeTopAna.h
	-@echo "[Example] Building $@";$(CXX) $(CXXFLAGS) -o $@ -c $<

$(EXETARGET): util/use.o
	-@echo "[Example] Building $@";$(CXX) -o $@ $< $(LDFLAGS) -Llib -lmyDT

util/use.o: util/use.cpp
	-@echo "[Example] Building $@";$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	-@$(RM) src/*.o lib/*.$(LIBSUFFIX) $(EXETARGET) util/*.o

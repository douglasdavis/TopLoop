###############################################################
## Common make values for the DT (Duke Single Top) library   ## 
## Author: Douglas Davis < ddavis@phy.duke.edu >             ##
##                       < douglas.davis@cern.ch >           ##
###############################################################

CXX      := c++
CXXFLAGS  = -fPIC -Wall -O2 $(shell root-config --cflags) -I$(DT_BASE)/include
LDFLAGS   = -L$(DT_BASE)/lib $(shell root-config --glibs) -lTreePlayer -lboost_filesystem

ARCH      = $(shell uname)
ifeq ($(ARCH),Darwin)
	LIBSUFFIX = dylib
else
	LIBSUFFIX = so
endif

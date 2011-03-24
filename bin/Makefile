################################################################################
# Noxim 3d honeycomb - Extended Noc simualtor supporting 3d honeycomb topology #
################################################################################

# NOTE FOR NOXIM USERS: before running "make" you just have to modify
# the following variable to point to your SystemC installation path
TARGET_ARCH = linux
SYSTEMC = /home/nan/dev/systemc-2.2.0
DS = /home/nan/chen-work/c-data-structures-analysis
QT = /home/nan/qtsdk-2010.05/qt

CC     = g++
OPT    = -O0
DEBUG  = -g
OTHER  = # -Wall -Wno-deprecated
CFLAGS = $(OPT) $(OTHER) # $(DEBUG)

VPATH = ../src:.

SRCS := $(wildcard ../src/*.cpp)
OBJS = $(subst ../src/,,$(SRCS:.cpp=.o))

INCDIR = -I. -I.. -I../src -I$(SYSTEMC)/include -I$(QT)/include -I$(DS)/ds-lib 
LIBDIR = -L. -L.. -L../src -L$(SYSTEMC)/lib-$(TARGET_ARCH) -L$(QT)/lib -L$(DS)/bin/Debug

#EXTRA_LIBS = -lpthread
LIBS = -lsystemc -lm $(EXTRA_LIBS) -lc-data-structures-analysis

.SUFFIXES: .cpp .o 

noxim: $(OBJS) $(SYSTEMC)/lib-$(TARGET_ARCH)/libsystemc.a /home/nan/chen-work/c-data-structures-analysis/bin/Debug/libc-data-structures-analysis.a
	$(CC) $(CFLAGS) $(INCDIR) $(LIBDIR) -fexceptions -o $@ $(OBJS) $(LIBS) 2>&1 | c++filt

.cpp.o:
	$(CC) $(CFLAGS) $(INCDIR) -c -g $<

%.d: %.cpp
	@set -e; rm -f $@; \
         $(CC) -M $(CFLAGS) $< >; $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ >; $@; \
         rm -f $@.$$$$

clean:
	rm -f $(OBJS) *~ noxim core

#include ./Makefile.deps


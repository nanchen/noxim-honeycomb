#-------------------------------------------------------
#              Unit Testing
#-------------------------------------------------------

TARGET_ARCH = linux
SYSTEMC = /home/nan/dev/systemc-2.2.0

DS = /home/nan/chen-work/c-data-structures-analysis
CC     = g++
OPT    = -O0
DEBUG  = -g
OTHER  = # -Wall -Wno-deprecated
CFLAGS = $(OPT) $(OTHER) # $(DEBUG)

VPATH = ../tests:../src:.

SRCS := $(wildcard ../src/*.cpp)
OBJS = $(subst ../src/,,$(SRCS:.cpp=.o))

TEST_SRCS := $(wildcard ../tests/*.cpp)
TEST_OBJS = $(subst ../tests/,,$(SRCS:.cpp=.o))


INCDIR = -I. -I.. -I../src -I../tests -I$(SYSTEMC)/include -I$(DS)/ds-lib 
LIBDIR = -L. -L.. -L../src -L$(SYSTEMC)/lib-$(TARGET_ARCH) -L$(DS)/bin/Debug

LIBS = -lsystemc -lm $(EXTRA_LIBS) -lc-data-structures-analysis

.SUFFIXES: .cpp .o 

runAll: runRouterTB 


all: RouterTB

runRouterTB: RouterTB
	./RouterTB
	
RouterTB: $(OBJS) NoximHMRouterTB.o
	$(CC) $(CFLAGS) $(INCDIR) $(LIBDIR) -o $@ $(OBJS) NoximHMRouterTB.o $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCDIR) -c -g $<

%.d: %.cpp
		@set -e; rm -f $@; \
         $(CC) -M $(CFLAGS) $< >; $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ >; $@; \
         rm -f $@.$$$$

cleanRouterTB:
	rm RouterTB NoximHMRouterTB.o

clean:
	rm -f $(OBJS) *~ core *.o

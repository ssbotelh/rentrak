TARGET = exe

OBJS = \
main.o \
CmdLineArgs.o \
Field.o \
Record.o \
TaskExecutor.o \
Operation.o \
SelectOperation.o \
FileManager.o \
DataStore.o \

#########################################################
#XMLRPCDIR=$(HOME)/XmlRpc/xmlrpc++0.7_p27/

INCLUDE_DIRS=-I. # -I$(XMLRPCDIR)/include/
#LIB_DIRS=-L$(BASE)/lib -L$(XMLRPCDIR)/gcc-64-release/

#GCC = g++
GCC = /usr/gcc-4.8.4/bin/g++

#CXXFLAGS = -I$(INCLUDE_DIRS) -Wall -g -gdwarf-2 -gstrict-dwarf 
#CXXFLAGS = -I$(INCLUDE_DIRS) -Wall -g  -gdwarf-2 -O2 -std=c++0x
CXXFLAGS = $(INCLUDE_DIRS) -Wall -g -gdwarf-2 -std=c++11 -O0
libdir =  $(LIB_DIRS)
libs = -lm -pthread
#libs = -lcubpack -lm -lgsl -lgslcblas

# Default build rule (can be omitted)
.c.o:  $(GCC) $(CXXFLAGS) -c $*.c

%.o: %.cpp
	$(GCC) $(CXXFLAGS) -c -o $*.o $<

$(TARGET): $(OBJS)
	$(GCC) $(OBJS) $(libdir) $(libs) -o $(TARGET) 

clean:
	/bin/rm -f *.o $(TARGET)* core*

all: 
	make clean
	make


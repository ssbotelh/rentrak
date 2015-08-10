TARGET = exe

GCC = /usr/gcc-4.8.4/bin/g++

#########################################################

OBJS = \
main.o \
CmdLineArgs.o \
Field.o \
Record.o \
TaskExecutor.o \
SelectOperation.o \
SortOperation.o \
FilterOperation.o \
DataStore.o \
Utility.o \

#########################################################

INCLUDE_DIRS=-I.

CXXFLAGS = $(INCLUDE_DIRS) -Wall -g -gdwarf-2 -std=c++11 -O0
libdir =  $(LIB_DIRS)
#libs = -lm -pthread

# Default build rule
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


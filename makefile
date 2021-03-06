TARGET = query

GCC = g++

#########################################################

OBJS = \
main.o \
Utility.o \
CmdLineArgs.o \
DataStore.o \
Field.o \
Record.o \
TaskExecutor.o \
SelectOperation.o \
SortOperation.o \
FilterOperation.o \
GroupOperation.o \

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


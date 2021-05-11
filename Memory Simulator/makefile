#
# AUTHOR:  Cody McAntire
#


# Compiler variables
CCFLAGS = -g -std=c++11

# Rule to link object code files to create executable file
memory: main.o memory.o hex.o
	g++ $(CCFLAGS) -o memory main.o memory.o hex.o

# Rules to compile source code files to object code
main.o: main.cpp hex.h memory.h
	g++ $(CCFLAGS) -c main.cpp

hex.o: hex.cpp hex.h
	g++ $(CCFLAGS) -c hex.cpp

memory.o: memory.cpp memory.h
	g++ $(CCFLAGS) -c memory.cpp

# Pseudo-target to remove object code and executable files
clean:
	-rm -f *.o main

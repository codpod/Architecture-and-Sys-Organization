#
# AUTHOR:  Cody McAntire
#

# Compiler variables
CCFLAGS = -g -std=c++11

# Rule to link object code files to create executable file
rv32i: main.o memory.o hex.o rv32i.o
	g++ $(CCFLAGS) -o rv32i main.o memory.o hex.o rv32i.o

# Rules to compile source code files to object code
main.o: main.cpp hex.h memory.h rv32i.h
	g++ $(CCFLAGS) -c main.cpp

hex.o: hex.cpp hex.h
	g++ $(CCFLAGS) -c hex.cpp

memory.o: memory.cpp memory.h
	g++ $(CCFLAGS) -c memory.cpp

rv32i.o: rv32i.cpp rv32i.h
	g++ $(CCFLAGS) -c rv32i.cpp

# Pseudo-target to remove object code and executable files
clean:
	-rm *.o rv32i
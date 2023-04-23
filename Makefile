## Makefile for gerp project
## Author: Cheng Li [cli31]
MAKEFLAGS += -L

CXX = clang++
CXXFLAGS = -g2 -Wall -Wextra -Wpedantic -Wshadow
LDFLAGS = -g2


# rule for making treeTraversal (for testing)
FSTreeTraversal.o: FSTreeTraversal.cpp
	${CXX} ${CXXFLAGS} -c $^
treeTraversal: FSTreeTraversal.o FSTree.o DirNode.o
	${CXX} ${CXXFLAGS} -o $@ $^

# rule for compiling the hashExample
show_hashExample: hashExample.cpp
	${CXX} ${CXXFLAGS} -o $@ $^

# rules for compiling classes
Word.o: Word.cpp Word.h File.h
	${CXX} ${CXXFLAGS} -c Word.cpp

File.o: File.cpp File.h
	${CXX} ${CXXFLAGS} -c File.cpp

hashTable.o: hashTable.cpp hashTable.h Word.h
	${CXX} ${CXXFLAGS} -c hashTable.cpp

gerp.o: gerp.cpp gerp.h Word.h File.h hashTable.h DirNode.h FSTree.h
	${CXX} ${CXXFLAGS} -c gerp.cpp

main.o: main.cpp gerp.h
	${CXX} ${CXXFLAGS} -c main.cpp

# rule for linking
gerp: main.o gerp.o hashTable.o File.o Word.o DirNode.o FSTree.o
	${CXX} ${CXXFLAGS} -o $@ $^


# submit for phase one
provide_one:
	provide comp15 proj4_gerp_phase1 FSTreeTraversal.cpp \
	stringProcessing.h stringProcessing.cpp README
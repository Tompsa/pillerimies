# http://mrbook.org/blog/tutorials/make/

# Compiler to be used
CC=g++

# Flags passed to compiler
CFLAGS=-I./include -c -g -Wall -Wextra -std=c++0x -pedantic

# Libraries
LIBS=-lsfml-graphics -lsfml-window -lsfml-system

#Source files
SOURCES=$(wildcard source/*.cpp)

#Object files
OBJECTS=$(SOURCES:.cpp=.o)

EXE=pillerimies

all: $(SOURCES) $(EXE)

$(EXE): $(OBJECTS) ; $(CC) $(OBJECTS) -o $@ $(LIBS)
    
.cpp.o: ; $(CC) $(CFLAGS) $< -o $@   
    
clean: ; rm -f $(OBJECTS) $(EXE)

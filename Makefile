CC=g++
CFLAGS=-c -O3
LDFLAGS=-O3
FLAGS=-std=c++14
INC=$(CURDIR)

SOURCES=SimpleHashFunctor.cpp dic.cpp

OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=dic

.PHONY: all clean

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(FLAGS) $^ -o $@ -I $(LDFLAGS) 

.cpp.o:
	$(CC) $(CFLAGS) $(FLAGS) $< -o $@ -I $(INC)

clean:
	rm -f *.o *~ $(EXECUTABLE)


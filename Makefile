CC=mpic++
CCFLAGS=-Wall -O3 -g -std=c++11
LDFLAGS=

all: nqueen

nqueen: main.o solver.o utils.o
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.cpp %.h
	$(CC) $(CCFLAGS) -c $<

%.o: %.cpp
	$(CC) $(CCFLAGS) -c $<

clean:
	rm -f *.o nqueen

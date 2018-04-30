TARGET=example

CSVLIB=../lib/
CXX=g++
CXXFLAGS=-std=c++11 -I$(CSVLIB)

all: main.o
	$(CXX) $(CXXFLAGS) *.o -o $(TARGET).x

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm *.o *.x


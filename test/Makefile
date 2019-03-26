CXXFLAGS=-std=c++11 -O3
LIBFLAGS=$(shell pkg-config --cflags --libs multilingual)

all: test.out

%.out: %.cpp
	$(CXX) $(LIBFLAGS) $< -o $@ $(CXXFLAGS)

clean:
	rm -f *.out

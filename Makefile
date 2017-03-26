CXXFLAGS = -Wall -Wextra -Werror -std=c++14 -g

sources := $(wildcard *.cpp)
objects := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all: fssp

run: fssp
	./fssp

fssp: $(objects)
	$(CXX) $(objects) -o fssp

clean:
	rm -rf $(objects) fssp

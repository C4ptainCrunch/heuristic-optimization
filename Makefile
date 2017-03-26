CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -floop-interchange -floop-strip-mine -floop-block -floop-parallelize-all -ftree-parallelize-loops=4 -Ofast
# CXXFLAGS :=  -Wall -Wextra -Werror -std=c++17 -g

sources := $(wildcard *.cpp)
headers := $(wildcard *.hpp)
objects := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all: fssp

run: fssp
	./fssp

fssp: $(objects) $(headers)
	$(CXX) $(CXXFLAGS) $(objects) -o fssp

clean:
	rm -rf $(objects) fssp

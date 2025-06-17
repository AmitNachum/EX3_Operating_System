CXX = g++
CXXFLAGS = -shared -g -std=c++17 -Wall -pthread -fPIC
LIB_REACTOR = reactor.so
SRC = reactor_pattern.cpp
OBJ = reactor_pattern.o

$(LIB_REACTOR): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(LIB_REACTOR) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f *.o $(LIB_REACTOR)

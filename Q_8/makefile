CXX = g++
CXXFLAGS = -std=c++17 -Wall -pthread -fPIC -g
LDFLAGS = -shared

LIB_PROACTOR = libproactor.so
SRC = proactor.cpp
OBJ = proactor.o

all: $(LIB_PROACTOR)

$(LIB_PROACTOR): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f *.o $(LIB_PROACTOR)

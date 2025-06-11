CXX = g++
CXXFLAGS = -Wall -std=c++17
TARGET = client_CH
SRC = client_CH.cpp
HEADERS = point.hpp

all: $(TARGET)

$(TARGET): $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

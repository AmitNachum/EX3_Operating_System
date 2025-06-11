CXX = g++
CXXFLAGS = -Wall -std=c++17 -pg
TARGET = profile_client
SRC = profile_client.cpp
HEADERS = ../Q_1/point.hpp

all: $(TARGET)

$(TARGET): $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

runprofile:
	./$(TARGET)
	gprof $(TARGET) gmon.out > report.txt
	less report.txt


clean:
	rm -f $(TARGET) gmon.out report.txt

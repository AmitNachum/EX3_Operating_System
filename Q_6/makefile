CXX = g++
CXXFLAGS = -std=c++17 -Wall -pthread -g -I../Q_5

all: server client

server: server.cpp ../Q_5/reactor_pattern.cpp
	$(CXX) $(CXXFLAGS) server.cpp ../Q_5/reactor_pattern.cpp -o server

client: client.cpp
	$(CXX) $(CXXFLAGS) client.cpp -o client

clean:
	rm -f server client

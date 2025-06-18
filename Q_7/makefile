CXX = g++
CXXFLAGS = -std=c++17 -Wall -pthread -g

# Targets
all: server client

server: server_CH.cpp
	$(CXX) $(CXXFLAGS) server_CH.cpp -o server

client: client.cpp
	$(CXX) $(CXXFLAGS) client.cpp -o client

# Clean build artifacts
clean:
	rm -f server client

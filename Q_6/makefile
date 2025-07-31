CXX       = g++
CXXFLAGS  = -std=c++17 -Wall -pthread -g -I../Q_5

# change this to whatever port your server actually uses
PORT      ?= 9034

# Top-level: stop any old server, then build both targets
all: stop server client

stop:
	-@fuser -k $(PORT)/tcp 2>/dev/null || true

server: server.cpp ../Q_5/reactor_pattern.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

client: client.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

# Also free the port before cleaning
clean: stop
	rm -f server client

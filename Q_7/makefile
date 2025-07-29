CXX       = g++
CXXFLAGS  = -std=c++17 -Wall -pthread -g

# change this to whatever port your server actually uses
PORT      ?= 9034

# Targets
all: server client

server: server_CH.cpp
	$(CXX) $(CXXFLAGS) server_CH.cpp -o server

client: client.cpp
	$(CXX) $(CXXFLAGS) client.cpp -o client

# Clean build artifacts and kill any server still bound to $(PORT)
clean:
	-@echo "→ killing anything on port $(PORT)…"
	-@fuser -k -TERM $(PORT)/tcp 2>/dev/null || true
	@echo "→ removing binaries"
	rm -f server client

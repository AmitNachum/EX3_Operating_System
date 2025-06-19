CXX = g++
CXXFLAGS = -std=c++17 -Wall -pthread -g
LDFLAGS = -L../Q_8 -lproactor -Wl,-rpath=../Q_8
PORT = 9034

SERVER_BIN = server
CLIENT_BIN = client

all: lib clean_port $(SERVER_BIN) $(CLIENT_BIN)

# Build the proactor library in Q_8
lib:
	$(MAKE) -C ../Q_8

# Server build linking to libproactor
$(SERVER_BIN): server_CH.cpp
	$(CXX) $(CXXFLAGS) server_CH.cpp -o $(SERVER_BIN) $(LDFLAGS)

# Client build
$(CLIENT_BIN): client.cpp
	$(CXX) $(CXXFLAGS) client.cpp -o $(CLIENT_BIN)

# Free the port if it's stuck
clean_port:
	@echo "Freeing port $(PORT)..."
	@fuser -k $(PORT)/tcp 2>/dev/null || true

# Cleanup
clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN) && cd .. && cd Q_8 && make clean

CXX = g++
CXXFLAGS = -std=c++17 -Wall -pthread

SERVER = server
CLIENT = client

SERVER_SRC = server_CH.cpp
CLIENT_SRC = client.cpp

HEADERS = interface.hpp ../Q_1/point.hpp

.PHONY: all clean

all: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER_SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(SERVER) $(SERVER_SRC)

$(CLIENT): $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) -o $(CLIENT) $(CLIENT_SRC)

clean:
	rm -f $(SERVER) $(CLIENT)

CXX = g++
CXXFLAGS = -std=c++17 -Wall -pthread -g
LDFLAGS = -L../Q_8 -lproactor -Wl,-rpath=../Q_8
PORT = 9034

all: clean_port ../Q_8/libproactor.so server client

../Q_8/libproactor.so:
	@echo ">>> Building Proactor library from Q_8..."
	@$(MAKE) -C ../Q_8

server: server_CH.cpp
	$(CXX) $(CXXFLAGS) server_CH.cpp -o server $(LDFLAGS)

client: client.cpp
	$(CXX) $(CXXFLAGS) client.cpp -o client

clean_port:
	@echo "Freeing port $(PORT)..."
	@fuser -k $(PORT)/tcp 2>/dev/null || true

clean:
	rm -f server client && cd .. && cd Q_8 && make clean

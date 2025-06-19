
# Operating Systems Convex Hull Project

This project is a modular, multi-question implementation of a Convex Hull system in C++, progressively integrating advanced OS concepts across 10 parts. The system is built around socket communication between clients and a server that manages a shared convex hull structure.

## 📁 Project Structure

```
.
├── Q_1         # Basic Convex Hull computation using Point abstraction
├── Q_2         # Performance profiling using vector vs deque
├── Q_3 & Q_4   # Interactive Convex Hull with menu system
├── Q_5         # Server using epoll (Reactor pattern - synchronous)
├── Q_6         # Reactor design pattern using poll()
├── Q_7         # Multi-threaded server using std::thread
├── Q_8         # Proactor pattern (async-style threads per client)
├── Q_9         # Proactor + Proactor-per-client model
├── Q_10        # Proactor + Conditional Watcher thread (area threshold)
```

Each subdirectory contains:
- A `makefile` for compilation
- A `server_CH.cpp` and `client.cpp` (Q_5–Q_10)
- Shared `point.hpp` and interface headers (e.g. `interface_Q6.hpp`)

## 🧠 Concepts Demonstrated

| Question | Concept |
|----------|---------|
| Q_1      | Convex Hull area calculation, pointer management |
| Q_2      | Performance timing (`chrono`), STL containers |
| Q_3      | User interaction with convex hull (add/remove/area) |
| Q_4      | Interactive CH with safe input and formatting |
| Q_5      | Server with `epoll` I/O multiplexing |
| Q_6      | Custom `Reactor` pattern with `poll()` |
| Q_7      | Thread-per-client design using `std::thread` |
| Q_8      | Proactor pattern using worker thread encapsulation |
| Q_9      | Proactor-per-client architecture |
| Q_10     | Proactor + Watcher thread (area ≥ 100 notification) |

## ⚙️ Compilation & Execution

### To Build All:
From the project root:
```bash
make
```

### To Clean All:
```bash
make clean
```

> Each `makefile` in `Q_*/` subfolders supports both `make` and `make clean`.

## 🚀 Running a Server-Client Demo (e.g., Q_10)

```bash
# In one terminal
cd Q_10
./server_CH

# In another terminal
cd Q_10
./client
```

## ✅ Features

- Modular C++ code with strong header separation
- Socket-based TCP client/server communication
- Custom memory management for dynamic points
- Area watcher using `std::condition_variable`
- Pattern separation: Proactor, Reactor, Threaded
- Full error handling and connection lifecycle support

## 🧼 Notes

- All server ports are fixed at **9034**
- Points are stored as `Point*` in STL containers and cleaned on shutdown
- Synchronization is ensured via `std::mutex` and `std::condition_variable`

## 👨‍💻 Author

Amit Nachum  
Ariel University  
Operating Systems Course – Convex Hull Project
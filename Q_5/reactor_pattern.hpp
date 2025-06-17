#pragma once

#include <functional>     // for std::function
#include <unordered_map>  // for tracking fd-to-callbacks
#include <vector>         // for managing active file descriptors
#include <poll.h>         // for select
#include <unistd.h>       // for close()
#include <fcntl.h>        // for non-blocking settings
#include <sys/types.h>    // for fd_set, select()
#include <sys/socket.h>   // for socket functions (if needed)
#include <cerrno>         // for errno
#include <iostream>       // for optional debug output
using reactorFunc = std::function<void(int)>;
class Reactor{

private:
std::vector<struct pollfd> fds;
std::unordered_map<int,reactorFunc> callbacks;
bool running = false;

public:
void add_fd(int fd,reactorFunc callback);
void remove_fd(int fd);
void start();
void stop();
};


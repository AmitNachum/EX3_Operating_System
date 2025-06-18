#pragma once

#include <functional>
#include <thread>
#include <unistd.h>

using proactorFunc = std::function<void*(int)>;

class Proactor {
public:
    Proactor(int fd, proactorFunc fun);
    Proactor(const Proactor&) = delete;
    Proactor& operator=(const Proactor&) = delete;
    Proactor(Proactor&& other);
    Proactor& operator=(Proactor&& other);
    ~Proactor();

private:
    int sockfd;
    proactorFunc func;
    std::thread worker;
};

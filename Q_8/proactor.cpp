#include "proactor.hpp"
#include <thread>
#include <unistd.h>

Proactor::Proactor(int fd, proactorFunc fun){

    std::thread([fd,fun](){

        fun(fd);
        ::close(fd);
    }).
    detach();

}
        

Proactor::Proactor(Proactor&& other)
    : sockfd(other.sockfd), func(std::move(other.func)), worker(std::move(other.worker)) {}

Proactor& Proactor::operator=(Proactor&& other) {
    if (this != &other) {
        sockfd = other.sockfd;
        func = std::move(other.func);
        worker = std::move(other.worker);
    }
    return *this;
}


Proactor::~Proactor() = default;


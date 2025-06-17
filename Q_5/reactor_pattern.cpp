#include "reactor_pattern.hpp"
#include <algorithm>


void Reactor::add_fd(int fd,reactorFunc callback){
    fds.push_back({fd,POLLIN,0});
    callbacks[fd] = callback;
}

void Reactor::remove_fd(int fd){
    fds.erase(std::remove_if(fds.begin(),fds.end(),[fd](const pollfd& pdf){
        return pdf.fd == fd;
    }),fds.end());
    callbacks.erase(fd);
    close(fd);
}

void Reactor::start(){
    running = true;

    while(running){
        int ret = poll(fds.data(),fds.size(),-1);
        if( ret < 0) continue;

        for(auto& pdf : fds){
            if(pdf.revents & POLLIN){
                if(callbacks.count(pdf.fd)){
                    callbacks[pdf.fd](pdf.fd);
                }
            }
        }
    }
}

void Reactor::stop(){
    running = false;
}

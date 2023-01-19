#pragma once 
#include <sys/epoll.h>
#include <vector>

#define MAX_EVENTS 1024

class Epoll 
{
public:
    Epoll();
    ~Epoll();

    void addFd(int fd,uint32_t op);
    std::vector<epoll_event> poll(int timeout = -1);

private:
    int epfd;
    struct epoll_event *events;
};
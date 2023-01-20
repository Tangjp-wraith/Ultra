#pragma once 
#include <sys/epoll.h>
#include <vector>

#define MAX_EVENTS 1024
class Channel;
class Epoll 
{
public:
    Epoll();
    ~Epoll();

    // void addFd(int fd,uint32_t op);
    void updateChannel(Channel *channel);
    std::vector<Channel*> poll(int timeout = -1);

private:
    int epfd;
    struct epoll_event *events;
};
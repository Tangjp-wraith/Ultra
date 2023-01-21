#pragma once 
#include <functional>

class EventLoop;
class Socket;
class Channel;

class Connection
{
public:
    Connection(EventLoop *_loop,Socket *_sock);
    ~Connection();
    
    void echo(int sockfd);
    void setDeleteConnectionCallback(std::function<void(Socket*)> _callback);

private:
    EventLoop *loop;
    Socket *sock;
    Channel *channel;
    std::function<void(Socket*)> deleteConnectionCallback;
};
#pragma once 
#include <functional>

class EventLoop;
class Socket;
class InetAddress;
class Channel;

class Acceptor
{
public:
    Acceptor(EventLoop *_loop);
    ~Acceptor();
    void acceptConnection();
    void setNewConnectionCallback(std::function<void(Socket*)> _callback); 

private:
    EventLoop *loop;
    Socket *sock;
    InetAddress *addr;
    Channel *acceptChannel;
    std::function<void(Socket*)> newConnectionCallback;
};
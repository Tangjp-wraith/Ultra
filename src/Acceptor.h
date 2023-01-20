#pragma once 
#include <functional>

class EventLoop;
class Socket;
class InetAddress;
class Channel;

class Acceptor
{
public:
    

private:
    EventLoop *loop;
    Socket *sock;
    InetAddress *addr;
    Channel *acceptChannel;
};
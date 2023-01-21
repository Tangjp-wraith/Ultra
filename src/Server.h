#pragma once
#include <map>

class EventLoop;
class Socket;
class Acceptor;
class Connection;

class Server
{
public:
    Server(EventLoop *_loop);
    ~Server();

    void handleReadEvent(int sockfd); // 处理客户端请求
    void newConnection(Socket *sock); // 新建TCP连接
    void deleteConnection(Socket *sock); // 断开TCP连接

private:
    EventLoop *loop; // 事件循环
    Acceptor *acceptor; // 用于接受TCP连接
    std::map<int,Connection*> connections; // 所有TCP连接
};
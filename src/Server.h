#pragma once

class EventLoop;
class Socket;

class Server
{
public:
    Server(EventLoop *_loop);
    ~Server();

    void handleReadEvent(int sockfd);
    void newConnection(Socket *serv_sock);

private:
    EventLoop *loop;
};
#pragma once

class InetAddress;

class Socket
{
public:
    Socket();
    Socket(int fd);
    ~Socket();

    void bind(InetAddress *_addr);
    void listen();
    void setnonbocking();

    int accept(InetAddress *_addr);

    int getFd();

private:
    int fd;
};
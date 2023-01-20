#pragma once

class InetAddress;

class Socket
{
public:
    Socket();
    Socket(int fd);
    ~Socket();

    void bind(InetAddress *addr);
    void listen();
    void setnonbocking();

    int accept(InetAddress *addr);

    int getFd();

private:
    int fd;
};
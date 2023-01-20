#include "Server.h"
#include "Socket.h"
#include "Channel.h"
#include "InetAddress.h"
#include "util.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <functional>

#define READ_BUFFER 1024

Server::Server(EventLoop* _loop):loop(_loop) {
    Socket *serv_sock=new Socket();
    InetAddress *serv_addr=new InetAddress("127.0.0.1",8888);
    serv_sock->bind(serv_addr);
    serv_sock->listen();
    serv_sock->setnonbocking();

    Channel *servChannel = new Channel(loop,serv_sock->getFd());
    std::function<void()> callback_=std::bind(&Server::newConnection,this,serv_sock);
    servChannel->setCallback(callback_);
    servChannel->enableReading();
}

Server::~Server() {}

void Server::handleReadEvent(int sockfd) {
    char buf[READ_BUFFER]; 
    while(true) {
        //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        bzero(&buf,sizeof(buf));  
        // 从客户端socket读到缓冲区，返回已读数据大小
        ssize_t read_bytes=read(sockfd,buf,sizeof(buf)); 
        if(read_bytes>0){
            printf("message from client fd %d: %s\n",sockfd,buf);
            // 将相同的数据写回到客户端  
            write(sockfd,buf,sizeof(buf));       
        }else if (read_bytes == -1 && errno == EINTR){
            //客户端正常中断、继续读取
            printf("continue reading");
            continue;
        }else if(read_bytes==-1 && ((errno == EAGAIN) || (errno==EWOULDBLOCK))){
            // 非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once, errno: %d\n", errno);
            break;
        }else if(read_bytes==0){
            // EOF，客户端断开连接
            printf("client fd %d disconnected\n",sockfd);
            // 关闭socket会自动将文件描述符从epoll树上移除
            close(sockfd);
            break;
        }
    }
}

void Server::newConnection(Socket* serv_sock) {
    InetAddress *clnt_addr=new InetAddress();
    Socket *clnt_sock=new Socket(serv_sock->accept(clnt_addr));
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->getFd(), inet_ntoa(clnt_addr->addr.sin_addr), ntohs(clnt_addr->addr.sin_port));
    clnt_sock->setnonbocking();
    Channel *clntChannel = new Channel(loop,clnt_sock->getFd());
    std::function<void()> callback_=std::bind(&Server::handleReadEvent,this,clnt_sock->getFd());
    clntChannel->setCallback(callback_);
    clntChannel->enableReading();
}

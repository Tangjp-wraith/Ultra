#include "Connection.h"
#include "Socket.h"
#include "Channel.h"
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define READ_BUFFER 1024

Connection::Connection(EventLoop* _loop, Socket* _sock) 
    :loop(_loop),sock(_sock),channel(nullptr) {
    channel=new Channel(loop,sock->getFd());
    std::function<void()> callback_=std::bind(&Connection::echo,this,sock->getFd());
    channel->setCallback(callback_);
    channel->enableReading();
}

Connection::~Connection() {
    delete channel;
    delete sock;
}

void Connection::echo(int sockfd) {
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
            deleteConnectionCallback(sock);
            break;
        }
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(Socket*)> _callback) {
    deleteConnectionCallback=_callback;
}

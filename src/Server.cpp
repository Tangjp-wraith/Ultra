#include "Server.h"
#include "Socket.h"
#include "Channel.h"
#include "InetAddress.h"
#include "Connection.h"
#include "Acceptor.h"
#include "util.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <functional>

#define READ_BUFFER 1024

Server::Server(EventLoop* _loop):loop(_loop),acceptor(nullptr) {
    acceptor = new Acceptor(loop);
    std::function<void(Socket*)> callback_=std::bind(&Server::newConnection,this,std::placeholders::_1);
    acceptor->setNewConnectionCallback(callback_);
}

Server::~Server() {
    delete acceptor;
}

void Server::newConnection(Socket* sock) {
    Connection *conn = new Connection(loop,sock);
    std::function<void(Socket*)> callback_=std::bind(&Server::deleteConnection,this,std::placeholders::_1);
    conn->setDeleteConnectionCallback(callback_);
    connections[sock->getFd()]=conn;
}

void Server::deleteConnection(Socket *sock) {
    Connection *conn=connections[sock->getFd()];
    connections.erase(sock->getFd());
    delete conn;
}

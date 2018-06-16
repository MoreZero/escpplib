#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include "log.h"
#include "es_socket.h"


namespace escpplib{



CSocket::~CSocket() {
    Close();
}


int CSocket::Bind(const char* ip, int port) {
    if (status_ != CLOSED) return -1;

    // 创建socket
    fd_ = socket(AF_INET, SOCK_STREAM, 0); 
    if (fd_ == -1) 
    {   
        return -1;
    }

    // bind
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);

    port_ = port;
    ip_ = ip;

    int ret = bind(fd_, (struct sockaddr*) &addr, sizeof(addr));
    if (ret == -1) {
        close(fd_);
        fd_ = -1;
        return -1;
    }

    type_ = TCP_LISTEN;
    status_ = BIND;
    
    return 0;
}

int CSocket::Listen(int backlog) {
    if (status_ != BIND) return -1;

    LOG_DEBUG("Listen ip:%s, port:%d", ip_.c_str(), port_);
    int ret = listen(fd_, backlog);
    if (ret == -1) return -1;

    status_ = LISTENING;
    return 0;
}

int CSocket::Accept(CSocket &srv_socket) {
    sockaddr_in addr;
    socklen_t addrlen;
    int connfd = accept(fd_, (sockaddr*) &addr, &addrlen);
    if (connfd == -1) return -1;

    char ip[INET_ADDRSTRLEN+1];
    inet_ntop(AF_INET, &addr.sin_addr.s_addr, ip, INET_ADDRSTRLEN);
  
    
    srv_socket.fd_ = connfd;
    srv_socket.status_ = CONNECTING;
    srv_socket.type_ = TCP_SERVER;
    srv_socket.ip_ = ip;
    srv_socket.port_ = ntohs(addr.sin_port);
    
    return 0; 
}

int CSocket::Connect(const char* ip, int port, bool is_block) {
    if (status_ != CLOSED) return -1;

    // 创建socket
    fd_ = socket(AF_INET, SOCK_STREAM, 0); 
    if (fd_ == -1) 
    {   
        return -1;
    }


    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);

    int ret =  connect(fd_, (const struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1) {
        close(fd_);
        fd_ = -1;
        return -1;
    }

    ip_ = ip;
    port_ = port;
    status_ = CONNECTING;
    type_ = TCP_CLIENT;
    return 0;
}

ssize_t CSocket::Recv(void *buf, size_t len, int flags) {
    return recv(fd_, buf, len, flags);
}

ssize_t CSocket::Send(const void *buf, size_t len, int flags) {
    return send(fd_, buf, len, flags);
}

int CSocket::Close() {
    if(status_ == CLOSED) return 0;

    status_ = CLOSED;
    close(fd_);
    fd_ = -1;
    type_ = UNSET;
    ip_ = "";
    port_ = 0;

    return 0;
}

int CSocket::RecvStr(std::string &str) {
    char buff[8*1024];
    ssize_t ret = Recv(buff, 8*1024, 0);
    if (ret == -1) return -1;
    if (ret == 0) return 0;

    str = std::string(buff, ret);
    return ret;
}

ssize_t CSocket::SendStr(std::string &str) {
    return Send(str.data(), str.size(), 0);
}

int  CSocket::SetNonblocking() {
    int old_option = fcntl(fd_, F_GETFL);
    fcntl(fd_, F_SETFL, old_option | O_NONBLOCK);
    return old_option; 
}

int  CSocket::SetBlocking() {
    int old_option = fcntl(fd_, F_GETFL);
    fcntl(fd_, F_SETFL, old_option & (~O_NONBLOCK));
    return old_option; 
}






}// namespace escpplib

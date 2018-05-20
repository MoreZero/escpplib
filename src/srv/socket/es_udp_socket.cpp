#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include "log.h"
#include "es_udp_socket.h"


namespace escpplib{



CUdpSocket::~CUdpSocket() {
    Close();
}


int CUdpSocket::Bind(const char* ip, int port) {
    if (status_ != CLOSED) return -1;
    if (0 != Init()) {
        return -1;
    }

    // bind
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);

    int ret = bind(fd_, (struct sockaddr*)&addr, sizeof(addr));
    if (ret == -1) {
        fd_ = -1;
        return -1;
    }

    status_ = BIND;
    return 0;
}

ssize_t CUdpSocket::RecvFrom(void* buf)
{
    if (status_ != BIND) {
        return -1;
    }
    
    //buf = recv_buff_;
    socklen_t len = sizeof(struct sockaddr_in);
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    ssize_t cnt = recvfrom(fd_, buf, 65534, 0, (struct sockaddr *)&addr, &len);
    if (cnt == -1) return -1;

    char ip[INET_ADDRSTRLEN+1];
    inet_ntop(AF_INET, &addr.sin_addr, ip, INET_ADDRSTRLEN);
    ip_ = ip;
    port_ = ntohs(addr.sin_port);
    return cnt;
}


ssize_t CUdpSocket::SendTo(const void* buf, size_t len, const char* ip, int port)
{
    if (status_ == CLOSED) {
        if (0 != Init()) {
            return -1;
        }
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);

    return sendto(fd_, buf, len, 0, (const struct sockaddr*)&addr, sizeof(addr));
}

int CUdpSocket::Init()
{
    if (status_ != CLOSED) return -1;

    fd_ = socket(AF_INET, SOCK_DGRAM, 0); 
    if (fd_ == -1) 
    {   
        return -1;
    }
    
    status_ = INITED;
    return 0;
}



int CUdpSocket::Close() {
    if(status_ == CLOSED) return 0;

    status_ = CLOSED;
    close(fd_);
    fd_ = -1;
    type_ = UNSET;
    ip_ = "";
    port_ = 0;

    return 0;
}


int  CUdpSocket::SetNonblocking() {
    int old_option = fcntl(fd_, F_GETFL);
    fcntl(fd_, F_SETFL, old_option | O_NONBLOCK);
    return old_option; 
}

int  CUdpSocket::SetBlocking() {
    int old_option = fcntl(fd_, F_GETFL);
    fcntl(fd_, F_SETFL, old_option & (~O_NONBLOCK));
    return old_option; 
}






}// namespace escpplib

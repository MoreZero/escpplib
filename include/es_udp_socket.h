#ifndef ESCPPLIB_UDP_SOCKET_H
#define ESCPPLIB_UDP_SOCKET_H

#include <string>

namespace escpplib{

    
class CUdpSocket 
{
public:
    enum STATUS{
        CLOSED=0,
        BIND,
        INITED,
    };
    enum TYPE{
        UNSET=0,
        TCP_CLIENT,
        TCP_SERVER,
        TCP_LISTEN,
        UDP
    };

    CUdpSocket(): fd_(-1),status_(CLOSED),port_(0){}
    ~CUdpSocket();
   
    // 系统调用相关封装
    ssize_t RecvFrom(void* buf);
    ssize_t SendTo(const void* buf, size_t len, const char* ip, int port);
    int     Init();
    int     Bind(const char* ip, int port);
    
    
    int     Close();
    

    // 属性设置
    int SetNonblocking();
    int SetBlocking();

    // 状态数据
    int GetFd() { return fd_;}
    int GetStatus() { return status_;}
    int GetType() { return type_;}
    const std::string& GetIp() { return ip_;}
    int GetPort() { return port_;}

    
   
private:
    CUdpSocket(CUdpSocket &socket);


private:
    int     fd_;
    char recv_buff_[65535];
    STATUS  status_;
    TYPE    type_;

    std::string  ip_;    //如果为LISTEN类型，为本地ip:port
    int     port_;  //如果为CLIENT类型，为目标ip:port
                    //如果为SERVER类型，为来源ip:port
                
};



}// namespace escpplib



#endif

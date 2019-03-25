#ifndef ESCPPLIB_SOCKET_H
#define ESCPPLIB_SOCKET_H

#include <string>

namespace escpplib{

    
class CSocket 
{
public:
    enum STATUS{
        CLOSED=0,
        CONNECTING,
        BIND,
        LISTENING
    };
    enum TYPE{
        UNSET=0,
        TCP_CLIENT,
        TCP_SERVER,
        TCP_LISTEN,
        UDP
    };

    CSocket(): fd_(-1),status_(CLOSED),port_(0){}
    ~CSocket();
   
    // 系统调用相关封装
    int     Bind(const char* ip, int port);
    int     Listen(int backlog);
    int     Accept(CSocket &srv_socket);
    int     Connect(const char* ip, int port, bool is_block=true); 
    ssize_t Recv(void *buf, size_t len, int flags);
    ssize_t Send(const void *buf, size_t len, int flags);
    int     Close();
    
    int RecvStr(std::string &str);
    ssize_t SendStr(std::string &str);

    // 属性设置
    int SetNonblocking();
    int SetBlocking();

    // 状态数据
    int GetFd() { return fd_;}
    int GetStatus() { return status_;}
    int GetType() { return type_;}
    const std::string& GetIp() { return ip_;}
    int GetPort() { return port_;}

    int GetDestAddr(std::string& ip, int32_t& port);
    
   
private:
    CSocket(CSocket &socket);


private:
    int     fd_;
    STATUS  status_;
    TYPE    type_;

    std::string  ip_;    //如果为LISTEN类型，为本地ip:port
    int     port_;  //如果为CLIENT类型，为目标ip:port
                    //如果为SERVER类型，为来源ip:port
                
};



}// namespace escpplib



#endif

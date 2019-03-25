#include "log.h"
#include "es_socket.h"
#include "timer/timer.h"
#include <string> 
//using namespace escpplib;

int main(){
    escpplib::CCostTimer timer(); 
    LOG_INFO("socket test");


    escpplib::CSocket listen_socket;
    int ret = listen_socket.Bind("127.0.0.1", 50000);
    if (ret != 0){
        LOG_FATAL_MSG("listen_socket.Bind");
    }

    ret = listen_socket.Listen(1024);
    if (ret != 0){
        LOG_FATAL_MSG("listen_socket.Listen");
    }

    while(1) {
        LOG_INFO("start accept" );
        escpplib::CSocket server_socket;
        ret = listen_socket.Accept(server_socket);
        if (ret != 0){
            LOG_FATAL_MSG("listen_socket.Listen");
        }
         
        LOG_INFO("get connect addr:%s:%d", server_socket.GetIp().c_str(), server_socket.GetPort() );
        std::string dst_ip = "";
        int32_t port = 0;
        ret = server_socket.GetDestAddr(dst_ip, port);
        LOG_INFO("dst_ip:%s, port:%d", dst_ip.c_str(), port);
        if (ret != 0){
            LOG_FATAL_MSG("GetDestAddr Failed, ret:%d", ret);
        }
       
        std::string ack_msg;
        std::string recv_msg;
        //LOG_DEBUG("EWOULDBLOCK:%d EAGAIN:%d", EWOULDBLOCK, EAGAIN ); all == 11
        for (int i=0; i<5; i++ ) {
            int ret = server_socket.RecvStr(recv_msg);
            if (ret == -1) {
                if (errno == EAGAIN ){
                    LOG_ERROR_MSG("EAGAIN");
                    server_socket.SetBlocking();
                }else {
                    LOG_ERROR_MSG("server_socket.RecvStr:");
                }
                continue;
            }
            
            if (ret == 0) {
                LOG_INFO("need close");
                break;
            }
            LOG_INFO("recv:%s", recv_msg.c_str());
            ack_msg = "ack_msg:"+recv_msg;
            ret = server_socket.SendStr(ack_msg);
            LOG_INFO("server_socket.SendStr:%d", ret);
            server_socket.SetNonblocking();
        }

    }


    return 0;
}

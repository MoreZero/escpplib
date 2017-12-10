#include "log.h"
#include "es_socket.h"
#include "timer/timer.h"
#include <string> 
#include <stdlib.h> 
#include <iostream> 
//using namespace escpplib;

int main(int argc, char** argv){
    if (argc < 3) {
        LOG_FATAL("please input ip and port");
    }

    escpplib::CCostTimer timer(); 
    LOG_INFO("socket client test");

    escpplib::CSocket client_socket;
    int ret = client_socket.Connect(argv[1], atoi(argv[2]));
    if (ret != 0){
        LOG_FATAL_MSG("listen_socket.Connect");
    }

    while(1){
        std::string send_msg;
        std::string recv_msg;
        std::cin>>send_msg;
        if (send_msg == "exit") break;
        ret = client_socket.SendStr(send_msg);
        LOG_INFO("client_socket.SendStr:%d", ret);
        ret = client_socket.RecvStr(recv_msg);
        if (ret == -1) {
            LOG_ERROR_MSG("server_socket.RecvStr:");
            continue;
        }
        
        if (ret == 0) {
            LOG_INFO("need close");
            client_socket.Close();
            break;
        }
        LOG_INFO("recv:%s", recv_msg.c_str());

    }


    return 0;
}

#include "log.h"
#include "es_udp_socket.h"
#include "timer/timer.h"
#include <string> 
using namespace std;
//using namespace escpplib;

int main(int argc, char** argv){
    
    if (argc < 3) {
        LOG_FATAL_MSG("please input dest_ip,dest_port,bind_port, interval");
    }
    string dest_ip = argv[1];
    int dest_port = atoi(argv[2]);
    int bind_port = atoi(argv[3]);
    int interval = atoi(argv[4]);
    LOG_INFO("dest_ip:%s\n,dest_port:%d",
            dest_ip.c_str(), dest_port);

    escpplib::CCostTimer timer(); 
    LOG_INFO("socket test");

    escpplib::CUdpSocket udp_socket;
    if (0 != udp_socket.Bind("0.0.0.0", bind_port)) {
        LOG_FATAL_MSG("udp_socket.Bind");
    }
    while(1) {
        udp_socket.SendTo("test msg", 8, dest_ip.c_str(), dest_port);
        char  buff[65535];
        ssize_t cnt = udp_socket.RecvFrom(buff);
        if (cnt == -1) {
            LOG_ERROR_MSG("udp_socket.RecvFrom");
            continue;
        }
        buff[cnt] = 0;
        LOG_INFO("recv from ip:%s, port:%d cnt:%ld recv msg:%s",
                udp_socket.GetIp().c_str(), udp_socket.GetPort(), cnt, buff);

        return 0;
        sleep(interval);
    }

    return 0;
}

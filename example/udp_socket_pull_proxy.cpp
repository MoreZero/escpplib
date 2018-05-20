#include "log.h"
#include "es_udp_socket.h"
#include "timer/timer.h"
#include <string> 
using namespace std;
//using namespace escpplib;

int main(int argc, char** argv){
    
    if (argc < 7) {
        LOG_FATAL_MSG("please input srv_ip,srv_prot,local_ip,local_port,dest_ip,dest_port");
    }
    string srv_ip = argv[1];
    int srv_port = atoi(argv[2]);
    string local_ip = argv[3];
    int local_port = atoi(argv[4]);
    string dest_ip = argv[5];
    int dest_port = atoi(argv[6]);
    LOG_INFO("srv_ip:%s\nsrv_prot:%d\n,local_ip:%s\nlocal_prot:%d\n,dest_ip:%s\n,dest_port:%d",
            srv_ip.c_str(), srv_port, local_ip.c_str(), local_port, dest_ip.c_str(), dest_port);

    escpplib::CCostTimer timer(); 
    LOG_INFO("socket test");

    escpplib::CUdpSocket udp_socket;
    if (0 != udp_socket.Bind(local_ip.c_str(), local_port)) {
        LOG_FATAL_MSG("udp_socket.Bind");
    }

    while(1) {
        char  buff[65535];
        udp_socket.SendTo("xxx", 3, srv_ip.c_str(), srv_port);
        ssize_t cnt = udp_socket.RecvFrom(buff);
        if (cnt == -1) {
            LOG_ERROR_MSG("udp_socket.RecvFrom");
            continue;
        }
        buff[cnt] = 0;
        LOG_INFO("recv from ip:%s, port:%d cnt:%ld recv msg:%s",
                udp_socket.GetIp().c_str(), udp_socket.GetPort(), cnt, buff);
        sleep(10);
    }

    return 0;
}

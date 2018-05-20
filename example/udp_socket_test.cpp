#include "log.h"
#include "es_udp_socket.h"
#include "timer/timer.h"
#include <string> 
using namespace std;
//using namespace escpplib;

int main(int argc, char** argv){
    
    if (argc < 5) {
        LOG_FATAL_MSG("please input src_ip,src_prot,dest_ip,dest_port");
    }
    string src_ip = argv[1];
    int src_port = atoi(argv[2]);
    string dest_ip = argv[3];
    int dest_port = atoi(argv[4]);
    LOG_INFO("src_ip:%s\nsrc_prot:%d\n,dest_ip:%s\n,dest_port:%d",
            src_ip.c_str(), src_port, dest_ip.c_str(), dest_port);

    escpplib::CCostTimer timer(); 
    LOG_INFO("socket test");

    escpplib::CUdpSocket udp_socket;
    //if (0 != udp_socket.Bind("119.29.155.157", 8000)) {
    if (0 != udp_socket.Bind(src_ip.c_str(), src_port)) {
        LOG_FATAL_MSG("udp_socket.Bind");
    }

    while(1) {
        char  buff[65535];
        ssize_t cnt = udp_socket.RecvFrom(buff);
        if (cnt == -1) {
            LOG_ERROR_MSG("udp_socket.RecvFrom");
            continue;
        }
        buff[cnt] = 0;
        LOG_INFO("recv cnt:%d recv msg:%s", cnt, buff);
        udp_socket.SendTo(buff, cnt, dest_ip.c_str(), dest_port);
    }

    return 0;
}

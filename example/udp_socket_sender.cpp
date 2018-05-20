#include "log.h"
#include "es_udp_socket.h"
#include "timer/timer.h"
#include <string> 
using namespace std;
//using namespace escpplib;

int main(int argc, char** argv){
    
    if (argc < 3) {
        LOG_FATAL_MSG("please input dest_ip,dest_port, interval");
    }
    string dest_ip = argv[1];
    int dest_port = atoi(argv[2]);
    int interval = atoi(argv[3]);
    LOG_INFO("dest_ip:%s\n,dest_port:%d",
            dest_ip.c_str(), dest_port);

    escpplib::CCostTimer timer(); 
    LOG_INFO("socket test");

    escpplib::CUdpSocket udp_socket;

    while(1) {
        udp_socket.SendTo("test msg", 8, dest_ip.c_str(), dest_port);
        sleep(interval);
    }

    return 0;
}

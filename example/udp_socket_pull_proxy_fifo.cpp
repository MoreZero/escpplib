#include "log.h"
#include "es_udp_socket.h"
#include "timer/timer.h"
#include <string> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 

using namespace std;
//using namespace escpplib;

int main(int argc, char** argv){
    
    if (argc < 6) {
        LOG_FATAL_MSG("please input srv_ip,srv_prot,local_ip,local_port,fifo_file");
    }
    string srv_ip = argv[1];
    int srv_port = atoi(argv[2]);
    string local_ip = argv[3];
    int local_port = atoi(argv[4]);
    string fifo_file = argv[5];
    LOG_INFO("srv_ip:%s\nsrv_prot:%d\n,local_ip:%s\nlocal_prot:%d\n,fifo_file:%s\n",
            srv_ip.c_str(), srv_port, local_ip.c_str(), local_port, fifo_file.c_str());

    escpplib::CCostTimer timer(); 
    LOG_INFO("socket test");

    escpplib::CUdpSocket udp_socket;
    if (0 != udp_socket.Bind(local_ip.c_str(), local_port)) {
        LOG_FATAL_MSG("udp_socket.Bind");
    }

    udp_socket.SendTo("start connet", 12, srv_ip.c_str(), srv_port);

    int32_t fifo_fd = open(fifo_file.c_str(),O_WRONLY);
    if (fifo_fd == -1) {
        LOG_FATAL_MSG("open fifo failed:%s", fifo_file.c_str());
    }

    while(1) {
        char  buff[65535];
        ssize_t cnt = udp_socket.RecvFrom(buff);
        if (cnt == -1) {
            LOG_ERROR_MSG("udp_socket.RecvFrom");
            continue;
        }
        buff[cnt] = 0;
        LOG_INFO("recv from ip:%s, port:%d cnt:%ld",
                udp_socket.GetIp().c_str(), udp_socket.GetPort(), cnt);

        ssize_t ret = write(fifo_fd, buff, cnt);
        if (ret != cnt) {
            LOG_FATAL_MSG("write fifo failed:%s", fifo_file.c_str());
        }
        //udp_socket.SendTo(buff, cnt, dest_ip.c_str(), dest_port);
    }

    return 0;
}

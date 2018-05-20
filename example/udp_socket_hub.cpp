#include "log.h"
#include "event.h"
#include "es_udp_socket.h"
#include "timer/timer.h"
#include <string> 
using namespace std;
//using namespace escpplib;


escpplib::CEventEpoll event;


int main(int argc, char** argv){
    
    if (argc < 5) {
        LOG_FATAL_MSG("please input input_ip,recv_prot,output_ip,output_port");
    }
    string input_ip = argv[1];
    int input_port = atoi(argv[2]);
    string output_ip = argv[3];
    int output_port = atoi(argv[4]);



    LOG_INFO("input_ip:%s\nsrv_prot:%d\n,output_ip:%s\nlocal_prot:%d",
            input_ip.c_str(), input_port, output_ip.c_str(), output_port);


    escpplib::CUdpSocket udp_input_socket;
    if (0 != udp_input_socket.Bind(input_ip.c_str(), input_port)) {
        LOG_FATAL_MSG("udp_input_socket.Bind");
    }
    escpplib::CUdpSocket udp_output_socket;
    if (0 != udp_output_socket.Bind(output_ip.c_str(), output_port)) {
        LOG_FATAL_MSG("udp_output_socket.Bind");
    }

    if (event.RegEvent(udp_input_socket.GetFd(), EPOLLIN) != 0) {
        LOG_ERROR_MSG("event.RegEvent udp_input_socket");
    }
    if (event.RegEvent(udp_output_socket.GetFd(), EPOLLIN) != 0) {
        LOG_ERROR_MSG("event.RegEvent udp_output_socket");
    }

    string client_ip = "";
    int client_port = 0;
    char  buff[65535];
    while(true) {
        LOG_INFO("event.Wait(60*1000)");
        int32_t ret = event.Wait(60 * 1000);
        if (ret == -1) {
            LOG_FATAL_MSG("event.Wait error");
        }

        LOG_INFO("ret:%d fd:", ret);
        for (int i=0; i<ret; ++i) {
            struct epoll_event & events = event.epoll_event_[i];
            LOG_INFO("event fd:%d", events.data.fd);
            if (events.data.fd == udp_input_socket.GetFd()) {
                ssize_t cnt = udp_input_socket.RecvFrom(buff);
                if (cnt == -1) {
                    LOG_ERROR_MSG("udp_input_socket.RecvFrom");
                    continue;
                }
                buff[cnt] = 0;
                LOG_INFO("recv from ip:%s, port:%d cnt:%ld",
                        udp_input_socket.GetIp().c_str(), udp_input_socket.GetPort(), cnt);
                if (client_port != 0)
                {
                    udp_output_socket.SendTo(buff, cnt, client_ip.c_str(), client_port);
                }

            } else if (events.data.fd == udp_output_socket.GetFd()) {
                ssize_t cnt = udp_output_socket.RecvFrom(buff);
                if (cnt == -1) {
                    LOG_ERROR_MSG("udp_output_socket.RecvFrom");
                    continue;
                }
                buff[cnt] = 0;
                client_ip = udp_output_socket.GetIp();
                client_port = udp_output_socket.GetPort();
                LOG_INFO("recv from ip:%s, port:%d cnt:%ld",
                        client_ip.c_str(), client_port, cnt );
            } else {
                LOG_ERROR("unsearch fd");
            }
        } 
    }

    return 0;
}

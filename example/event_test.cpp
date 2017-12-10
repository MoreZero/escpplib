#include "log.h"
#include "event.h"
#include "es_socket.h"
#include "timer/timer.h"
#include <string> 
#include <unistd.h> 
#include <errno.h> 

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//using namespace escpplib;

int main(){
    LOG_INFO("event cost test");
    escpplib::CCostTimer timer("event cost test"); 

    
    escpplib::CEventEpoll event( 1000);
   
    int file_fd = open("./test_event_file", O_RDWR);
    if (file_fd == -1) {
        LOG_FATAL_MSG("open error");
    }

    int pipefd[2];

    pipe(pipefd);
    LOG_INFO("pipe %d %d", pipefd[0], pipefd[1]);

    escpplib::CSocket listen_socket;
    int ret = listen_socket.Bind("127.0.0.1", 50000);
    if (ret != 0){
        LOG_FATAL_MSG("listen_socket.Bind");
    }
    ret = listen_socket.Listen(1024);
    if (ret != 0){
        LOG_FATAL_MSG("listen_socket.Listen");
    }
    LOG_INFO("socket fd:%d", listen_socket.GetFd());
    
    if (escpplib::SetNonblock(listen_socket.GetFd()) != 0) {
        LOG_ERROR_MSG("SetNonblock(listen_socket.GetFd())");
    }
    if (escpplib::SetNonblock(file_fd) != 0) {
        LOG_ERROR_MSG("SetNonblock(file_fd)");
    }

    if (event.RegEvent(pipefd[0], EPOLLIN) != 0) {
        LOG_ERROR_MSG("event.RegEvent pipefd[0]");
    }
    if (event.RegEvent(pipefd[1], EPOLLIN) != 0) {
        LOG_ERROR_MSG("event.RegEvent pipefd[1]");
    }
    if (event.RegEvent(listen_socket.GetFd(), EPOLLIN) != 0) {
        LOG_ERROR_MSG("event.RegEvent listen_socket");
    }
    if (event.RegEvent(file_fd, EPOLLIN) != 0) {
        LOG_ERROR_MSG("event.RegEvent file_fd");
    }

    write(pipefd[1], "hehe", 4);

    ret = event.Wait(10 * 1000);
    if (ret == -1) {
        LOG_FATAL_MSG("event.Wait error");
    }

    
    LOG_INFO("ret:%d fd:", ret);
    for (int i=0; i<ret; ++i) {
        struct epoll_event & events = event.epoll_event_[i];
        LOG_INFO("event fd:%d", events.data.fd);
        
        //escpplib::CSocket server_socket;
        //ret = listen_socket.Accept(server_socket);
        //if (ret != 0){
        //    LOG_ERROR_MSG("listen_socket.Listen errno:%d EAGAIN:%d", errno, EAGAIN);
        //}
    } 

    return 0;
}

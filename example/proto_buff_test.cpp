
#include "log.h"
#include "event.h"
#include "es_socket.h"
#include "timer/timer.h"
#include <string> 
#include <unistd.h> 
#include <errno.h> 
#include <pthread.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "proto_buff.h"

//using namespace escpplib;
using namespace std::placeholders;
    
escpplib::CEventEpoll event;

class ProtoBuffTest
{
public:
    ProtoBuffTest() {
        proto_buff_.SetProtoHeadParseFunc(std::bind(&ProtoBuffTest::HeadParseFunc, this, _1, _2, _3, _4, _5));
        proto_buff_.RegisterFunc(1, std::bind(&ProtoBuffTest::TestHandle1, this, _1, _2, _3, _4));
        proto_buff_.RegisterFunc(2, std::bind(&ProtoBuffTest::TestHandle2, this, _1, _2, _3, _4));
    }

    int32_t TestHandle1(const int8_t* head, int32_t head_size, const int8_t* body, int32_t body_size) {
        LOG_INFO("TestHaneld1 head_size:%d, body_size:%d", head_size, body_size);  
        return 0;
    }

    int32_t TestHandle2(const int8_t* head, int32_t head_size, const int8_t* body, int32_t body_size) {
        LOG_INFO("TestHaneld2 head_size:%d, body_size:%d", head_size, body_size);  
        return 0;
    }
    
    int32_t HeadParseFunc(const int8_t* buff, int32_t size, 
                                 int32_t& cmd, int32_t& head_size, int32_t& body_size) {
        if (size < 8) return 0;
        if (buff[0] == 'a') {
            cmd = 1;
        }else if (buff[0] == 'b') {
            cmd = 2;
        }else {
            return -1;
        }

        head_size = 2;
        body_size = 6;
        return 1;
    }

    int32_t Recv() {
        int8_t buf[65536];

        ssize_t cnt = socket_.Recv(buf, 65536, 0);
        if (cnt <= 0) return cnt;

        LOG_INFO("Recv cnt:%d", (int)cnt);
        int32_t ret = proto_buff_.Write(buf, cnt);
        if (ret < 0) return -1;

        return 1;
    }

    int32_t Close() {
        return socket_.Close();
    }
    escpplib::CSocket& Socket(){return socket_;}
private:
    escpplib::ProtoBuff proto_buff_;
    escpplib::CSocket socket_;

};

ProtoBuffTest* g_proto_buff_test=NULL;



int main(){
    LOG_INFO("proto_buff_test test");
    escpplib::CCostTimer timer("proto_buff_test"); 

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
    
    //if (escpplib::SetNonblock(listen_socket.GetFd()) != 0) {
    //    LOG_ERROR_MSG("SetNonblock(listen_socket.GetFd())");
    //}

    if (event.RegEvent(listen_socket.GetFd(), EPOLLIN) != 0) {
        LOG_ERROR_MSG("event.RegEvent listen_socket");
    }

    
    int32_t current_fd=-1;
    while(true) {
        LOG_INFO("event.Wait(60*1000)");
        ret = event.Wait(60 * 1000);
        if (ret == -1) {
            LOG_FATAL_MSG("event.Wait error");
        }

        LOG_INFO("ret:%d fd:", ret);
        for (int i=0; i<ret; ++i) {
            struct epoll_event& events = event.epoll_event_[i];
            LOG_INFO("event fd:%d", events.data.fd);
            if (events.data.fd == listen_socket.GetFd()) {
                LOG_INFO("listen_socket event");

                ProtoBuffTest* proto_buff_test = new ProtoBuffTest();
                ret = listen_socket.Accept(proto_buff_test->Socket());
                if (ret != 0){
                    LOG_ERROR_MSG("listen_socket.Listen errno:%d EAGAIN:%d", errno, EAGAIN);
                }
                LOG_INFO("new connect fd:%d", proto_buff_test->Socket().GetFd());
    

                if (g_proto_buff_test != NULL && g_proto_buff_test->Socket().GetStatus() != escpplib::CSocket::CLOSED) {
                    LOG_ERROR_MSG("already connect, close");
                    proto_buff_test->Close();
                    delete proto_buff_test;
                    continue;
                }


                if (event.RegEvent(proto_buff_test->Socket().GetFd(), EPOLLIN) != 0) {
                    LOG_ERROR_MSG("event.RegEvent socket");
                    proto_buff_test->Close();
                    continue;
                }
                g_proto_buff_test = proto_buff_test;
                current_fd = proto_buff_test->Socket().GetFd();

            } else if (events.data.fd == current_fd) {
                int32_t ret = g_proto_buff_test->Recv();
                if (ret <= 0) {
                    LOG_INFO("need close fd:%d",g_proto_buff_test->Socket().GetFd());
                    // TODO 取消注册
                    if (event.RmEvent(g_proto_buff_test->Socket().GetFd()) != 0) {
                        LOG_ERROR_MSG("event.RegEvent socket");
                    }
                    g_proto_buff_test->Close();

                    delete g_proto_buff_test;
                    g_proto_buff_test = NULL;
                }
            } else {
                LOG_INFO("bug fd");
            }
        } 
    }

    return 0;
}

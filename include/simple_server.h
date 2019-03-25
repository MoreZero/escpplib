#ifndef ESCPPLIB_LF_SERVER_H
#define ESCPPLIB_LF_SERVER_H

#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <list>
#include <map>

#include <functional>
#include <memory>

#include "event.h"
#include "config.h"
#include "es_socket.h"

namespace escpplib{

class CEvent;
void DefaultClientCreater(CEvent* event, std::shared_ptr<CSocket> p_client_socket);
class CDefaultClient;

typedef std::function<int()> WriteAbleFunc;
typedef std::function<int()> ReadAbleFunc;
typedef std::function<int()> CloseAbleFunc;


class CEvent
{

public:
    CEvent():p_event_epoll_(NULL){
        LOG_DEBUG("CEvent()");
    }
    CEvent( std::shared_ptr<void> p_obj,
            ReadAbleFunc read_able_func, 
            WriteAbleFunc write_able_func, 
            CloseAbleFunc close_able_func
          ):p_obj_(p_obj),
            read_able_func_(read_able_func),
            write_able_func_(write_able_func),
            close_able_func_(close_able_func) {
    
    }
    int32_t RegEvent(CEventEpoll *p_event_epoll, uint32_t events) {
        p_event_epoll_ = p_event_epoll;
        return p_event_epoll_->RegEvent(this, fd_, events);
    }

    ~CEvent(){
        LOG_DEBUG("~CEvent()");
        // 存在epoll，则注销事件
        if (p_event_epoll_ != NULL) {
            if (0 != p_event_epoll_->RmEvent(fd_)) {
                LOG_ERROR_MSG("event_.RmEvent");
            }
        }
    }

    int32_t HandleEvent(uint32_t events) {
        //结束事件
        if ((events & EPOLLRDHUP) || (events & EPOLLERR)) {
            LOG_DEBUG("close event");
            if (!close_able_func_) {
                LOG_ERROR("close_able_func_ unable");
            }
            close_able_func_();
            // 注销事件
            return 0;
        }

        // 可读事件
        if (events & EPOLLIN) {
            LOG_DEBUG("read event");
            if (!read_able_func_) {
                LOG_ERROR("read_able_func_ unable");
            }
            read_able_func_();
        }

        // 可写事件
        if (events & EPOLLOUT) {
            LOG_DEBUG("write event");
            if (!write_able_func_) {
                LOG_ERROR("write_able_func_ unable");
            }
            write_able_func_();
        }
        return 1;
    }

public:
    int                     fd_;
    std::shared_ptr<void>   p_obj_;
    ReadAbleFunc            read_able_func_;
    WriteAbleFunc           write_able_func_;
    CloseAbleFunc           close_able_func_;
    CEventEpoll *           p_event_epoll_;
};


class CDefaultClient {
public:
    CDefaultClient(std::shared_ptr<CSocket> p_socket):p_socket_(p_socket) {
        LOG_DEBUG("CDefaultClient");
        LOG_DEBUG("new connect from:%s", p_socket_->GetIp().c_str());
    }
    ~CDefaultClient() {
        LOG_DEBUG("~CDefaultClient");
    }

    int OnWriteAble() {
        LOG_INFO("OnWriteAble");
        return 0;
    }
    int OnReadAble() {
        LOG_INFO("OnReadAble");
        std::string buf;
        int ret = p_socket_->RecvStr(buf);
        if (ret == -1) {
            LOG_ERROR_MSG("recv error"); 
            return -1;
        }
        if (ret == 0) {
            LOG_ERROR_MSG("need close");
            return 0;
        }

        LOG_INFO("recv from:%s, msg:%s", p_socket_->GetIp().c_str(), buf.c_str());

        buf = "ack from server";
        ret = p_socket_->SendStr(buf);
        if (ret != (int)buf.size()) {
            LOG_ERROR_MSG("ret != buf.size %d", ret);
        }
        
        return 0;
    }
    int OnCloseAble() {
        LOG_INFO("OnCloseAble");
    

        return 0;
    }
private:
    std::shared_ptr<CSocket> p_socket_;
};



extern void DefaultClientCreater(CEvent* event, std::shared_ptr<CSocket> p_client_socket);


class CSimpleServer
{
public:

    CSimpleServer();
    ~CSimpleServer();
    int32_t Init(const std::string conf_section, CConfig &config);
    int32_t RegisterClientCreate(std::function<void(CEvent*,std::shared_ptr<CSocket>)> creater);
    
    int32_t Run();
    int32_t Stop();

private:

    int32_t _StartListen();
    int32_t _OnAcceptAble();
    
private:
    int32_t         listen_port_;
    std::string     listen_ip_;
    CEvent          listen_event_;

    bool            is_stop_;
    CSocket         listen_socket_;
    CEventEpoll     event_;

    std::map<int32_t,std::shared_ptr<CEvent>> event_map_;
    std::map<int32_t,std::shared_ptr<CSocket>> socket_map_;
    std::function<void(CEvent*,std::shared_ptr<CSocket>)> client_creater_;
};




}// namespace escpplib


#endif

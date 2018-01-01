#include "simple_server.h"
#include "log.h"

#include <functional>
#include <memory>


namespace escpplib{

void DefaultClientCreater(CEvent* event, std::shared_ptr<CSocket> p_client_socket) {
    CDefaultClient * default_client = new CDefaultClient(p_client_socket);

    event->write_able_func_ = std::bind(&CDefaultClient::OnWriteAble, default_client);
    event->read_able_func_ = std::bind(&CDefaultClient::OnReadAble, default_client);
    event->close_able_func_ = std::bind(&CDefaultClient::OnCloseAble, default_client);
    event->p_obj_.reset( default_client);
}



CSimpleServer::CSimpleServer():is_stop_(true) {
    client_creater_ = DefaultClientCreater;
}

CSimpleServer::~CSimpleServer() {

}

int32_t CSimpleServer::Init(const std::string conf_section, CConfig &config) {
    listen_ip_ = config.Get(conf_section, "listen_ip", "127.0.0.1");
    listen_port_ = config.Get(conf_section, "listen_port", 9527);
    return 0;
}

int32_t CSimpleServer::RegisterClientCreate(std::function<void(CEvent*,std::shared_ptr<CSocket>)> creater) {
    client_creater_ = creater;
    return 0;
}

int32_t CSimpleServer::_StartListen() {
    int ret = listen_socket_.Bind(listen_ip_.c_str(), listen_port_);
    if (ret != 0) {
        return -1;
    }

    ret = listen_socket_.Listen(1024);
    if (ret != 0) {
        return -1;
    }

    listen_event_.read_able_func_ = std::bind(&CSimpleServer::_OnAcceptAble, this);
    listen_event_.p_obj_.reset(this);

    if (0 != event_.RegEvent(&listen_event_, listen_socket_.GetFd(), EPOLLIN) ) {
        LOG_DEBUG_MSG("event_.RegEvent()");
        return -1;
    }
    return 0;
}

int32_t CSimpleServer::_OnAcceptAble() {
    std::shared_ptr<CSocket> p_client_socket(new CSocket());
    if (0 != listen_socket_.Accept(*(p_client_socket.get())) ) {
        LOG_DEBUG_MSG("Accept Error");
        return -1;
    }
    p_client_socket->SetBlocking();
    std::shared_ptr<CEvent>& p_client_event = event_map_[p_client_socket->GetFd()];
    p_client_event.reset(new CEvent);
    p_client_event->fd_ = p_client_socket->GetFd();
    client_creater_(p_client_event.get(), p_client_socket);

    if (0 != p_client_event->RegEvent(&event_, EPOLLIN | EPOLLRDHUP)) {
        LOG_DEBUG_MSG("event_.RegEvent()");
        event_map_.erase(p_client_socket->GetFd());
        return -1;
    }

    //socket_map_[p_client_socket->GetFd()] = p_client_socket;

    return 0;
}

int32_t CSimpleServer::Run() {
    is_stop_ = false;
    int32_t ret;
    
    if (0 != _StartListen() ) {
        LOG_DEBUG_MSG("");
        return -1; 
    }

    while(!is_stop_) {
        LOG_INFO("start event_wait");
        ret = event_.Wait(60*1000);
        if (ret == -1) {
            LOG_FATAL_MSG("event.Wait failed");
        }

        LOG_INFO("Get Event:%d, all connect:%lu", ret, event_map_.size());
        for (int i=0; i<ret; ++i) {
            struct epoll_event& event = event_.epoll_event_[i];
            //LOG_INFO("u32:%u", event.data.u32);
            //LOG_INFO("u64:%lu", event.data.u64);
            if (0 == ((CEvent*)event.data.ptr)->HandleEvent(event.events)) {
                event_map_.erase(((CEvent*)event.data.ptr)->fd_);
            }
            
        }

    }

    return 0;
}



















}




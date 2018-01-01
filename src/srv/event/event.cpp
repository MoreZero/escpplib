
#include "log.h"
#include "event.h"


namespace escpplib{


int SetNonblock(int fd) {
	int old_option = fcntl(fd, F_GETFL);		//设置套接字非阻塞
	return fcntl(fd, F_SETFL, old_option | O_NONBLOCK);
}


CEventEpoll::CEventEpoll(int event_size):event_size_(event_size) {
    epoll_event_ = new epoll_event[event_size];

    epoll_fd_ = epoll_create(event_size);
    if (epoll_fd_ == -1)
        LOG_FATAL_MSG("epoll_create error");

}

CEventEpoll::~CEventEpoll() {
    if (epoll_fd_ != -1) {
        close(epoll_fd_);
    }
    delete [] epoll_event_;
}

int32_t CEventEpoll::RegEvent(int fd, uint32_t events) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;
    return epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev);
}

int32_t CEventEpoll::RegEvent(void* obj, int fd, uint32_t events) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.ptr = obj;
    return epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev);
}

int32_t CEventEpoll::RmEvent(int fd) {
    return epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, 0);
}
//int32_t CEventEpoll::ResetEvent();

int32_t CEventEpoll::Wait(int timeout) {
    return epoll_wait(epoll_fd_, epoll_event_, event_size_, timeout);
}


}// namespace escpplib

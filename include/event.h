#ifndef ESCPPLIB_EVENT_H
#define ESCPPLIB_EVENT_H

#include <stdint.h>
#include <string>
#include <fcntl.h>
#include <sys/epoll.h>

#include "log.h"

namespace escpplib{

//class CEvent
//{
//public:
//    CEvent();
//    ~CEvent();
//
//    virtual void Wait();
//    virtual void EventAdd();
//    virtual void EventDel();
//private:
//
//};


extern int SetNonblock(int fd); 

class CEventEpoll
{
public:
    CEventEpoll(int event_size=1024);
    ~CEventEpoll();

    int32_t RegEvent(int fd, uint32_t events);
    int32_t RegEvent(void* obj, int fd, uint32_t events);

    int32_t RmEvent(int fd);
    int32_t ResetEvent();

    int32_t Wait(int timeout);

public:
    struct  epoll_event *epoll_event_;
private:
    int     epoll_fd_;

    int     event_size_;
};



}// namespace escpplib

#endif

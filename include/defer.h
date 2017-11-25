#ifndef ESCPPLIB_DEFER_H
#define ESCPPLIB_DEFER_H

#include <stdio.h>
#include <unistd.h>

namespace escpplib{

class CDeferClose
{
public:
    enum TYPE{
        POSIX=0,
        ISO_C,
    };
    CDeferClose(int fd);
    CDeferClose(FILE* file);
    ~CDeferClose() ;
private:
    CDeferClose();

    int     type_;
    int     fd_;
    FILE*   file_;

};





}// namespace escpplib


#endif

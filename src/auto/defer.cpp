#include "defer.h"

namespace escpplib{


CDeferClose::CDeferClose(int fd):fd_(fd) { 
    type_ = POSIX;
}

CDeferClose::CDeferClose(FILE* file):file_(file) {
    type_ = ISO_C;
}

CDeferClose::~CDeferClose() {
    switch(type_){
        case POSIX: close(fd_); break;
        case ISO_C: fclose(file_); break;
    }
}


}// namespace escpplib

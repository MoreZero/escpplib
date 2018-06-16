#include <string.h>
#include "proto_buff.h"


namespace escpplib{

using namespace std::placeholders;

ProtoBuff::ProtoBuff():buff_size_(0) {
    proto_head_parse_func_ = std::bind(&ProtoBuff::DefaultHeadParseFunc, this, _1, _2, _3, _4, _5);
}

void ProtoBuff::RegisterFunc(int32_t cmd, ProtoHandleFunc handle_func) {
    handle_func_map_[cmd] = handle_func;
}

int32_t ProtoBuff::Write(const int8_t* buf, int32_t size) {
    if (65536 - buff_size_ < size) {
        return -1;
    }
    memcpy(buff_+buff_size_, buf, size);
    buff_size_ += size;

    // TODO memmove可以放循环外进行优化
    do {
        int32_t cmd=0, head_size=0, body_size=0;
        int32_t ret = proto_head_parse_func_(buff_, buff_size_, cmd, head_size, body_size);
        if (ret == 0) return 0;
        if (ret == -1) return -2;

        auto iter = handle_func_map_.find(cmd);
        if (iter == handle_func_map_.end()) {
            return -3;
        }

        ret = iter->second(buff_, head_size, buff_+head_size, body_size);
        int32_t surplus_size = buff_size_ - head_size - body_size;
        if (surplus_size == 0) {
            buff_size_ = 0;
            return 0;
        }
        memmove(buff_, buff_+head_size+body_size, surplus_size);
        buff_size_ = surplus_size;
    }while(1);
    return 0;
}


int32_t ProtoBuff::DefaultHeadParseFunc(const int8_t* buff, int32_t size, 
                                        int32_t& cmd, int32_t& head_size, int32_t& body_size) {
    if (size < (int32_t)sizeof(default_proto_head_t)) return 0;
    
    default_proto_head_t* head = (default_proto_head_t*)buff;
    head_size = head->head_size;
    body_size = head->body_size;
    if (size >= (int32_t)sizeof(default_proto_head_t) + body_size) {
        cmd = head->cmd;
        return 1;
    }
    return 0;
}

void ProtoBuff::SetProtoHeadParseFunc(ProtoHeadParseFunc proto_head_parse_func) {
    proto_head_parse_func_ = proto_head_parse_func;
}


}// namespace escpplib

#ifndef ESCPPLIB_PROTO_BUFF_H
#define ESCPPLIB_PROTO_BUFF_H

#include <functional>
#include <string>
#include <map>


namespace escpplib{

using namespace std;

typedef function<int32_t(const int8_t*, int32_t, const int8_t*, int32_t)> ProtoHandleFunc;
typedef function<int32_t(const int8_t*, int32_t, int32_t&, int32_t&, int32_t&)> ProtoHeadParseFunc;


typedef struct {
    int32_t head_size;
    int32_t body_size;
    int32_t cmd;
} default_proto_head_t;

class ProtoBuff
{
public:
    ProtoBuff();
   
    void RegisterFunc(int32_t cmd, ProtoHandleFunc handle_func);
    void SetProtoHeadParseFunc(ProtoHeadParseFunc proto_head_parse_func);
    
    int32_t Write(const int8_t* buf, int32_t size);
    int32_t DefaultHeadParseFunc(const int8_t* buff, int32_t size, 
                                 int32_t& cmd, int32_t& head_size, int32_t& body_size);

private:

private:

    int8_t buff_[65536];
    int32_t buff_size_;

    map<int32_t, ProtoHandleFunc> handle_func_map_;
    ProtoHeadParseFunc proto_head_parse_func_;
};


}// namespace escpplib



#endif

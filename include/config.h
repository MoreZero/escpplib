#ifndef ESCPPLIB_CONFIG_H
#define ESCPPLIB_CONFIG_H

#include <stdint.h>
#include <map>
#include <string>

namespace escpplib{



class CConfig
{
public:

    typedef std::map<std::string, std::string> MapValue;
    typedef std::map<std::string, MapValue> MapSection;


    CConfig();
    ~CConfig();
    int32_t ReadConfig(std::string config_path);


    std::string Get(std::string section, std::string key, std::string default_value);
    uint32_t Get(std::string section, std::string key, uint32_t default_value);
    int32_t Get(std::string section, std::string key, int32_t default_value);
    uint64_t Get(std::string section, std::string key, uint64_t default_value);
    int64_t Get(std::string section, std::string key, int64_t default_value);


private:
    MapSection conf_map_;
    bool is_init_;

};





}// namespace escpplib


#endif

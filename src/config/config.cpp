#include <fstream> 
#include <stdlib.h> 
#include <string> 
#include <vector> 
#include "config.h"



namespace escpplib{



CConfig::CConfig() {

}

CConfig::~CConfig() {

}


int32_t CConfig::ReadConfig(std::string config_path) {
  	std::ifstream ifs;

  	ifs.open (config_path, std::ifstream::in);

	if (!ifs) {
		return -1;
	}

    std::vector<std::string> lines;
  	while (ifs.good()) {
		std::string line;
		std::getline(ifs,line);
		lines.push_back(line);
  	}
  	ifs.close();

	std::string section;
	for (uint32_t i=0; i<lines.size(); ++i) {
		std::string& line = lines[i];
		if (line[0] == '#' || line.empty()) continue; //注释或空行

		if (line[0] == '[') {
			size_t index = line.find(']');
			if (index > 0) {
				section = line.substr(1, index -1);
				continue;
			}
		}
		
		if (!section.empty()){
			size_t index = line.find('=');
			std::string key = line.substr(0,index);
			std::string value = line.substr(index+1);
			conf_map_[section][key] = value;
		}
	}

    return 0;
}


std::string CConfig::Get(std::string section, std::string key, std::string default_value) {
	auto iter = conf_map_.find(section);
	if (iter == conf_map_.end())
		return default_value;
	
	auto value_iter = iter->second.find(key);
	if (value_iter == iter->second.end()) 
		return default_value;

	return value_iter->second;
}

uint32_t CConfig::Get(std::string section, std::string key, uint32_t default_value) {
	auto iter = conf_map_.find(section);
	if (iter == conf_map_.end())
		return default_value;
	
	auto value_iter = iter->second.find(key);
	if (value_iter == iter->second.end()) 
		return default_value;

	return strtoul(value_iter->second.c_str(), NULL, 10);
}


int32_t CConfig::Get(std::string section, std::string key, int32_t default_value) {
	auto iter = conf_map_.find(section);
	if (iter == conf_map_.end())
		return default_value;
	
	auto value_iter = iter->second.find(key);
	if (value_iter == iter->second.end()) 
		return default_value;

	return strtol(value_iter->second.c_str(), NULL, 10);
}

uint64_t CConfig::Get(std::string section, std::string key, uint64_t default_value) {
	auto iter = conf_map_.find(section);
	if (iter == conf_map_.end())
		return default_value;
	
	auto value_iter = iter->second.find(key);
	if (value_iter == iter->second.end()) 
		return default_value;

	return strtoull(value_iter->second.c_str(), NULL, 10);
}

int64_t CConfig::Get(std::string section, std::string key, int64_t default_value) {
	auto iter = conf_map_.find(section);
	if (iter == conf_map_.end())
		return default_value;
	
	auto value_iter = iter->second.find(key);
	if (value_iter == iter->second.end()) 
		return default_value;

	return strtoll(value_iter->second.c_str(), NULL, 10);
}



}// namespace escpplib










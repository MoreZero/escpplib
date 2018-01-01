
#包含的头文件
INCLUDE_DIRECTORIES(../include)

# 需要编译的源文件目录
AUX_SOURCE_DIRECTORY(./log SRC_LIST)
AUX_SOURCE_DIRECTORY(./timer SRC_LIST)
AUX_SOURCE_DIRECTORY(./auto SRC_LIST)
AUX_SOURCE_DIRECTORY(./config SRC_LIST)
AUX_SOURCE_DIRECTORY(./srv/socket SRC_LIST)
AUX_SOURCE_DIRECTORY(./srv/event SRC_LIST)
AUX_SOURCE_DIRECTORY(./srv/simple_server SRC_LIST)
message("SRC_LIST src:${SRC_LIST}")

#设置编译参数
SET(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall ")
message("==> CXX flags: ${CMAKE_CXX_FLAGS}")

# 添加静态库  
ADD_LIBRARY(escpplib STATIC ${SRC_LIST})  

# 安装头文件路径
INSTALL(DIRECTORY ../include/ 
    DESTINATION /usr/local/include/escpplib PATTERN 
    "*.h")  

#AUX_SOURCE_DIRECTORY(../include/log INCLUDE_FILE)
#AUX_SOURCE_DIRECTORY(../include/timer INCLUDE_FILE)
#message("include file src:${INCLUDE_FILE}")


#AUX_SOURCE_DIRECTORY(${${PROJECT_NAME}_SOURCE_DIR}/src/timer SRC_LIST)
#AUX_SOURCE_DIRECTORY(${${PROJECT_NAME}_SOURCE_DIR}/src/log SRC_LIST)


##添加动态库  
#ADD_LIBRARY(escpplib SHARED ${SRC_LIST})  
#  
##生成动态库的版本号  
#SET_TARGET_PROPERTIES(hello PROPERTIES VERSION 1.2 SOVERSION 1)  
##将静态库重新命名为hello  
#SET_TARGET_PROPERTIES(hello_static PROPERTIES OUTPUT_NAME "hello")  
#  
##安装静态库和动态库  
#INSTALL(TARGETS hello   hello_static   
#        LIBRARY DESTINATION lib   
#            ARCHIVE DESTINATION lib)  
##安装hello.h  
#INSTALL(FILES hello.h DESTINATION include/hello)  



#include <iostream>
#include <vector>
#include <pthread.h>
#include "log.h"


using namespace std;




void * Worker(void *arg) {
    int id = *((int*)arg);

    for (int i=0; i<1000000; i++){
        //LOG_INFO("thread id:%d worker, cnt:%d", id, i);
        LOG_INFO("hfdsfjdkfjkdjfffffffffffffffffffffffffffffffffffffffffffx");
    }

    return NULL;
}


int main(){
    LOG_INFO("test");
    LOG_INFO("test %d", 1);
    LOG_ERROR("test %d", 1);
    LOG_ERROR_MSG("test %d", 1);
    LOG_DEBUG("test %d", 1);
    //LOG_FATAL("test %d", 1);

    int thread_num_ = 10;
    std::vector<pthread_t> pthread_vec_;
    std::vector<int> pthread_id_vec_;
    pthread_vec_.resize(thread_num_);
    pthread_id_vec_.resize(thread_num_);

    for( int i=0; i<thread_num_; i++) {
        pthread_id_vec_[i]=i;
        if ( pthread_create( &pthread_vec_[i], NULL, Worker, (void*)&pthread_id_vec_[i]) != 0) {
            LOG_ERROR_MSG("pthread_create error");
        }
    }


    for ( int i=0; i<thread_num_; i++ ) {
        int ret = pthread_join(pthread_vec_[i], NULL);
        if (ret != 0) {
            LOG_ERROR_MSG("pthread_join");
        }
        LOG_INFO("join a thread:%d", i);
    }

    return 0;
}



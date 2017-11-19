#ifndef SEQ_THREAD_PROCESSOR_H
#define SEQ_THREAD_PROCESSOR_H

#include <stdio.h>
#include <stdint.h>
#include <list>
#include <vector>
#include <semaphore.h>
#include <utility>
#include <pthread.h>


template< typename T>
class SeqThreadProcessor
{
public:
    enum TASK_STATUS{
        TASK_WAITING=0,
        TASK_DOING,
        TASK_COMPLETE
    };
    SeqThreadProcessor();
    ~SeqThreadProcessor();
    void SetThreadNum(int32_t thread_num) { thread_num_ = thread_num;};
    int32_t Start();
    int32_t PushTask(T * task);
    T * GetCompleteTask();

private:
    static void * _Worker(void *arg);
    void _Run(void *arg);
    void _Recycle(int32_t num);
    int32_t _InitSemAndMutex();
    void _DestroySemAndMutex();


private:
    int32_t thread_num_;
    int32_t complete_num_;
//    int32_t complete_index_;
//    int32_t need_handle_index_;

    typename std::list< std::pair<T*,int> >::iterator wait_complete_iter_;  
    typename std::list< std::pair<T*,int> >::iterator need_handle_iter_;  

    typename std::list< std::pair<T*,int> > task_list_;
    std::vector<pthread_t> pthread_vec_;
    bool stop_;
    bool is_start_;

    sem_t complete_sem_;
    sem_t consume_sem_;
    pthread_mutex_t consume_mutex_;
};



template< typename T>
SeqThreadProcessor<T>::SeqThreadProcessor()
    :thread_num_(4), complete_num_(0), stop_(false), is_start_(false){
    wait_complete_iter_ = task_list_.begin();    
    need_handle_iter_ = task_list_.begin();    
}

template< typename T>
SeqThreadProcessor<T>::~SeqThreadProcessor() {
    fprintf(stderr, "~SeqThreadProcessor\n");
    if (is_start_) {
        _DestroySemAndMutex();
        _Recycle(thread_num_);
    }
    fprintf(stderr, "finish ~SeqThreadProcessor\n");

}

template< typename T>
int32_t SeqThreadProcessor<T>::_InitSemAndMutex() {
    if ( sem_init(&complete_sem_, 0, 0) != 0 ) {
        return -1;
    }

    if ( sem_init(&consume_sem_, 0, 0) != 0 ) {
        sem_destroy(&complete_sem_);
        return -1;
    }
    
    if ( pthread_mutex_init(&consume_mutex_, NULL) != 0 ) {
        sem_destroy(&complete_sem_);
        sem_destroy(&consume_sem_);
        return -1;
    }
    return 0;
}

template< typename T>
void SeqThreadProcessor<T>::_DestroySemAndMutex() {
    int ret = sem_destroy( &complete_sem_);
    if (ret != 0)
        printf("err\n");
    ret = sem_destroy( &consume_sem_);
    if (ret != 0)
        printf("err\n");
    ret = pthread_mutex_destroy( &consume_mutex_);
    if (ret != 0)
        printf("err\n");
}

template< typename T>
int32_t SeqThreadProcessor<T>::Start() {
    if (is_start_) return 0;

    if ( _InitSemAndMutex() != 0 ) {
        return -1;
    }
    
    pthread_vec_.resize(thread_num_);

    for( int i=0; i<thread_num_; i++) {
        if ( pthread_create( &pthread_vec_[i], NULL, _Worker, this) != 0) {
            stop_ = true;
            _Recycle(i);
            _DestroySemAndMutex();
            return -1; 
        }
    }

    is_start_ = true;
    return 0;
}


template< typename T>
void SeqThreadProcessor<T>::_Recycle(int32_t num) {
    for ( int i=0; i<num; i++ ) {
        int ret = pthread_cancel(pthread_vec_[i]);
        if (ret != 0) {
            printf("cancel pthread error \n");
        }
        printf("cancel pthread \n");
    }
    for ( int i=0; i<num; i++ ) {
        int ret = pthread_join(pthread_vec_[i], NULL);
        if (ret != 0) {
            printf("pthread join error \n");
        }
        printf("pthread join \n");
    }
}

template< typename T>
void * SeqThreadProcessor<T>::_Worker(void * arg) {
    SeqThreadProcessor * obj = (SeqThreadProcessor *)arg;
    obj->_Run(NULL);
    return NULL;
}


template< typename T>
void SeqThreadProcessor<T>::_Run(void *arg) {
    int ret;
    while( !stop_ ) {
        ret = sem_wait( &consume_sem_ );
        if (ret != 0){
            printf("sem_wait error\n");
            return;
        }
        
        //从need_handle_iter_ 获取任务
        pthread_mutex_lock( &consume_mutex_);
        if (need_handle_iter_ == task_list_.end()) {
            pthread_mutex_unlock( &consume_mutex_);
            continue;
        }

        typename std::list< std::pair<T*,int> >::iterator iter = need_handle_iter_++;
        iter->second = TASK_DOING;
        
        pthread_mutex_unlock( &consume_mutex_);
      
        if (iter->first != NULL)
            iter->first->STP_Handle();
         
        //任务完成时，对比need_handle_iter_ == wait_complete_iter_
        //如相等，wait_complete_iter_移动到下一个状态为TASK_DOING或队尾；
        pthread_mutex_lock( &consume_mutex_);
        iter->second = TASK_COMPLETE;
        if (iter == wait_complete_iter_ ) {
            sem_post(&complete_sem_);
            ++wait_complete_iter_;
            for (; wait_complete_iter_ != task_list_.end(); ++wait_complete_iter_) {
                if ( wait_complete_iter_->second == TASK_DOING ||  wait_complete_iter_->second == TASK_WAITING) {
                    break;
                }
                sem_post(&complete_sem_);
            }
        }
        pthread_mutex_unlock( &consume_mutex_);
    }
}


template< typename T>
int32_t SeqThreadProcessor<T>::PushTask(T * task) {
    pthread_mutex_lock( &consume_mutex_);
    task_list_.push_back( std::make_pair(task, TASK_WAITING) );
    auto iter_end = task_list_.end();
    if (need_handle_iter_ == iter_end) {
        need_handle_iter_ = --iter_end;
    }
    if ( wait_complete_iter_ == task_list_.end()) {
        wait_complete_iter_ = iter_end; 
    }
    sem_post(&consume_sem_);
    pthread_mutex_unlock( &consume_mutex_);
    return 0;
}

template< typename T>
T * SeqThreadProcessor<T>::GetCompleteTask() {
    
    int ret = sem_wait( &complete_sem_ );
    if (ret != 0){
        printf("sem_wait error\n");
        return NULL;
    }
    
    pthread_mutex_lock( &consume_mutex_);
    typename std::pair<T*,int> task_pair = task_list_.front();
    task_list_.pop_front();
    pthread_mutex_unlock( &consume_mutex_);
    return task_pair.first;
}




#endif

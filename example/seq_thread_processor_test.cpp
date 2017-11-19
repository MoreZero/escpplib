#include "seq_thread_processor.h"

#include <iostream>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

class test
{
public:
    void set(int a){this->a = a;}
    void STP_Handle() {
        //printf("sleep 10 %ld\n", time(NULL));
        //sleep(10);
        usleep(10);
        fprintf(stderr,"handle end %ld  %d  %ld\n", time(NULL), a, (int64_t)this);
    }

    int a;


};


int main()
{
    SeqThreadProcessor<test> stp;
    stp.SetThreadNum(10);
    stp.Start();

    test a[100000];
    for(int i=0; i<100000; i++) {
        a[i].set(i);
        stp.PushTask(&a[i]);
    }
    stp.PushTask(NULL);


    while(1) {
        test * p = stp.GetCompleteTask(); 
        if (p==NULL) {
            break;
        }
        fprintf(stderr,"get one a:%d %ld\n", p->a,time(NULL));
    }
    fprintf(stderr,"finish %ld\n", time(NULL));
    return 0;
}

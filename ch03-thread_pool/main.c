#include <stdio.h>
#include <unistd.h>
#include "threadpool.h"


void task(void* arg){
    printf("No.%d Task is Running...\n", *((int*)arg));
}

int main(){
    ThreadPool* tp = createThreadPool(10, 2, 5);
    int count = 1;
    Task one_task = {task, (void*)&count};
    while(1){
	if (InQueue(tp->taskq, one_task)){
	    pthread_mutex_lock(&tp->mutexPool);
	    pthread_cond_wait(&tp->notFull, &tp->mutexPool);
	}else{
	    count++;
	    sleep(2);
	}
    }
    return 0;
}

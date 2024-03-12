#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "threadpool.h"

// #define DEBUG

TaskQueue* createQueue(int capacity){
    TaskQueue* taskq = (TaskQueue*)malloc(sizeof(TaskQueue));
    taskq->task_queue = (Task*)calloc(capacity, sizeof(Task));
    taskq->queueCapacity = capacity;
    taskq->tasksInQueue = 0;
    taskq->queueFront = 0;
    taskq->queueRear = 0;

    return taskq;
}

ThreadPool* createThreadPool(int capacity, int min, int max){
    TaskQueue* taskq = createQueue(capacity);
    ThreadPool* threadp = (ThreadPool*)malloc(sizeof(ThreadPool));
    threadp->taskq = taskq;
#ifdef DEBUG
    printf("[main] Task Queue created, address: %p\n", threadp->taskq);
#endif
    threadp->minNum = min;
    threadp->maxNum = max;
    threadp->busyNum = 0;
    //threadp->mutexPool = PTHREAD_MUTEX_INITIALIZER;
    //threadp->notFull = PTHREAD_COND_INITIALIZER;
    pthread_mutex_init(&threadp->mutexPool, NULL);
    pthread_cond_init(&threadp->notFull, NULL);
    pthread_create(&threadp->managerID, NULL, managerThread, NULL);
    threadp->workerIDs = (pthread_t*)calloc(threadp->maxNum, sizeof(pthread_t));
    for(int i = 0; i < threadp->maxNum; i++){
        pthread_create(threadp->workerIDs+i, NULL, workerThread, (void*)threadp);
    }

    return threadp;
}

int InQueue(TaskQueue* taskq, Task task){
    if (taskq->tasksInQueue == taskq->queueCapacity){
	return 1;
    }
    *(taskq->task_queue+taskq->queueRear) = task;
    taskq->queueRear++;
    if (taskq->queueRear == taskq->queueCapacity){
        taskq->queueRear = 0;
    }
    taskq->tasksInQueue++;
    return 0;
    
}

Task* DeQueue(TaskQueue* taskq){
    if (taskq->tasksInQueue == 0){
	return NULL;
    }
    Task* task =  taskq->task_queue+taskq->queueFront;
    taskq->queueFront++;
    if (taskq->queueFront == taskq->queueCapacity){
        taskq->queueFront = 0;
    }
    taskq->tasksInQueue--;
#ifdef DEBUG
    printf("Task %p left Queue\n", task);
#endif
    return task;
}

void* workerThread(void* arg){
    ThreadPool* tp = (ThreadPool*)arg;
    int tid = (int)pthread_self();
    Task* one_task = NULL;
    printf("[%X] Worker thread created!\n", tid);
#ifdef DEBUG
    printf("[%X] Thread pool address: %p, task queue address: %p\n", tp, tp->taskq);
#endif
    while(1){
	if(pthread_mutex_trylock(&tp->mutexPool)){
	    printf("[%X] Thread Pool has locked! Sleep!\n", tid);
	    sleep(5);
	    continue;
	}
        //if(one_task = DeQueue(tp->taskq)){
	one_task = DeQueue(tp->taskq);
#ifdef DEBUG
	printf("[%X] one_task = %p\n", tid, one_task);
#endif
	if(!one_task){
	    printf("[%X] No Task in Pool!\n", tid);
	    pthread_mutex_unlock(&tp->mutexPool);
	    sleep(2);
	} else {
	    printf("[%X] Get Task from Pool!\n", tid);
	    pthread_mutex_unlock(&tp->mutexPool);
	    pthread_cond_signal(&tp->notFull);
	    one_task->func(one_task->arg);
#ifdef DEBUG
	    printf("[%X] Task Func: %p, Task Args: %p\n", tid, one_task, one_task);
#endif
	    sleep(1);
	}
    }
}

void* managerThread(void* arg){
}

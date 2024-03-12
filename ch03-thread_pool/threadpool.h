#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Task{
    void (*func)(void* arg);
    void* arg;
}Task;

typedef struct TaskQueue{
    Task* task_queue;
    int queueCapacity;
    int tasksInQueue;
    int queueFront;
    int queueRear;
}TaskQueue;

typedef struct ThreadPool{
    TaskQueue* taskq;
    int minNum;
    int maxNum;
    int busyNum;
    pthread_t managerID;
    pthread_t* workerIDs;
    pthread_mutex_t mutexPool;
    pthread_cond_t notFull;
}ThreadPool;

extern TaskQueue* createQueue(int);
extern ThreadPool* createThreadPool(int, int, int);
extern void* workerThread(void*);
extern void* managerThread(void*);
extern int InQueue(TaskQueue*, Task);
extern Task* DeQueue(TaskQueue*);

#endif

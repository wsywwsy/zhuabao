#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef struct{
    void (*zhixing)(void *arg); //任务进行执行用的函数
    void *arg;  //传递给任务函数的参数
} renwu;

typedef struct{
    pthread_t xczu; //工作线程数组
    renwu *rwlie; //任务队列
    int rwlie_daxiao; //任务列大小
    int tou,wei; //任务列头尾
    int 
} xcchi;
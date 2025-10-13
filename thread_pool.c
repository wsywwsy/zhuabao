#include <pthread.h>  // 线程相关函数和数据类型
#include <stdlib.h>   // 内存分配、退出等标准库函数
#include <stdio.h>    // 输入输出函数
#include <unistd.h>   // 系统调用，如sleep等

typedef struct{
    void (*zhixing)(void *arg); //任务进行执行用的函数
    void *arg;  //传递给任务函数的参数
} renwu;

typedef struct{
    pthread_t xczu; //工作线程数组
    renwu *rwlie; //任务队列
    int rwlie_daxiao; //任务列大小
    int tou,wei; //任务列头尾
    int xcshu; //线程数
    pthread_mutex_t suo; //锁，保护任务队列
    pthread_cond_t cond; //条件变量唤醒工作线程
    int shutdown; //是否关闭线程池
} xcchi;

void *gongzuo(void *arg){
    xcchi *pool = (xcchi *)arg; //arg实际上是一个线程管理器，这步是告诉*pool使用在线程管理器被创建时候带有数值的xcchi结构体
    while(1){
        pthread_mutex_lock(&pool->suo); //上锁保护临界资源
        while(pool->tou == pool->wei && !pool->shutdown) //检查队列是否为空和线程池是否关闭
        {
            pthread_cond_wait(&pool->cond,&pool->suo); //等待任务
        }
        //检查线程池有没有关闭
        if (pool->shutdown)
        {
        pthread_mutex_unlock(&pool->suo); //释放锁
        break;
        }
        renwu task = pool->rwlie[pool->tou]; //获取任务
        pool->tou = (pool->tou+1) & pool->rwlie_daxiao; //移动头指针
        pthreand_mutex_unlock(&pool->suo); //释放锁
        task.zhixing(task.arg); //执行任务
    }
    pthread_exit(NULL);
}
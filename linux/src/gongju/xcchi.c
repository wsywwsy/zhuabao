#include <pthread.h>  // 线程相关函数和数据类型
#include <stdlib.h>   // 内存分配、退出等标准库函数
#include <stdio.h>    // 输入输出函数
#include <unistd.h>   // 系统调用，如sleep等

typedef struct{
    void (*zhixing)(void *arg); //任务进行执行用的函数
    void *arg;  //传递给任务函数的参数
} renwu;

typedef struct{
    pthread_t *xczu; //工作线程数组
    renwu *rwlie; //任务队列
    int rwlie_daxiao; //任务列大小
    int tou,wei; //任务列头尾
    int xcshu; //线程数
    pthread_mutex_t suo; //锁，保护任务队列
    pthread_cond_t cond; //条件变量唤醒工作线程
    int shutdown; //是否关闭线程池
} xcchi;

//任务调度功能
void *gongzuo(void *arg){
    xcchi *pool = (xcchi *)arg; //arg实际上是一个线程管理器，这步是告诉*pool使用在线程管理器被创建时候带有数值的xcchi结构体
                                //理解：创建了一个叫gongzuo的线程函数，他的参数是一个void类型的指针叫arg，这个指针实际上指向一个xcchi结构体，
                                //所以我们把这个void指针强制转换成xcchi类型的指针，然后赋值给pool变量，这样我们就可以通过pool变量访问线程池的各种属性和方法了
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
        pthread_mutex_unlock(&pool->suo); //释放锁
        task.zhixing(task.arg); //执行任务
    }
    pthread_exit(NULL); //显示退出线程（还有隐式
}

//线程池初始化，pool是线程池管理器
void xcchi_chushi(xcchi *pool,int xcshu,int rwlie_daxiao)
{
    pool->xczu = (pthread_t *)malloc(xcshu * sizeof(pthread_t)); //为工作线程id数组分配内存，他现在指向一个存储线程id的数组，malloc：动态分配内存，(xcshu * sizeof(pthreand_t))自动计算内存大小，xczu里面存放的是各个线程在内存当中的id
    pool->rwlie = (renwu *)malloc(rwlie_daxiao * sizeof(renwu)); //为任务队列分配内存，这里使用的是我自己写的renwu结构体，所以强制转换成renwu *
    pool->rwlie_daxiao = rwlie_daxiao; //记录任务队列容量
    pool->tou = pool->wei =0; //初始化队列的头尾指针
    pool->xcshu = xcshu; //设置线程数量
    pool->shutdown = 0; //设置线程池状态，0开1关
    pthread_mutex_init(&pool->suo,NULL); //初始化线程锁
    pthread_cond_init(&pool->cond,NULL); //初始化线程间通信的条件变量
    for(int i = 0; i <xcshu; i++)
    {
        pthread_create(&pool->xczu[i],NULL,gongzuo,pool); //把新创建的线程id存储到xczhu[i]中
    }
}

void xcchi_guanbi(xcchi *pool)
{
    pthread_mutex_lock(&pool->suo); //给临界资源上锁
    pool->shutdown=1; //设置关闭标志
    pthread_cond_broadcast(&pool->cond); //唤醒所有等待进程
    pthread_mutex_unlock(&pool->suo); //解锁
    //等待所有线程退出
    for(int i = 0; i < pool->xcshu ; i++)
    {
        pthread_join(pool->xczu[i],NULL); 
    }
    //释放动态内存
    free(pool->xczu);
    free(pool->rwlie);
    //销毁同步工具
    pthread_mutex_destroy(&pool->suo);
    pthread_mutex_destroy(&pool->cond);
}
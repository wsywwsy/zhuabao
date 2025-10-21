#include "peizhi.h" //包含配置头文件
#include <stdio.h>  //标准输入输出
#include <stdlib.h> //用于atoi等函数
#include <string.h> //用于字符串处理
#include <stdbool.h> //用于布尔类型

//定义配置结构体
typedef struct{
    char wk[32]; //网卡接口名称，32字节的大小
    char glbao[128]; //包过滤表达式如tcp port 80，128字节的大小
    int hcq_daxiao; //缓冲区大小决定一次能缓冲多少包
    int zuida_bao; //最大包数量决定能捕获多少包
    int timeout; //超时时间，单位毫秒,防止无限运行
    char wjm_shuchu[128]; //输出文件名,抓包结果保存地址
    bool kzt_xianshi; //是否在控制台显示
    char rz_jibie[16];  //日志级别
    char rz_ming[128]; //日志文件名
}peizhi_t;

//定义全局变量
peizhi_t g_peizhi; //全局配置变量

//初始化配置
void peizhi_chushi(){
    //设置默认的网络配置
    strcpy(g_peizhi.wk, "eth0"); //默认网卡为eth0
    strcpy(g_peizhi.glbao, ""); //默认不过滤任何包

    //设置默认的抓包配置
    g_peizhi.hcq_daxiao = 1024 * 1024; //1MB缓冲区，平衡性能和内存使用
    g_peizhi.zuida_bao = 1000; //最大抓1000个包
    g_peizhi.timeout =  60; //默认60秒超时
    
    //设置默认的输出配置
    strcpy(g_peizhi.wjm_shuchu, "capture.pcap"); //默认输出文件名
    g_peizhi.kzt_xianshi = true; //默认在控制台显示抓包信息

    //设置默认的日志配置
    strcpy(g_peizhi.rz_jibie, "INFO"); //默认日志级别为只显示重要信息
    strcpy(g_peizhi.rz_ming, "sniffer.log"); //默认日志文件名
}

//从配置文件加载配置
bool peizhi_jiazai(const char* wjm_peizhi){
    FILE* file = fopen(wjm_peizhi, "r");
    if (file == NULL){
        return false; //无法打开文件
    }

    char line[256]; //缓冲区，用于存储读取的每一行配置
    while (fgets(line,sizeof(line),file != NULL)){
        
    }
}
#include "peizhi.h" //包含配置头文件
#include <stdio.h>  //标准输入输出
#include <stdlib.h> //用于atoi等函数
#include <string.h> //用于字符串处理
#include <stdbool.h> //用于布尔类型
#include <time.h> //添加时间头文件
//定义配置结构体
typedef struct{
    char wk[32]; //网卡接口名称，32字节的大小
    char gl_tiaojian[128]; //包过滤表达式如tcp port 80，128字节的大小
    int hcq_daxiao; //缓冲区大小决定一次能缓冲多少包
    int zd_bao; //最大包数量决定能捕获多少包
    int chaoshi; //超时时间，单位毫秒,防止无限运行
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
    strcpy(g_peizhi.gl_tiaojian, ""); //默认不过滤任何包

    //设置默认的抓包配置
    g_peizhi.hcq_daxiao = 1024 * 1024; //1MB缓冲区，平衡性能和内存使用
    g_peizhi.zd_bao = 1000; //最大抓1000个包
    g_peizhi.chaoshi =  60; //默认60秒超时
    
    //获取当前时间用于日志文件命名
    time_t now = time(NULL); //获取当前时间
    struct tm* xz = localtime(&now); //转换为本地时间结构  

    //生成格式
    char sj[32];
    strftime(sj,sizeof(sj),"%Y-%m-%d-%H-%M-%S",xz); //格式化时间字符串
    snprintf(g_peizhi.wjm_shuchu,sizeof(g_peizhi.wjm_shuchu),"%s.pcap",sj); //使用时间作为文件名

    //设置默认的输出配置
    // strcpy(g_peizhi.wjm_shuchu, "capture.pcap"); //默认输出文件名
    g_peizhi.kzt_xianshi = true; //默认在控制台显示抓包信息

    //设置默认的日志配置
    strcpy(g_peizhi.rz_jibie, "INFO"); //默认日志级别为只显示重要信息
    strcpy(g_peizhi.rz_ming, "sniffer.log"); //默认日志文件名
}

//从配置文件加载配置
bool peizhi_jiazai(const char* wjm_peizhi){
    FILE* wj = fopen(wjm_peizhi, "r");
    if (wj == NULL){
        return false; //无法打开文件
    }

    char duqu[256]; //缓冲区，用于存储读取的每一行配置
    while (fgets(duqu,sizeof(duqu),wj != NULL)){
        //跳过空行
        if(duqu[0] == '\n' || duqu[0] == '#'){
            continue; //跳过注释行
        }

        char key[64]; //用于存储配置键
        char value[128]; //用于存储配置值

        //解析键值对
        if(sscanf(duqu, "%63[^=]=%127[^\n]",key,value) == 2){
            //去除value前后的空白空格
            char* cun_value =value;
            while (*cun_value == ' ') cun_value++;

            //根据key设置对应的配置值
            //对比key与变量名是否相等，相等就写入值
            if (strcmp(key,"wk") == 0){
                strcpy(g_peizhi.wk,cun_value);
            }else if(strcmp(key,"gl_tiaojian") == 0){
                strcpy(g_peizhi.gl_tiaojian,cun_value);
            }else if(strcmp(key,"hcq_daxiao") == 0){
                strcpy(g_peizhi.hcq_daxiao,cun_value);
            }else if(strcmp(key,"zuida_bao") == 0){
                strcpy(g_peizhi.zd_bao,cun_value);
            }else if(strcmp(key,"chaoshi") == 0){
                strcpy(g_peizhi.chaoshi,cun_value);
            }else if(strcmp(key,"wjm_shuchu") == 0){
                strcpy(g_peizhi.wjm_shuchu,cun_value);
            }else if(strcmp(key,"kzt_xianshi") == 0){
                strcpy(g_peizhi.kzt_xianshi,cun_value);
            }else if(strcmp(key,"rz_jibie") == 0){
                strcpy(g_peizhi.rz_jibie,cun_value);
            }else if(strcmp(key,"rz_ming") == 0){
                strcpy(g_peizhi.rz_ming,cun_value);
            }
        }
    }
    fclose(wj); //关闭文件
    return true; //返回成功
}

//在控制台显示当前配置
void peizhi_xianshi(){
    printf("网卡接口:  %s\n",g_peizhi.wk);
    printf("过滤条件： %s\n",g_peizhi.gl_tiaojian);
    printf("缓冲区大小： %d\n",g_peizhi.hcq_daxiao);
    printf("最大包数量： %d\n",g_peizhi.zd_bao);
    printf("超时时间： %d 秒\n",g_peizhi.chaoshi);
    printf("输出文件名 %s\n",g_peizhi.wjm_shuchu);
    printf("控制台输出 %s\n",g_peizhi.kzt_xianshi ? "是" : "否");
    printf("日志级别： %s\n",g_peizhi.rz_jibie);
    printf("日志文件名： %s\n",g_peizhi.rz_ming);
}

//验证配置有效性
bool peizhi_yanzheng(){
    if(g_peizhi.hcq_daxiao <=0){
        printf("错误：缓冲区大小必须大于0\n");
        return false;
    }
    if(g_peizhi.zd_bao <=0){
        printf("错误：最大包数量必须大于0\n");
        return false;
    }
    if(g_peizhi.chaoshi <=0){
        printf("错误：超时时间必须大于0\n");
        return false;
    }
    return true; //配置有效
}

//将所有参数使用接口的方式传递降低耦合性
const char* peizhi_huoqu_wk(){
    return g_peizhi.wk;
}

const char* peizhi_huoqu_gl_tiaojian(){
    return g_peizhi.gl_tiaojian;
}

int peizhi_huoqu_hcq_daxiao(){
    return g_peizhi.hcq_daxiao;
}

int peizhi_huoqu_zuida_bao(){
    return g_peizhi.zd_bao;
}

int peizhi_huoqu_chaoshi(){
    return g_peizhi.chaoshi;
}

const char* peizhi_huoqu_wjm_shuchu(){
    return g_peizhi.wjm_shuchu;
}

bool peizhi_huoqu_kzt_xianshi(){
    return g_peizhi.kzt_xianshi;
}

const char* peizhi_huoqu_rz_jibie(){
    return g_peizhi.rz_jibie;
}

const char* peizhi_huoqu_rz_ming(){
    return g_peizhi.rz_ming;
}



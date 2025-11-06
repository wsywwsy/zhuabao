#ifndef peizhi_h
#define peizhi_h

#include <stdbool.h> //用于布尔类型
//结构体定义
typedef struct{
    char wk[32]; //网卡接口名称
    char gl_tiaojian[128]; //包过滤条件
    int hcq_daxiao; //缓冲区大小
    int zd_bao; //最大包数量
    int chaoshi; //超时时间
    char wjm_shuchu[128]; //输出文件名
    bool kzt_xianshi; //控制台显示
    char rz_jibie[16]; //日志级别
    char rz_ming[128]; //日志文件名
}peizhi_t;

//申明全局变量
extern peizhi_t g_peizhi;

//函数声明
void peizhi_chushihua(); //初始化配置


#endif //peizhi_h
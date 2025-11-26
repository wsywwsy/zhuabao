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
bool peizhi_jiazai(const char* wjm_peizhi); //加载配置文件
void peizhi_xianshi(); //显示当前配置
bool peizhi_yanzheng(); //验证有效性
const char* peizhi_huoqu_wk(); //获取网卡接口
const char* peizhi_huoqu_gl_tiaojian(); //获取过滤条件
int peizhi_huoqu_hcaq_daxiao(); //获取缓冲区大小
int peizhi_huoqu_zuida_bao(); //获取最大包数量
int peizhi_huoqu_chaoshi(); //获取超时时间
const char* peizhi_huoqu_wjm_shuchu(); //获取输出文件名
bool peizhi_huoqu_kzt_xianshi(); //获取控制台显示选
const char* peizhi_huoqu_rz_jibie(); //获取日志级别
const char* peizhi_huoqu_rz_ming(); //获取日志文件名

#endif //结束保护
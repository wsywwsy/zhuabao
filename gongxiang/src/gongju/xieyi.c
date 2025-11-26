//协议解析模块实现
//负责解析各种网络协议

#include <stdio.h>      //标准输入输出库
#include <string.h>     //字符串处理库
#include <time.h>       //时间处理库
#include <ctype.h>      //字符处理库

//包含平台相关的头文件
#ifdef _WIN32
    #include <winsock2.h> //Windows套接字库
    #include <ws2tcpip.h> //Windows TCP/IP扩展
#else
    #include <arpa/inet.h> //Unix/Linux网络地址转换
    #include <netinet/in.h> //Unix/Linux网络协议族
#endif

//定义协力类型枚举
typedef enum {
    xy_yitaiwang = 0, //以太网协议
    xy_ip,          //IP协议
    xy_ipv6,        //IPv6协议
    xy_arp,         //ARP协议
    xy_http,         //HTTP协议
    xy_dns,         //DNS协议
    xy_tcp,         //TCP协议
    xy_udp,         //UDP协议
    xy_icmp,         //ICMP协议
} xy_leixing;

//定义以太网帧头结构 - 使用1字节对齐确保准确解析
#pragma pack(push,1)
typedef struct{
    uint8_t mb_mac[6]; //目的MAC地址
    uint8_t ydz_mac[6]; //源MAC地址
    uint16_t lx; //以太网类型
}ytw_tou; //以太网帧头
#pragma pack(pop)

//定义IP数据头结构 - 使用1字节对齐确保准确解析
#pragma pack(push,1)
typedef struct{
    uint8_t bb_touchang; //版本(4位)和头长度（4位）
    uint8_t fw_leixing; //服务类型
    uint16_t zcd; //总长度
    uint16_t bj_pianpianyi; //标识和片偏移
    uint8_t sj; //生存时间
    uint8_t xy; //协议
    uint16_t jyh; //头部校验和
    uint32_t ydz_ip; //源IP地址
    uint32_t mb_ip; //目的IP地址
} IPtou; //IP数据头
#pragma pack(pop)

#pragma pack(push,1)
typedef struct{
    uint16_t y_duankou; //源端口
    uint16_t mb_duankou; //目的端口
    uint32_t slh; //序列号
    uint32_t qfh; //确认号
    uint8_t sj_pianyi; //数据偏移和保留位
    uint8_t bz; //标志位
    uint16_t ck; //接收窗口
    uint16_t jyh; //校验和
    uint16_t jj_zhizhen; //紧急指针
}TCPtou; //TCP数据头
#pragma pack(pop) 

#pragma pack(push,1)
typedef struct{
    uint16_t y_duankou; //源端口
    uint16_t mb_duankou; //目的端口
    uint16_t cd; //长度
    uint16_t jyh; //校验和
}UDPtou; //UDP数据头
#pragma pack(pop)

//定义协议解析结果结构
typedef struct{
    XieYiLeiXing leixing; //协议类型
    char xy_ming[32]; //协议名称
    char yuan_dizhi[64]; //源地址
    char mu_dizhi[64]; //目的地址
    uint16_t y_duankou; //源端口
    uint16_t mb_duankou; //目的端口
    uint32_t sj_changdu; //数据长度
    char zy[256]; //摘要信息
    time_t sjc; //时间戳
}XieyiXinxi;

//模块内部状态结构 - 只在源文件种使用
typedef struct{
    int csh; //初始化状态
    uint32_t jx_jishu; //解析数据包计数器
    uint32_t cw_jishu; //错误数据包计数器
}XieyiNeibu;

//模块全局上下文
static XieyiNeibu xy_shuju = {0};

//内部辅助函数声明 - 这些函数不暴露给外部
static const char* hq_xy_ming(uint8_t xy); //根据协议号获取协议名称
static void gsh_mac_dizhi(const uint8_t *mac, char *hcq, size_t hc_changdu); //格式化MAC地址，规定缓冲区和缓冲区大小
static void gsh_ip_dizhi(uint32_t ip, char *hcq, size_t hc_changdu); //格式化IP地址，规定缓冲区和缓冲区大小
static int yz_sjb_changdu(uint32_t yq_changdu, uint32_t sj_changdu); //验证数据包长度,定义预期长度和实际长度

//初始化协议解析模块，返回值：成功返回0失败返回-1
int xy_chushihua(void){
    if (xy_shuju.csh){
        printf("[协议模块]已经初始化，跳过重复初始化\n");
        return 0;
    }
    
    //初始化上下文结构
    memset(&xy_shuju,0,sizeof(xy_shuju));
    xy_shuju.csh = 1; //标记为已初始化
    xy_shuju.jx_jishu = 0; //重置计数器
    xy_shuju.cw_jishu = 0; //重置错误计数器

    printf("[协议模块]初始化成功\n");
    return 0;
}
//清理协议解析模块资源,无返回
void xy_qingli(void){
    //检查是否已初始化
    if(!xy_shuju.csh)
}
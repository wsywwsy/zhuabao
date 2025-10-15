// src/rizhi.c
#include <stdio.h> //用于printf函数使用
#include "rizhi.h" //保护头文件

//实现普通信息日志
void ptrizhi(char* xinxi)
{
  printf("[正常] %s\n",xinxi);
}
//实现错误信息日志
void cwrizhi(char* xinxi)
{
  printf("[错误] %s\n",xinxi);
}
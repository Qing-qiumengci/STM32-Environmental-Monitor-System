#ifndef __DHT11_H				//定义以防止递归包含			
#define __DHT11_H

//=====一、头文件=========
#include "stm32f10x.h"		
#include <stdio.h>	

//=====二、函数声明=======
//1、温湿度模块引脚(PB14)，初始化函数
void DHT11_Init(void);
//2、转换温湿度模块引脚(PB14)的模式
void DHT11_Change_Mode(GPIOMode_TypeDef dht11_mode);
//3、获取温湿度的数据
int  DHT11_Get_Humi_Temp(uint8_t  *dht11_buf);

#endif /* __DHT11_H */

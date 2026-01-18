#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "stm32f10x.h"
#include "config.h"

// 显示初始化
void Display_Init(void);

// 显示处理函数
void Display_Process(void);

// 显示主界面
void Display_ShowMainScreen(void);

// 显示关机界面
void Display_ShowOffScreen(void);

// 更新传感器数据显示
void Display_UpdateSensorData(float temp, float humi, uint16_t water, uint8_t light);

#endif

#ifndef __ALARM_H
#define __ALARM_H

#include "stm32f10x.h"
#include "config.h"  // 包含配置头文件

// 报警状态结构体
typedef struct {
    uint8_t water_alarm;
    uint8_t temp_alarm;
    uint8_t humi_alarm;
    uint8_t light_alarm;
} Alarm_Detection;

// 初始化报警模块
void Alarm_Init(void);

// 报警处理函数
void Alarm_Process(void);

// 检测报警状态
Alarm_Detection Alarm_Check(float temp, float humi, uint16_t water, uint8_t light);

// 激活报警
void Alarm_Activate(void);

// 关闭报警
void Alarm_Deactivate(void);

#endif

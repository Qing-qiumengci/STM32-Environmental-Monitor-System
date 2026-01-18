#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "stm32f10x.h"

// 系统状态枚举
typedef enum {
    SYSTEM_OFF = 0,
    SYSTEM_ON = 1
} System_Status;

// 报警状态结构体
typedef struct {
    uint8_t water_alarm;
    uint8_t temp_alarm;
    uint8_t humi_alarm;
    uint8_t light_alarm;
    uint8_t overall_alarm;
} Alarm_Status;

// 初始化系统
void System_Init(void);

// 系统启动
void System_Start(void);

// 系统停止
void System_Stop(void);

// 获取系统状态
System_Status System_GetStatus(void);

// 系统主循环处理
void System_Process(void);

// 按键处理
void System_ProcessKey(uint8_t KeyNum);

// 更新报警状态
void System_UpdateAlarm(uint8_t water, uint8_t temp, uint8_t humi, uint8_t light);

// 获取报警状态
Alarm_Status System_GetAlarmStatus(void);

#endif

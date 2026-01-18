#include "System.h"
#include "LED.h"
#include "Buzzer.h"
#include "Alarm.h"
#include "Display.h"
#include "Sensor.h" 
#include "Delay.h"
#include "Serial.h"
#include <stdio.h>

static System_Status system_status = SYSTEM_OFF;
static Alarm_Status alarm_status = {0};

// 初始化系统
void System_Init(void)
{
    system_status = SYSTEM_OFF;
    Alarm_Init();      // 初始化报警模块
    Sensor_Init();     // 初始化传感器模块
    Display_Init();    // 初始化显示模块
}

// 系统启动
void System_Start(void)
{
    if (system_status == SYSTEM_OFF)
    {
        system_status = SYSTEM_ON;
        LED1_ON();  // 系统运行指示灯
        
        // 初始化显示界面
        Display_ShowMainScreen();
        
        Serial_SendString("System ON\r\n");
    }
}

// 系统停止
void System_Stop(void)
{
    if (system_status == SYSTEM_ON)
    {
        system_status = SYSTEM_OFF;
        LED1_OFF();
        LED2_OFF();
        
        // 关闭所有报警
        Alarm_Deactivate();
        
        // 显示关机界面
        Display_ShowOffScreen();
        
        Serial_SendString("System OFF\r\n");
    }
}

// 获取系统状态
System_Status System_GetStatus(void)
{
    return system_status;
}

// 系统主循环处理
void System_Process(void)
{
    static uint32_t system_tick = 0;
    system_tick++;
    
    // 系统开启时的处理
    if (system_status == SYSTEM_ON)
    {
        // 传感器数据采集和显示（每50ms循环）
        Sensor_Process();
        
        // 报警检测和处理
        Alarm_Process();
        
        // 更新显示
        Display_Process();
    }
}

// 按键处理
void System_ProcessKey(uint8_t KeyNum)
{
    // 处理B1按键（系统开关）
    if (KeyNum == 1)
    {
        if (system_status == SYSTEM_OFF)
        {
            System_Start();
            Serial_SendString("System ON (Key Pressed)\r\n");
        }
        else
        {
            System_Stop();
            Serial_SendString("System OFF (Key Pressed)\r\n");
        }
        Delay_ms(200);
    }
    
    // 处理B11按键（LED2控制）
    if (KeyNum == 2 && system_status == SYSTEM_ON)
    {
        LED2_Turn();
        Serial_SendString("LED2 Toggled\r\n");
        Delay_ms(200);
    }
}

// 更新报警状态
void System_UpdateAlarm(uint8_t water, uint8_t temp, uint8_t humi, uint8_t light)
{
    alarm_status.water_alarm = water;
    alarm_status.temp_alarm = temp;
    alarm_status.humi_alarm = humi;
    alarm_status.light_alarm = light;
    alarm_status.overall_alarm = water || temp || humi || light;
}

// 获取报警状态
Alarm_Status System_GetAlarmStatus(void)
{
    return alarm_status;
}

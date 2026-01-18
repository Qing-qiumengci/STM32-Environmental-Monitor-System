#include "Alarm.h"
#include "Buzzer.h"
#include "LED.h"
#include "System.h"
#include "Sensor.h"
#include "Serial.h"
#include "Delay.h"
#include <stdio.h>

// 初始化报警LED引脚
static void AlarmLED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_SetBits(GPIOA, GPIO_Pin_2);
}

// 初始化报警模块
void Alarm_Init(void)
{
    AlarmLED_Init();
    Alarm_Deactivate();
}

// 报警处理函数
void Alarm_Process(void)
{
    if (System_GetStatus() == SYSTEM_ON)
    {
        // 获取传感器数据
        Sensor_Data data = Sensor_GetData();
        
        // 检测报警
        Alarm_Detection alarm = Alarm_Check(data.temperature, 
                                           data.humidity, 
                                           data.water_level, 
                                           data.light_state);
        
        // 更新系统报警状态
        System_UpdateAlarm(alarm.water_alarm, 
                          alarm.temp_alarm, 
                          alarm.humi_alarm, 
                          alarm.light_alarm);
        
        // 控制报警设备
        Alarm_Status sys_alarm = System_GetAlarmStatus();
        if (sys_alarm.overall_alarm)
        {
            Alarm_Activate();
        }
        else
        {
            Alarm_Deactivate();
        }
    }
}

// 检测报警状态
Alarm_Detection Alarm_Check(float temp, float humi, uint16_t water, uint8_t light)
{
    Alarm_Detection alarm = {0};
    static uint8_t last_water_alarm = 0;
    static uint8_t last_light_alarm = 0;
    
    // 温度报警
    alarm.temp_alarm = (temp > TEMP_ALARM_THRESHOLD);
    
    // 湿度报警
    alarm.humi_alarm = (humi > HUMI_ALARM_THRESHOLD);
    
    // 水位报警（反向传感器：大于阈值报警）
    if (water > WATER_LEVEL_THRESHOLD)
    {
        alarm.water_alarm = 1;
        if (last_water_alarm == 0)
        {
            Serial_SendString("Water Alarm! Level too high\r\n");
            last_water_alarm = 1;
        }
    }
    else
    {
        alarm.water_alarm = 0;
        if (last_water_alarm == 1)
        {
            Serial_SendString("Water Level Normal\r\n");
            last_water_alarm = 0;
        }
    }
    
    // 光照报警
    if (light == LIGHT_ALARM_THRESHOLD)
    {
        alarm.light_alarm = 1;
        if (last_light_alarm == 0)
        {
            Serial_SendString("Light Alarm! Environment too dark\r\n");
            last_light_alarm = 1;
        }
    }
    else
    {
        alarm.light_alarm = 0;
        if (last_light_alarm == 1)
        {
            Serial_SendString("Light Normal\r\n");
            last_light_alarm = 0;
        }
    }
    
    return alarm;
}

// 激活报警
void Alarm_Activate(void)
{
    Buzzer_ON();
    GPIO_ResetBits(GPIOA, GPIO_Pin_2);  // 报警LED亮
}

// 关闭报警
void Alarm_Deactivate(void)
{
    Buzzer_OFF();
    GPIO_SetBits(GPIOA, GPIO_Pin_2);    // 报警LED灭
}

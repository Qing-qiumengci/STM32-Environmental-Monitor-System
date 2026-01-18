#include "Display.h"
#include "OLED.h"
#include "Sensor.h"
#include "System.h"
#include <stdio.h>

static uint32_t display_timer = 0;

/**
  * @brief  显示初始化
  */
void Display_Init(void)
{
    OLED_Init();
    OLED_Clear();           
    Display_ShowOffScreen(); 
}

/**
  * @brief  显示处理逻辑
  */
void Display_Process(void)
{
    static uint32_t system_tick = 0;
    system_tick++;
    
    if (display_timer == 0 || (system_tick - display_timer) > 20)
    {
        display_timer = system_tick;
        
        if (System_GetStatus() == SYSTEM_ON)
        {
            Sensor_Data data = Sensor_GetData();
            if (data.dht11_available)
            {
                Display_UpdateSensorData(data.temperature, 
                                        data.humidity, 
                                        data.water_level, 
                                        data.light_state);
            }
        }
        else
        {
            Display_ShowOffScreen();
        }
    }
}

/**
  * @brief  显示主界面固定标签
  * 注意：此函数必须存在，否则链接会报错
  */
void Display_ShowMainScreen(void)
{
    OLED_ShowString(1, 1, "T:");    
    OLED_ShowString(2, 1, "H:");    
    OLED_ShowString(3, 1, "L:");    
    OLED_ShowString(4, 1, "W:");    
}

/**
  * @brief  显示待机界面（关机状态）
  */
void Display_ShowOffScreen(void)
{
    OLED_ShowString(1, 1, "T:--.-C       ");
    OLED_ShowString(2, 1, "H:--.-%       ");
    OLED_ShowString(3, 1, "L:---         ");
    OLED_ShowString(4, 1, "W:----        ");
}

/**
  * @brief
  */
void Display_UpdateSensorData(float temp, float humi, uint16_t water, uint8_t light)
{
    char buffer[17]; 
    
    // --- 1. 温度 ---
    if (temp >= 0) {
        sprintf(buffer, "%2d.%1dC", (int)temp, (int)(temp * 10) % 10);
    } else {
        float abs_temp = -temp;
        sprintf(buffer, "-%1d.%1dC", (int)abs_temp, (int)(abs_temp * 10) % 10);
    }
    OLED_ShowString(1, 1, "T:"); 
    OLED_ShowString(1, 3, buffer);
    if (temp > TEMP_ALARM_THRESHOLD) {
        OLED_ShowString(1, 9, "HIGH  "); 
    } else {
        OLED_ShowString(1, 9, "      "); 
    }
    
    // --- 2. 湿度 ---
    sprintf(buffer, "%2d.%1d%%", (int)humi, (int)(humi * 10) % 10);
    OLED_ShowString(2, 1, "H:");
    OLED_ShowString(2, 3, buffer);
    if (humi > HUMI_ALARM_THRESHOLD) {
        OLED_ShowString(2, 9, "HIGH  ");
    } else {
        OLED_ShowString(2, 9, "      ");
    }
    
    // --- 3. 光照 ---
    OLED_ShowString(3, 1, "L:");
    if (light == 0) {
        OLED_ShowString(3, 3, "DARK  "); 
        OLED_ShowString(3, 9, "ALARM!"); 
    } else {
        OLED_ShowString(3, 3, "BRIGHT"); 
        OLED_ShowString(3, 9, "      "); 
    }
    
    // --- 4. 水位 ---
    sprintf(buffer, "%4d", water);
    OLED_ShowString(4, 1, "W:");
    OLED_ShowString(4, 3, buffer);
    if (water > WATER_LEVEL_THRESHOLD) {
        OLED_ShowString(4, 9, "HIGH! "); 
    } else {
        OLED_ShowString(4, 9, "      ");
    }
}


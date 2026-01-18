#include "stm32f10x.h"
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "OLED.h"
#include "Serial.h"
#include "DHT11.h"
#include <stdio.h>
#include "water.h"
#include "System.h"
#include "Command.h"

int main(void)
{
    /* 外设初始化 */
    LED_Init();             // LED初始化
    Key_Init();             // 按键初始化
    Buzzer_Init();          // 蜂鸣器初始化
    LightSensor_Init();     // 光敏传感器初始化
    Serial_Init();          // 串口初始化
    DHT11_Init();           // DHT11温湿度传感器初始化
    WaterSensor_Init();     // 水位传感器初始化
    
    /* 系统初始化 */
    System_Init();          // 系统初始化
    Command_Init();         // 命令处理初始化
    
    /* OLED初始显示 */
    OLED_ShowString(1, 1, "Initializing...");
    
    while (1)
    {
        // 获取按键
        uint8_t KeyNum = Key_GetNum();
        if (KeyNum != 0)
        {
            System_ProcessKey(KeyNum);
        }
        
        // 处理串口命令
        if (Serial_GetRxFlag() == 1)
        {
            uint8_t RxData = Serial_GetRxData();
            Command_Process(RxData);
        }
        
        // 系统主循环处理
        System_Process();
        
        // 延时
        Delay_ms(50);
    }
}

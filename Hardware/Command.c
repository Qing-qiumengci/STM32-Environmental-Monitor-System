#include "Command.h"
#include "System.h"
#include "Sensor.h"
#include "Serial.h"
#include <stdio.h>

// 发送数字到串口
static void Serial_SendNum(float num, uint8_t decimal_places)
{
    char buffer[20];
    
    if (decimal_places == 0) {
        sprintf(buffer, "%d", (int)num);
    } else if (decimal_places == 1) {
        sprintf(buffer, "%.1f", num);
    } else {
        sprintf(buffer, "%.2f", num);
    }
    
    Serial_SendString(buffer);
}

// 初始化命令处理
void Command_Init(void)
{
    // 发送欢迎信息
    Serial_SendString("=====================================\r\n");
    Serial_SendString("Sensor Monitor System Ready\r\n");
    Serial_SendString("=====================================\r\n");
    
    Command_SendHelp();
}

// 处理串口命令
void Command_Process(uint8_t cmd)
{
    Serial_SendByte(cmd);  // 回显
    
    switch(cmd)
    {
        case '0':  // 关闭系统
            System_Stop();
            Serial_SendString("System OFF\r\n");
            break;
            
        case '1':  // 开启系统
            System_Start();
            Serial_SendString("System ON\r\n");
            break;
            
        case 'w':  // 读取水位
            if (System_GetStatus() == SYSTEM_ON)
            {
                Sensor_Data data = Sensor_GetData();
                Serial_SendString("Water Level: ");
                Serial_SendNum(data.water_level, 0);
                Serial_SendString(" / 4095\r\n");
            }
            else
            {
                Serial_SendString("System is OFF\r\n");
            }
            break;
            
        case 'l':  // 读取光照
            if (System_GetStatus() == SYSTEM_ON)
            {
                Sensor_Data data = Sensor_GetData();
                Serial_SendString("Light State: ");
                Serial_SendNum(data.light_state, 0);
                Serial_SendString(" (0=Dark, 1=Bright)\r\n");
            }
            else
            {
                Serial_SendString("System is OFF\r\n");
            }
            break;
            
        case 's':  // 系统状态
            Command_SendStatus();
            break;
            
        default:   // 未知命令
            Command_SendHelp();
            break;
    }
}

// 发送帮助信息
void Command_SendHelp(void)
{
    Serial_SendString("Commands:\r\n");
    Serial_SendString("  0 = System OFF\r\n");
    Serial_SendString("  1 = System ON\r\n");
    Serial_SendString("  w = Water Level\r\n");
    Serial_SendString("  l = Light State\r\n");
    Serial_SendString("  s = System Status\r\n");
    Serial_SendString("  h = Help\r\n");
}

// 发送系统状态
void Command_SendStatus(void)
{
    Serial_SendString("=== System Status ===\r\n");
    Serial_SendString("System: ");
    Serial_SendString(System_GetStatus() == SYSTEM_ON ? "ON\r\n" : "OFF\r\n");
    
    if (System_GetStatus() == SYSTEM_ON)
    {
        Sensor_Data data = Sensor_GetData();
        Alarm_Status alarm = System_GetAlarmStatus();
        
        Serial_SendString("Sensors:\r\n");
        
        if (data.dht11_available)
        {
            Serial_SendString("  Temp: ");
            Serial_SendNum(data.temperature, 1);
            Serial_SendString("C (");
            Serial_SendString(alarm.temp_alarm ? "ALARM" : "Normal");
            Serial_SendString(")\r\n");
            
            Serial_SendString("  Humi: ");
            Serial_SendNum(data.humidity, 1);
            Serial_SendString("% (");
            Serial_SendString(alarm.humi_alarm ? "ALARM" : "Normal");
            Serial_SendString(")\r\n");
        }
        else
        {
            Serial_SendString("  Temp/Humi: Not Available\r\n");
        }
        
        Serial_SendString("  Light: ");
        Serial_SendNum(data.light_state, 0);
        Serial_SendString(" (");
        Serial_SendString(alarm.light_alarm ? "ALARM" : "Normal");
        Serial_SendString(")\r\n");
        
        Serial_SendString("  Water: ");
        Serial_SendNum(data.water_level, 0);
        Serial_SendString(" (");
        Serial_SendString(alarm.water_alarm ? "ALARM" : "Normal");
        Serial_SendString(")\r\n");
        
        Serial_SendString("Overall Alarm: ");
        Serial_SendString(alarm.overall_alarm ? "ACTIVE" : "Inactive");
        Serial_SendString("\r\n");
    }
}

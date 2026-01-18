#include "Sensor.h"
#include "DHT11.h"
#include "water.h"
#include "LightSensor.h"
#include "System.h"
#include "Delay.h"
#include <stdio.h>

static Sensor_Data sensor_data = {0};
static uint32_t dht11_timer = 0;
static uint32_t water_timer = 0;
static uint32_t light_timer = 0;

// 初始化所有传感器
void Sensor_Init(void)
{
    DHT11_Init();
    WaterSensor_Init();
    LightSensor_Init();
    
    sensor_data.temperature = 0;
    sensor_data.humidity = 0;
    sensor_data.water_level = 0;
    sensor_data.light_state = 0;
    sensor_data.dht11_available = 0;
}

// 传感器处理函数
void Sensor_Process(void)
{
    static uint32_t system_tick = 0;
    system_tick++;
    
    // 定时读取DHT11（每2秒）
    if (dht11_timer == 0 || (system_tick - dht11_timer) > 40)
    {
        dht11_timer = system_tick;
        
        if (Sensor_ReadDHT11(&sensor_data.temperature, &sensor_data.humidity))
        {
            sensor_data.dht11_available = 1;
        }
        else
        {
            sensor_data.dht11_available = 0;
        }
    }
    
    // 定时读取水位（每1秒）
    if (water_timer == 0 || (system_tick - water_timer) > 20)
    {
        water_timer = system_tick;
        sensor_data.water_level = Sensor_ReadWaterLevel();
    }
    
    // 定时读取光照（每0.5秒）
    if (light_timer == 0 || (system_tick - light_timer) > 10)
    {
        light_timer = system_tick;
        sensor_data.light_state = Sensor_ReadLightState();
    }
}

// 获取传感器数据
Sensor_Data Sensor_GetData(void)
{
    return sensor_data;
}

// 读取DHT11数据
uint8_t Sensor_ReadDHT11(float *temperature, float *humidity)
{
    uint8_t dht11_buf[5];
    int result = DHT11_Get_Humi_Temp(dht11_buf);
    
    if (result == 0)  // DHT11_SUCCESS
    {
        // 湿度
        *humidity = (float)dht11_buf[0] + (float)dht11_buf[1] * 0.1;
        
        // 温度
        if (dht11_buf[2] & 0x80)  // 负数
        {
            dht11_buf[2] &= 0x7F;
            *temperature = -(float)dht11_buf[2] - (float)dht11_buf[3] * 0.1;
        }
        else
        {
            *temperature = (float)dht11_buf[2] + (float)dht11_buf[3] * 0.1;
        }
        
        return 1;
    }
    
    return 0;
}

// 读取水位
uint16_t Sensor_ReadWaterLevel(void)
{
    return WaterSensor_GetValue();
}

// 读取光照状态
uint8_t Sensor_ReadLightState(void)
{
    return LightSensor_Get();
}

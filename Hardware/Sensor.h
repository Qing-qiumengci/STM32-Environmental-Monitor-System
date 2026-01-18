#ifndef __SENSOR_H
#define __SENSOR_H

#include "stm32f10x.h"

// 向前声明，避免循环依赖
struct Sensor_Data;

// 传感器数据结构体
typedef struct Sensor_Data {
    float temperature;
    float humidity;
    uint16_t water_level;
    uint8_t light_state;
    uint8_t dht11_available;
} Sensor_Data;

// 初始化所有传感器
void Sensor_Init(void);

// 传感器处理函数（需要在主循环中调用）
void Sensor_Process(void);

// 获取传感器数据
Sensor_Data Sensor_GetData(void);

// 读取DHT11数据
uint8_t Sensor_ReadDHT11(float *temperature, float *humidity);

// 读取水位
uint16_t Sensor_ReadWaterLevel(void);

// 读取光照状态
uint8_t Sensor_ReadLightState(void);

#endif

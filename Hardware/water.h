#ifndef __WATER_H
#define __WATER_H

#include "stm32f10x.h"

// º¯ÊıÉùÃ÷
void WaterSensor_Init(void);
uint16_t WaterSensor_GetValue(void);
uint8_t WaterSensor_IsHighLevel(void);

#endif

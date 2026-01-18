#ifndef __CONFIG_H
#define __CONFIG_H

// 报警阈值配置
#define TEMP_ALARM_THRESHOLD 30.0  // 温度超过30°C报警
#define HUMI_ALARM_THRESHOLD 85.0  // 湿度超过85%报警
#define WATER_LEVEL_THRESHOLD 800 // 当ADC值超过1500时报警（反向传感器）
#define LIGHT_ALARM_THRESHOLD 0    // 当光照状态为0（暗）时报警

#endif

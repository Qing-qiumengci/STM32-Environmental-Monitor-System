# STM32-Environmental-Monitor-System 🌡️

基于STM32F103的多传感器环境监测与智能报警系统。

---

## ✨ 核心功能

- 📊 **四参数监测**：温度、湿度、光照、水位
- 🚨 **智能报警**：阈值触发，声光+屏幕+串口三重提示
- 🎛️ **双控模式**：本地按键 + 串口远程控制
- 📱 **实时显示**：OLED屏幕实时数据显示
- ⚡ **快速响应**：报警响应 <100ms

---

## 🛠️ 硬件清单

| 组件 | 型号 | 连接引脚 |
|------|------|----------|
| 主控 | STM32F103C8T6 | - |
| 温湿度 | DHT11 | PB14 |
| 光敏传感器 | 数字式 | PB13 |
| 水位传感器 | 模拟式 | PA3(ADC) |
| OLED屏 | SSD1306 | PB6(I2C_SCL), PB7(I2C_SDA) |
| 蜂鸣器 | 有源 | PB12 |
| 系统指示LED | LED | PA1 |
| 报警LED | LED | PA2 |
| 按键1 | 系统总开关 | PB1|
| 按键2 | 报警灯测试开关 | PB11|

---

## 🚀 快速开始

1. **连接硬件**：按上表连接传感器和外设
2. **打开工程**：Keil打开 `Project.uvprojx`
3. **编译下载**：编译后下载到STM32
4. **启动系统**：按键B1（总开关）或串口发送 `1`
5. **串口调试**：9600bps，文本模式，发送命令控制

---

## 📖 串口命令

```
1    - 开启系统
0    - 关闭系统
s    - 查看系统状态
w    - 读取水位
l    - 读取光照
h    - 显示帮助
```

---

## 🏗️ 项目结构

```
├── main.c              # 主程序
├── config.h            # 配置参数
├── Hardware/           # 硬件驱动
│   ├── System.c/.h     # 系统管理
│   ├── Sensor.c/.h     # 传感器模块
│   ├── Alarm.c/.h      # 报警模块
│   ├── Display.c/.h    # 显示模块
│   ├── Command.c/.h    # 命令处理
│   └── [各外设驱动]
├── Libraries/          # ST标准库
└── Project/            # Keil工程文件
```

---

## 🔧 配置参数

```c
// config.h
#define TEMP_ALARM_THRESHOLD   30.0    // 温度阈值(°C)
#define HUMI_ALARM_THRESHOLD   85.0    // 湿度阈值(%)
#define WATER_LEVEL_THRESHOLD  1000    // 水位阈值
```

---

## 📱 OLED显示

```
T:25.3°C    Normal
H:65.2%     Normal  
L:1         Bright
W:0850      Normal
```
- **HIGH**：超阈值报警
- **Normal**：正常状态
- **Dark/Bright**：光照状态

---
**特别感谢：Bilibili@江协科技**

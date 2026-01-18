#ifndef __COMMAND_H
#define __COMMAND_H

#include "stm32f10x.h"

// 初始化命令处理
void Command_Init(void);

// 处理串口命令
void Command_Process(uint8_t cmd);

// 发送帮助信息
void Command_SendHelp(void);

// 发送系统状态
void Command_SendStatus(void);

#endif

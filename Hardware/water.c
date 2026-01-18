#include "water.h"
#include "stm32f10x.h"
#include "Delay.h"

// 水位传感器连接到PA（ADC通道3）
#define WATER_SENSOR_PIN  GPIO_Pin_3
#define WATER_SENSOR_PORT GPIOA
#define WATER_SENSOR_ADC_CHANNEL ADC_Channel_3

// 初始化水位传感器
void WaterSensor_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    
    // 使能GPIOB和ADC1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
    
    // 配置PB15为模拟输入
    GPIO_InitStructure.GPIO_Pin = WATER_SENSOR_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;  // 模拟输入
    GPIO_Init(WATER_SENSOR_PORT, &GPIO_InitStructure);
    
    // ADC初始化
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);  // 设置ADC时钟为12MHz（72MHz/6）
    
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  // 独立模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;       // 非扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // 单次转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 软件触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // 右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;             // 1个转换通道
    
    ADC_Init(ADC1, &ADC_InitStructure);
    
    // 使能ADC1
    ADC_Cmd(ADC1, ENABLE);
    
    // ADC校准
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
}

// 获取水位传感器值（ADC值）
uint16_t WaterSensor_GetValue(void)
{
    uint16_t adc_value;
    
    // 设置ADC通道
    ADC_RegularChannelConfig(ADC1, WATER_SENSOR_ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);
    
    // 启动ADC转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    
    // 等待转换完成
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    
    // 获取ADC值
    adc_value = ADC_GetConversionValue(ADC1);
    
    return adc_value;
}

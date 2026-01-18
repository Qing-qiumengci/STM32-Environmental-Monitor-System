#include "dht11.h"
#include "delay.h"

// 内部使用的静态变量
static GPIO_InitTypeDef GPIO_InitStructure;

/**
  * @brief  DHT11初始化
  */
void DHT11_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP; // 初始推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_SetBits(GPIOB, GPIO_Pin_14); // 总线空闲高电平
}

/**
  * @brief  切换引脚模式
  */
void DHT11_Change_Mode(GPIOMode_TypeDef dht11_mode)
{    
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode  = dht11_mode;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief
  * @retval
  */
int DHT11_Get_Humi_Temp(uint8_t *dht11_buf)
{
    uint32_t dht11_sum = 0;
    uint8_t temp = 0;    
    uint16_t timeout = 0;
    int i = 0, j = 0;

    // --- 1. 发送起始信号 ---
    DHT11_Change_Mode(GPIO_Mode_Out_PP);
    GPIO_ResetBits(GPIOB, GPIO_Pin_14); // 拉低
    Delay_ms(20);                       // 保持20ms，确保唤醒
    GPIO_SetBits(GPIOB, GPIO_Pin_14);   // 拉高
    Delay_us(30);                       // 等待总线释放

    // --- 2. 关键时序开始：关闭中断防止干扰 ---
    __disable_irq(); 

    DHT11_Change_Mode(GPIO_Mode_IPU);   // 切换输入
    
    // 等待DHT11拉低（响应信号）
    timeout = 0;
    while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 1) {
        timeout++; Delay_us(1);
        if(timeout > 200) { __enable_irq(); return -1; }
    }
    
    // 等待DHT11拉高
    timeout = 0;
    while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0) {
        timeout++; Delay_us(1);
        if(timeout > 200) { __enable_irq(); return -2; }
    }
    
    // 等待DHT11再次拉低（准备发送数据）
    timeout = 0;
    while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 1) {
        timeout++; Delay_us(1);
        if(timeout > 200) { __enable_irq(); return -3; }
    }

    // --- 3. 读取40位数据 ---
    for(i = 0; i < 5; i++) {
        temp = 0;
        for(j = 0; j < 8; j++) {
            // 等待低电平结束
            timeout = 0;
            while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0) {
                timeout++; Delay_us(1);
                if(timeout > 200) { __enable_irq(); return -4; }
            }    
        
            // 关键采样点：延时50us跳过0码的高电平区
            Delay_us(50); 
            
            if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 1) {
                temp |= (1 << (7 - j));      
                
                // 等待当前位的高电平结束
                timeout = 0;
                while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 1) {
                    timeout++; Delay_us(1);
                    if(timeout > 200) break; 
                }
            }    
        }
        dht11_buf[i] = temp;    
    }

    // --- 4. 关键时序结束：恢复中断 ---
    __enable_irq(); 

    // 5. 校验数据
    dht11_sum = dht11_buf[0] + dht11_buf[1] + dht11_buf[2] + dht11_buf[3];
    if(dht11_buf[4] != (uint8_t)(dht11_sum & 0xFF)) {
        return -5; 
    }
            
    // 恢复空闲状态
    DHT11_Change_Mode(GPIO_Mode_Out_PP);
    GPIO_SetBits(GPIOB, GPIO_Pin_14);
    
    return 0; 
}

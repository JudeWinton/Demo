// Copyright 2021 IOsetting <iosetting(at)outlook.com>
//
// 根据Apache License 2.0协议授权

#include "fw_tim.h" // 包含计时器相关的头文件
#include "fw_sys.h" // 包含系统相关的头文件
#include "fw_util.h" // 包含实用工具的头文件

/**
 * 计算 Timer0 和 Timer1 计数器的初始值
 * - 如果频率过高，将返回 `limit` 的值，计时器将以最高频率运行
 * - 如果频率过低，将返回 0，计时器将以最低可能频率运行
*/
int16_t TIM_Timer0n1_CalculateInitValue(uint16_t frequency, HAL_State_t freq1t, uint16_t limit)
{
    uint32_t value = __SYSCLOCK; // 获取系统时钟值
    if (!freq1t)
        value = value / 12; // 如果 freq1t 为0，那么将 value 除以 12
    value = value / frequency; // 将 value 除以频率
    if (value > limit)
        return 0; // 如果 value 大于上限，则返回 0
    else
        return limit - value; // 否则返回上限减去 value 的值
}

// 配置 Timer0 的函数
void TIM_Timer0_Config(HAL_State_t freq1t, TIM_TimerMode_t mode, uint16_t frequency)
{
    uint16_t init;
    TIM_Timer0_Set1TMode(freq1t); // 设置 Timer0 的 1T 模式
    TIM_Timer0_SetMode(mode); // 设置 Timer0 的模式
    if (mode == TIM_TimerMode_8BitAuto) // 如果模式为 8 位自动模式
    {
        init = TIM_Timer0n1_CalculateInitValue(frequency, freq1t, 0xFF); // 计算初始值
        TIM_Timer0_SetInitValue(init & 0xFF, init & 0xFF); // 设置 Timer0 的初始值
    }
    else // 如果模式不是 8 位自动模式
    {
        init = TIM_Timer0n1_CalculateInitValue(frequency, freq1t, 0xFFFF); // 计算初始值
        TIM_Timer0_SetInitValue(init >> 8, init & 0xFF); // 设置 Timer0 的初始值
    }
}

// 配置 Timer1 的函数
void TIM_Timer1_Config(HAL_State_t freq1t, TIM_TimerMode_t mode, uint16_t frequency)
{
    uint16_t init;
    TIM_Timer1_Set1TMode(freq1t); // 设置 Timer1 的 1T 模式
    TIM_Timer1_SetMode(mode); // 设置 Timer1 的模式
    if (mode == TIM_TimerMode_8BitAuto) // 如果模式为 8 位自动模式
    {
        init = TIM_Timer0n1_CalculateInitValue(frequency, freq1t, 0xFF); // 计算初始值
        TIM_Timer1_SetInitValue(init & 0xFF, init & 0xFF); // 设置 Timer1 的初始值
    }
    else // 如果模式不是 8 位自动模式
    {
        init = TIM_Timer0n1_CalculateInitValue(frequency, freq1t, 0xFFFF); // 计算初始值
        TIM_Timer1_SetInitValue(init >> 8, init & 0xFF); // 设置 Timer1 的初始值
    }
}

// 计算 Timer2, Timer3, Timer4 的初始值的函数
int16_t _TIM_Timer234_InitValueCalculate(
    uint16_t frequency, uint8_t prescaler, HAL_State_t freq1t)
{
    uint32_t value = __SYSCLOCK; // 获取系统时钟值
    if (!freq1t)
        value = value / 12; // 如果 freq1t 为0，那么将 value 除以 12
    value = value / ((prescaler + 1) * frequency); // 将 value 除以 prescaler+1 和 frequency 的乘积

    if (value > 0xFFFF)
        return 0; // 如果 value 大于 0xFFFF，则返回 0
    else
        return 0xFFFF - value; // 否则返回 0xFFFF 减去 value 的值
}

/**
 * Timer2,3,4 的配置
 * 
 * 1. 没有中断优先级
 * 2. 频率 = SYSCLK / (TMxPS + 1) / (0xFFFF - TxH,TxL) / (1T? 1 : 12)
*/

// 配置 Timer2 的函数
void TIM_Timer2_Config(HAL_State_t freq1t, uint8_t prescaler, uint16_t frequency)
{
    uint16_t init = _TIM_Timer234_InitValueCalculate(frequency, prescaler, freq1t); // 计算初始值
    TIM_Timer2_Set1TMode(freq1t); // 设置 Timer2 的 1T 模式
    TIM_Timer2_SetPreScaler(prescaler); // 设置 Timer2 的预分频值
    TIM_Timer2_SetInitValue(init >> 8, init & 0xFF); // 设置 Timer2 的初始值
}

// 配置 Timer3 的函数
void TIM_Timer3_Config(
    HAL_State_t freq1t, uint8_t prescaler, uint16_t frequency, HAL_State_t intState)
{
    uint16_t init = _TIM_Timer234_InitValueCalculate(frequency, prescaler, freq1t); // 计算初始值
    TIM_Timer3_Set1TMode(freq1t); // 设置 Timer3 的 1T 模式
    TIM_Timer3_SetPreScaler(prescaler); // 设置 Timer3 的预分频值
    TIM_Timer3_SetInitValue(init >> 8, init & 0xFF); // 设置 Timer3 的初始值
    EXTI_Timer3_SetIntState(intState); // 设置 Timer3 的中断状态
}

// 配置 Timer4 的函数
void TIM_Timer4_Config(
    HAL_State_t freq1t, uint8_t prescaler, uint16_t frequency, HAL_State_t intState)
{
    uint16_t init = _TIM_Timer234_InitValueCalculate(frequency, prescaler, freq1t); // 计算初始值
    TIM_Timer4_Set1TMode(freq1t); // 设置 Timer4 的 1T 模式
    TIM_Timer4_SetPreScaler(prescaler); // 设置 Timer4 的预分频值
    TIM_Timer4_SetInitValue(init >> 8, init & 0xFF); // 设置 Timer4 的初始值
    EXTI_Timer4_SetIntState(intState); // 设置 Timer4 的中断状态
}

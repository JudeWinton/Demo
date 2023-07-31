// Copyright 2021 IOsetting <iosetting(at)outlook.com>
//
// ����Apache License 2.0Э����Ȩ

#include "fw_tim.h" // ������ʱ����ص�ͷ�ļ�
#include "fw_sys.h" // ����ϵͳ��ص�ͷ�ļ�
#include "fw_util.h" // ����ʵ�ù��ߵ�ͷ�ļ�

/**
 * ���� Timer0 �� Timer1 �������ĳ�ʼֵ
 * - ���Ƶ�ʹ��ߣ������� `limit` ��ֵ����ʱ���������Ƶ������
 * - ���Ƶ�ʹ��ͣ������� 0����ʱ��������Ϳ���Ƶ������
*/
int16_t TIM_Timer0n1_CalculateInitValue(uint16_t frequency, HAL_State_t freq1t, uint16_t limit)
{
    uint32_t value = __SYSCLOCK; // ��ȡϵͳʱ��ֵ
    if (!freq1t)
        value = value / 12; // ��� freq1t Ϊ0����ô�� value ���� 12
    value = value / frequency; // �� value ����Ƶ��
    if (value > limit)
        return 0; // ��� value �������ޣ��򷵻� 0
    else
        return limit - value; // ���򷵻����޼�ȥ value ��ֵ
}

// ���� Timer0 �ĺ���
void TIM_Timer0_Config(HAL_State_t freq1t, TIM_TimerMode_t mode, uint16_t frequency)
{
    uint16_t init;
    TIM_Timer0_Set1TMode(freq1t); // ���� Timer0 �� 1T ģʽ
    TIM_Timer0_SetMode(mode); // ���� Timer0 ��ģʽ
    if (mode == TIM_TimerMode_8BitAuto) // ���ģʽΪ 8 λ�Զ�ģʽ
    {
        init = TIM_Timer0n1_CalculateInitValue(frequency, freq1t, 0xFF); // �����ʼֵ
        TIM_Timer0_SetInitValue(init & 0xFF, init & 0xFF); // ���� Timer0 �ĳ�ʼֵ
    }
    else // ���ģʽ���� 8 λ�Զ�ģʽ
    {
        init = TIM_Timer0n1_CalculateInitValue(frequency, freq1t, 0xFFFF); // �����ʼֵ
        TIM_Timer0_SetInitValue(init >> 8, init & 0xFF); // ���� Timer0 �ĳ�ʼֵ
    }
}

// ���� Timer1 �ĺ���
void TIM_Timer1_Config(HAL_State_t freq1t, TIM_TimerMode_t mode, uint16_t frequency)
{
    uint16_t init;
    TIM_Timer1_Set1TMode(freq1t); // ���� Timer1 �� 1T ģʽ
    TIM_Timer1_SetMode(mode); // ���� Timer1 ��ģʽ
    if (mode == TIM_TimerMode_8BitAuto) // ���ģʽΪ 8 λ�Զ�ģʽ
    {
        init = TIM_Timer0n1_CalculateInitValue(frequency, freq1t, 0xFF); // �����ʼֵ
        TIM_Timer1_SetInitValue(init & 0xFF, init & 0xFF); // ���� Timer1 �ĳ�ʼֵ
    }
    else // ���ģʽ���� 8 λ�Զ�ģʽ
    {
        init = TIM_Timer0n1_CalculateInitValue(frequency, freq1t, 0xFFFF); // �����ʼֵ
        TIM_Timer1_SetInitValue(init >> 8, init & 0xFF); // ���� Timer1 �ĳ�ʼֵ
    }
}

// ���� Timer2, Timer3, Timer4 �ĳ�ʼֵ�ĺ���
int16_t _TIM_Timer234_InitValueCalculate(
    uint16_t frequency, uint8_t prescaler, HAL_State_t freq1t)
{
    uint32_t value = __SYSCLOCK; // ��ȡϵͳʱ��ֵ
    if (!freq1t)
        value = value / 12; // ��� freq1t Ϊ0����ô�� value ���� 12
    value = value / ((prescaler + 1) * frequency); // �� value ���� prescaler+1 �� frequency �ĳ˻�

    if (value > 0xFFFF)
        return 0; // ��� value ���� 0xFFFF���򷵻� 0
    else
        return 0xFFFF - value; // ���򷵻� 0xFFFF ��ȥ value ��ֵ
}

/**
 * Timer2,3,4 ������
 * 
 * 1. û���ж����ȼ�
 * 2. Ƶ�� = SYSCLK / (TMxPS + 1) / (0xFFFF - TxH,TxL) / (1T? 1 : 12)
*/

// ���� Timer2 �ĺ���
void TIM_Timer2_Config(HAL_State_t freq1t, uint8_t prescaler, uint16_t frequency)
{
    uint16_t init = _TIM_Timer234_InitValueCalculate(frequency, prescaler, freq1t); // �����ʼֵ
    TIM_Timer2_Set1TMode(freq1t); // ���� Timer2 �� 1T ģʽ
    TIM_Timer2_SetPreScaler(prescaler); // ���� Timer2 ��Ԥ��Ƶֵ
    TIM_Timer2_SetInitValue(init >> 8, init & 0xFF); // ���� Timer2 �ĳ�ʼֵ
}

// ���� Timer3 �ĺ���
void TIM_Timer3_Config(
    HAL_State_t freq1t, uint8_t prescaler, uint16_t frequency, HAL_State_t intState)
{
    uint16_t init = _TIM_Timer234_InitValueCalculate(frequency, prescaler, freq1t); // �����ʼֵ
    TIM_Timer3_Set1TMode(freq1t); // ���� Timer3 �� 1T ģʽ
    TIM_Timer3_SetPreScaler(prescaler); // ���� Timer3 ��Ԥ��Ƶֵ
    TIM_Timer3_SetInitValue(init >> 8, init & 0xFF); // ���� Timer3 �ĳ�ʼֵ
    EXTI_Timer3_SetIntState(intState); // ���� Timer3 ���ж�״̬
}

// ���� Timer4 �ĺ���
void TIM_Timer4_Config(
    HAL_State_t freq1t, uint8_t prescaler, uint16_t frequency, HAL_State_t intState)
{
    uint16_t init = _TIM_Timer234_InitValueCalculate(frequency, prescaler, freq1t); // �����ʼֵ
    TIM_Timer4_Set1TMode(freq1t); // ���� Timer4 �� 1T ģʽ
    TIM_Timer4_SetPreScaler(prescaler); // ���� Timer4 ��Ԥ��Ƶֵ
    TIM_Timer4_SetInitValue(init >> 8, init & 0xFF); // ���� Timer4 �ĳ�ʼֵ
    EXTI_Timer4_SetIntState(intState); // ���� Timer4 ���ж�״̬
}

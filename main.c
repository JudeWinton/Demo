// Copyright 2021 IOsetting <iosetting(at)outlook.com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * STC8H3K64S2
 *   AGrnd   -> GND
 *   AVcc    -> VCC
 *   AVref   -> VCC 
 *   Vcc     -> VCC
 *   Gnd     -> GND
 *   ADC1    -> Test voltage
 * 
 * STC8H1K08
 *   AVref   -> VCC 
 *   Vcc     -> VCC
 *   Gnd     -> GND
 *   ADC1    -> Test voltage
*/
#include "fw_hal.h"

uint16_t res;

void GPIO_Init(void)
{
//    // P3.2, P3.3
//    GPIO_P3_SetMode(GPIO_Pin_2|GPIO_Pin_3, GPIO_Mode_Output_PP);
//    // P5.4, 上拉电阻
//    GPIO_P5_SetMode(GPIO_Pin_4, GPIO_Mode_Input_HIP);
//    GPIO_SetPullUp(GPIO_Port_5, GPIO_Pin_4, HAL_State_ON);
    //输入；S01-P5.2,S02-P5.3,S03-P0.5,S04-P0.6,S05-P0.7,S06-P6.0,S07-P6.1,S08-P6.2,S09-P6.3,
    GPIO_P5_SetMode(GPIO_Pin_2|GPIO_Pin_3, GPIO_Mode_Input_HIP);
    GPIO_P0_SetMode(GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_Input_HIP);
    GPIO_P6_SetMode(GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3, GPIO_Mode_Input_HIP);
    //输入S10-P4.7
    GPIO_P4_SetMode(GPIO_Pin_7, GPIO_Mode_Input_HIP);
    //输入S11-P1.4,S12-P1.5,S13-P1.6,S14-P1.7,S15-P1.3
		GPIO_P1_SetMode(GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_3, GPIO_Mode_Input_HIP);
		//输入S16-P5.4
		GPIO_P5_SetMode(GPIO_Pin_4, GPIO_Mode_Input_HIP);
    //SW1,SW2,SW3输出
		GPIO_P6_SetMode(GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6, GPIO_Mode_Output_PP);
		//io power配置P4.0输出
		GPIO_P4_SetMode(GPIO_Pin_0, GPIO_Mode_Output_PP);
		//io pwm P6.7 输出
		GPIO_P6_SetMode(GPIO_Pin_7, GPIO_Mode_Output_PP);
		//d01-P3.2,d02-P3.3,d03-P3.4,d04-P5.0,d05-P4.3,d06-P4.4
		//d07-P2.0,d08-P2.1,d09-P2.2,d10-P2.3,d11-P2.4,d12-P2.5,d13-P2.6,d14-P2.7,
		//d15-P7.4,d16-P7.5,d17-P7.6,d18-P7.7,
		//d19-P4.5,d20-P4.6,输出
		
		//IOrx3-P0.0 IOtx3-P0.1
		
		//AD-TEM1-P0.3  AD-TEM2-P0.4
		
		
		//dir1-p3.5,dir2-p3.7,dir3-p7.1,dir4-p7.3,dir5-p4.2,
		
		
		//pul-p5.1,pul2-p3.6,pul3-p7.0,pul4-p7.2,pul5-p4.1
		
		
		
}


/*
INTERRUPT(ADC_Routine, EXTI_VectADC)
{
    ADC_ClearInterrupt();
    res = ADC_RESL;
    res |= (ADC_RES & 0x0F) << 8;
    // Restart ADC for continuous sampling
    ADC_Start();
}
*/

void main(void)
{
/*	
    SYS_SetClock();
    // For debug print
    UART1_Config8bitUart(UART1_BaudSource_Timer2, HAL_State_ON, 115200);
    // Set ADC1(GPIO P1.1) HIP
    GPIO_P1_SetMode(GPIO_Pin_1, GPIO_Mode_Input_HIP);
    // Channel: ADC1
    ADC_SetChannel(0x01);
    // ADC Clock = SYSCLK / 2 / (1+15) = SYSCLK / 32
    ADC_SetClockPrescaler(0x0F);
    // Right alignment, high 2-bit in ADC_RES, low 8-bit in ADC_RESL
    ADC_SetResultAlignmentRight();
    // Enable interrupts
    EXTI_Global_SetIntState(HAL_State_ON);
    EXTI_ADC_SetIntState(HAL_State_ON);
    // Turn on ADC power
    ADC_SetPowerState(HAL_State_ON);
    // Start ADC
    ADC_Start();
*/
	
	
    while(1)
    {
			
			
/*
        UART1_TxString("Result: ");
        UART1_TxHex(res >> 8);
        UART1_TxHex(res & 0xFF);
        UART1_TxString("\r\n");
        SYS_Delay(100);
*/
			
			
			
    }
}
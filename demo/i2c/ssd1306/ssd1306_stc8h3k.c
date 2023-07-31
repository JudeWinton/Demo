// ������Ҫ��ͷ�ļ�����Щ�ļ������˺����õ���һЩ�����ͱ���
#include "fw_hal.h"
#include "ssd1306.h"



// ����I2C��ʼ������
void I2C_Init(void)
{
    // ����I2CΪ����ģʽ
    I2C_SetWorkMode(I2C_WorkMode_Master);

    // ����I2Cʱ�ӷ�Ƶ����������I2Cͨ��ʱ��
    // I2Cʱ�� = FOSC / 2 / (__prescaler__ * 2 + 4)
    // SSD1306���֧�ֵ�I2Cʱ��Ƶ��Ϊ1.3MHz���������ֵ���ܵ�����ʾʧ��
    I2C_SetClockPrescaler(0x10);

    // �л�������I2C�˿�
    I2C_SetPort(I2C_AlterPort_P32_P33);

    // ����I2C
    I2C_SetEnabled(HAL_State_ON);
}

// ����GPIO��ʼ������
void GPIO_Init(void)
{
    // ����SDA�˿ڣ�P3.3��Ϊ����/����˿�
    GPIO_P3_SetMode(GPIO_Pin_3, GPIO_Mode_InOut_QBD);

    // ����SCL�˿ڣ�P3.2��Ϊ��������˿�
    GPIO_P3_SetMode(GPIO_Pin_2, GPIO_Mode_Output_PP);
		// ����P2.2��P2.7Ϊ�������ģʽ
    GPIO_P2_SetMode(GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_Output_PP);
}
static uint16_t counter = 0; // ��̬���������������ڼ��㶨ʱ���жϵĴ���
// LED��ˮ��״̬
uint8_t led_state = 0;

// Timer0 ���жϷ�������
INTERRUPT(Timer0_Routine, EXTI_VectTimer0)
{
    counter++; // ����������
    if (counter == 1000) // ������������1000ʱ
    {
        counter = 0; // ���ü�����
			// �ı�LED״̬
    P2 = (P2 & 0x03) | (led_state << 2);
    led_state = (led_state + 1) % 64;  // ��ˮ��״̬����������ÿ��LED
        /* λ���ã�P32����ߵ�ƽ��P33����͵�ƽ */
//        P22 = SET;
//        P23 = RESET;
//				P24 = SET;
//        P25 = RESET;
//				P26 = SET;
//        P27 = RESET;
        /**
         * ����ͨ���ֽ�����
         * P3 = P3 & 0xF3 | 0x08;
        */
		}
}
// ������
int main(void)
{
	
    int y1, y2;
    uint8_t d1, d2;

    // ����ϵͳʱ��
    SYS_SetClock();

    // ��ʼ��GPIO
    GPIO_Init();

    // ��ʼ��I2C
    I2C_Init();

    // ��ʼ��SSD1306��ʾ��
    SSD1306_Init();
	
	
 // ����Timer0��1Tģʽ��16λ�Զ�����ģʽ��Ƶ��1000Hz
    TIM_Timer0_Config(HAL_State_ON, TIM_TimerMode_16BitAuto, 1000);
    // ����Timer0���ж�
    EXTI_Timer0_SetIntState(HAL_State_ON);
    // ����Timer0�ж����ȼ�Ϊ��
    EXTI_Timer0_SetIntPriority(EXTI_IntPriority_High);
    // ����ȫ���ж�
    EXTI_Global_SetIntState(HAL_State_ON);
    // ����Timer0Ϊ����״̬
    TIM_Timer0_SetRunState(HAL_State_ON);
    // ��ѭ��
    while(1)
    {
			
			
       
        
        // ����Ļ�ϻ�һ������
        SSD1306_DrawLine(0,   0, 127,  0, 1);
        SSD1306_DrawLine(0,   0,   0, 63, 1);
        SSD1306_DrawLine(127, 0, 127, 63, 1);
        SSD1306_DrawLine(0,  63, 127, 63, 1);

        // ������ʾ��
        SSD1306_UpdateScreen();

        // ��ʱ1��
        SYS_Delay(1000);
			/* λ���ã�P32����͵�ƽ��P33����ߵ�ƽ */
        P22 = RESET;
        P23 = SET;
        /**
         * ����ͨ���ֽ�����
         * P3 = P3 & 0xF3 | 0x04;
        */
        // ����
        SSD1306_Fill(0);

        // ��ת��ʾ
        SSD1306_ToggleInvert();

        // ������ʾ��
        SSD1306_UpdateScreen();

        // ��ʱ1��
        SYS_Delay(1000);

        // ��ת��ʾ
        SSD1306_ToggleInvert();

        // ������ʾ��
        SSD1306_UpdateScreen();

        // ��ʱ1��
        SYS_Delay(1000);

        // ����Ļ�ϻ���һϵ���߶Σ��γɶ�̬Ч��
        y1 = 64, y2 = 0;
        while (y1 > 0)
        {
            SSD1306_DrawLine(0, y1, 127, y2, 1);
            SSD1306_UpdateScreen();
            y1 -= 2;
            y2 += 2;
        }
        SYS_Delay(1000);

        // ����
        SSD1306_Fill(0);

        // ����Ļ�ϻ���һϵ���߶Σ��γɶ�̬Ч��
        y1 = 127, y2 = 0;
        while (y1 > 0)
        {
            SSD1306_DrawLine(y1, 0, y2, 63, 1);
            SSD1306_UpdateScreen();
            y1 -= 2;
            y2 += 2;
        }
        SYS_Delay(1000);
    }
}

void INT0_int (void) interrupt 0      //���ж�ʱ�Ѿ������־
{
    EXTI_Int0_SetTrigByFall;
}
//#define EXTI_Int0_SetTrigByFall    SBIT_SET(IT0)      // �½��ش���
//#define EXTI_Int0_SetTrigByBoth    SBIT_RESET(IT0)    // �������½��ض�����
//#define EXTI_Int1_SetTrigByFall    SBIT_SET(IT1)      // �½��ش���
//#define EXTI_Int1_SetTrigByBoth    SBIT_RESET(IT1)    // �������½��ض�����

//#define EXTI_VectInt0       0      // �ⲿ�ж�0����
//#define EXTI_VectTimer0     1      // ��ʱ��0�ж�����
//#define EXTI_VectInt1       2      // �ⲿ�ж�1����
//#define EXTI_VectTimer1     3      // ��ʱ��1�ж�����
//#define EXTI_VectUART1      4      // UART1�ж�����
//#define EXTI_VectADC        5      // ADC�ж�����
//#define EXTI_VectLowVoltDect 6     // �͵�ѹ����ж�����
//#define EXTI_VectPCA        7      // PCA�ж�����
//#define EXTI_VectUART2      8      // UART2�ж�����
//#define EXTI_VectSPI        9      // SPI�ж�����
//#define EXTI_VectInt2       10     // �ⲿ�ж�2����
//#define EXTI_VectInt3       11     // �ⲿ�ж�3����
//#define EXTI_VectTimer2     12     // ��ʱ��2�ж�����
//#define EXTI_VectUser       13     // �û��Զ����ж�����������Cx51����hack
//#define EXTI_VectInt4       16     // �ⲿ�ж�4����
//#define EXTI_VectUART3      17     // UART3�ж�����
//#define EXTI_VectUART4      18     // UART4�ж�����
//#define EXTI_VectTimer3     19     // ��ʱ��3�ж�����
//#define EXTI_VectTimer4     20     // ��ʱ��4�ж�����
//#define EXTI_VectCMP        21     // �Ƚ����ж�����
 
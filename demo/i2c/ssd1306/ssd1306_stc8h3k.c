// 引入需要的头文件，这些文件定义了后续用到的一些函数和变量
#include "fw_hal.h"
#include "ssd1306.h"



// 定义I2C初始化函数
void I2C_Init(void)
{
    // 设置I2C为主机模式
    I2C_SetWorkMode(I2C_WorkMode_Master);

    // 设置I2C时钟分频器，以生成I2C通信时钟
    // I2C时钟 = FOSC / 2 / (__prescaler__ * 2 + 4)
    // SSD1306最大支持的I2C时钟频率为1.3MHz，超过这个值可能导致显示失败
    I2C_SetClockPrescaler(0x10);

    // 切换到备用I2C端口
    I2C_SetPort(I2C_AlterPort_P32_P33);

    // 开启I2C
    I2C_SetEnabled(HAL_State_ON);
}

// 定义GPIO初始化函数
void GPIO_Init(void)
{
    // 设置SDA端口（P3.3）为输入/输出端口
    GPIO_P3_SetMode(GPIO_Pin_3, GPIO_Mode_InOut_QBD);

    // 设置SCL端口（P3.2）为推挽输出端口
    GPIO_P3_SetMode(GPIO_Pin_2, GPIO_Mode_Output_PP);
		// 设置P2.2到P2.7为推挽输出模式
    GPIO_P2_SetMode(GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_Output_PP);
}
static uint16_t counter = 0; // 静态变量计数器，用于计算定时器中断的次数
// LED流水灯状态
uint8_t led_state = 0;

// Timer0 的中断服务例程
INTERRUPT(Timer0_Routine, EXTI_VectTimer0)
{
    counter++; // 计数器增加
    if (counter == 1000) // 当计数器到达1000时
    {
        counter = 0; // 重置计数器
			// 改变LED状态
    P2 = (P2 & 0x03) | (led_state << 2);
    led_state = (led_state + 1) % 64;  // 流水灯状态，依次亮起每个LED
        /* 位设置，P32输出高电平，P33输出低电平 */
//        P22 = SET;
//        P23 = RESET;
//				P24 = SET;
//        P25 = RESET;
//				P26 = SET;
//        P27 = RESET;
        /**
         * 或者通过字节设置
         * P3 = P3 & 0xF3 | 0x08;
        */
		}
}
// 主程序
int main(void)
{
	
    int y1, y2;
    uint8_t d1, d2;

    // 设置系统时钟
    SYS_SetClock();

    // 初始化GPIO
    GPIO_Init();

    // 初始化I2C
    I2C_Init();

    // 初始化SSD1306显示屏
    SSD1306_Init();
	
	
 // 配置Timer0：1T模式，16位自动重载模式，频率1000Hz
    TIM_Timer0_Config(HAL_State_ON, TIM_TimerMode_16BitAuto, 1000);
    // 开启Timer0的中断
    EXTI_Timer0_SetIntState(HAL_State_ON);
    // 设置Timer0中断优先级为高
    EXTI_Timer0_SetIntPriority(EXTI_IntPriority_High);
    // 开启全局中断
    EXTI_Global_SetIntState(HAL_State_ON);
    // 设置Timer0为运行状态
    TIM_Timer0_SetRunState(HAL_State_ON);
    // 主循环
    while(1)
    {
			
			
       
        
        // 在屏幕上画一个矩形
        SSD1306_DrawLine(0,   0, 127,  0, 1);
        SSD1306_DrawLine(0,   0,   0, 63, 1);
        SSD1306_DrawLine(127, 0, 127, 63, 1);
        SSD1306_DrawLine(0,  63, 127, 63, 1);

        // 更新显示屏
        SSD1306_UpdateScreen();

        // 延时1秒
        SYS_Delay(1000);
			/* 位设置，P32输出低电平，P33输出高电平 */
        P22 = RESET;
        P23 = SET;
        /**
         * 或者通过字节设置
         * P3 = P3 & 0xF3 | 0x04;
        */
        // 清屏
        SSD1306_Fill(0);

        // 反转显示
        SSD1306_ToggleInvert();

        // 更新显示屏
        SSD1306_UpdateScreen();

        // 延时1秒
        SYS_Delay(1000);

        // 反转显示
        SSD1306_ToggleInvert();

        // 更新显示屏
        SSD1306_UpdateScreen();

        // 延时1秒
        SYS_Delay(1000);

        // 在屏幕上绘制一系列线段，形成动态效果
        y1 = 64, y2 = 0;
        while (y1 > 0)
        {
            SSD1306_DrawLine(0, y1, 127, y2, 1);
            SSD1306_UpdateScreen();
            y1 -= 2;
            y2 += 2;
        }
        SYS_Delay(1000);

        // 清屏
        SSD1306_Fill(0);

        // 在屏幕上绘制一系列线段，形成动态效果
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

void INT0_int (void) interrupt 0      //进中断时已经清除标志
{
    EXTI_Int0_SetTrigByFall;
}
//#define EXTI_Int0_SetTrigByFall    SBIT_SET(IT0)      // 下降沿触发
//#define EXTI_Int0_SetTrigByBoth    SBIT_RESET(IT0)    // 上升和下降沿都触发
//#define EXTI_Int1_SetTrigByFall    SBIT_SET(IT1)      // 下降沿触发
//#define EXTI_Int1_SetTrigByBoth    SBIT_RESET(IT1)    // 上升和下降沿都触发

//#define EXTI_VectInt0       0      // 外部中断0向量
//#define EXTI_VectTimer0     1      // 定时器0中断向量
//#define EXTI_VectInt1       2      // 外部中断1向量
//#define EXTI_VectTimer1     3      // 定时器1中断向量
//#define EXTI_VectUART1      4      // UART1中断向量
//#define EXTI_VectADC        5      // ADC中断向量
//#define EXTI_VectLowVoltDect 6     // 低电压检测中断向量
//#define EXTI_VectPCA        7      // PCA中断向量
//#define EXTI_VectUART2      8      // UART2中断向量
//#define EXTI_VectSPI        9      // SPI中断向量
//#define EXTI_VectInt2       10     // 外部中断2向量
//#define EXTI_VectInt3       11     // 外部中断3向量
//#define EXTI_VectTimer2     12     // 定时器2中断向量
//#define EXTI_VectUser       13     // 用户自定义中断向量，用于Cx51向量hack
//#define EXTI_VectInt4       16     // 外部中断4向量
//#define EXTI_VectUART3      17     // UART3中断向量
//#define EXTI_VectUART4      18     // UART4中断向量
//#define EXTI_VectTimer3     19     // 定时器3中断向量
//#define EXTI_VectTimer4     20     // 定时器4中断向量
//#define EXTI_VectCMP        21     // 比较器中断向量
 
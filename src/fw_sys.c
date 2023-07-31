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

#include "fw_sys.h"
/**
 * 这是对一秒内指令周期的大致估计，可能会在不同的编译器甚至不同版本的编译器中有所不同，
 * 如果你发现延迟过慢或过快，可以调整它。
*/

/**
 * An approximate estimate of instruction cycles in one second, may vary in
 * different compilers even differnt versions, adjust it if you find the
 * delay too slow or fast.
*/
#if defined (__SDCC_SYNTAX_FIX)
    #define __CLK_REF 10000
#elif defined (SDCC) || defined (__SDCC)
    #define __CLK_REF 9000
#elif defined __CX51__ || defined __C51__
    #define __CLK_REF 10000
#endif

__CODE uint16_t ticks_ms    = (__CONF_FOSC / ((__CONF_CLKDIV == 0)? 1 : __CONF_CLKDIV) / __CLK_REF);
__CODE uint8_t  ticks_us    = (__CONF_FOSC / ((__CONF_CLKDIV == 0)? 1 : __CONF_CLKDIV) / __CLK_REF / 1000);

/**
 * 修改系统时钟
 * - 仅适用于SDCC编译器。
 * - 对于Keil C51编译器，使用STC-ISP进行IRC修整会更准确
 * - 在main()函数开始时调用这个函数
*/

/**
 * Change system clock
 * - For SDCC only.
 * - For Keil C51, trim IRC in STC-ISP will be more accurate
 * - Invoke this in the beginning of main()
*/
void SYS_SetClock(void)
{
#if defined (SDCC) || defined (__SDCC)
    uint16_t i = 0; uint8_t j = 5;
    P_SW2 = 0x80;
    if (CLKDIV != (__CONF_CLKDIV))
    {
        CLKDIV = (__CONF_CLKDIV);
        do { //更改时钟后等待一段时间，否则可能会阻塞主进程 Wait a while after clock changed, or it may block the main process
            while (--i);
        } while (--j);
    }
    P_SW2 = 0x00;
    SYS_SetFOSC(__CONF_IRCBAND, __CONF_VRTRIM, __CONF_IRTRIM, __CONF_LIRTRIM);
    while (--i); // Wait
#endif
}

// 定义了一个函数，名为SYS_TrimClock，接受两个无符号8位整数参数
void SYS_TrimClock(uint8_t vrtrim, uint8_t irtrim)
{
    uint16_t i = 0;  // 定义一个无符号16位整数变量i，并初始化为0
    SYS_SetFOSC(__CONF_IRCBAND, vrtrim, irtrim, __CONF_LIRTRIM); // 调用SYS_SetFOSC函数，传入四个参数
    while (--i); // 当i递减后不为0时，继续等待
}

// 定义了一个函数，名为SYS_Delay，接受一个无符号16位整数参数
void SYS_Delay(uint16_t t)
{
    uint16_t i; // 定义一个无符号16位整数变量i
    do
    {
        i = ticks_ms;  // 将ticks_ms的值赋给i
        while (--i);   // 当i递减后不为0时，继续等待
    } while (--t);  // 当t递减后不为0时，继续执行上述do...while循环
}

// 定义了一个函数，名为SYS_DelayUs，接受一个无符号16位整数参数
void SYS_DelayUs(uint16_t t)
{
    uint8_t i;  // 定义一个无符号8位整数变量i
    do
    {
        i = ticks_us; // 将ticks_us的值赋给i
        while (--i);  // 当i递减后不为0时，继续等待
    } while (--t);   // 当t递减后不为0时，继续执行上述do...while循环
}

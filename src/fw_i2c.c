#include "fw_i2c.h"

// 定义I2C写入函数
// devAddr: 设备地址
// memAddr: 存储器地址
// dat: 数据
// size: 数据大小
uint8_t I2C_Write(uint8_t devAddr, uint8_t memAddr, uint8_t *dat, uint16_t size)
{
    SFRX_ON();  // 打开特殊功能寄存器的交换功能
    I2C_MasterStart();  // I2C主设备启动信号
    I2C_MasterSendData(devAddr & 0xFE);  // 发送设备地址
    I2C_MasterRxAck();  // 接收应答信号
    I2C_MasterSendData(memAddr);  // 发送存储器地址
    I2C_MasterRxAck();  // 接收应答信号
    while(size--)  // 循环发送数据
    {
        I2C_MasterSendData(*dat++);  // 发送数据
        I2C_MasterRxAck();  // 接收应答信号
    }
    I2C_MasterStop();  // I2C主设备停止信号
    SFRX_OFF();  // 关闭特殊功能寄存器的交换功能
    return HAL_OK;  // 返回操作成功状态
}

// 定义I2C读取函数
// devAddr: 设备地址
// memAddr: 存储器地址
// buf: 数据缓存
// size: 数据大小
uint8_t I2C_Read(uint8_t devAddr, uint8_t memAddr, uint8_t *buf, uint16_t size)
{
    SFRX_ON();  // 打开特殊功能寄存器的交换功能
    I2C_MasterStart();  // I2C主设备启动信号
    I2C_MasterSendData(devAddr & 0xFE);  // 发送设备地址
    I2C_MasterRxAck();  // 接收应答信号
    I2C_MasterSendData(memAddr);  // 发送存储器地址
    I2C_MasterRxAck();  // 接收应答信号
    I2C_MasterStart();  // I2C主设备再次启动信号，表示读操作
    I2C_MasterSendData(devAddr | 0x01);  // 发送设备地址
    I2C_MasterRxAck();  // 接收应答信号
    while(size--)  // 循环接收数据
    {
        I2C_SendMasterCmd(I2C_MasterCmd_Recv);  // I2C主设备接收命令
        *buf++ = I2CRXD;  // 读取数据
        if (size == 0)  // 如果是最后一位数据，发送NACK
        {
            I2C_MasterNAck();
        }
        else  // 如果不是最后一位数据，发送ACK
        {
            I2C_MasterAck();
        }
    }
    I2C_MasterStop();  // I2C主设备停止信号
    SFRX_OFF();  // 关闭特殊功能寄存器的交换功能
    return HAL_OK;  // 返回操作成功状态
}

// 定义16位地址的I2C写入函数
// devAddr: 设备地址
// memAddr: 存储器地址
// dat: 数据
// size: 数据大小
uint8_t I2C_Write16BitAddr(uint8_t devAddr, uint16_t memAddr, uint8_t *dat, uint16_t size)
{
    // 其实这里和I2C_Write类似，区别在于地址是16位，所以需要发送两次
    // 具体的解析和注释与上面的I2C_Write类似，不再赘述
		SFRX_ON();
    I2C_MasterStart();
    I2C_MasterSendData(devAddr & 0xFE);
    I2C_MasterRxAck();
    I2C_MasterSendData(memAddr >> 8);
    I2C_MasterRxAck();
    I2C_MasterSendData(memAddr & 0xFF);
    I2C_MasterRxAck();
    while(size--)
    {
        I2C_MasterSendData(*dat++);
        I2C_MasterRxAck();
    }
    I2C_MasterStop();
    SFRX_OFF();
    return HAL_OK;
}

// 定义16位地址的I2C读取函数
// devAddr: 设备地址
// memAddr: 存储器地址
// buf: 数据缓存
// size: 数据大小
uint8_t I2C_Read16BitAddr(uint8_t devAddr, uint16_t memAddr, uint8_t *buf, uint16_t size)
{
    // 这里和I2C_Read类似，区别在于地址是16位，所以需要发送两次
    // 具体的解析和注释与上面的I2C_Read类似，不再赘述
		SFRX_ON();
    I2C_MasterStart();
    I2C_MasterSendData(devAddr & 0xFE);
    I2C_MasterRxAck();
    I2C_MasterSendData(memAddr >> 8);
    I2C_MasterRxAck();
    I2C_MasterSendData(memAddr & 0xFF);
    I2C_MasterRxAck();
    I2C_MasterStart();
    I2C_MasterSendData(devAddr | 0x01);
    I2C_MasterRxAck();
    while(size--)
    {
        I2C_SendMasterCmd(I2C_MasterCmd_Recv);
        *buf++ = I2CRXD;
        if (size == 0)
        {
            I2C_MasterNAck();
        }
        else
        {
            I2C_MasterAck();
        }
    }
    I2C_MasterStop();
    SFRX_OFF();
    return HAL_OK;
}

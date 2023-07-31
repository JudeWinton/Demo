#include "fw_i2c.h"

// ����I2Cд�뺯��
// devAddr: �豸��ַ
// memAddr: �洢����ַ
// dat: ����
// size: ���ݴ�С
uint8_t I2C_Write(uint8_t devAddr, uint8_t memAddr, uint8_t *dat, uint16_t size)
{
    SFRX_ON();  // �����⹦�ܼĴ����Ľ�������
    I2C_MasterStart();  // I2C���豸�����ź�
    I2C_MasterSendData(devAddr & 0xFE);  // �����豸��ַ
    I2C_MasterRxAck();  // ����Ӧ���ź�
    I2C_MasterSendData(memAddr);  // ���ʹ洢����ַ
    I2C_MasterRxAck();  // ����Ӧ���ź�
    while(size--)  // ѭ����������
    {
        I2C_MasterSendData(*dat++);  // ��������
        I2C_MasterRxAck();  // ����Ӧ���ź�
    }
    I2C_MasterStop();  // I2C���豸ֹͣ�ź�
    SFRX_OFF();  // �ر����⹦�ܼĴ����Ľ�������
    return HAL_OK;  // ���ز����ɹ�״̬
}

// ����I2C��ȡ����
// devAddr: �豸��ַ
// memAddr: �洢����ַ
// buf: ���ݻ���
// size: ���ݴ�С
uint8_t I2C_Read(uint8_t devAddr, uint8_t memAddr, uint8_t *buf, uint16_t size)
{
    SFRX_ON();  // �����⹦�ܼĴ����Ľ�������
    I2C_MasterStart();  // I2C���豸�����ź�
    I2C_MasterSendData(devAddr & 0xFE);  // �����豸��ַ
    I2C_MasterRxAck();  // ����Ӧ���ź�
    I2C_MasterSendData(memAddr);  // ���ʹ洢����ַ
    I2C_MasterRxAck();  // ����Ӧ���ź�
    I2C_MasterStart();  // I2C���豸�ٴ������źţ���ʾ������
    I2C_MasterSendData(devAddr | 0x01);  // �����豸��ַ
    I2C_MasterRxAck();  // ����Ӧ���ź�
    while(size--)  // ѭ����������
    {
        I2C_SendMasterCmd(I2C_MasterCmd_Recv);  // I2C���豸��������
        *buf++ = I2CRXD;  // ��ȡ����
        if (size == 0)  // ��������һλ���ݣ�����NACK
        {
            I2C_MasterNAck();
        }
        else  // ����������һλ���ݣ�����ACK
        {
            I2C_MasterAck();
        }
    }
    I2C_MasterStop();  // I2C���豸ֹͣ�ź�
    SFRX_OFF();  // �ر����⹦�ܼĴ����Ľ�������
    return HAL_OK;  // ���ز����ɹ�״̬
}

// ����16λ��ַ��I2Cд�뺯��
// devAddr: �豸��ַ
// memAddr: �洢����ַ
// dat: ����
// size: ���ݴ�С
uint8_t I2C_Write16BitAddr(uint8_t devAddr, uint16_t memAddr, uint8_t *dat, uint16_t size)
{
    // ��ʵ�����I2C_Write���ƣ��������ڵ�ַ��16λ��������Ҫ��������
    // ����Ľ�����ע���������I2C_Write���ƣ�����׸��
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

// ����16λ��ַ��I2C��ȡ����
// devAddr: �豸��ַ
// memAddr: �洢����ַ
// buf: ���ݻ���
// size: ���ݴ�С
uint8_t I2C_Read16BitAddr(uint8_t devAddr, uint16_t memAddr, uint8_t *buf, uint16_t size)
{
    // �����I2C_Read���ƣ��������ڵ�ַ��16λ��������Ҫ��������
    // ����Ľ�����ע���������I2C_Read���ƣ�����׸��
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

/**
  ****************************************************************************************************
  * @file    	imu901.c
  * @author		����ԭ���Ŷ�(ALIENTEK)
  * @version    V1.0
  * @date		2020-04-17
  * @brief   	imu901ģ��ָ������
  * @license   	Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
  ****************************************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����ԭ�� STM32������
  * ������Ƶ:www.yuanzige.com
  * ������̳:www.openedv.com
  * ��˾��ַ:www.alientek.com
  * �����ַ:openedv.taobao.com
  *
  * �޸�˵��
  * V1.0 2020-04-17
  * ��һ�η���
  *
  ****************************************************************************************************
  */


#include "./IMU901/imu901.h"
#include "usart.h"



/* ģ�������ϴ�������(���ڽ�����) */
attitude_t		attitude;		/*!< ��̬�� */
quaternion_t	quaternion;
gyroAcc_t 		gyroAccData;
mag_t			magData;
baro_t			baroData;
ioStatus_t		iostatus;

/* ģ��Ĵ�������ֵ */
regValue_t  	imu901Param;

/* ���ڽ��ս����ɹ������ݰ� */
atkp_t 			rxPacket;


/* �����Ǽ��ٶ����̱� */
const uint16_t gyroFsrTable[4] = {250, 500, 1000, 2000};
const uint8_t  accFsrTable[4] = {2, 4, 8, 16};






/**
  * @brief  ���մ������ݽ������
  */
static enum
{
    waitForStartByte1,
    waitForStartByte2,
    waitForMsgID,
    waitForDataLength,
    waitForData,
    waitForChksum1,
} rxState = waitForStartByte1;


/**
  * @brief  imu901ģ�鴮�����ݽ��������ڽ��յ�ÿһ�������贫�봦��
  *	@note	�˺�����Ҫʵʱ����
  * @param  ch: ���ڽ��յĵ�������
  * @retval uint8_t: 0 �ް� 1 ��Ч��
  */
uint8_t imu901_unpack(uint8_t ch)
{
    static uint8_t cksum = 0, dataIndex = 0;

    switch (rxState)
    {
        case waitForStartByte1:
            if (ch == UP_BYTE1)
            {
                rxState = waitForStartByte2;
                rxPacket.startByte1 = ch;
            }

            cksum = ch;
            break;

        case waitForStartByte2:
            if (ch == UP_BYTE2 || ch == UP_BYTE2_ACK)
            {
                rxState = waitForMsgID;
                rxPacket.startByte2 = ch;
            }
            else
            {
                rxState = waitForStartByte1;
            }

            cksum += ch;
            break;

        case waitForMsgID:
            rxPacket.msgID = ch;
            rxState = waitForDataLength;
            cksum += ch;
            break;

        case waitForDataLength:
            if (ch <= ATKP_MAX_DATA_SIZE)
            {
                rxPacket.dataLen = ch;
                dataIndex = 0;
                rxState = (ch > 0) ? waitForData : waitForChksum1;	/*ch=0,���ݳ���Ϊ0��У��1*/
                cksum += ch;
            }
            else
            {
                rxState = waitForStartByte1;
            }

            break;

        case waitForData:
            rxPacket.data[dataIndex] = ch;
            dataIndex++;
            cksum += ch;

            if (dataIndex == rxPacket.dataLen)
            {
                rxState = waitForChksum1;
            }

            break;

        case waitForChksum1:
            if (cksum == ch)	/*!< У׼��ȷ����1 */
            {
                rxPacket.checkSum = cksum;

                return 1;
            }
            else	/*!< У����� */
            {
                rxState = waitForStartByte1;
            }

            rxState = waitForStartByte1;
            break;

        default:
            rxState = waitForStartByte1;
            break;
    }

    return 0;
}



/**
  * @brief  ATKP���ݰ�����
  * @param  packet: atkp���ݰ�
  * @retval None
  */
void atkpParsing(atkp_t *packet)
{
    /* ��̬�� */
    if (packet->msgID == UP_ATTITUDE)
    {
        int16_t data = (int16_t) (packet->data[1] << 8) | packet->data[0];
        attitude.roll = (float) data / 32768 * 180;

        data = (int16_t) (packet->data[3] << 8) | packet->data[2];
        attitude.pitch = (float) data / 32768 * 180;

        data = (int16_t) (packet->data[5] << 8) | packet->data[4];
        attitude.yaw = (float) data / 32768 * 180;
    }

    /* ��Ԫ�� */
    else if (packet->msgID == UP_QUAT)
    {
        int16_t data = (int16_t) (packet->data[1] << 8) | packet->data[0];
        quaternion.q0 = (float) data / 32768;

        data = (int16_t) (packet->data[3] << 8) | packet->data[2];
        quaternion.q1 = (float) data / 32768;

        data = (int16_t) (packet->data[5] << 8) | packet->data[4];
        quaternion.q2 = (float) data / 32768;

        data = (int16_t) (packet->data[7] << 8) | packet->data[6];
        quaternion.q3 = (float) data / 32768;
    }

    /* �����Ǽ��ٶ����� */
    else if (packet->msgID == UP_GYROACCDATA)
    {
        gyroAccData.acc[0] = (int16_t) (packet->data[1] << 8) | packet->data[0];
        gyroAccData.acc[1] = (int16_t) (packet->data[3] << 8) | packet->data[2];
        gyroAccData.acc[2] = (int16_t) (packet->data[5] << 8) | packet->data[4];

        gyroAccData.gyro[0] = (int16_t) (packet->data[7] << 8) | packet->data[6];
        gyroAccData.gyro[1] = (int16_t) (packet->data[9] << 8) | packet->data[8];
        gyroAccData.gyro[2] = (int16_t) (packet->data[11] << 8) | packet->data[10];

        gyroAccData.faccG[0] = (float)gyroAccData.acc[0] / 32768 * accFsrTable[imu901Param.accFsr]; 		/*!< 4����4G����λ�����úõ����� */
        gyroAccData.faccG[1] = (float)gyroAccData.acc[1] / 32768 * accFsrTable[imu901Param.accFsr];
        gyroAccData.faccG[2] = (float)gyroAccData.acc[2] / 32768 * accFsrTable[imu901Param.accFsr];

        gyroAccData.fgyroD[0] = (float)gyroAccData.gyro[0] / 32768 * gyroFsrTable[imu901Param.gyroFsr]; 	/*!< 2000����2000��/S����λ�����úõ����� */
        gyroAccData.fgyroD[1] = (float)gyroAccData.gyro[1] / 32768 * gyroFsrTable[imu901Param.gyroFsr];
        gyroAccData.fgyroD[2] = (float)gyroAccData.gyro[2] / 32768 * gyroFsrTable[imu901Param.gyroFsr];
    }

    /* �ų����� */
    else if (packet->msgID == UP_MAGDATA)
    {
        magData.mag[0] = (int16_t) (packet->data[1] << 8) | packet->data[0];
        magData.mag[1] = (int16_t) (packet->data[3] << 8) | packet->data[2];
        magData.mag[2] = (int16_t) (packet->data[5] << 8) | packet->data[4];

        int16_t data = (int16_t) (packet->data[7] << 8) | packet->data[6];
        magData.temp = (float) data / 100;
    }

    /* ��ѹ������ */
    else if (packet->msgID == UP_BARODATA)
    {
        baroData.pressure = (int32_t) (packet->data[3] << 24) | (packet->data[2] << 16) |
                            (packet->data[1] << 8) | packet->data[0];

        baroData.altitude = (int32_t) (packet->data[7] << 24) | (packet->data[6] << 16) |
                            (packet->data[5] << 8) | packet->data[4];

        int16_t data = (int16_t) (packet->data[9] << 8) | packet->data[8];
        baroData.temp = (float) data / 100;
    }

    /* �˿�״̬���� */
    else if (packet->msgID == UP_D03DATA)
    {
        iostatus.d03data[0] = (uint16_t) (packet->data[1] << 8) | packet->data[0];
        iostatus.d03data[1] = (uint16_t) (packet->data[3] << 8) | packet->data[2];
        iostatus.d03data[2] = (uint16_t) (packet->data[5] << 8) | packet->data[4];
        iostatus.d03data[3] = (uint16_t) (packet->data[7] << 8) | packet->data[6];
    }
}


/**
  * @brief  д�Ĵ���
  * @param  reg: �Ĵ����б��ַ
  * @param  data: ����
  * @param  datalen: ���ݵĳ���ֻ���� 1��2
  * @retval None
  */
void atkpWriteReg(enum regTable reg, uint16_t data, uint8_t datalen)
{
    uint8_t buf[7];

    buf[0] = 0x55;
    buf[1] = 0xAF;
    buf[2] = reg;
    buf[3] = datalen; 	/*!< datalenֻ����1����2 */
    buf[4] = data;

    if (datalen == 2)
    {
        buf[5] = data >> 8;
        buf[6] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5];
        imu901_uart_send(buf, 7);
    }
    else
    {
        buf[5] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4];
        imu901_uart_send(buf, 6);
    }
}


/**
  * @brief  ���Ͷ��Ĵ�������
  * @param  reg: �Ĵ����б��ַ
  * @retval None
  */
static void atkpReadRegSend(enum regTable reg)
{
    uint8_t buf[7];

    buf[0] = 0x55;
    buf[1] = 0xAF;
    buf[2] = reg | 0x80;
    buf[3] = 1;
    buf[4] = 0;
    buf[5] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4];
    imu901_uart_send(buf, 6);
}



/**
  * @brief  ���Ĵ���
  * @param  reg: �Ĵ�����ַ
  * @param  data: ��ȡ��������
  * @retval uint8_t: 0��ȡʧ�ܣ���ʱ�� 1��ȡ�ɹ�
  */
uint8_t atkpReadReg(enum regTable reg, int16_t *data)
{
    uint8_t ch;
    uint16_t timeout = 0;

    atkpReadRegSend(reg);

    while (1)
    {
        if (imu901_uart_receive(&ch, 1)) 	/*!< ��ȡ����fifoһ���ֽ� */
        {
            if (imu901_unpack(ch)) 			/*!< ����Ч���ݰ� */
            {
                if (rxPacket.startByte2 == UP_BYTE2) /*!< �����ϴ��� */
                {
                    atkpParsing(&rxPacket);
                }
                else if (rxPacket.startByte2 == UP_BYTE2_ACK) /*!< ���Ĵ���Ӧ��� */
                {
                    if (rxPacket.dataLen == 1)
                        *data = rxPacket.data[0];
                    else if (rxPacket.dataLen == 2)
                        *data = (rxPacket.data[1] << 8) | rxPacket.data[0];

                    return 1;
                }
            }
        }
        else
        {
            HAL_Delay(1);
            timeout++;

            if (timeout > 200) /*!< ��ʱ���� */
                return 0;
        }
    }
}



/**
  * @brief  ģ���ʼ��
  * @param  None
  * @retval None
  */
void imu901_init(void)
{
    int16_t data;

    /**
      *	 д��Ĵ������������ԣ�
      *	 �����ṩд���������ӣ��û���������д��һЩĬ�ϲ�����
      *  �������Ǽ��ٶ����̡������ش����ʡ�PWM����ȡ�
      */
    atkpWriteReg(REG_GYROFSR, 3, 1);
    atkpWriteReg(REG_ACCFSR, 1, 1);
	atkpWriteReg(REG_SAVE, 0, 1); 	/* ���ͱ��������ģ���ڲ�Flash������ģ����粻���� */

    /* �����Ĵ������������ԣ� */
    atkpReadReg(REG_GYROFSR, &data);
    imu901Param.gyroFsr = data;

    atkpReadReg(REG_ACCFSR, &data);
    imu901Param.accFsr = data;

    atkpReadReg(REG_GYROBW, &data);
    imu901Param.gyroBW = data;

    atkpReadReg(REG_ACCBW, &data);
    imu901Param.accBW = data;
}










/*******************************END OF FILE************************************/




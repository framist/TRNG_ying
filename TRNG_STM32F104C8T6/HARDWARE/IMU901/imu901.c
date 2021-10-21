/**
  ****************************************************************************************************
  * @file    	imu901.c
  * @author		正点原子团队(ALIENTEK)
  * @version    V1.0
  * @date		2020-04-17
  * @brief   	imu901模块指令驱动
  * @license   	Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
  ****************************************************************************************************
  * @attention
  *
  * 实验平台:正点原子 STM32开发板
  * 在线视频:www.yuanzige.com
  * 技术论坛:www.openedv.com
  * 公司网址:www.alientek.com
  * 购买地址:openedv.taobao.com
  *
  * 修改说明
  * V1.0 2020-04-17
  * 第一次发布
  * 
  * framist 修改
  *
  ****************************************************************************************************
  */


#include "./IMU901/imu901.h"
#include "usart.h"
#include "./HOSTPC/hostPC.h"



/* 模块主动上传的数据(串口解析后) */
attitude_t		attitude;		/*!< 姿态角 */
quaternion_t	quaternion;
gyroAcc_t 		gyroAccData;
mag_t			magData;
baro_t			baroData;
ioStatus_t		iostatus;

/* 模块寄存器参数值 */
regValue_t  	imu901Param;

/* 串口接收解析成功的数据包 */
atkp_t 			rxPacket;


/* 陀螺仪加速度量程表 */
const uint16_t gyroFsrTable[4] = {250, 500, 1000, 2000};
const uint8_t  accFsrTable[4] = {2, 4, 8, 16};


/**
  * @brief  接收串口数据解包流程
  */
static enum {
    waitForStartByte1,
    waitForStartByte2,
    waitForMsgID,
    waitForDataLength,
    waitForData,
    waitForChksum1,
} rxState = waitForStartByte1;


/**
  * @brief  imu901模块串口数据解析（串口接收的每一个数据需传入处理）
  *	@note	此函数需要实时调用
  * @param  ch: 串口接收的单个数据
  * @retval uint8_t: 0 无包 1 有效包
  */
uint8_t imu901_unpack(uint8_t ch)
{
    static uint8_t cksum = 0, dataIndex = 0;

    switch (rxState) {
        case waitForStartByte1:
            if (ch == UP_BYTE1) {
                rxState = waitForStartByte2;
                rxPacket.startByte1 = ch;
            }

            cksum = ch;
            break;

        case waitForStartByte2:
            if (ch == UP_BYTE2 || ch == UP_BYTE2_ACK) {
                rxState = waitForMsgID;
                rxPacket.startByte2 = ch;
            } else {
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
            if (ch <= ATKP_MAX_DATA_SIZE) {
                rxPacket.dataLen = ch;
                dataIndex = 0;
                rxState = (ch > 0) ? waitForData : waitForChksum1;	/*ch=0,数据长度为0，校验1*/
                cksum += ch;
            } else {
                rxState = waitForStartByte1;
            }

            break;

        case waitForData:
            rxPacket.data[dataIndex] = ch;
            dataIndex++;
            cksum += ch;

            if (dataIndex == rxPacket.dataLen) {
                rxState = waitForChksum1;
            }

            break;

        case waitForChksum1:
            if (cksum == ch)	/*!< 校准正确返回1 */
            {
                rxPacket.checkSum = cksum;

                return 1;
            }
            else	/*!< 校验错误 */
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
  * @brief  ATKP数据包解析
  * @param  packet: atkp数据包
  * @retval None
  */
void atkpParsing(atkp_t *packet) {
    /* 姿态角 */
    if (packet->msgID == UP_ATTITUDE) {
        int16_t data = (int16_t) (packet->data[1] << 8) | packet->data[0];
        attitude.roll = (float) data / 32768 * 180;

        data = (int16_t) (packet->data[3] << 8) | packet->data[2];
        attitude.pitch = (float) data / 32768 * 180;

        data = (int16_t) (packet->data[5] << 8) | packet->data[4];
        attitude.yaw = (float) data / 32768 * 180;
    }

    /* 四元数 */
    else if (packet->msgID == UP_QUAT) {
        int16_t data = (int16_t) (packet->data[1] << 8) | packet->data[0];
        quaternion.q0 = (float) data / 32768;

        data = (int16_t) (packet->data[3] << 8) | packet->data[2];
        quaternion.q1 = (float) data / 32768;

        data = (int16_t) (packet->data[5] << 8) | packet->data[4];
        quaternion.q2 = (float) data / 32768;

        data = (int16_t) (packet->data[7] << 8) | packet->data[6];
        quaternion.q3 = (float) data / 32768;
    }

    /* 陀螺仪加速度数据 */
    else if (packet->msgID == UP_GYROACCDATA) {
        gyroAccData.acc[0] = (int16_t) (packet->data[1] << 8) | packet->data[0];
        gyroAccData.acc[1] = (int16_t) (packet->data[3] << 8) | packet->data[2];
        gyroAccData.acc[2] = (int16_t) (packet->data[5] << 8) | packet->data[4];

        gyroAccData.gyro[0] = (int16_t) (packet->data[7] << 8) | packet->data[6];
        gyroAccData.gyro[1] = (int16_t) (packet->data[9] << 8) | packet->data[8];
        gyroAccData.gyro[2] = (int16_t) (packet->data[11] << 8) | packet->data[10];

        gyroAccData.faccG[0] = (float)gyroAccData.acc[0] / 32768 * accFsrTable[imu901Param.accFsr]; 		/*!< 4代表4G，上位机设置好的量程 */
        gyroAccData.faccG[1] = (float)gyroAccData.acc[1] / 32768 * accFsrTable[imu901Param.accFsr];
        gyroAccData.faccG[2] = (float)gyroAccData.acc[2] / 32768 * accFsrTable[imu901Param.accFsr];

        gyroAccData.fgyroD[0] = (float)gyroAccData.gyro[0] / 32768 * gyroFsrTable[imu901Param.gyroFsr]; 	/*!< 2000代表2000°/S，上位机设置好的量程 */
        gyroAccData.fgyroD[1] = (float)gyroAccData.gyro[1] / 32768 * gyroFsrTable[imu901Param.gyroFsr];
        gyroAccData.fgyroD[2] = (float)gyroAccData.gyro[2] / 32768 * gyroFsrTable[imu901Param.gyroFsr];
    }

    /* 磁场数据 */
    else if (packet->msgID == UP_MAGDATA) {
        magData.mag[0] = (int16_t) (packet->data[1] << 8) | packet->data[0];
        magData.mag[1] = (int16_t) (packet->data[3] << 8) | packet->data[2];
        magData.mag[2] = (int16_t) (packet->data[5] << 8) | packet->data[4];

        int16_t data = (int16_t) (packet->data[7] << 8) | packet->data[6];
        magData.temp = (float) data / 100;
    }

    /* 气压计数据 */
    else if (packet->msgID == UP_BARODATA) {
        baroData.pressure = (int32_t) (packet->data[3] << 24) | (packet->data[2] << 16) |
                            (packet->data[1] << 8) | packet->data[0];

        baroData.altitude = (int32_t) (packet->data[7] << 24) | (packet->data[6] << 16) |
                            (packet->data[5] << 8) | packet->data[4];

        int16_t data = (int16_t) (packet->data[9] << 8) | packet->data[8];
        baroData.temp = (float) data / 100;
    }

    /* 端口状态数据 */
    else if (packet->msgID == UP_D03DATA) {
        iostatus.d03data[0] = (uint16_t) (packet->data[1] << 8) | packet->data[0];
        iostatus.d03data[1] = (uint16_t) (packet->data[3] << 8) | packet->data[2];
        iostatus.d03data[2] = (uint16_t) (packet->data[5] << 8) | packet->data[4];
        iostatus.d03data[3] = (uint16_t) (packet->data[7] << 8) | packet->data[6];
    }
}


/**
  * @brief  写寄存器
  * @param  reg: 寄存器列表地址
  * @param  data: 数据
  * @param  datalen: 数据的长度只能是 1或2
  * @retval None
  */
void atkpWriteReg(enum regTable reg, uint16_t data, uint8_t datalen)
{
    uint8_t buf[7];

    buf[0] = 0x55;
    buf[1] = 0xAF;
    buf[2] = reg;
    buf[3] = datalen; 	/*!< datalen只能是1或者2 */
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
  * @brief  发送读寄存器命令
  * @param  reg: 寄存器列表地址
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
  * @brief  读寄存器
  * @param  reg: 寄存器地址
  * @param  data: 读取到的数据
  * @retval uint8_t: 0读取失败（超时） 1读取成功
  */
uint8_t atkpReadReg(enum regTable reg, int16_t *data)
{
    uint8_t ch;
    uint16_t timeout = 0;

    atkpReadRegSend(reg);

    while (1) {
        if (imu901_uart_receive(&ch, 1)) 	/*!< 获取串口fifo一个字节 */
        {
            if (imu901_unpack(ch)) 			/*!< 有有效数据包 */
            {
                if (rxPacket.startByte2 == UP_BYTE2) /*!< 主动上传包 */
                {
                    atkpParsing(&rxPacket);
                }
                else if (rxPacket.startByte2 == UP_BYTE2_ACK) /*!< 读寄存器应答包 */
                {
                    if (rxPacket.dataLen == 1)
                        *data = rxPacket.data[0];
                    else if (rxPacket.dataLen == 2)
                        *data = (rxPacket.data[1] << 8) | rxPacket.data[0];

                    return 1;
                }
            }
        }
        else {
            HAL_Delay(5);
            timeout++;

            if (timeout > 200) /*!< 超时返回 */
                return 0;
        }
    }
}



/**
  * @brief  模块初始化
  * @param  None
  * @retval uint8_t: 0读取失败（超时） 1读取成功
  */
int imu901_init(void)
{
    int16_t data;
    /**
      *	 写入寄存器参数（测试）
      *	 这里提供写入引用例子，用户可以在这写入一些默认参数，
      *  如陀螺仪加速度量程、带宽、回传速率、PWM输出等。
      */
    //量程都为最小
    atkpWriteReg(REG_GYROFSR, 0, 1); 
    atkpWriteReg(REG_ACCFSR, 0, 1);
    //D端口都为模拟输入
    atkpWriteReg(REG_D0MODE, 0, 1);
    atkpWriteReg(REG_D1MODE, 0, 1);
    atkpWriteReg(REG_D2MODE, 0, 1);
    atkpWriteReg(REG_D3MODE, 0, 1);
    //回传速率50hz (但是磁力计最快10Hz，气压计20Hz 先不管)
    atkpWriteReg(REG_UPRATE, 4, 1);
    //发送保存参数至模块内部Flash，否则模块掉电不保存
	atkpWriteReg(REG_SAVE, 0, 1); 	

    /* 读出寄存器参数（测试） */
    atkpReadReg(REG_GYROFSR, &data);
    imu901Param.gyroFsr = data;

    atkpReadReg(REG_ACCFSR, &data);
    imu901Param.accFsr = data;

    atkpReadReg(REG_GYROBW, &data);
    imu901Param.gyroBW = data;

    atkpReadReg(REG_ACCBW, &data);
    imu901Param.accBW = data;
    return 1;
}


void imu901_read_once(void){
    uint8_t ch;
    while (imu901_uart_receive(&ch, 1)) /*!< 获取串口fifo一个字节 */
    {
        if (imu901_unpack(ch)) /*!< 解析出有效数据包 */
        {
            if (rxPacket.startByte2 == UP_BYTE2) /*!< 主动上传的数据包 */
            {
                atkpParsing(&rxPacket);
            }
        }
    }
}

void imu901_print(void) {
    printf("\r\n");
    printf("姿态角[XYZ]: %-6.1f %-6.1f %-6.1f (°)\r\n", attitude.roll, attitude.pitch, attitude.yaw);
    printf("加速度[XYZ]: %-6.3f %-6.3f %-6.3f (g)\r\n", gyroAccData.faccG[0], gyroAccData.faccG[1], gyroAccData.faccG[2]);
    printf("角速度[XYZ]: %-6.1f %-6.1f %-6.1f (°/s)\r\n", gyroAccData.fgyroD[0], gyroAccData.fgyroD[1], gyroAccData.fgyroD[2]);
    printf("磁场[XYZ]: %-6d %-6d %-6d (uT)\r\n", magData.mag[0], magData.mag[1], magData.mag[2]);
    printf("气压:    %-6dPa   %-6dcm\r\n", baroData.pressure, baroData.altitude);
}


static uint32_t Entropy; //一次32bit熵
static void _get_entropy(atkp_t *packet) {
    int16_t data;

    /* 姿态角 3*2 bit */
    if (packet->msgID == UP_ATTITUDE) {
        data = (int16_t) (packet->data[1] << 8) | packet->data[0];
        Entropy = Entropy << 2 | (data & 0x0003);

        data = (int16_t) (packet->data[3] << 8) | packet->data[2];
        Entropy = Entropy << 2 | (data & 0x0003);

        data = (int16_t) (packet->data[5] << 8) | packet->data[4];
        Entropy = Entropy << 2 | (data & 0x0003);
    }

    /* 四元数 不使用 */

    /* 陀螺仪加速度数据 6*2 bit */
    else if (packet->msgID == UP_GYROACCDATA) {

        Entropy = Entropy << 2 | (packet->data[0] & 0x0003);
        Entropy = Entropy << 2 | (packet->data[2] & 0x0003);
        Entropy = Entropy << 2 | (packet->data[4] & 0x0003);

        Entropy = Entropy << 2 | (packet->data[6] & 0x0003);
        Entropy = Entropy << 2 | (packet->data[8] & 0x0003);
        Entropy = Entropy << 2 | (packet->data[10] & 0x0003);
    }

    /* 磁场数据 3bit 温度舍弃 */
    else if (packet->msgID == UP_MAGDATA) {
        Entropy = Entropy << 1 | (packet->data[0] & 0x0001);
        Entropy = Entropy << 1 | (packet->data[2] & 0x0001);
        Entropy = Entropy << 1 | (packet->data[4] & 0x0001);

        // data = (int16_t) (packet->data[7] << 8) | packet->data[6];
        // magData.temp = (float) data / 100;
    }

    /* 气压计数据 气压舍弃 2bit */
    else if (packet->msgID == UP_BARODATA) {
        // baroData.pressure = (int32_t) (packet->data[3] << 24) | (packet->data[2] << 16) |
        //                     (packet->data[1] << 8) | packet->data[0];

        Entropy = Entropy << 1 | (packet->data[4] & 0x0001);

        // 温度
        Entropy = Entropy << 1 | (packet->data[8] & 0x0001);

    }

    /* 端口状态数据 需配置为模拟输入 3 bit */
    else if (packet->msgID == UP_D03DATA) {
        Entropy = Entropy << 1 | (packet->data[0] & 0x0001);
        Entropy = Entropy << 1 | (packet->data[2] & 0x0001);
        Entropy = Entropy << 1 | (packet->data[4] & 0x0001);
        // Entropy = Entropy << 1 | (packet->data[6] & 0x0001);
    }
}

uint8_t imu901_read_print(void) {
    uint8_t ch;
    while (imu901_uart_receive(&ch, 1)) /*!< 获取串口fifo一个字节 */
    {
        if (imu901_unpack(ch)) /*!< 解析出有效数据包 */
        {
            if (rxPacket.startByte2 == UP_BYTE2) /*!< 主动上传的数据包 */
            {
                _get_entropy(&rxPacket);
            }
        }
    }
    // printf("%08x\n",Entropy);
    if(Entropy==0 || Entropy == UINT32_MAX){
        host_msg_analysis(HOST_MSG_TRNG_ERROR);
    }
    return Entropy;
    // uint8_t * ch1 = (uint8_t *) & Entropy;
    // uint8_t chEnd = '\0';
    // HAL_UART_Transmit(&huart1, ch1, 4, 0xffff);
    // HAL_UART_Transmit(&huart1, &chEnd, 1, 0xffff);
}

/*******************************END OF FILE************************************/

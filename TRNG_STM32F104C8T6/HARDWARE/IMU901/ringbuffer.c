/**
  ****************************************************************************************************
  * @file    	ringbuffer.c
  * @author		正点原子团队(ALIENTEK)
  * @version    V1.0
  * @date		2020-04-17
  * @brief   	环形型缓冲区操作驱动
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
  ****************************************************************************************************
  */


#include "./IMU901/ringbuffer.h"

/**
  * @brief  fifo初始化
  * @param  fifo: 实例
  * @param  buffer: fifo的缓冲区
  * @param  size: 缓冲区大小
  * @retval None
  */
void ringbuffer_init(ringbuffer_t *fifo, uint8_t *buffer, uint16_t size)
{
    fifo->buffer = buffer;
    fifo->size = size;
    fifo->in = 0;
    fifo->out = 0;
}


/**
  * @brief  获取已经使用的空间
  * @param  fifo: 实例
  * @retval uint16_t: 已使用个数
  */
uint16_t ringbuffer_getUsedSize(ringbuffer_t *fifo)
{
    if (fifo->in >= fifo->out)
        return (fifo->in - fifo->out);
    else
        return (fifo->size - fifo->out + fifo->in);
}


/**
  * @brief  获取未使用空间
  * @param  fifo: 实例
  * @retval uint16_t: 剩余个数
  */
uint16_t ringbuffer_getRemainSize(ringbuffer_t *fifo)
{
    return (fifo->size - ringbuffer_getUsedSize(fifo) - 1);
}


/**
  * @brief  FIFO是否为空
  * @param  fifo: 实例
  * @retval uint8_t: 1 为空 0 不为空（有数据）
  */
uint8_t ringbuffer_isEmpty(ringbuffer_t *fifo)
{
    return (fifo->in == fifo->out);
}


/**
  * @brief  发送数据到环形缓冲区（不检测剩余空间）
  * @param  fifo: 实例
  * @param  data: &#&
  * @param  len: &#&
  * @retval none
  */
void ringbuffer_in(ringbuffer_t *fifo, uint8_t *data, uint16_t len)
{
    for (int i = 0; i < len; i++)
    {
        fifo->buffer[fifo->in] = data[i];
        fifo->in = (fifo->in + 1) % fifo->size;
    }
}


/**
  * @brief  发送数据到环形缓冲区(带剩余空间检测，空间不足发送失败)
  * @param  fifo: 实例
  * @param  data: &#&
  * @param  len: &#&
  * @retval uint8_t: 0 成功 1失败（空间不足）
  */
uint8_t ringbuffer_in_check(ringbuffer_t *fifo, uint8_t *data, uint16_t len)
{
    uint16_t remainsize = ringbuffer_getRemainSize(fifo);

    if (remainsize < len) //空间不足
        return 1;

    ringbuffer_in(fifo, data, len);

    return 0;
}


/**
  * @brief  从环形缓冲区读取数据
  * @param  fifo: 实例
  * @param  buf: 存放数组
  * @param  len: 存放数组长度
  * @retval uint16_t: 实际读取个数
  */
uint16_t ringbuffer_out(ringbuffer_t *fifo, uint8_t *buf, uint16_t len)
{
    uint16_t remainToread = ringbuffer_getUsedSize(fifo);

    if (remainToread > len) {
        remainToread = len;
    }

    for (int i = 0; i < remainToread; i++) {
        buf[i] = fifo->buffer[fifo->out];
        fifo->out = (fifo->out + 1) % fifo->size;
    }

    return remainToread;
}







/*******************************END OF FILE************************************/



/**
  ****************************************************************************************************
  * @file    	ringbuffer.h
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

#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_


#include "main.h"


/*环形缓冲区数据结构*/
typedef struct
{
    uint8_t  *buffer;
    uint16_t size;
    uint16_t in;
    uint16_t out;
} ringbuffer_t;




void ringbuffer_init(ringbuffer_t *fifo, uint8_t *buffer, uint16_t size);

uint16_t ringbuffer_getUsedSize(ringbuffer_t *fifo);
uint16_t ringbuffer_getRemainSize(ringbuffer_t *fifo);
uint8_t ringbuffer_isEmpty(ringbuffer_t *fifo);

void ringbuffer_in(ringbuffer_t *fifo, uint8_t *data, uint16_t len);
uint8_t ringbuffer_in_check(ringbuffer_t *fifo, uint8_t *data, uint16_t len);
uint16_t ringbuffer_out(ringbuffer_t *fifo, uint8_t *buf, uint16_t len);



#endif /* _RINGBUFFER_H_ */

/*******************************END OF FILE************************************/



#ifndef _HOSTPC_H_
#define _HOSTPC_H_

#include "stm32f1xx_hal.h"


enum HOST_MSG {
    HOST_MSG_NULL,        // 无消息
    HOST_MSG_ACK,         // Acknowledgment OK
    HOST_MSG_NAK,         // Negative-Acknowledgment 无效消息，备用
    HOST_MSG_STOP,        // 上位机主动终止
    HOST_MSG_PKC_ERROR,   // public-key Certificate 证书认证错误
    HOST_MSG_TRNG_ERROR,  // 随机数错误，如判断出现非随机现象
    HOST_MSG_PC_ERROR,    // 上位机错误
};

enum HOST_MSG host_msg_wait(void);
int host_connect(void);
int host_msg_analysis(enum HOST_MSG m);

#endif /* _HOSTPC_H_ */

/*******************************END OF FILE************************************/

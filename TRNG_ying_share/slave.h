/**
 * @file slave.h
 * @author framist (framist@163.com)
 * @brief 
 * @version 0.1
 * @date 2021-10-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _SLAVE_H_
#define _SLAVE_H_

#include <sys/types.h>

#define DBG_SLAVE 1

#define SLAVE_MSG_LEN_MAX 512

enum HOST_MSG {
    HOST_MSG_NULL,        // 无消息
    HOST_MSG_ACK,         // Acknowledgment OK
    HOST_MSG_NAK,         // Negative-Acknowledgment 无效消息，备用
    HOST_MSG_STOP,        // 上位机主动终止
    HOST_MSG_PKC_ERROR,   // public-key Certificate 证书认证错误
    HOST_MSG_TRNG_ERROR,  // 随机数错误，如判断出现非随机现象
    HOST_MSG_PC_ERROR,    // 上位机错误
};

u_int32_t slave_ntropy_solve(char * buffer);
int slave_read(char * buffer);
int slave_write(enum HOST_MSG msg);
int slave_close(void);

#endif /* _SLAVE_H_ */


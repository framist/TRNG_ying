/**
 * @file hostPC.c
 * @author framist (framist@163.com)
 * @brief 连接上位机
 * @version 0.1
 * @date 2021-10-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "./HOSTPC/hostPC.h"
#include "./OLED/oled.h"
#include <stdio.h>

/**
 * @brief 
 * 
 * @param m 
 * @return int 成功:1 ; 其他:阻塞
 */
static int _host_msg_analysis(enum HOST_MSG m) {
    switch (m) {

    case HOST_MSG_NULL      : 
        OLED_ShowString(0, 0, ":( msg time out ", 16, 0);
        break;
    case HOST_MSG_ACK       :
        
        return 1;
        //break;
    case HOST_MSG_NAK       :
        OLED_ShowString(0, 0, ":(   NAK ERROR  ", 16, 0);
        break;
    case HOST_MSG_STOP      :
        OLED_ShowString(0, 0, "[TRNG]host stop ", 16, 0);
        Display_bye();
        break;
    case HOST_MSG_PKC_ERROR : 
        OLED_ShowString(0, 0, ":(   PKC ERROR  ", 16, 0);
        break;
        
    case HOST_MSG_TRNG_ERROR: 
        OLED_ShowString(0, 0, ":(   TRNG ERROR ", 16, 0);
        break;
    case HOST_MSG_PC_ERROR  :
        OLED_ShowString(0, 0, ":(    PC ERROR  ", 16, 0);
        break;

    default:
        OLED_ShowString(0, 0, ":(  udef ERROR  ", 16, 0);
        break;
    }
    Display_reset();
    return -1;
}

enum HOST_MSG host_msg_wait() {
    enum HOST_MSG m = HOST_MSG_NULL;
    uint32_t i;

    // //串口1的停止等待协议
    // while (m == HOST_MSG_NULL) {
    //     scanf("%u",&i);
    //     m = (enum HOST_MSG)i;
    // }

    //超时允许
    
    scanf("%u",&i);
    m = (enum HOST_MSG)i;

    _host_msg_analysis(m);

    return m;

}

/**
 * @brief 连接PC
 * 
 * @return int 成功:0 ; 其他:阻塞
 */
int host_connect(void) {
    enum HOST_MSG m = HOST_MSG_NULL;
    uint32_t i;
    OLED_Clear();
    
    OLED_ShowString(0,0,"Connect to PC...",16,0);

    // TODO 认证

    //串口1的停止等待协议
    while (m == HOST_MSG_NULL) {
        scanf("%u",&i);
        m = (enum HOST_MSG)i;
    }

    if(m==HOST_MSG_ACK) {
        OLED_ShowString(0,2,"    OK : )",16,0);
    }else {
        OLED_ShowString(0,2,"  fault :(",16,0);
        while(1);
    }
    
    HAL_Delay(1000);
    return 0;
}



/*******************************END OF FILE************************************/

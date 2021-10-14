#ifndef OLED_H__
#define OLED_H__

#include "stm32f1xx_hal.h"

void OLED_Init(int OLED_0_91);
void OLED_Clear(void);
void OLED_Display_On(void);
void OLED_Display_Off(void);

void OLED_On(void);

void OLED_ShowNum(uint8_t x,uint8_t y,unsigned int num,uint8_t len,uint8_t size2,int mode);
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size,int mode);

void Display_welcome(void);
void Display_bye(void);
void Display_reset(void);
#endif

/**
 * @file oled.c
 * @author modificator: framist (framist@163.com)
 * @brief 
 * @version 0.1
 * @date 2021-10-14
 * 
 * @copyright Copyright (c) 2021
 * @TODO 待重构
 */

#include "./OLED/oled.h"
#include "i2c.h"
#include "./OLED/oledfont.h"

void OLED_WR_CMD(uint8_t cmd);
void OLED_WR_DATA(uint8_t data);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size, int mode);

//初始化命令
static uint8_t CMD_Data[] = {
	0xAE, 0x00, 0x10, 0x40, 0xB0, 0x81, 0xFF, 0xA1, 0xA6, 0xA8, 0x3F,
	0xC8, 0xD3, 0x00, 0xD5, 0x80, 0xD8, 0x05, 0xD9, 0xF1, 0xDA, 0x12,
	0xD8, 0x30, 0x8D, 0x14, 0xAF
};

//向设备写控制命令
void OLED_WR_CMD(uint8_t cmd)
{
	HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 0x100);
}

//向设备写数据
void OLED_WR_DATA(uint8_t data)
{
	HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x40, I2C_MEMADD_SIZE_8BIT, &data, 1, 0x100);
}

/**
 * @brief 初始化oled屏幕
 * 
 * @param OLED_0_91 1:是0.91'OLED
 */
void OLED_Init(int OLED_0_91)
{
	HAL_Delay(200);
	if (OLED_0_91 == 1) {
		//初始化指令 由厂商给定
		OLED_WR_CMD(0xAE); //display off
		OLED_WR_CMD(0xD5); //set memory addressing Mode
		OLED_WR_CMD(0X80); //分频因子
		OLED_WR_CMD(0xA8); //设置驱动路数
		OLED_WR_CMD(0x1F); //默认0X3f(1/64) 0x1f(1/32)
		OLED_WR_CMD(0xD3); //设置显示偏移
		OLED_WR_CMD(0x00); //默认值00

		OLED_WR_CMD(0x40); //设置开始行 【5:0】，行数

		OLED_WR_CMD(0x8D); //电荷泵设置
		OLED_WR_CMD(0x14); //bit2,开启/关闭

		OLED_WR_CMD(0x20); //设置内存地址模式
		OLED_WR_CMD(0x02); //[[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
		OLED_WR_CMD(0xA1); //段重定义设置,bit0:0,0->0;1,0->127;
		OLED_WR_CMD(0xC8); //设置COM扫描方向

		OLED_WR_CMD(0xDA); //设置COM硬件引脚配置
		OLED_WR_CMD(0x02); //0.91英寸128*32分辨率

		OLED_WR_CMD(0x81); //对比度设置
		OLED_WR_CMD(0xff); //1~255(亮度设置,越大越亮)

		OLED_WR_CMD(0xD9); //设置预充电周期
		OLED_WR_CMD(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
		OLED_WR_CMD(0xDB); //设置VCOMH 电压倍率
		OLED_WR_CMD(0x40); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

		OLED_WR_CMD(0xA4); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
		OLED_WR_CMD(0xA6); //设置显示方式;bit0:1,反相显示;0,正常显示

		OLED_WR_CMD(0x2E); //停用滚动条

		OLED_WR_CMD(0xAF); //开启显示
	}
	else {
		uint8_t i = 0;
		for (i = 0; i < 27; i++) {
			HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, CMD_Data + i, 1, 0x100);
		}
	}
	OLED_Clear();
}
//清屏
void OLED_Clear()
{
	uint8_t i, n;
	for (i = 0; i < 8; i++) {
		OLED_WR_CMD(0xb0 + i);
		OLED_WR_CMD(0x00);
		OLED_WR_CMD(0x10);
		for (n = 0; n < 128; n++)
			OLED_WR_DATA(0);
	}
}
//开启OLED显示
void OLED_Display_On(void)
{
	OLED_WR_CMD(0X8D); //SET DCDC命令
	OLED_WR_CMD(0X14); //DCDC ON
	OLED_WR_CMD(0XAF); //DISPLAY ON
}
//关闭OLED显示
void OLED_Display_Off(void)
{
	OLED_WR_CMD(0X8D); //SET DCDC命令
	OLED_WR_CMD(0X10); //DCDC OFF
	OLED_WR_CMD(0XAE); //DISPLAY OFF
}
void OLED_Set_Pos(uint8_t x, uint8_t y)
{
	OLED_WR_CMD(0xb0 + y);
	OLED_WR_CMD(((x & 0xf0) >> 4) | 0x10);
	OLED_WR_CMD(x & 0x0f);
}

void OLED_On(void)
{
	uint8_t i, n;
	for (i = 0; i < 8; i++) {
		OLED_WR_CMD(0xb0 + i); //设置页地址(0~7)
		OLED_WR_CMD(0x00);	   //设置显示地址·-列低地址·
		OLED_WR_CMD(0x10);	   //设置显示地址·-列高地址·
		for (n = 0; n < 128; n++)
			OLED_WR_DATA(1);
	} //?更新显示
}
unsigned int oled_pow(uint8_t m, uint8_t n)
{
	unsigned int result = 1;
	while (n--)
		result *= m;
	return result;
}
//显示数字
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//mode:模式	
//num:数值(0~4294967295);
void OLED_ShowNum(uint8_t x, uint8_t y, unsigned int num, uint8_t len, uint8_t size2, int mode)
{
	uint8_t t, temp;
	uint8_t enshow = 0;
	for (t = 0; t < len; t++) {
		temp = (num / oled_pow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1)) {
			if (temp == 0) {
				OLED_ShowChar(x + (size2 / 2) * t, y, ' ', size2, mode);
				continue;
			}
			else
				enshow = 1;
		}
		OLED_ShowChar(x + (size2 / 2) * t, y, temp + '0', size2, mode);
	}
}
//在指定位置显示一个字符?包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示
//size:选择字体：16/12
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size, int mode)
{
	unsigned char c = 0, i = 0;
	c = chr - ' '; //得到偏移后的值
	if (x > 128 - 1) {
		x = 0;
		y = y + 2;
	}
	if (mode == 0) {
		if (Char_Size == 16) {
			OLED_Set_Pos(x, y);
			for (i = 0; i < 8; i++)
				OLED_WR_DATA(F8X16[c * 16 + i]);
			OLED_Set_Pos(x, y + 1);
			for (i = 0; i < 8; i++)
				OLED_WR_DATA(F8X16[c * 16 + i + 8]);
		}
		else {
			OLED_Set_Pos(x, y);
			for (i = 0; i < 6; i++)
				OLED_WR_DATA(F6x8[c][i]);
		}
	}
	else {
		if (Char_Size == 16) {
			OLED_Set_Pos(x, y);
			for (i = 0; i < 8; i++)
				OLED_WR_DATA(~F8X16[c * 16 + i]);
			OLED_Set_Pos(x, y + 1);
			for (i = 0; i < 8; i++)
				OLED_WR_DATA(~F8X16[c * 16 + i + 8]);
		}
		else {
			OLED_Set_Pos(x, y);
			for (i = 0; i < 6; i++)
				OLED_WR_DATA(~F6x8[c][i]);
		}
	}
}

//显示字符串
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t Char_Size, int mode) {
	unsigned char j = 0;
	while (chr[j] != '\0') {
		OLED_ShowChar(x, y, chr[j], Char_Size, mode);
		x += Char_Size / 2;
		if (x > 120) {
			x = 0;
			y += Char_Size == 16 ? 2 : 1;
		}
		j++;
	}
}
//显示汉字
//hzk 用取模软件得出的数组
void OLED_ShowCHinese1(uint8_t x, uint8_t y, uint8_t no) {
	uint8_t t, adder = 0;
	OLED_Set_Pos(x, y);
	for (t = 0; t < 16; t++) {
		OLED_WR_DATA(Hzk1[2 * no][t]);
		adder += 1;
	}
	OLED_Set_Pos(x, y + 1);
	for (t = 0; t < 16; t++) {
		OLED_WR_DATA(Hzk1[2 * no + 1][t]);
		adder += 1;
	}
}
void OLED_ShowCHinese2(uint8_t x, uint8_t y, uint8_t no) {
	uint8_t t, adder = 0;
	OLED_Set_Pos(x, y);
	for (t = 0; t < 16; t++) {
		OLED_WR_DATA(Hzk2[2 * no][t]);
		adder += 1;
	}
	OLED_Set_Pos(x, y + 1);
	for (t = 0; t < 16; t++) {
		OLED_WR_DATA(Hzk2[2 * no + 1][t]);
		adder += 1;
	}
}

void Display_welcome(void) {
	OLED_ShowString(0, 0, "init param:          ", 12, 0);
	OLED_ShowString(0, 1, "1LTKvNaux7CjrNHJ09C0y87Eo7/Rrbu3zfm4tKOsus7K", 12, 0);
	HAL_Delay(100);
	OLED_ShowString(0, 1, "sbb41rmjv9a5zqrWucv51rmjrLTLzsTT1rrOtOajvw==", 12, 0);
	OLED_Clear();

	// //显示西安电子科技大学
	// OLED_ShowCHinese1(0, 0, 0);
	// OLED_ShowCHinese1(16, 0, 1);
	// OLED_ShowCHinese1(32, 0, 2);
	// OLED_ShowCHinese1(48, 0, 3);
	// OLED_ShowCHinese1(64, 0, 4);
	// OLED_ShowCHinese1(80, 0, 5);
	// OLED_ShowCHinese1(96, 0, 6);
	// OLED_ShowCHinese1(112, 0, 7);
	

	OLED_ShowString(0, 0, "   [ T R N G ]  ", 16, 0);
	OLED_ShowString(0, 2, "ying ying ying ~", 16, 0);
	//		OLED_ShowCHinese2(64,2,0);
	//		OLED_ShowCHinese2(80,2,1);
	
	HAL_Delay(1000);
	

}
/**
 * @brief 永不返回
 * 
 */
void Display_bye(void) {
	HAL_Delay(1000);
	OLED_ShowString(0, 2, "   bye bye ~ ~  ", 16, 0);
	while (1) ;
}
/**
 * @brief 永不返回
 * 
 */
void Display_reset(void) {
	HAL_Delay(1000);
	OLED_ShowString(0, 2, "you can RESET me", 16, 0);
	while (1) ;
}

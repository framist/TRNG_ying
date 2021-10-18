/**
 * @file oled.c
 * @author modificator: framist (framist@163.com)
 * @brief 
 * @version 0.1
 * @date 2021-10-14
 * 
 * @copyright Copyright (c) 2021
 * @TODO ���ع�
 */

#include "./OLED/oled.h"
#include "i2c.h"
#include "./OLED/oledfont.h"

void OLED_WR_CMD(uint8_t cmd);
void OLED_WR_DATA(uint8_t data);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size, int mode);

//��ʼ������
static uint8_t CMD_Data[] = {
	0xAE, 0x00, 0x10, 0x40, 0xB0, 0x81, 0xFF, 0xA1, 0xA6, 0xA8, 0x3F,
	0xC8, 0xD3, 0x00, 0xD5, 0x80, 0xD8, 0x05, 0xD9, 0xF1, 0xDA, 0x12,
	0xD8, 0x30, 0x8D, 0x14, 0xAF
};

//���豸д��������
void OLED_WR_CMD(uint8_t cmd)
{
	HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 0x100);
}

//���豸д����
void OLED_WR_DATA(uint8_t data)
{
	HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x40, I2C_MEMADD_SIZE_8BIT, &data, 1, 0x100);
}

/**
 * @brief ��ʼ��oled��Ļ
 * 
 * @param OLED_0_91 1:��0.91'OLED
 */
void OLED_Init(int OLED_0_91)
{
	HAL_Delay(200);
	if (OLED_0_91 == 1) {
		//��ʼ��ָ�� �ɳ��̸���
		OLED_WR_CMD(0xAE); //display off
		OLED_WR_CMD(0xD5); //set memory addressing Mode
		OLED_WR_CMD(0X80); //��Ƶ����
		OLED_WR_CMD(0xA8); //��������·��
		OLED_WR_CMD(0x1F); //Ĭ��0X3f(1/64) 0x1f(1/32)
		OLED_WR_CMD(0xD3); //������ʾƫ��
		OLED_WR_CMD(0x00); //Ĭ��ֵ00

		OLED_WR_CMD(0x40); //���ÿ�ʼ�� ��5:0��������

		OLED_WR_CMD(0x8D); //��ɱ�����
		OLED_WR_CMD(0x14); //bit2,����/�ر�

		OLED_WR_CMD(0x20); //�����ڴ��ַģʽ
		OLED_WR_CMD(0x02); //[[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
		OLED_WR_CMD(0xA1); //���ض�������,bit0:0,0->0;1,0->127;
		OLED_WR_CMD(0xC8); //����COMɨ�跽��

		OLED_WR_CMD(0xDA); //����COMӲ����������
		OLED_WR_CMD(0x02); //0.91Ӣ��128*32�ֱ���

		OLED_WR_CMD(0x81); //�Աȶ�����
		OLED_WR_CMD(0xff); //1~255(��������,Խ��Խ��)

		OLED_WR_CMD(0xD9); //����Ԥ�������
		OLED_WR_CMD(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
		OLED_WR_CMD(0xDB); //����VCOMH ��ѹ����
		OLED_WR_CMD(0x40); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

		OLED_WR_CMD(0xA4); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
		OLED_WR_CMD(0xA6); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ

		OLED_WR_CMD(0x2E); //ͣ�ù�����

		OLED_WR_CMD(0xAF); //������ʾ
	}
	else {
		uint8_t i = 0;
		for (i = 0; i < 27; i++) {
			HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, CMD_Data + i, 1, 0x100);
		}
	}
	OLED_Clear();
}
//����
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
//����OLED��ʾ
void OLED_Display_On(void)
{
	OLED_WR_CMD(0X8D); //SET DCDC����
	OLED_WR_CMD(0X14); //DCDC ON
	OLED_WR_CMD(0XAF); //DISPLAY ON
}
//�ر�OLED��ʾ
void OLED_Display_Off(void)
{
	OLED_WR_CMD(0X8D); //SET DCDC����
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
		OLED_WR_CMD(0xb0 + i); //����ҳ��ַ(0~7)
		OLED_WR_CMD(0x00);	   //������ʾ��ַ��-�е͵�ַ��
		OLED_WR_CMD(0x10);	   //������ʾ��ַ��-�иߵ�ַ��
		for (n = 0; n < 128; n++)
			OLED_WR_DATA(1);
	} //?������ʾ
}
unsigned int oled_pow(uint8_t m, uint8_t n)
{
	unsigned int result = 1;
	while (n--)
		result *= m;
	return result;
}
//��ʾ����
//x,y :�������
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	
//num:��ֵ(0~4294967295);
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
//��ָ��λ����ʾһ���ַ�?���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ
//size:ѡ�����壺16/12
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size, int mode)
{
	unsigned char c = 0, i = 0;
	c = chr - ' '; //�õ�ƫ�ƺ��ֵ
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

//��ʾ�ַ���
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
//��ʾ����
//hzk ��ȡģ����ó�������
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

	// //��ʾ�������ӿƼ���ѧ
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
 * @brief ��������
 * 
 */
void Display_bye(void) {
	HAL_Delay(1000);
	OLED_ShowString(0, 2, "   bye bye ~ ~  ", 16, 0);
	while (1) ;
}
/**
 * @brief ��������
 * 
 */
void Display_reset(void) {
	HAL_Delay(1000);
	OLED_ShowString(0, 2, "you can RESET me", 16, 0);
	while (1) ;
}

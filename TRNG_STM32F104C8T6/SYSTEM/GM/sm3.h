/*
* sm3.h
*
* Ϊʹ���㷨����32λ��64λ��Linux��Windowsϵͳ��
* ѡ�� int ����ʾ 32 λ������
* ��Ϣ��������޶�Ϊ 2**32 - 1����λ�����أ���
* ��Ϊ 8 �ı�������Ϣ����С��ԪΪ�ֽڣ���
*/
#ifndef _SM3_H_
#define _SM3_H_
 
/*
* SM3�㷨�����Ĺ�ϣֵ��С����λ���ֽڣ�
*/
#define SM3_HASH_SIZE 32 
 
/*
* SM3������
*/
typedef struct SM3Context
{
	unsigned int intermediateHash[SM3_HASH_SIZE / 4];
	unsigned char messageBlock[64];
} SM3Context;
 
/*
* SM3���㺯��
*/
unsigned char *SM3Calc(const unsigned char *message,
	unsigned int messageLen, unsigned char digest[SM3_HASH_SIZE]);
 
#endif // _SM3_H_



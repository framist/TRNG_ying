#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./GM/miracl.h"
#include "./GM/sm2.h"
#include "./GM/sm3.h"
//#include "global_variable.h" //这个头文件用于提供在滴答计时器中我的一个全局计数变量sysTick给
                            //irand使用产生随机数
unsigned int sysTick = 0;

#define SM2_PAD_ZERO TRUE
//#define SM2_PAD_ZERO FALSE
 
 
#define SM2_DEBUG   0
/*
#define QBITS 256
#define PBITS 3072
#define MAX_ECC_KEY_LEN		256
#define MAX_ECC_KEY_SIZE	(MAX_ECC_KEY_LEN/8)
*/
 
struct FPECC{
char *p;
char *a;
char *b;
char *n;
char *x;
char *y;
};
 
 
void PrintBuf(unsigned char *buf, int	buflen)
{
	int i;
	printf("\r\n");
	printf("len = %d\r\n", buflen);
	for(i=0; i<buflen; i++) {
  	if (i % 32 != 31)
  	  printf("%02x", buf[i]);
  	  else
  	  printf("%02x\r\n", buf[i]);
  }
  printf("\r\n");
  return;
}
 
void Printch(unsigned char *buf, int	buflen)
{
	int i;
	for (i = 0; i < buflen; i++) {
		if (i % 32 != 31)
			printf("%c", buf[i]);
		else
			printf("%c\n", buf[i]);
	}
	printf("\n");
	//return 0;
}
#if SM2_DEBUG
 
void PrintBig(big data)
{
 int len=0;
 unsigned char buf[10240];
 
 len=big_to_bytes(0,data,(char *)buf,0);
 PrintBuf(buf,len);
}
 
unsigned char radom[]  = {0x6C,0xB2,0x8D,0x99,0x38,0x5C,0x17,0x5C,0x94,0xF9,0x4E,0x93,0x48,0x17,0x66,0x3F,0xC1,0x76,0xD9,0x25,0xDD,0x72,0xB7,0x27,0x26,0x0D,0xBA,0xAE,0x1F,0xB2,0xF9,0x6F};
unsigned char radom1[] = {0x4C,0x62,0xEE,0xFD,0x6E,0xCF,0xC2,0xB9,0x5B,0x92,0xFD,0x6C,0x3D,0x95,0x75,0x14,0x8A,0xFA,0x17,0x42,0x55,0x46,0xD4,0x90,0x18,0xE5,0x38,0x8D,0x49,0xDD,0x7B,0x4F};
unsigned char randkey[] = {0x83,0xA2,0xC9,0xC8,0xB9,0x6E,0x5A,0xF7,0x0B,0xD4,0x80,0xB4,0x72,0x40,0x9A,0x9A,0x32,0x72,0x57,0xF1,0xEB,0xB7,0x3F,0x5B,0x07,0x33,0x54,0xB2,0x48,0x66,0x85,0x63};
unsigned char randkeyb[]= {0x33,0xFE,0x21,0x94,0x03,0x42,0x16,0x1C,0x55,0x61,0x9C,0x4A,0x0C,0x06,0x02,0x93,0xD5,0x43,0xC8,0x0A,0xF1,0x97,0x48,0xCE,0x17,0x6D,0x83,0x47,0x7D,0xE7,0x1C,0x80};
 
struct FPECC Ecc256={
"8542D69E4C044F18E8B92435BF6FF7DE457283915C45517D722EDB8B08F1DFC3",
"787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498",
"63E4C6D3B23B0C849CF84241484BFE48F61D59A5B16BA06E6E12D1DA27C5249A",
"8542D69E4C044F18E8B92435BF6FF7DD297720630485628D5AE74EE7C32E79B7",
"421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D",
"0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2",
};
unsigned char sm2_par_dig[128] = {
0x78,0x79,0x68,0xB4,0xFA,0x32,0xC3,0xFD,0x24,0x17,0x84,0x2E,0x73,0xBB,0xFE,0xFF,
0x2F,0x3C,0x84,0x8B,0x68,0x31,0xD7,0xE0,0xEC,0x65,0x22,0x8B,0x39,0x37,0xE4,0x98,
0x63,0xE4,0xC6,0xD3,0xB2,0x3B,0x0C,0x84,0x9C,0xF8,0x42,0x41,0x48,0x4B,0xFE,0x48,
0xF6,0x1D,0x59,0xA5,0xB1,0x6B,0xA0,0x6E,0x6E,0x12,0xD1,0xDA,0x27,0xC5,0x24,0x9A,
0x42,0x1D,0xEB,0xD6,0x1B,0x62,0xEA,0xB6,0x74,0x64,0x34,0xEB,0xC3,0xCC,0x31,0x5E,
0x32,0x22,0x0B,0x3B,0xAD,0xD5,0x0B,0xDC,0x4C,0x4E,0x6C,0x14,0x7F,0xED,0xD4,0x3D,
0x06,0x80,0x51,0x2B,0xCB,0xB4,0x2C,0x07,0xD4,0x73,0x49,0xD2,0x15,0x3B,0x70,0xC4,
0xE5,0xD7,0xFD,0xFC,0xBF,0xA3,0x6E,0xA1,0xA8,0x58,0x41,0xB9,0xE4,0x6E,0x09,0xA2,
};
 
#else
/*SM2*/
struct FPECC Ecc256={
"FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF",
"FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC",
"28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93",
"FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123",
"32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7",
"BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0",
};

unsigned char sm2_par_dig[128] = {
0xFF,0xFF,0xFF,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,
0x28,0xE9,0xFA,0x9E,0x9D,0x9F,0x5E,0x34,0x4D,0x5A,0x9E,0x4B,0xCF,0x65,0x09,0xA7,
0xF3,0x97,0x89,0xF5,0x15,0xAB,0x8F,0x92,0xDD,0xBC,0xBD,0x41,0x4D,0x94,0x0E,0x93,
0x32,0xC4,0xAE,0x2C,0x1F,0x19,0x81,0x19,0x5F,0x99,0x04,0x46,0x6A,0x39,0xC9,0x94,
0x8F,0xE3,0x0B,0xBF,0xF2,0x66,0x0B,0xE1,0x71,0x5A,0x45,0x89,0x33,0x4C,0x74,0xC7,
0xBC,0x37,0x36,0xA2,0xF4,0xF6,0x77,0x9C,0x59,0xBD,0xCE,0xE3,0x6B,0x69,0x21,0x53,
0xD0,0xA9,0x87,0x7C,0xC6,0x2A,0x47,0x40,0x02,0xDF,0x32,0xE5,0x21,0x39,0xF0,0xA0,
};
 
unsigned char enkey[32] = {
0xB1,0x6B,0xA0,0xDA,0x27,0xC5,0x24,0x9A,0xF6,0x1D,0x6E,0x6E,0x12,0xD1,0x59,0xA5,
0xB6,0x74,0x64,0x34,0xEB,0xD6,0x1B,0x62,0xEA,0xEB,0xC3,0xCC,0x31,0x5E,0x42,0x1D,
};
#endif
 
 
#define SEED_CONST 0x1BD8C95A

/**
	* @brief 生成SM2公私钥对
	* @param wx：         公钥的X坐标，不足32字节在前面加0x00
	* @param wxlen:       wx的字节数，32
	* @param wy：         公钥的Y坐标，不足32字节在前面加0x00
	* @param wylen:       wy的字节数，32
	* @param privkey：    私钥，不足32字节在前面加0x00
	* @param privkeylen： privkey的字节数，32
  * @retval void
  */
void sm2_keygen(unsigned char *wx, int *wxlen, unsigned char *wy, \
	              int *wylen, unsigned char *privkey, int *privkeylen)
{
	struct FPECC *cfig = &Ecc256;
	epoint *g;
	big a,b,p,n,x,y,key1;
	
	miracl instance;
	miracl *mip = &instance;

	char mem[MR_BIG_RESERVE(7)];
	memset(mem, 0, MR_BIG_RESERVE(7));
	
	mip = mirsys(mip, 20, 0);   /* Use Hex Internally */
	mip->IOBASE = 16;

	p = mirvar_mem(mip, mem, 0);
	a = mirvar_mem(mip, mem, 1);
	b = mirvar_mem(mip, mem, 2);
	n = mirvar_mem(mip, mem, 3);
	x = mirvar_mem(mip, mem, 4);
	y = mirvar_mem(mip, mem, 5);
	key1 = mirvar_mem(mip, mem, 6);

	cinstr(mip, p, cfig->p);
	cinstr(mip, a, cfig->a);
	cinstr(mip, b, cfig->b);
	cinstr(mip, n, cfig->n);
	cinstr(mip, x, cfig->x);
	cinstr(mip, y, cfig->y);

	ecurve_init(mip, a, b, p, MR_PROJECTIVE);

	char mem1[MR_ECP_RESERVE(1)]; 
	memset(mem1 ,0, MR_ECP_RESERVE(1));
	
  g = epoint_init_mem(mip, mem1, 0);
	epoint_set(mip, x, y, 0, g);
	
	irand(mip, sysTick);
	
	/* generate public/private keys */
	bigrand(mip, n, key1);
	
	ecurve_mult(mip, key1,g,g);
	epoint_get(mip, g,x,y); /* compress point */
	
	*wxlen = big_to_bytes(mip, 32, x, (char *)wx, TRUE);
	*wylen = big_to_bytes(mip, 32, y, (char *)wy, TRUE);
	*privkeylen = big_to_bytes(mip, 32, key1, (char *)privkey, TRUE);

	/* clear all memory used */
  memset(mem,0,MR_BIG_RESERVE(7));
  memset(mem1,0,MR_ECP_RESERVE(1));
	mirexit(mip);
}

/**
  * @brief  密钥派生函数
  * @param  zl  要处理的数据——list（x）
  * @param  zr  要处理的数据——row（y）
  * @param  keylen  需要派生得到的长度
  * @param  kbuf    计算后返回的内容（二进制值）,分配空间至少为需要keylen
  * @retval 0：成功 其他失败
  */
int kdf(unsigned char *zl, unsigned char *zr, int klen, unsigned char *kbuf)
{

	unsigned char buf[70];
	unsigned char digest[32];
	unsigned int ct = 0x00000001; //初始化一个32比特构成的计数器ct=0x00000001
	int i, m, n;
	unsigned char *p;
	
	memcpy(buf, zl, 32);
	memcpy(buf+32, zr, 32);
	m = klen / 32;
	n = klen % 32;
	p = kbuf;
	for(i = 0; i < m; i++)
	{
		buf[64] = (ct >> 24) & 0xFF;
		buf[65] = (ct >> 16) & 0xFF;
		buf[66] = (ct >> 8) & 0xFF;
		buf[67] = ct & 0xFF;
		SM3Calc(buf, 68, p);
		p += 32;
		ct++;
	}
	
	if(n != 0)
	{
		buf[64] = (ct >> 24) & 0xFF;
		buf[65] = (ct >> 16) & 0xFF;
		buf[66] = (ct >> 8) & 0xFF;
		buf[67] = ct & 0xFF;
		SM3Calc(buf, 68, digest);
	}
	
	memcpy(p, digest, n);

	for(i = 0; i < klen; i++)
	{
		if(kbuf[i] != 0)
			break;
	}
 
	if(i < klen)
		return 1;
	else
		return 0;
}

/**
  * @brief  SM2加密
  * @param  msg：    要加密的明文数据
  * @param  msglen： 明文数据长度
  * @param  wx：     公钥的x坐标
  * @param  wxlen：  公钥的x坐标长度，不超过32
  * @param  wy：     公钥的y坐标
  * @param  wylen：  公钥的y坐标长度，不超过32
  * @param  outmsg： 加密后密文 长度为明文 + 96
  * @retval -1：失败 msglen + 96：成功
  */
int sm2_encrypt(unsigned char *msg,int msglen, unsigned char *wx,int wxlen, \
	              unsigned char *wy,int wylen, unsigned char *outmsg)
{
 
	struct FPECC *cfig = &Ecc256;
	big x2, y2, c1, c2, k;
	big a,b,p,n,x,y;
	epoint *g, *w;
	int ret = -1;
	int i;
	unsigned char zl[32], zr[32];
	unsigned char *tmp;
	
	miracl instance;
  miracl *mip = &instance;

	tmp = malloc(msglen+64);
	if(tmp == NULL)
		return -1;
	
	mip = mirsys(mip, 20, 0);   /* Use Hex Internally */
	mip->IOBASE = 16;
	
	char mem[MR_BIG_RESERVE(11)];
  memset(mem, 0, MR_BIG_RESERVE(11));
	
	p= mirvar_mem(mip, mem, 0);
	a=mirvar_mem(mip, mem, 1);
	b=mirvar_mem(mip, mem, 2);
	n=mirvar_mem(mip, mem, 3);
	x=mirvar_mem(mip, mem, 4);
	y=mirvar_mem(mip, mem, 5);
	k=mirvar_mem(mip, mem, 6);
	x2=mirvar_mem(mip, mem, 7);
	y2=mirvar_mem(mip, mem, 8);
	c1=mirvar_mem(mip, mem, 9);
	c2=mirvar_mem(mip, mem, 10);
	
	cinstr(mip, p,cfig->p);
	cinstr(mip, a,cfig->a);
	cinstr(mip, b,cfig->b);
	cinstr(mip, n,cfig->n);
	cinstr(mip, x,cfig->x);
	cinstr(mip, y,cfig->y);
	
	ecurve_init(mip, a,b,p,MR_PROJECTIVE);

	char mem1[MR_ECP_RESERVE(2)]; 
	memset(mem1 ,0, MR_ECP_RESERVE(2));

	g = epoint_init_mem(mip, mem1,0);
	w = epoint_init_mem(mip, mem1,1);
	 
  epoint_set(mip, x,y,0,g);
	bytes_to_big(mip, wxlen,(char *)wx,x);
	bytes_to_big(mip, wylen,(char *)wy,y);
	epoint_set(mip, x,y,0,w);
	
	irand(mip, sysTick);
sm2_encrypt_again:
#if SM2_DEBUG
	bytes_to_big(32, (char *)radom1, k);
#else
	do
	{
		bigrand(mip, n, k);
	} 
	while (k->len == 0);
#endif
	ecurve_mult(mip, k, g, g);
	epoint_get(mip, g, c1, c2);
	big_to_bytes(mip, 32, c1, (char *)outmsg, TRUE);
	big_to_bytes(mip, 32, c2, (char *)outmsg+32, TRUE);
	//计算椭圆曲线点C1
	if(point_at_infinity(w))
		goto exit_sm2_encrypt;
	//计算椭圆曲线点S
	ecurve_mult(mip, k, w, w);
	epoint_get(mip, w, x2, y2);
	big_to_bytes(mip, 32, x2, (char *)zl, TRUE);
	big_to_bytes(mip, 32, y2, (char *)zr, TRUE);
	//计算椭圆曲线点[k]PB
	if (kdf(zl, zr, msglen, outmsg+64) == 0)
		goto sm2_encrypt_again;
	//计算t = KDF,如果t全零,返回A1
	for(i = 0; i < msglen; i++)
	{
		outmsg[64+i] ^= msg[i];
	}
	//计算C2
	memcpy(tmp, zl, 32);
	memcpy(tmp+32, msg, msglen);
	memcpy(tmp+32+msglen, zr, 32);
	SM3Calc(tmp, 64+msglen, &outmsg[64+msglen]);
	//计算C3
	ret = msglen+64+32;
	
exit_sm2_encrypt:

	memset(mem,0,MR_BIG_RESERVE(11));
	memset(mem1,0,MR_ECP_RESERVE(2));
	mirexit(mip);
	free(tmp);
	return ret;
}
 
/**
* @brief  SM2解密
* @param  msg：        要解密的密文数据
* @param  msglen：     密文数据长度
* @param  privkey：    私钥
* @param  privkeylen： 私钥长度
* @param  outmsg： 解密后的明文 长度为明文 - 96
* @retval -1：失败 msglen - 96：成功
*/
int sm2_decrypt(unsigned char *msg,int msglen, unsigned char *privkey, \
	              int privkeylen, unsigned char *outmsg)
{
 
	struct FPECC *cfig = &Ecc256;
	big x2, y2, c, k;
	big a,b,p,n,x,y,key1;
	epoint *g;
	unsigned char c3[32];
	unsigned char zl[32], zr[32];
	int i, ret = -1;
	unsigned char *tmp;
	
	miracl instance;
	miracl *mip = &instance;
	
	if(msglen < 96)
		return 0;
	msglen -= 96;
	tmp = malloc(msglen+64);
	if(tmp == NULL)
		return 0;
	
	mip = mirsys(mip, 20, 0);   /* Use Hex Internally */
	mip->IOBASE = 16;
 
	char mem[MR_BIG_RESERVE(11)];
  memset(mem, 0, MR_BIG_RESERVE(11));
 
	x2 = mirvar_mem(mip, mem, 0);
	y2 = mirvar_mem(mip, mem, 1);
	c = mirvar_mem(mip, mem, 2);
	k = mirvar_mem(mip, mem, 3);
	p = mirvar_mem(mip, mem, 4);
	a = mirvar_mem(mip, mem, 5);
	b = mirvar_mem(mip, mem, 6);
	n = mirvar_mem(mip, mem, 7);
	x = mirvar_mem(mip, mem, 8);
	y = mirvar_mem(mip, mem, 9);
	key1 = mirvar_mem(mip, mem, 10);
	
	bytes_to_big(mip, privkeylen,(char *)privkey,key1);
	
	cinstr(mip, p,cfig->p);
	cinstr(mip, a,cfig->a);
	cinstr(mip, b,cfig->b);
	cinstr(mip, n,cfig->n);
	cinstr(mip, x,cfig->x);
	cinstr(mip, y,cfig->y);
	
	ecurve_init(mip, a,b,p,MR_PROJECTIVE);

	char mem1[MR_ECP_RESERVE(1)]; 
	memset(mem1 ,0, MR_ECP_RESERVE(1));

	g = epoint_init_mem(mip, mem1,0);

	bytes_to_big(mip, 32, (char *)msg, x);
	bytes_to_big(mip, 32, (char *)msg+32, y);
	
  if(!epoint_set(mip, x,y,0,g))
		goto exit_sm2_decrypt;  //检验是否为椭圆曲线
	if(point_at_infinity(g))
		goto exit_sm2_decrypt;  //计算S
	
	ecurve_mult(mip, key1, g, g);
	epoint_get(mip, g, x2, y2);	
	
	big_to_bytes(mip, 32, x2, (char *)zl, TRUE);
	big_to_bytes(mip, 32, y2, (char *)zr, TRUE); //计算[db]c1
	
	if (kdf(zl, zr, msglen, outmsg) == 0)
		goto exit_sm2_decrypt;    //计算t
	
	for(i = 0; i < msglen; i++)
	{
		outmsg[i] ^= msg[i+64];
	}   //计算M到outsmg
	
	memcpy(tmp, zl, 32);
	memcpy(tmp+32, outmsg, msglen);
	memcpy(tmp+32+msglen, zr, 32);
	
	SM3Calc(tmp, 64+msglen, c3);//计算u
	if(memcmp(c3, msg+64+msglen, 32) != 0)
	{
		goto exit_sm2_decrypt;
	}
	
	ret =  msglen;
exit_sm2_decrypt:
	memset(mem,0,MR_BIG_RESERVE(11));
	memset(mem1,0,MR_ECP_RESERVE(1));
	mirexit(mip);
	free(tmp);
	return ret;
}

int sm2_encrypt_test(void)
{	
	unsigned char dB[] = { 0x16,0x49,0xAB,0x77,0xA0,0x06,0x37,0xBD,0x5E,0x2E,0xFE,0x28,0x3F,0xBF,0x35,0x35,0x34,0xAA,0x7F,0x7C,0xB8,0x94,0x63,0xF2,0x08,0xDD,0xBC,0x29,0x20,0xBB,0x0D,0xA0 };
	unsigned char xB[] = { 0x43,0x5B,0x39,0xCC,0xA8,0xF3,0xB5,0x08,0xC1,0x48,0x8A,0xFC,0x67,0xBE,0x49,0x1A,0x0F,0x7B,0xA0,0x7E,0x58,0x1A,0x0E,0x48,0x49,0xA5,0xCF,0x70,0x62,0x8A,0x7E,0x0A };
	unsigned char yB[] = { 0x75,0xDD,0xBA,0x78,0xF1,0x5F,0xEE,0xCB,0x4C,0x78,0x95,0xE2,0xC1,0xCD,0xF5,0xFE,0x01,0xDE,0xBB,0x2C,0xDB,0xAD,0xF4,0x53,0x99,0xCC,0xF7,0x7B,0xBA,0x07,0x6A,0x42 };
	unsigned char tx[20] = {0x16,0x49,0xAB,0x77,0xA0,0x06,0x37,0xBD,0x5E,0x2E,0x16,0x49,0xAB,0x77,0xA0,0x06,0x37,0xBD,0x5E,0x2E};
	unsigned char etx[256];
	unsigned char mtx[256];
        
    unsigned char std_priKey[32]={	
		0xEC,0xE3,0xB4,0x8C,0x86,0xF4,0x57,0xEA,0x69,0xE2,0x80,0xBC,0x5B,0x3C,0x53,0xD8,
		0x9D,0x64,0x1B,0xDB,0xE4,0x7F,0x62,0xCC,0x57,0x4C,0xA3,0x49,0x4D,0x59,0xF0,0x75};
    unsigned char std_Cipher[113]={		
		0xb6,0x6a,0xe5,0xd2,0x31,0x48,0xb4,0x92,0x2f,0xe2,0xc9,0x0e,0x0c,0xcb,0xcc,0xdc,
		0x2c,0xa1,0xfd,0x49,0x37,0x5a,0xa1,0x83,0xdf,0x5a,0x60,0x24,0x36,0x27,0xe5,0x1c,
		0xb1,0x4f,0x60,0xde,0x5a,0x67,0xbb,0x86,0xce,0x4c,0xfb,0xfa,0xcc,0xbc,0xc0,0x4d,
		0x1c,0x8f,0x98,0x12,0x6e,0x61,0x04,0xb4,0x2a,0x49,0xac,0xde,0x8d,0xc5,0x6f,0xe3,
		0x6c,0x66,0x35,0xbe,0xa2,0xc4,0xcb,0x09,0x45,0x84,0x81,0x7c,0xe7,0x84,0x44,0xcb,
		0x5b,0x57,0x94,0x16,0x01,0x1c,0x7d,0x98,0xbd,0x88,0x0b,0xa6,0x18,0xd0,0xd9,0x75,
		0xe5,0x6e,0xd3,0x2d,0xb2,0x16,0xd0,0x01,0xa9,0x0d,0x3a,0x99,0x05,0xf6,0x2c,0x6f,
		0x94};

//	int wxlen, wylen, privkeylen,len,ret;

//	sm2_keygen(xB, &wxlen, yB, &wylen, dB, &privkeylen);
//	printf("dB: ");
//	PrintBuf(dB, privkeylen);
//	printf("xB: ");
//	PrintBuf(xB, wxlen);
//	printf("yB: ");
//	PrintBuf(yB, wylen);

//	len = 20;
//		
//	printf("\n``````````````````before encrypt```````````````````\n");
//	PrintBuf(tx, len);

//	printf("\n``````````````````this is encrypt```````````````````\n");
//	sm2_encrypt(tx,len,xB,wxlen,yB,wylen,etx);
//	PrintBuf(etx, 64 +len + 32);

//	printf("\n``````````````````this is decrypt```````````````````\n");

//	ret = sm2_decrypt(etx,64+len+32,dB,privkeylen,mtx);
//	if( ret < 0)
//		printf("sm2_decrypt error!\n");
//	else
//	{
//		PrintBuf(mtx, len);
//	}	
    
    printf("\n``````````````````my decrypt```````````````````\n");
    printf("std_Cipher: ");
    PrintBuf(std_Cipher, 113);
    printf("\nstd_priKey: ");
    PrintBuf(std_priKey, 32);
    
    int len = 113-96;
	if( sm2_decrypt(std_Cipher,113,std_priKey,32,mtx) < 0)
		printf("sm2_decrypt error!\n");
	else
	{
		PrintBuf(mtx, len);
	}	
    printf("\n``````````````````sm2 end```````````````````\n");
	return 0;
}

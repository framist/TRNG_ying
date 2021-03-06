#include <miracl/miracl.h>
#include <miracl/mirdef.h>
#include "kdf.h"

#define LEN 17
// int SM2_ENC_SelfTest()
int main()
{
	int tmp=0,i=0;unsigned char Cipher[115]={0};
	unsigned char M[LEN]={0};
	unsigned char kGxy[SM2_NUMWORD*2]={0};
	big ks,x,y;
	epoint *kG;
	//standard data
	unsigned char std_priKey[32]={
		// 0x39,0x45,0x20,0x8F,0x7B,0x21,0x44,0xB1,0x3F,0x36,0xE3,0x8A,0xC6,0xD3,0x9F,0x95,
		// 0x88,0x93,0x93,0x69,0x28,0x60,0xB5,0x1A,0x42,0xFB,0x81,0xEF,0x4D,0xF7,0xC5,0xB8};
		0xEC,0xE3,0xB4,0x8C,0x86,0xF4,0x57,0xEA,0x69,0xE2,0x80,0xBC,0x5B,0x3C,0x53,0xD8,
		0x9D,0x64,0x1B,0xDB,0xE4,0x7F,0x62,0xCC,0x57,0x4C,0xA3,0x49,0x4D,0x59,0xF0,0x75,};
	unsigned char std_pubKey[64]={
		// 0x09,0xF9,0xDF,0x31,0x1E,0x54,0x21,0xA1,0x50,0xDD,0x7D,0x16,0x1E,0x4B,0xC5,0xC6,
		// 0x72,0x17,0x9F,0xAD,0x18,0x33,0xFC,0x07,0x6B,0xB0,0x8F,0xF3,0x56,0xF3,0x50,0x20,
		// 0xCC,0xEA,0x49,0x0C,0xE2,0x67,0x75,0xA5,0x2D,0xC6,0xEA,0x71,0x8C,0xC1,0xAA,0x60,
		// 0x0A,0xED,0x05,0xFB,0xF3,0x5E,0x08,0x4A,0x66,0x32,0xF6,0x07,0x2D,0xA9,0xAD,0x13};
		0x2E,0x81,0xE7,0xDA,0x01,0x7E,0x96,0x11,0xCC,0x63,0x3F,0x53,0x75,0xDA,0x6A,0x0E,
		0x71,0x4E,0xB9,0xCD,0x9A,0xD8,0x41,0xCA,0x8F,0xBD,0xB3,0xE2,0x54,0xCA,0xA3,0x2B,
		0x14,0x26,0x62,0xBD,0xC6,0x73,0x60,0x1C,0x13,0x8C,0x8F,0x92,0x1C,0x61,0x72,0xD7,
		0xD4,0x8D,0xAE,0xB3,0x0A,0xE1,0xDE,0x88,0x4D,0xDA,0xA3,0x39,0x1F,0x9F,0xA0,0xED,};
	unsigned char std_rand[32]={
		// 0x59,0x27,0x6E,0x27,0xD5,0x06,0x86,0x1A,0x16,0x68,0x0F,0x3A,0xD9,0xC0,0x2D,0xCC,
		// 0xEF,0x3C,0xC1,0xFA,0x3C,0xDB,0xE4,0xCE,0x6D,0x54,0xB8,0x0D,0xEA,0xC1,0xBC,0x21};
		0xac,0x84,0xc4,0xf2,0xab,0x4f,0xc9,0x23,0x88,0xa0,0x94,0x8f,0xd7,0xd9,0x7a,0x04,
		0x84,0x82,0x65,0xd8,0x3e,0xc4,0x17,0x69,0x63,0xaf,0x06,0x87,0x77,0x30,0xde,0x23};
	unsigned char std_Message[LEN]={
		// 0x65,0x6E,0x63,0x72,0x79,0x70,0x74,0x69,0x6F,0x6E,0x20,0x73,0x74,0x61,0x6E,0x64,
		// 0x61,0x72,0x64};//encryption standard
		0x66,0x6c,0x61,0x67,0x7b,0x66,0x72,0x61,0x6d,0x69,0x73,0x74,0x5f,0x74,0x71,0x6c,0x7d};
	unsigned char std_Cipher[113]={
		// 0x04,0xEB,0xFC,0x71,0x8E,0x8D,0x17,0x98,0x62,0x04,0x32,0x26,0x8E,0x77,0xFE,0xB6,
		// 0x41,0x5E,0x2E,0xDE,0x0E,0x07,0x3C,0x0F,0x4F,0x64,0x0E,0xCD,0x2E,0x14,0x9A,0x73,
		// 0xE8,0x58,0xF9,0xD8,0x1E,0x54,0x30,0xA5,0x7B,0x36,0xDA,0xAB,0x8F,0x95,0x0A,0x3C,
		// 0x64,0xE6,0xEE,0x6A,0x63,0x09,0x4D,0x99,0x28,0x3A,0xFF,0x76,0x7E,0x12,0x4D,0xF0,
		// 0x59,0x98,0x3C,0x18,0xF8,0x09,0xE2,0x62,0x92,0x3C,0x53,0xAE,0xC2,0x95,0xD3,0x03,
		// 0x83,0xB5,0x4E,0x39,0xD6,0x09,0xD1,0x60,0xAF,0xCB,0x17,0x08,0xD0,0xBD,0x87,0x66,
		// 0x21,0x88,0x6C,0xA9,0x89,0xCA,0x9C,0x7D,0x58,0x08,0x73,0x07,0xCA,0x93,0x09,0x2D,
		// 0x65,0x1E,0xFA};
		0xb6,0x6a,0xe5,0xd2,0x31,0x48,0xb4,0x92,0x2f,0xe2,0xc9,0x0e,0x0c,0xcb,0xcc,0xdc,
		0x2c,0xa1,0xfd,0x49,0x37,0x5a,0xa1,0x83,0xdf,0x5a,0x60,0x24,0x36,0x27,0xe5,0x1c,
		0xb1,0x4f,0x60,0xde,0x5a,0x67,0xbb,0x86,0xce,0x4c,0xfb,0xfa,0xcc,0xbc,0xc0,0x4d,
		0x1c,0x8f,0x98,0x12,0x6e,0x61,0x04,0xb4,0x2a,0x49,0xac,0xde,0x8d,0xc5,0x6f,0xe3,
		0x6c,0x66,0x35,0xbe,0xa2,0xc4,0xcb,0x09,0x45,0x84,0x81,0x7c,0xe7,0x84,0x44,0xcb,
		0x5b,0x57,0x94,0x16,0x01,0x1c,0x7d,0x98,0xbd,0x88,0x0b,0xa6,0x18,0xd0,0xd9,0x75,
		0xe5,0x6e,0xd3,0x2d,0xb2,0x16,0xd0,0x01,0xa9,0x0d,0x3a,0x99,0x05,0xf6,0x2c,0x6f,
		0x94};

	mip= mirsys(1000, 16);
	mip->IOBASE=16;
	x=mirvar(0);
	y=mirvar(0);
	ks=mirvar(0);
	kG=epoint_init();
	bytes_to_big(32,std_priKey,ks); //ks is the standard private key
	//initiate SM2 curve
	SM2_Init();
	//generate key pair
	tmp=SM2_KeyGeneration_enc(ks,kG);
	if (tmp!=0)
		return tmp;


	epoint_get(kG,x,y);
	big_to_bytes(SM2_NUMWORD,x,kGxy,1);
	big_to_bytes(SM2_NUMWORD,y,kGxy+SM2_NUMWORD,1);
	if(memcmp(kGxy,std_pubKey,SM2_NUMWORD*2)!=0)
		return ERR_SELFTEST_KG;

	//encrypt data and compare the result with the standard data
	tmp=SM2_Encrypt(std_rand,kG,std_Message,LEN,Cipher);
	if(tmp!=0)
		return tmp;
	// int i;
	for(i=0;i<LEN+SM2_NUMWORD*3;i++)printf("0x%02x,",Cipher[i]);
	if(memcmp(Cipher,std_Cipher,LEN+SM2_NUMWORD*3)!=0)
		return ERR_SELFTEST_ENC;

	//decrypt cipher and compare the result with the standard data
	tmp=SM2_Decrypt(ks,Cipher,113,M);
	if(tmp!=0)
		return tmp;
	printf("M:");
	for(i=0;i<17;i++)printf("0x%02x,",M[i]);
	if(memcmp(M,std_Message,LEN)!=0){
		printf("sm2 env err \n");
		return ERR_SELFTEST_DEC;
	}else 
		printf(" sm2 env ok \n");
	return 0;
}

// int SM2_SelfCheck()
int a()
{
	int i;
	//the private key
	unsigned char dA[32]={
		0x39,0x45,0x20,0x8f,0x7b,0x21,0x44,0xb1,0x3f,0x36,0xe3,0x8a,0xc6,0xd3,0x9f,0x95,
		0x88,0x93,0x93,0x69,0x28,0x60,0xb5,0x1a,0x42,0xfb,0x81,0xef,0x4d,0xf7,0xc5,0xb8};

	unsigned char rand[32]={
		0x59,0x27,0x6E,0x27,0xD5,0x06,0x86,0x1A,0x16,0x68,0x0F,0x3A,0xD9,0xC0,0x2D,0xCC,
		0xEF,0x3C,0xC1,0xFA,0x3C,0xDB,0xE4,0xCE,0x6D,0x54,0xB8,0x0D,0xEA,0xC1,0xBC,0x21};

	//the public key
	unsigned char xA[32]={
		   0x09,0xf9,0xdf,0x31,0x1e,0x54,0x21,0xa1,0x50,0xdd,0x7d,0x16,0x1e,0x4b,0xc5,0xc6,
		   0x72,0x17,0x9f,0xad,0x18,0x33,0xfc,0x07,0x6b,0xb0,0x8f,0xf3,0x56,0xf3,0x50,0x20};
	unsigned char yA[32]={
	  	 0xcc,0xea,0x49,0x0c,0xe2,0x67,0x75,0xa5,0x2d,0xc6,0xea,0x71,0x8c,0xc1,0xaa,0x60,
	   	0x0a,0xed,0x05,0xfb,0xf3,0x5e,0x08,0x4a,0x66,0x32,0xf6,0x07,0x2d,0xa9,0xad,0x13};

	unsigned char r[32],s[32];// Signature
	unsigned char IDA[16]={
		0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38};

	int IDA_len=16;
	unsigned char ENTLA[2]={0x00,0x80};//the length of userA's identification,presentation in ASCII code
	unsigned char *message="message digest";//the message to be signed
	int len=strlen(message);//the length of message
	unsigned char ZA[SM3_len/8];//ZA=Hash(ENTLA|| IDA|| a|| b|| Gx || Gy || xA|| yA)
	unsigned char Msg[210]; //210=IDA_len+2+SM2_NUMWORD*6
	int temp;

	miracl *mip=mirsys(10000,16);
	mip->IOBASE=16;
	temp=SM2_KeyGeneration(dA,xA,yA);
	if(temp)
		return temp;

	// ENTLA|| IDA|| a|| b|| Gx || Gy || xA|| yA
	memcpy(Msg,ENTLA,2);
	memcpy(Msg+2,IDA,IDA_len);
	memcpy(Msg+2+IDA_len,SM2_a,SM2_NUMWORD);
	memcpy(Msg+2+IDA_len+SM2_NUMWORD,SM2_b,SM2_NUMWORD);
	memcpy(Msg+2+IDA_len+SM2_NUMWORD*2,SM2_Gx,SM2_NUMWORD);
	memcpy(Msg+2+IDA_len+SM2_NUMWORD*3,SM2_Gy,SM2_NUMWORD);
	memcpy(Msg+2+IDA_len+SM2_NUMWORD*4,xA,SM2_NUMWORD);
	memcpy(Msg+2+IDA_len+SM2_NUMWORD*5,yA,SM2_NUMWORD);

	SM3_256(Msg,210,ZA);

	temp=SM2_Sign(message,len,ZA,rand,dA,r,s);
	if(temp)
		return temp;

	
	for(i=0;i<1000;i++)
		temp=SM2_Verify(message,len,ZA,xA,yA,r,s);
	if(temp){
		printf(" sm2 sv err \n");
		return temp;
	}else
		printf("sm2 sv ok \n");
	return 0;
}

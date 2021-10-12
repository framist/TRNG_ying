#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <openssl/crypto.h>
// #include <crypto/include/internal/sm3.h>
#include "sm3.h"
#include <openssl/evp.h>
unsigned char *sm3_hash(SM3_CTX* p,unsigned char *message,int len)
{
    
    // printf("num: %d\n",p->num);
    
    sm3_update(p,message,3);
    unsigned char *s=(unsigned char*)malloc(sizeof(char)*32);
    sm3_final(s,p);
    return s;    
}
unsigned char *get_random_block(SM3_CTX* p)
{
    int i,j;
    int plot;
    unsigned char *source;
    source=(unsigned char*)malloc(32*sizeof(char));
    for(i=0;i<64;i++)
    {        
        source[i]='a';        
    }
    
    return sm3_hash(p,(unsigned char*)source,32);
}
void get_trng_random(unsigned char* buf,int len)
{
    if(len<=0)
    {
        printf("Length can't below 0 !\n");
        exit(0);
    }
    
    SM3_CTX result;
    SM3_CTX* p=&result;
    sm3_init(p);

    int block=len/32;
    int times=len%32;
    
    char *current=buf;
    while(block>0)
    {
        memcpy(current,get_random_block(p),32);
        current=current+32;
        block--;
    }
    if(times>0)
    {
        memcpy(current,get_random_block(p),times);        
    }    
}

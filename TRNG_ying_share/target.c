#include<stdio.h>
#include<openssl/crypto.h>
#include "TRNG_ying.h"
int main()
{
    unsigned char* buf;
    buf=(unsigned char*)malloc(sizeof(unsigned char)*6);
    get_trng_random(buf,5);
    puts((const char*)buf);
    // printf("%d\n",)
}
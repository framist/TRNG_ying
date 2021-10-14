#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <openssl/crypto.h>
#include <crypto/include/internal/sm3.h>
#include <openssl/evp.h>
//unsigned char *sm3_hash(unsigned char *message,int len);
void get_trng_random(unsigned char* buf,int len);

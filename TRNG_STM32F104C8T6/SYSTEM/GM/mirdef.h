/* 
 *   MIRACL compiler/hardware definitions - mirdef.h
 *   This version suitable for use with most 32-bit computers
 *   e.g. 80386+ PC, VAX, ARM etc. Assembly language versions of muldiv,
 *   muldvm, muldvd and muldvd2 will be necessary. See mrmuldv.any 
 *
 *   Also suitable for DJGPP GNU C Compiler
 *   ... but change __int64 to long long
 */


/*
 *   MIRACL compiler/hardware definitions - mirdef.h
 */


#define MIRACL 32
#define MR_LITTLE_ENDIAN      

/* or possibly 
#define MR_BIG_ENDIAN      
*/

#define mr_utype int
#define MR_IBITS 32
#define mr_dltype __int64
#define mr_unsign32 unsigned int
#define mr_unsign64 unsigned __int64
#define MAXBASE ((mr_small)1<<(MIRACL-1))
#define MR_STATIC 20
#define MR_GENERIC_MT
#define MR_NOASM


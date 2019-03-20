//*----------------------------------------------------------------------------
//*         ATMEL Microcontroller Software Support  -  ROUSSET  -
//*----------------------------------------------------------------------------
//* The software is delivered "AS IS" without warranty or condition of any
//* kind, either express, implied or statutory. This includes without
//* limitation any warranty or condition with respect to merchantability or
//* fitness for any particular purpose, or against the infringements of
//* intellectual property rights of others.
//*----------------------------------------------------------------------------
//* File Name           : std_c.h
//* Object              : Standard C Header File
//*
//* 1.0 01/04/00 JCZ    : Creation
//*----------------------------------------------------------------------------

#ifndef std_c_h
#define std_c_h

/*----------------*/
/* Standard types */
/*----------------*/

#ifndef unsigned_int
#define unsigned_int
typedef unsigned int        u_int   ;
#endif

#ifndef unsigned_short
#define unsigned_short
typedef unsigned short      u_short ;
#endif

#ifndef unsigned_char
#define unsigned_char
typedef unsigned char       u_char  ;
#endif


/* 16-bit Flash Data type */
typedef volatile unsigned short flash_word ;

/* AT91 Register type */
typedef volatile unsigned int at91_reg ;

/*----------------*/
/* Boolean values */
/*----------------*/

#define TRUE                1
#define FALSE               0

#endif /* std_c_h */






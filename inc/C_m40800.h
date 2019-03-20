//*----------------------------------------------------------------------------
//* File Name           : m40800.h
//* Object              : AT91M40800 Definition File
//*
//* 1.0 03/02/00 wxl    : Creation
//*----------------------------------------------------------------------------
#ifndef C_m40800_h
#define C_m40800_h
#include "std_c.h"	

#define CPU_CLOCK		40000000

/*-------------------------------------*/
/* Peripheral Identifier and Interrupt */
/*-------------------------------------*/

#define FIQ_ID          0       /* Fast Interrupt */
#define SWIRQ_ID        1       /* Soft Interrupt (generated by the AIC) */
#define US0_ID          2       /* USART Channel 0 interrupt */
#define US1_ID          3       /* USART Channel 1 interrupt */
#define TC0_ID          4       /* Timer Channel 0 interrupt */
#define TC1_ID          5       /* Timer Channel 1 interrupt */
#define TC2_ID          6       /* Timer Channel 2 interrupt */
#define WDI_ID          7       /* Watchdog interrupt */
#define PIO_ID          8       /* Parallel I/O Controller A interrupt */

#define IRQ0_ID         16      /* External interrupt 0 */
#define IRQ1_ID         17      /* External interrupt 1 */
#define IRQ2_ID         18      /* External interrupt 2 */


#define US0_CHAR_ID     21       /* USART Channel 0 interrupt */
#define US0_BLOCK_ID    22       /* USART Channel 0 interrupt */

#define US1_CHAR_ID     23       /* USART Channel 1 interrupt */
#define US1_BLOCK_ID    24       /* USART Channel 1 interrupt */



/*-------------------------*/
/* Internal RAM Definition */
/*-------------------------*/

#define RAM_SIZE        (8*1024)
#define RAM_BASE        ((u_int *)0x00000000)
#define RAM_LIMIT       (((u_int)RAM_BASE) + RAM_SIZE)


/* The internal RAM is mapped at address 0x00300000 after reset until
   Remap command is performed on the EBI. */

#define RAM_BASE_BOOT   ((u_int *)0x00300000)


/*-----------------------*/
/* Peripheral Memory Map */
/*-----------------------*/
#define WD_BASE             (( StructWD*)  0xFFFF8000)      /* WD: Watchdog Timer Base Address */
#define PIO_BASE            ((StructPIO*)  0xFFFF0000)      /* Parallel I/O Controller Base Address  */


typedef struct
{
    at91_reg    SF_CIDR ;       /* Chip Identification Register */
    at91_reg    SF_EXID ;       /* Chip Identification Extension */
    at91_reg    SF_RSR ;		/* Reset Status Flag */
    at91_reg    SF_MMR ;		/* Memory Mode Register */
    at91_reg    Reserved0 ;
    at91_reg    Reserved1 ;
    at91_reg    SF_PMR ;        /* Protect Mode Register */
} StructSF ;

#define SF_BASE            ((StructSF *)  0xFFF00000)	/* Special Function Base Address */

/*------------------------*/
/* External Bus Interface */
/*------------------------*/

typedef struct
{
    at91_reg        EBI_CSR[8] ;        /* Chip Select Register */
    at91_reg        EBI_RCR ;           /* Remap Control Register */
    at91_reg        EBI_MCR ;           /* Memory Control Register */
} StructEBI ;

#define EBI_BASE            ((StructEBI *)0xFFE00000)	/* EBI Base Address */

/*--------------------------*/
/* Power Saving Controller  */
/*--------------------------*/
typedef struct
{
    at91_reg    PS_CR ;     /* Control Register */
    at91_reg    PS_PCER ;   /* Peripheral Clock Enable Register */
    at91_reg    PS_PCDR ;   /* Peripheral Clock Disable Register */
    at91_reg    PS_PCSR ;   /* Peripheral Clock Status Register */  
} StructPS ;

#define PS_BASE         (( StructPS *) 0xFFFF4000)	/* Power Saving Base Address */

/*-------------------------------*/
/* Advanced Interrupt Controller */
/*-------------------------------*/

typedef struct
{
    at91_reg        AIC_SMR[32] ;       /* Source Mode Register */
    at91_reg        AIC_SVR[32] ;       /* Source Vector Register */
    at91_reg        AIC_IVR ;           /* IRQ Vector Register */
    at91_reg        AIC_FVR ;           /* FIQ Vector Register */
    at91_reg        AIC_ISR ;           /* Interrupt Status Register */
    at91_reg        AIC_IPR ;           /* Interrupt Pending Register */
    at91_reg        AIC_IMR ;           /* Interrupt Mask Register */
    at91_reg        AIC_CISR ;          /* Core Interrupt Status Register */
    at91_reg        reserved0 ;
    at91_reg        reserved1 ;
    at91_reg        AIC_IECR ;          /* Interrupt Enable Command Register */
    at91_reg        AIC_IDCR ;          /* Interrupt Disable Command Register */
    at91_reg        AIC_ICCR ;          /* Interrupt Clear Command Register */
    at91_reg        AIC_ISCR ;          /* Interrupt Set Command Register */
    at91_reg        AIC_EOICR ;         /* End of Interrupt Command Register */
    at91_reg        AIC_SPU ;           /* Spurious Vector Register */
} StructAIC ;

#define AIC_BASE                        ((StructAIC *)0xFFFFF000)	/* Advanced Interrupt Controller */

/*-------------------------*/
/* Parallel I/O Controller */
/*-------------------------*/

typedef struct
{
    at91_reg        PIO_PER ;           /* PIO Enable Register */
    at91_reg        PIO_PDR ;           /* PIO Disable Register */
    at91_reg        PIO_PSR ;           /* PIO Status Register */
    at91_reg        Reserved0 ;
    at91_reg        PIO_OER ;           /* Output Enable Register */
    at91_reg        PIO_ODR ;           /* Output Disable Register */
    at91_reg        PIO_OSR ;           /* Output Status Register */
    at91_reg        Reserved1 ;
    at91_reg        PIO_IFER ;          /* Input Filter Enable Register */
    at91_reg        PIO_IFDR ;          /* Input Filter Disable Register */
    at91_reg        PIO_IFSR ;          /* Input Filter Status Register */
    at91_reg        Reserved2 ;
    at91_reg        PIO_SODR ;          /* Set Output Data Register */
    at91_reg        PIO_CODR ;          /* Clear Output Data Register */
    at91_reg        PIO_ODSR ;          /* Output Data Status Register */
    at91_reg        PIO_PDSR ;          /* Pin Data Status Register */
    at91_reg        PIO_IER ;           /* Interrupt Enable Register */
    at91_reg        PIO_IDR ;           /* Interrupt Disable Register */
    at91_reg        PIO_IMR ;           /* Interrupt Mask Register */
    at91_reg        PIO_ISR ;           /* Interrupt Status Register */
} StructPIO ;


#define NB_PIO          31          /* Number of PIO Lines */

/*------------------*/
/* Pin Multiplexing */
/*------------------*/
/* PIO Controller  */

#define PIOTCLK0        0          /* Timer 0 Clock signal */
#define PIOTIOA0        1          /* Timer 0 Signal A */
#define PIOTIOB0        2          /* Timer 0 Signal B */

#define PIOTCLK1        3          /* Timer 1 Clock signal  */
#define PIOTIOA1        4          /* Timer 1 Signal A */
#define PIOTIOB1        5          /* Timer 1 Signal B */

#define PIOTCLK2        6          /* Timer 2 Clock signal input */
#define PIOTIOA2        7          /* Timer 2 Signal A */
#define PIOTIOB2        8          /* Timer 2 Signal B */

#define PIOIRQ0         9          /* External Interrupt 0 */
#define PIOIRQ1         10         /* External Interrupt 1 */
#define PIOIRQ2         11         /* External Interrupt 2 */
#define PIOFIQ          12         /* Fast Interrupt input */

#define PIOSCK0         13         /* USART 0 clock signal */
#define PIOTXD0         14         /* USART 0 transmit data signal */
#define PIORXD0         15         /* USART 0 receive data signal */

#define PIOSCK1         20         /* USART 1 clock signal */
#define PIOTXD1         21         /* USART 1 transmit data signal */
#define PIORXD1         22         /* USART 1 receive data signal */

#define PIOMCK0         25         /* Master Clock Output */
#define PIONCS2         26         /* Chip Select 2 */
#define PIONCS3         27         /* Chip Select 3 */
#define PIOCS7          28         /* Address 20/Chip Select 7 */
#define PIOCS6          29         /* Address 21/Chip Select 6 */
#define PIOCS5          30         /* Address 22/Chip Select 5 */
#define PIOCS4          31         /* Address 23/Chip Select 4 */

/*--------*/
/* USARTs */
/*--------*/

#define USART1_BASE         ((StructUSART*)0xFFFCC000)      /* USART 1 Base Address */
#define USART0_BASE         ((StructUSART*)0xFFFD0000)      /* USART 0 Base Address */

typedef struct
{
    at91_reg            US_CR ;         /* Control Register */
    at91_reg            US_MR ;         /* Mode Register */
    at91_reg            US_IER ;        /* Interrupt Enable Register */
    at91_reg            US_IDR ;        /* Interrupt Disable Register */
    at91_reg            US_IMR ;        /* Interrupt Mask Register */
    at91_reg            US_CSR ;        /* Channel Status Register */
    at91_reg            US_RHR ;        /* Receive Holding Register */
    at91_reg            US_THR ;        /* Transmit Holding Register */
    at91_reg            US_BRGR ;       /* Baud Rate Generator Register */
    at91_reg            US_RTOR ;       /* Receiver Timeout Register */
    at91_reg            US_TTGR ;       /* Transmitter Time-guard Register */
    at91_reg            Reserved ;
    at91_reg            US_RPR ;        /* Receiver Pointer Register */
    at91_reg            US_RCR ;        /* Receiver Counter Register */
    at91_reg            US_TPR ;        /* Transmitter Pointer Register */
    at91_reg            US_TCR ;        /* Transmitter Counter Register */
} StructUSART ;

/*----------*/
/* Watchdog */
/*----------*/

typedef struct
{
    at91_reg        WD_OMR ;        /* Overflow Mode Register */
    at91_reg        WD_CMR ;        /* Clock Mode Register */
    at91_reg        WD_CR ;         /* Control Register */
    at91_reg        WD_SR ;         /* Status Register */
} StructWD ;


/*---------------*/
/* Timer Counter */
/*---------------*/

/* Timer Counter Base Addresses */
#define TCB0_BASE           ((StructTCBlock*)0xFFFE00C0)
#define TC0_BASE            ((StructTC *)0xFFFE0000)
#define TC1_BASE            ((StructTC *)0xFFFE0040)
#define TC2_BASE            ((StructTC *)0xFFFE0080)

typedef struct
{
    at91_reg        TC_CCR ;        /* Control Register */
    at91_reg        TC_CMR ;        /* Mode Register */
    at91_reg        Reserved0 ;
    at91_reg        Reserved1 ;
    at91_reg        TC_CV ;         /* Counter value */
    at91_reg        TC_RA ;         /* Register A */
    at91_reg        TC_RB ;         /* Register B */
    at91_reg        TC_RC ;         /* Register C */
    at91_reg        TC_SR ;         /* Status Register */
    at91_reg        TC_IER ;        /* Interrupt Enable Register */
    at91_reg        TC_IDR ;        /* Interrupt Disable Register */
    at91_reg        TC_IMR ;        /* Interrupt Mask Register */
} StructTC ;


//#define NB_TC_CHANNEL       3

typedef struct
{
//    StructTC        TC[NB_TC_CHANNEL] ;
    at91_reg        TC_BCR ;        /* Block Control Register */
    at91_reg        TC_BMR ;        /* Block Mode Register  */
} StructTCBlock ;


#endif 






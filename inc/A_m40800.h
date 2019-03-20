//********************************************************************************
//*	�汾��	loaderv200 															**
//* ����:	2005-03-13															**
//*	��д:	dv	in  Beijing														**
//* ����:	1 ���cpu������														**
//*			2 �����뿽����sram������											**
//********************************************************************************
#ifndef			__ARM_m40800_H_
#define 		__ARM_m40800_H_	

//*����CPU��CS

//*���߿��			λ��<<0
#define EBI_DBW_0				(0<<0)			//����
#define EBI_DBW_16				(1<<0)			//16λ����
#define	EBI_DBW_8				(2<<0)			//0λ����
#define	EBI_DBW_4				(3<<0)			//����

//*������ʱ����		λ��<<2
#define	EBI_NWS_1				(0<<2)			//������ʱ 1 ʱ������
#define	EBI_NWS_2				(1<<2)			//������ʱ 2 ʱ������
#define	EBI_NWS_3				(2<<2)			//������ʱ 3 ʱ������
#define	EBI_NWS_4				(3<<2)			//������ʱ 4 ʱ������
#define	EBI_NWS_5				(4<<2)			//������ʱ 5 ʱ������
#define	EBI_NWS_6				(5<<2)			//������ʱ 6 ʱ������
#define	EBI_NWS_7				(6<<2)			//������ʱ 7 ʱ������
#define	EBI_NWS_8				(7<<2)			//������ʱ 8 ʱ������

//*������ʱ����		λ��<<5
#define	EBI_WSE_ON				(1<<5)			//������ʱʹ��
#define	EBI_WSE_OFF				(0<<5)			//������ʱ��ֹ

//*Ƭѡҳ�ߴ�		λ��<<7
#define	EBI_PAGES_1M			(0<<7)			//Ƭѡҳ�ߴ�1M  �ֽ�
#define	EBI_PAGES_4M			(1<<7)			//Ƭѡҳ�ߴ�4M  �ֽ�
#define	EBI_PAGES_16M			(2<<7)			//Ƭѡҳ�ߴ�16M �ֽ�
#define	EBI_PAGES_64M			(3<<7)			//Ƭѡҳ�ߴ�64M �ֽ�

//*�������������ʱ������	λ��<<9
#define	EBI_TDF_0				(0<<9)			//�������������ʱ�� 0 ����
#define	EBI_TDF_1				(1<<9)			//�������������ʱ�� 0 ����
#define	EBI_TDF_2				(2<<9)			//�������������ʱ�� 0 ����
#define	EBI_TDF_3				(3<<9)			//�������������ʱ�� 0 ����
#define	EBI_TDF_4				(4<<9)			//�������������ʱ�� 0 ����
#define	EBI_TDF_5				(5<<9)			//�������������ʱ�� 0 ����
#define	EBI_TDF_6				(6<<9)			//�������������ʱ�� 0 ����
#define	EBI_TDF_7				(7<<9)			//�������������ʱ�� 0 ����

//*�ֽڷ�������
#define	EBI_B_WR				(0<<12)			//�ֽ�д
#define	EBI_B_SEL				(1<<12)			//�ֽ�ѡ��

//*Ƭѡʹ��
#define	EBI_CSD					(0<<13)			//Ƭѡ��ֹ
#define	EBI_CSEN				(1<<13)			//Ƭѡʹ��


//Interrupt 
#define IRQ  					0
#define FIQ  					1

//CPU
#define ARM_MODE_USER			0x10
#define ARM_MODE_FIQ			0x11
#define ARM_MODE_IRQ			0x12
#define ARM_MODE_SVC			0x13
#define ARM_MODE_ABORT      	0x17
#define ARM_MODE_UNDEF      	0x1B
#define ARM_MODE_SYS        	0x1F

#define I_BIT           		0x80
#define F_BIT           		0x40
#define T_BIT           		0x20

#define AIC_IVR     			0x100
#define AIC_SVR					0x080
#define AIC_SPU					0x134
#define AIC_EOICR				0x130

#define AIC_BASE           		0xFFFFF000
	
//flash
#define EBI_CSR_0       		0x01000000|EBI_CSEN|EBI_B_WR|EBI_TDF_0|EBI_PAGES_4M|EBI_WSE_ON|EBI_NWS_2|EBI_DBW_16
#define EBI_CSR_1				0x02000000|EBI_CSEN|EBI_B_WR|EBI_TDF_0|EBI_PAGES_4M|EBI_WSE_ON|EBI_NWS_2|EBI_DBW_16	
#define	EBI_CSR_2       		0x03000000|EBI_CSEN|EBI_B_WR|EBI_TDF_0|EBI_PAGES_4M|EBI_WSE_ON|EBI_NWS_4|EBI_DBW_16      
#define EBI_CSR_3       		0x04000000|EBI_CSEN|EBI_B_WR|EBI_TDF_2|EBI_PAGES_4M|EBI_WSE_ON|EBI_NWS_5|EBI_DBW_16		
#define EBI_CSR_4       		0x02402021 	
#define EBI_CSR_5       		0x02502021      
#define EBI_CSR_6       		0x07000000      
#define EBI_CSR_7       		0x08000000  
#define EBI_BASE        		0xFFE00000
		
#define RAM_BASE_BOOT   		0x300000			

//����Ӳ��������Ҫ�޸ĵĵط�
#define FLASH_BASE       		0x01000000		
#define RAM_BASE       			0x00000000	
#define COPY_TO_RAM_LENTH       1024*32	


#define	EXCEPTION_STACK_LENTH	0x1FF0
#define TOP_EXCEPTION_STACK     RAM_BASE+1024*256   
#define TOP_APPLICATION_STACK  	TOP_EXCEPTION_STACK-EXCEPTION_STACK_LENTH

#endif






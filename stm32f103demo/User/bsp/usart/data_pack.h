#ifndef __DATA_PACK_H
#define __DATA_PACK_H

#include "stm32f10x.h"
/* c ��׼�� */
#include <stdio.h>
#include <string.h>
#include <stdint.h>


/**
  ******************************************************************
													   �궨��
  ******************************************************************
  */ 
	
/**
* @name Log print macro definition
* @{
*/
#define DEBUG_LOG_ENABLE	1

#if  DEBUG_LOG_ENABLE 
#define DEBUG_LOG 	printf                 ///<Run log print

#ifndef DEBUG_LOG_NOFORMAT
#define DEBUG_LOG_NOFORMAT 	printf
#endif

#endif

/**@} */
	
//��Բ�ͬ�ı��������ò�ͬ��stdint.h�ļ�
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
    #include <stdint.h>
#endif

//����
#define AssertCalled(char,int) 	printf("Error:%s,%d\r\n",char,int)
#define ASSERT(x) if((x)==0) 		AssertCalled(__FILE__,__LINE__)
typedef enum {ASSERT_ERR = 0, ASSERT_SUCCESS = !ERROR} Assert_ErrorStatus;

	
	
#define USE_DATA_CRC  1
	

#define NAME_HEAD1   'x'
#define NAME_HEAD2   'c'
#define NAME_HEAD3   'a'
#define NAME_HEAD4   'i'

#define NAME_TAIL1   'x'
#define NAME_TAIL2   'j'
#define NAME_TAIL3   'i'
#define NAME_TAIL4   'e'

#define DATA_HEAD   Get_Data_Head()
#define DATA_TAIL   Get_Data_Tail()





/**
  ******************************************************************
													   ��������
  ******************************************************************
  */ 
typedef struct datapack
{
	uint16_t data_length; // ���ݳ���
	uint16_t data_crc; // ���ݳ���
	uint32_t data_head; //����ͷ
//	char data_type; 			//��������
	void* data; 		//����
	uint32_t data_tail; //����β
}DataPack;




/**
  ******************************************************************
														��������
  ******************************************************************
  */ 

int32_t Create_Pack(void *buff,uint16_t len);
int32_t Usart_Write(uint8_t *buf, uint32_t len);
uint32_t Get_Data_Head(void);
uint32_t Get_Data_Tail(void);





#endif /* __DATA_PACK_H */

#ifndef __DATA_PACK_H
#define __DATA_PACK_H

#include "stm32f10x.h"
#include "include.h"
/* c ��׼�� */
#include <stdio.h>
#include <string.h>
#include <stdint.h>


/**
  ******************************************************************
													   �궨��
  ******************************************************************
  */ 
#define NAME_HEAD   "cai"
#define NAME_TAIL   "jie"

#define DATA_HEAD   Get_Data_Head()
#define DATA_TAIL   Get_Data_Tail()



/**
  ******************************************************************
													   ��������
  ******************************************************************
  */ 
typedef struct datapack
{
	uint32_t data_head; //����ͷ
	uint32_t data_length; // ���ݳ���
//	char data_type; 			//��������
	void* data; 		//����
	uint32_t data_tail; //����β
}DataPack;




/**
  ******************************************************************
														��������
  ******************************************************************
  */ 

void Create_Pack(void *buff);

uint32_t Get_Data_Head(void);
uint32_t Get_Data_Tail(void);





#endif /* __DATA_PACK_H */

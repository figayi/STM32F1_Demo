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

#define USE_DATA_CRC  0
/* ����֡ͷ */
#define NAME_HEAD1   'n'
#define NAME_HEAD2   'c'
#define NAME_HEAD3   'a'
#define NAME_HEAD4   'i'
/* ����֡β */
#define NAME_TAIL1   'n'
#define NAME_TAIL2   'j'
#define NAME_TAIL3   'i'
#define NAME_TAIL4   'e'

//#define DATA_HEAD   Get_Data_Head()
//#define DATA_TAIL   Get_Data_Tail()



/**
  ******************************************************************
													   ��������
  ******************************************************************
  */ 
//typedef struct datapack
//{
//	uint16_t data_length; // ���ݳ���
//	uint16_t data_crc; // ���ݳ���
//	uint32_t data_head; //����ͷ
////	char data_type; 			//��������
//	void* data; 		//����
//	uint32_t data_tail; //����β
//}DataPack;




/**
  ******************************************************************
														��������
  ******************************************************************
  */ 
int32_t Send_DataPack(void *buff,uint16_t len);
int32_t Usart_Write(uint8_t *buf, uint32_t len);




#endif /* __DATA_PACK_H */

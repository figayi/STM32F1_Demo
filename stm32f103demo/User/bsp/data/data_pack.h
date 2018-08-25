#ifndef __DATA_PACK_H
#define __DATA_PACK_H

#include "stm32f10x.h"
/* c ��׼�� */
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "./usart/bsp_usart.h"

/**
  ******************************************************************
													   �궨��
  ******************************************************************
  */ 

#define USE_DATA_CRC  0
/* ����֡ͷ */
#define DATA_HEAD   0x02

/* ����֡β */
#define DATA_TAIL   0x03




/**
  ******************************************************************
													   ��������
  ******************************************************************
  */ 
typedef struct datapack
{
	uint8_t data_head; //����ͷ
	uint8_t data_tail; //����β
//	uint8_t data_type; 			//��������
	uint16_t data_length; // ���ݳ���
	uint32_t data_crc; // ����У��
}DataPack;




/**
  ******************************************************************
														��������
  ******************************************************************
  */ 
int32_t Send_DataPack(void *buff,uint16_t len);
int32_t DataPack_Handle(uint8_t* buff,DataPack* datapack);

#if USE_USART_DMA_RX
void Uart_DMA_Rx_Data(void);
#else
void Receive_DataPack(void);
#endif

#if USE_USART_DMA_TX
void DMA_Send_Data(uint32_t len);
#else
int32_t Usart_Write(uint8_t *buf, uint32_t len);
#endif




#endif /* __DATA_PACK_H */

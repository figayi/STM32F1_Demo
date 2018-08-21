/* ͷ�ļ� */

#include "include.h"

/**
  ******************************************************************
  * @brief   ����֡�����ʽ
  * @author  jiejie
  * @version V1.0
  * @date    2018-xx-xx
  * 
	* --------------------------------------------------------------------------
	* |     ��ʼ֡    |   ���ݳ���   |  ��Ч����   |   У��   |  ����֡        |
	* --------------------------------------------------------------------------
	* | cai(0x636169) |    length    |    buff     |   У��   |  jie(0x6a6965) |
	* --------------------------------------------------------------------------
	* |     uint32    |     uint32   |    buff     |  uint32  |     uint32     |
	* -------------------------------------------------------------------------- 
	* |     4�ֽ�     |     4�ֽ�    |    buff     |   4�ֽ�  |     4�ֽ�      |
	* -------------------------------------------------------------------------- 
	* 
	* 
	* 
	*  
	* 
	* 
	* 
	******************************************************************
  */ 

void Read_Ack(DataPack *p,u8 len);

/**
  ******************************************************************
  * @brief   ����һ�����ݰ�
  * @author  jiejie
  * @version V1.0
  * @date    2018-xx-xx
  * @param   buff:���ݵ�ַ
  * @return  �µ����ݰ���ַ
  ******************************************************************
  */ 
void Create_Pack(void *buff,uint32_t len)
{
//	DataPack data_pack;
	
//#if USE_USART_DMA_TX
//	
	uint8_t copy_buff[USART_RX_BUFF_SIZE];
	
	uint8_t *pTxBuf = copy_buff;
	
	*pTxBuf ++= NAME_HEAD1; 
	*pTxBuf ++= NAME_HEAD2; 
	*pTxBuf ++= NAME_HEAD3; 
	*pTxBuf ++= NAME_HEAD4; 

	memcpy(copy_buff + 8 , buff , len);
	pTxBuf = (copy_buff + 8 + len);
	
	
	
	*pTxBuf ++= NAME_TAIL1; 
	*pTxBuf ++= NAME_TAIL2; 
	*pTxBuf ++= NAME_TAIL3; 
	*pTxBuf ++= NAME_TAIL4; 
	
	
//	data_pack.data = &copy_buff;
	
	printf("%s",copy_buff);
//#else
//	data_pack.data_head = DATA_HEAD; 
//	data_pack.data_length = len;
//	data_pack.data = buff;
//	
//	data_pack.data_tail = DATA_TAIL;
//#endif	
	

	
	
#if USE_USART_DMA_TX
	
#else
	
	printf("%x",(uint32_t) data_pack.data_head);
	printf("%d",data_pack.data_length);
	printf("%s",(char *) data_pack.data);
	printf("%x",(uint32_t) data_pack.data_tail);
	
#endif	
}

/**
  ******************************************************************
  * @brief   ��ȡ���ݰ���ʼ֡(�����4�ֽ�)��Ŀǰ����Ϊ3�ֽڣ������޸ļ���
  * @author  jiejie
  * @version V1.0
  * @date    2018-xx-xx
  ******************************************************************
  */ 
uint32_t Get_Data_Head(void)
{
	return  (uint32_t)(NAME_TAIL1 << 24) | (NAME_TAIL2 << 16) | \
										(NAME_TAIL3 << 8) | (NAME_TAIL4);
}

/**
  ******************************************************************
  * @brief   ��ȡ���ݰ�����֡(�����4�ֽ�)��Ŀǰ����Ϊ3�ֽڣ�
  * @author  jiejie
  * @version V1.0
  * @date    2018-xx-xx
  ******************************************************************
  */ 
uint32_t Get_Data_Tail(void)
{
	return  (uint32_t)(NAME_HEAD1 << 24) | (NAME_HEAD2 << 16) | \
										(NAME_HEAD3 << 8) | (NAME_HEAD4);
}










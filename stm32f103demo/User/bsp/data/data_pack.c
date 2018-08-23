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
	* |     uint32    |     uint16   |    buff     |  uint16  |     uint32     |
	* -------------------------------------------------------------------------- 
	* |     4�ֽ�     |     2�ֽ�    |    buff     |   2�ֽ�  |     4�ֽ�      |
	* -------------------------------------------------------------------------- 
	* 
	******************************************************************
  */ 
/**
  ******************************************************************
														�ڲ����ú�������
  ******************************************************************
  */ 
//static uint32_t Get_Data_Head(void);
//static uint32_t Get_Data_Tail(void);

/**
  ******************************************************************
  * @brief   �������ݣ������ݰ���ʽ
  * @author  jiejie
  * @version V1.0
  * @date    2018-xx-xx
  * @param   buff:������ʼ��ַ
  * @return  �ɹ� or ʧ��
  ******************************************************************
  */ 
int32_t Send_DataPack(void *buff,uint16_t data_len)
{

#if USE_USART_DMA_TX	
	uint8_t *pTxBuf = Usart_Tx_Buf;
#else
	/* ʹ����ͨ���ڷ��� */
	int32_t res;
	uint8_t Usart_Tx_Buf[data_len+14];
	uint8_t *pTxBuf = Usart_Tx_Buf;
#endif
	
#if USE_DATA_CRC 
	__IO uint32_t CRCValue;
#endif
	/* buff ��Ч	*/
	if(NULL == buff)
	{
		DEBUG_LOG(">> [ERR] data Is Null \n");
		ASSERT(ASSERT_ERR);
		return -1;
	}
	/* ����֡ͷ */
	*pTxBuf ++= (uint8_t)NAME_HEAD1; 
	*pTxBuf ++= (uint8_t)NAME_HEAD2; 
	*pTxBuf ++= (uint8_t)NAME_HEAD3; 
	*pTxBuf ++= (uint8_t)NAME_HEAD4; 
	
	/* ���ݳ��� */
	*pTxBuf ++= (uint8_t)(data_len>>8);  
	*pTxBuf ++= (uint8_t)(data_len);
	
	/* ��Ч���� */
	if(0 != data_len)
	{
		memcpy(Usart_Tx_Buf + 6 , buff , data_len);
	}
	/* ����ƫ�� */
	pTxBuf = (Usart_Tx_Buf + 6 + data_len);
	
#if USE_DATA_CRC 
	/* ʹ��CRCУ�� */
	CRCValue = CRC_CalcBlockCRC((uint32_t *)buff, data_len);
	/* У��ֵ */
	*pTxBuf ++= CRCValue >> 24; 
	*pTxBuf ++= CRCValue >> 16; 
	*pTxBuf ++= CRCValue >> 8;  
	*pTxBuf ++= CRCValue ; 
#endif
	/* ����֡β */
	*pTxBuf ++= (uint8_t)NAME_TAIL1; 
	*pTxBuf ++= (uint8_t)NAME_TAIL2; 
	*pTxBuf ++= (uint8_t)NAME_TAIL3; 
	*pTxBuf ++= (uint8_t)NAME_TAIL4; 
	
#if USE_USART_DMA_TX	
	
#if USE_DATA_CRC 
	DMA_Send_Data(data_len + 14);
#else
	DMA_Send_Data(data_len + 10);
#endif

#else

#if USE_DATA_CRC 
	res = Usart_Write(Usart_Tx_Buf,data_len + 14);
#else
	res = Usart_Write(Usart_Tx_Buf,data_len + 10);
#endif
	if(res < 0)
	{
		DEBUG_LOG("uart write error %d \n", res);
		ASSERT(ASSERT_ERR);
		return -2;
	}
	
#endif
	
	return 0;
}

int32_t Usart_Write(uint8_t *buf, uint32_t len)
{
	uint32_t i = 0;
	/* �ж� buff �ǿ� */
	if(NULL == buf)
	{
		return -1;
	}
	/* ѭ�����ͣ�һ���ֽ� */
	for(i=0; i<len; i++)
	{
		Usart_SendByte(DEBUG_USARTx,buf[i]);
	}
	return len;
}  



///**
//  ******************************************************************
//  * @brief   ��ȡ���ݰ���ʼ֡(�����4�ֽ�)��Ŀǰ����Ϊ3�ֽڣ������޸ļ���
//  * @author  jiejie
//  * @version V1.0
//  * @date    2018-xx-xx
//  ******************************************************************
//  */ 
//static uint32_t Get_Data_Head(void)
//{
//	return  (uint32_t)(NAME_TAIL1 << 24) | (NAME_TAIL2 << 16) | \
//										(NAME_TAIL3 << 8) | (NAME_TAIL4);
//}

///**
//  ******************************************************************
//  * @brief   ��ȡ���ݰ�����֡(�����4�ֽ�)��Ŀǰ����Ϊ3�ֽڣ�
//  * @author  jiejie
//  * @version V1.0
//  * @date    2018-xx-xx
//  ******************************************************************
//  */ 
//static uint32_t Get_Data_Tail(void)
//{
//	return  (uint32_t)(NAME_HEAD1 << 24) | (NAME_HEAD2 << 16) | \
//										(NAME_HEAD3 << 8) | (NAME_HEAD4);
//}










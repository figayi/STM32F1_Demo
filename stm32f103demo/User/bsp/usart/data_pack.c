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
int32_t Create_Pack(void *buff,uint16_t data_len)
{
#if USE_USART_DMA_TX	
	uint8_t *pTxBuf = Usart_Tx_Buf;
#else
	uint8_t Usart_Tx_Buf[data_len+14];
	uint8_t *pTxBuf = Usart_Tx_Buf;
#endif
	uint32_t CRCValue;
	
	if(NULL != buff)
	{
		DEBUG_LOG(">> [ERR] data Is Null \n");
		ASSERT(ASSERT_ERR);
		return -1;
	}
	
	*pTxBuf ++= (uint8_t)NAME_HEAD1; 
	*pTxBuf ++= (uint8_t)NAME_HEAD2; 
	*pTxBuf ++= (uint8_t)NAME_HEAD3; 
	*pTxBuf ++= (uint8_t)NAME_HEAD4; 
	
	*pTxBuf ++= (uint8_t)(data_len>>8);  
	*pTxBuf ++= (uint8_t)(data_len);
	
	if(0 != data_len)
	{
		memcpy(Usart_Tx_Buf + 6 , buff , data_len);
	}
	
	pTxBuf = (Usart_Tx_Buf + 6 + data_len);
	
#if USE_DATA_CRC 
	CRCValue = CRC_CalcBlockCRC((uint32_t *)buff, data_len);
	
	*pTxBuf ++= CRCValue >> 24; 
	*pTxBuf ++= CRCValue >> 16; 
	*pTxBuf ++= CRCValue >> 8;  
	*pTxBuf ++= CRCValue ; 
#endif
	*pTxBuf ++= (uint8_t)NAME_TAIL1; 
	*pTxBuf ++= (uint8_t)NAME_TAIL2; 
	*pTxBuf ++= (uint8_t)NAME_TAIL3; 
	*pTxBuf ++= (uint8_t)NAME_TAIL4; 
	
#if USE_USART_DMA_TX	
	DMA_Send_Data(data_len + 14);
#else
//	printf("%s",(char *) Usart_Tx_Buf);
	Usart_Write(Usart_Tx_Buf,data_len + 14);
#endif

	return 0;
}

int32_t Usart_Write(uint8_t *buf, uint32_t len)
{
//		uint8_t crc[1] = {0x55};
    uint32_t i = 0;
	
    if(NULL == buf)
    {
        return -1;
    }

    for(i=0; i<len; i++)
    {
			Usart_SendByte(DEBUG_USARTx,buf[i]);
//        if(i >=2 && buf[i] == 0xFF)
//        {
//						HAL_UART_Transmit_IT(&huart2, (uint8_t *)&crc, 1);
//						while (huart2.gState != HAL_UART_STATE_READY);//Loop until the end of transmission
//        }
    }

#ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
    for(i=0; i<len; i++)
    {
        GIZWITS_LOG("%02x ", buf[i]);

        if(i >=2 && buf[i] == 0xFF)
        {
            GIZWITS_LOG("%02x ", 0x55);
        }
    }
    GIZWITS_LOG("\n");
#endif
		
		return len;
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










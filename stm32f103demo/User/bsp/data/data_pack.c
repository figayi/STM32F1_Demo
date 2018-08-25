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
	* |     ��ʼ֡    |   ���ݳ���   |  ��Ч����   |   У��   |    ����֡      |
	* --------------------------------------------------------------------------
	* | cai(0x636169) |    length    |    buff     |   У��   |     0x55       |
	* --------------------------------------------------------------------------
	* |     uint8     |     uint16   |    buff     |  uint32  |     uint8      |
	* -------------------------------------------------------------------------- 
	* |     1�ֽ�     |     2�ֽ�    |    buff     |   4�ֽ�  |     1�ֽ�      |
	* -------------------------------------------------------------------------- 
	* 
	******************************************************************
  */ 
  
/**                    Usart_Rx_Sta 
	* ----------------------------------------------------------
	* | 1  1  0  0 | 0  0  0  0 | 0  0  0  0 | 0  0  0  0|
	* -----------------------------------------------------------
	* | 0  0  0  0 | 0  0  0  0 | 0  0  0  0 | 0  0  0  0|
	* -----------------------------------------------------------
�����λ���ڱ��������Ƿ�������   0��δ��� 1�����
�θ�λ���ڱ����Ƿ��յ�����֡ͷ     0��δ��� 1�����
����λ���ڱ������ݳ���
*/
/* ����״̬��� */
uint32_t Usart_Rx_Sta = 0;  

  
/**
  ******************************************************************
														�ڲ����ú�������
  ******************************************************************
  */ 


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
		PRINT_ERR("data Is Null \n");
		ASSERT(ASSERT_ERR);
		return -1;
	}
	/* ����֡ͷ */
	*pTxBuf ++= (uint8_t)DATA_HEAD; 

	/* ���ݳ��� */
	*pTxBuf ++= (uint8_t)(data_len>>8);  
	*pTxBuf ++= (uint8_t)(data_len);
	
	/* ��Ч���� */
	if(0 != data_len)
	{
		memcpy(Usart_Tx_Buf + 3 , buff , data_len);
	}
	/* ����ƫ�� */
	pTxBuf = (Usart_Tx_Buf + 3 + data_len);
	
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
	*pTxBuf ++= (uint8_t)DATA_TAIL; 

	
#if USE_USART_DMA_TX	
	
#if USE_DATA_CRC 
	DMA_Send_Data(data_len + 8);
#else
	DMA_Send_Data(data_len + 4);
#endif

#else

#if USE_DATA_CRC 
	res = Usart_Write(Usart_Tx_Buf,data_len + 8);
#else
	res = Usart_Write(Usart_Tx_Buf,data_len + 4);
#endif
	if(res < 0)
	{
		PRINT_ERR("uart write error %d \n", res);
		ASSERT(ASSERT_ERR);
		return -2;
	}
	
#endif
	
	return 0;
}
#if USE_USART_DMA_TX
void DMA_Send_Data(uint32_t len)
{
 
	DMA_Cmd(USART_TX_DMA_CHANNEL, DISABLE);                     //�ر�DMA���� 
//	
////	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//ȷ��DMA���Ա�����  
//		
	DMA_SetCurrDataCounter(USART_TX_DMA_CHANNEL,len);          //�������ݴ�����  
// 
	DMA_Cmd(USART_TX_DMA_CHANNEL, ENABLE); 
  USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Tx, ENABLE);       //����DMA���� 
}	  
#else

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

#endif


/************************************************************
  * @brief   Usart_Receive_Data
  * @param   NULL
  * @return  NULL
  * @author  jiejie
  * @github  https://github.com/jiejieTop
  * @date    2018-xx-xx
  * @version v1.0
  * @note    ��ʹ�ô��� DMA ����ʱ���õĺ���
  ***********************************************************/
#if USE_USART_DMA_RX
void Uart_DMA_Rx_Data(void)
{
	/* ���յ����ݳ��� */
	uint32_t buff_length;
	
	/* �ر�DMA ����ֹ���� */
	DMA_Cmd(USART_RX_DMA_CHANNEL, DISABLE);  
	
	/* ��ȡ���յ������ݳ��� ��λΪ�ֽ�*/
	buff_length = USART_RX_BUFF_SIZE - DMA_GetCurrDataCounter(USART_RX_DMA_CHANNEL);
  
  /* ��ȡ���ݳ��� */
  Usart_Rx_Sta = buff_length;

	PRINT_DEBUG("buff_length = %d\n ",buff_length);
  
	/* ��DMA��־λ */
	DMA_ClearFlag( DMA1_FLAG_TC5 );          
	
	/* ���¸�ֵ����ֵ��������ڵ��������ܽ��յ�������֡��Ŀ */
	USART_RX_DMA_CHANNEL->CNDTR = USART_RX_BUFF_SIZE;    
  
//	DMA_Cmd(USART_RX_DMA_CHANNEL, ENABLE);    /* ��ʱ�ر�dma��������δ���� */   
	
	/* �����ź� �����ͽ��յ������ݱ�־����ǰ̨�����ѯ */
	
    /* ��ǽ�����ɣ��� DataPack_Handle ����*/
  Usart_Rx_Sta |= 0x8000;
  
	/* 
	DMA �������ȴ����ݡ�ע�⣬����жϷ�������֡�����ʺܿ죬MCU����������˴ν��յ������ݣ�
	�ж��ַ������ݵĻ������ﲻ�ܿ������������ݻᱻ���ǡ���2�ַ�ʽ�����

	1. �����¿�������DMAͨ��֮ǰ����LumMod_Rx_Buf��������������ݸ��Ƶ�����һ�������У�
	Ȼ���ٿ���DMA��Ȼ�����ϴ����Ƴ��������ݡ�

	2. ����˫���壬��LumMod_Uart_DMA_Rx_Data�����У���������DMA_MemoryBaseAddr �Ļ�������ַ��
	��ô�´ν��յ������ݾͻᱣ�浽�µĻ������У������ڱ����ǡ�
	*/
}

#else
void Receive_DataPack(void)
{
  uint8_t res;
  /* ��ȡ���ݻ�����жϱ�־λ */
  res = USART_ReceiveData(DEBUG_USARTx);

	if((Usart_Rx_Sta&0x80000000)==0)//����δ���
	{
		if(Usart_Rx_Sta&0x40000000)//���յ���DATA_HEAD
		{
			if(res!=DATA_TAIL)
			{
			Usart_Rx_Buf[Usart_Rx_Sta&0XFFFF]=res ;
			Usart_Rx_Sta++;
			}
			else
			{
			Usart_Rx_Buf[Usart_Rx_Sta&0XFFFF]=res ;
			Usart_Rx_Sta++;
			Usart_Rx_Sta|=0x80000000;		//��������� 
			PRINT_ERR("receive ok!");
			}
		}
		else //��û�յ�DATA_HEAD
		{	
			if(res==DATA_HEAD)
			{
				Usart_Rx_Buf[Usart_Rx_Sta&0XFFFF]=res ;
				Usart_Rx_Sta++;
				Usart_Rx_Sta|=0x40000000;
			}
			else
			{
				PRINT_ERR("receive fail!");
			}		 
		}
	} 
}
	
#endif

/************************************************************
  * @brief   DataPack_Handle
  * @param   NULL
  * @return  NULL
  * @author  jiejie
  * @github  https://github.com/jiejieTop
  * @date    2018-xx-xx
  * @version v1.0
  * @note    ���ݰ�������������
  ***********************************************************/
int32_t DataPack_Handle(uint8_t* buff,DataPack* datapack)
{
  uint16_t data_len;
  if((NULL == buff)||(NULL == datapack))
  {
    PRINT_ERR("buff or len is NULL\n");
    return -1;
  }
  /* ������� */
  if( Usart_Rx_Sta & 0x80000000 )
  {
    /* ��ȡ���ݳ��� */
    data_len = Usart_Rx_Sta & 0xffff;
#if USE_DATA_CRC
    *len = data_len - 8;
#else
    datapack->data_length = data_len - 4;
#endif
    
    /* ���������ɱ�־λ */
    Usart_Rx_Sta = 0;
    /* У�����ݰ��Ƿ�һ�� */
    if((DATA_HEAD == Usart_Rx_Buf[0])&&(DATA_TAIL == Usart_Rx_Buf[data_len-1]))
    {
      memcpy(buff,&Usart_Rx_Buf[3],datapack->data_length);
    }
    else
    {
			PRINT_DEBUG("data length is not equal!\n");
			memset(Usart_Rx_Buf,0,data_len+1);
      buff = NULL;
      datapack->data_length = 0;
    }
#if USE_USART_DMA_RX
    /* ��DMA�����Խ�����һ�ν��� */
    DMA_Cmd(USART_RX_DMA_CHANNEL, ENABLE);
#endif
  return 0;
  }
  return -1;
}











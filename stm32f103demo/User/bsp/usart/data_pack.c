/* ͷ�ļ� */
#include "./usart/data_pack.h"

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
	*      
	* 
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
void Create_Pack(void *buff)
{
	DataPack data_pack;
	DataPack  *send_data=&data_pack;  
	
	data_pack.data_head = DATA_HEAD; 
	data_pack.data_length = 50;
	data_pack.data = "faklfasd";
	data_pack.data_tail = DATA_TAIL;
	
//	USART_Send((u8*)&T,sizeof(T));
	Read_Ack(send_data,50);
//	printf("%s",(u8 *)&data_pack);
//	
}

void Read_Ack(DataPack *p,u8 len)     //�β�1Ϊ�ṹ����׵�ַ &read_back   �β�2 Ϊ����֡�ĳ���
{
         static u8 date,i;	   
	 for(i=0;i<len;i++)
	{
	 date= * (((u8*) &p->data_head)+i);    //ÿ��ѭ����ָ��ָ��ṹ���е���һ������
         USART_SendData(USART1,date);   //ͨ������3��������
         while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET); //�ȴ��������
         }
}


char Name_Head[] = NAME_HEAD;
char Name_Tail[] = NAME_TAIL;
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
	return  (uint32_t)(Name_Head[0] << 16) | (Name_Head[1] << 8) | Name_Head[2];
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
	return  (uint32_t)(Name_Tail[0] << 16) | (Name_Tail[1] << 8) | Name_Tail[2];
}










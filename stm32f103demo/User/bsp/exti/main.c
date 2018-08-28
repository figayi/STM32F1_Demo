/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   main
  ******************************************************************************
  * @attention ��demo����RTOS�汾����ҪRTOS��demo���ע�ܽܵ�GitHub��
  *	GitHub��https://github.com/jiejieTop
  *
  * ʵ��ƽ̨:Ұ�� F103-�Ե� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
//#include "stm32f10x.h"
#include "include.h"
/**
  ******************************************************************
														��������
  ******************************************************************
  */ 
static void BSP_Init(void);

/**
  ******************************************************************
													   ��������
  ******************************************************************
  */ 
	
	
/**
  ******************************************************************
  * @brief   ������
  * @author  jiejie
  * @version V1.0
  * @date    2018-xx-xx
  ******************************************************************
  */ 
int main(void)
{
//	char jie[] = "123456789";
	uint8_t res[50];
	DataPack_t datapack;
	int32_t err;
	BSP_Init();
	while(1)                            
	{
		err = DataPack_Process(res,&datapack);
		if(err == 0)
		{
			Send_DataPack(res,datapack.data_length);
		}
		
//		Delay_ms(3000);
	}
}

/**
  ******************************************************************
  * @brief   BSP_Init���������а弶��ʼ��
  * @author  jiejie
  * @version V1.0
  * @date    2018-xx-xx
  ******************************************************************
  */ 
static void BSP_Init(void)
{
	/* LED ��ʼ�� */
	LED_GPIO_Config();
	
	/* �δ�ʱ����ʼ�� */
	SysTick_Init();
	
	/* ���ڳ�ʼ�� */
	USART_Config();
	
	/* �ⲿ�жϳ�ʼ�� */
	EXTI_Key_Config(); 
	
	CRC_Config();
	
	/* ��ӡ��Ϣ */
	PRINT_INFO("welcome to learn jiejie stm32 library!\n");
	
}











/*********************************************END OF FILE**********************/

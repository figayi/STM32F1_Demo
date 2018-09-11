/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   串口中断接收测试
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "stm32f10x.h"
#include "./key/bsp_key.h"  
  

/** 
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
	/* 按键初始化 */
	EXTI_Key_Config();
	
  while(1)
	{	
		
	}	
}
/*********************************************END OF FILE**********************/

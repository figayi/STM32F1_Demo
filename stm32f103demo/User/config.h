#ifndef __CONFIG_H
#define __CONFIG_H

/************************************************************
  * @brief   config.h
  * @param   NULL
  * @return  NULL
  * @author  jiejie
  * @github  https://github.com/jiejieTop
  * @date    2018-xx-xx
  * @version v1.0
  * @note    ����ļ���������ȫ�ֵĺ궨�� ʹ��/ʧ��
  *          һ�㲻�ڴ��ļ��޸ĺ궨��
  *          ���ļ���֤ϵͳ����������
  ***********************************************************/
  
#include "include.h"

/* ʹ�ô��� DMA ���� */
#ifndef USE_USART_DMA_TX
#define  USE_USART_DMA_TX  1
#endif

#ifndef USART_TX_BUFF_SIZE
// һ�η��͵�������
#define  USART_TX_BUFF_SIZE            1000 
/* ��������buff���� */
extern uint8_t Usart_Tx_Buf[USART_TX_BUFF_SIZE];
#endif

/* ʹ�ô��� DMA ���� */
#ifndef USE_USART_DMA_RX
#define  USE_USART_DMA_RX  1
#endif

// һ�ν��յ����ݴ��� ���ֽڣ�
#ifndef USART_RX_BUFF_SIZE
#define  USART_RX_BUFF_SIZE            1000 
/* ��������buff���� */
extern uint8_t Usart_Rx_Buf[USART_RX_BUFF_SIZE];
#endif

/* ʹ��CRCk */
#ifndef USE_DATA_CRC
#define USE_DATA_CRC  0
#endif

/* ʹ�ܴ�ӡ������־ */
#ifndef PRINT_DEBUG_ENABLE
#define PRINT_DEBUG_ENABLE	0		/* ��ӡ������Ϣ */
#endif
#ifndef PRINT_ERR_ENABLE
#define PRINT_ERR_ENABLE		0		/* ��ӡ������Ϣ */
#endif
#ifndef PRINT_INFO_ENABLE
#define PRINT_INFO_ENABLE		1		/* ��ӡ������Ϣ */
#endif

#endif /* __CONFIG_H */

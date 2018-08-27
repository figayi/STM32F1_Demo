#ifndef __INCLUDE_H
#define __INCLUDE_H

/* stm32��Ҫ�ļ� */
#include "stm32f10x.h"
#include "core_cm3.h"
#include "misc.h"

/* c ��׼�� */
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* debugͷ�ļ� */
#include "debug.h"

/* �弶ͷ�ļ� */
#include "./exti/bsp_exti.h"

#include "./key/bsp_key.h" 

#include "./led/bsp_led.h" 

#include "./usart/bsp_usart.h"

#include "./crc/bsp_crc.h"

#include "./SysTick/bsp_SysTick.h"


/* include ���ݴ�������ļ� */
#include "./data/data_pack.h"
#include "./common/data_process.h"






#endif /* __INCLUDE_H */

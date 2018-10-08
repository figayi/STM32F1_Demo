#ifndef BUTTON_H
#define BUTTON_H

#include "include.h"


#define BTN_NAME_MAX  8

/* ��������ʱ��50ms, ��λ10ms
 ֻ��������⵽50ms״̬�������Ϊ��Ч����������Ͱ��������¼�
*/
#define BUTTON_DEBOUNCE_TIME 	5
#define BUTTON_LONG_TIME 	100		/* ����1�룬��Ϊ�����¼� */

typedef void (*Button_CallBack)(void);   /* ���������ص���������Ҫ�û�ʵ�� */

typedef enum {
  BUTTON_DOWM = 0,
  BUTTON_UP,
  BUTTON_DOUBLE,
  BUTTON_THREE,
  BUTTON_LONG,
  number_of_event, /* �����ص����¼� */
  NONE_TRIGGER
}Button_Event;

/*
	ÿ��������Ӧ1��ȫ�ֵĽṹ�������
	���Ա������ʵ���˲��Ͷ��ְ���״̬�������
*/
typedef struct Button_t
{
	/* ������һ������ָ�룬ָ���жϰ����ַ��µĺ��� */
	uint8_t (*Read_Button_Level)(void); /* ��ȡ������ƽ��������Ҫ�û�ʵ�� */
  
  char Name[BTN_NAME_MAX];
  
  uint8_t Button_Trigger_Event        :   3;	  /* ���������¼���������˫���������� */
	uint8_t Button_State                :   3;	  /* ������ǰ״̬�����»��ǵ��� */
  uint8_t Button_Trigger_Level        :   1;    /* ����������ƽ */
  uint8_t Button_Current_Level        :   1;    /* ������ǰ��ƽ */

  Button_CallBack CallBack_Function[number_of_event];
  
//	uint8_t Button_Cycle;	    /* ������������ */
//	uint8_t Button_Cycle_Count;	/* �������������� */
//  
//  uint8_t Count;			    /* �˲��������� */
	uint8_t Debounce_Time;		/* ����ʱ��(���255,��ʾ2550ms) */
//	uint16_t Long_Count;		/* ���������� */
	uint16_t Long_Time;		  /* �������³���ʱ��, 0��ʾ����ⳤ�� */
  
}Button_t;

/* �����ֵ����
	�Ƽ�ʹ��enum, ����#define��ԭ��
	(1) ����������ֵ,�������˳��ʹ���뿴���������
	(2)	�������ɰ����Ǳ����ֵ�ظ���
*/
typedef enum
{
	KEY_NONE = 0,			/* 0 ��ʾ�����¼� */

	KEY_DOWN_USER,			/* User������ */
	KEY_UP_USER,			/* User������ */
	KEY_LONG_USER,			/* User������ */

	KEY_DOWN_WAKEUP,		/* WakeUp������ */
	KEY_UP_WAKEUP,			/* WakeUp������ */

	KEY_DOWN_TAMPER,		/* Tamper������ */
	KEY_UP_TAMPER,			/* Tamper������ */

	KEY_DOWN_JOY_UP,		/* ҡ��UP������ */
	KEY_DOWN_JOY_DOWN,		/* ҡ��DOWN������ */
	KEY_DOWN_JOY_LEFT,		/* ҡ��LEFT������ */
	KEY_DOWN_JOY_RIGHT,		/* ҡ��RIGHT������ */
	KEY_DOWN_JOY_OK,		/* ҡ��OK������ */

	KEY_UP_JOY_OK,			/* ҡ��OK���ͷ� */
	
	KEY_DOWN_USER_TAMPER	/* ��ϼ���USER����WAKEUP��ͬʱ���� */
}KEY_ENUM;

/* ����ID */
enum
{
	KID_TAMPER = 0,
	KID_WAKEUP,
	KID_USER,
	KID_JOY_UP,
	KID_JOY_DOWN,
	KID_JOY_LEFT,
	KID_JOY_RIGHT,
	KID_JOY_OK
};

/* ����FIFO�õ����� */
#define KEY_FIFO_SIZE	20
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* ��ֵ������ */
	uint8_t Read;					/* ��������ָ�� */
	uint8_t Write;					/* ������дָ�� */
}KEY_FIFO_T;

/* ���ⲿ���õĺ������� */
/************************************************************
  * @note    example
uint8_t Read_TestBtn_Level(void)
{
  return GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN);
}

void TestBtn_CallBack(void)
{
  printf("��������!\n");
}

     Button_Create("test_button",
                  test_button, 
                  Read_TestBtn_Level,
                  1, 
                  BUTTON_DOWM,
                  TestBtn_CallBack);
  ***********************************************************/
void Button_Create(const char *name,
                  struct Button_t *btn, 
                  uint8_t(*read_btn_level)(void),
                  uint8_t btn_trigger_level, 
                  Button_Event btn_event,
                  Button_CallBack btn_callback);
                  
                  
#endif



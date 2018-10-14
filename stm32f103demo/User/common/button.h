#ifndef BUTTON_H
#define BUTTON_H

#include "include.h"

#define BTN_NAME_MAX  8

/* ��������ʱ��40ms, �����������Ϊ20ms
 ֻ��������⵽40ms״̬�������Ϊ��Ч����������Ͱ��������¼�
*/

#define CONTINUOS_TRIGGER     0  //�Ƿ�֧���������� 	

#define BUTTON_DEBOUNCE_TIME 	2   //����ʱ��      2*��������
#define BUTTON_CYCLE          2	 //��������ʱ��  2*��������  
#define BUTTON_DOUBLE_TIME    30 	//˫�����ʱ��  20*��������  ������300-700ms
#define BUTTON_LONG_TIME 	    50		/* ����1��(50*��������)����Ϊ�����¼� */

#define TRIGGER_CB(event)   \
        if(btn->CallBack_Function[event]) \
          btn->CallBack_Function[event]((Button_t*)btn)

typedef void (*Button_CallBack)(void*);   /* ���������ص���������Ҫ�û�ʵ�� */



typedef enum {
  BUTTON_DOWM = 0,
  BUTTON_UP,
  BUTTON_DOUBLE,
  BUTTON_LONG,
  BUTTON_ALL_RIGGER,
  number_of_event, /* �����ص����¼� */
  NONE_TRIGGER
}Button_Event;

/*
	ÿ��������Ӧ1��ȫ�ֵĽṹ�������
	���Ա������ʵ���˲��Ͷ��ְ���״̬�������
*/
typedef struct button
{
	/* ������һ������ָ�룬ָ���жϰ����ַ��µĺ��� */
	uint8_t (*Read_Button_Level)(void); /* ��ȡ������ƽ��������Ҫ�û�ʵ�� */
  
  char Name[BTN_NAME_MAX];
  
  uint8_t Button_Trigger_Event      :   3;	  /* ���������¼���������˫���������� */
	uint8_t Button_State              :   3;	  /* ������ǰ״̬�����»��ǵ��� */
  uint8_t Button_Trigger_Level      :   1;    /* ����������ƽ */
  uint8_t Button_Last_Level         :   1;    /* ������ǰ��ƽ */
  
  uint8_t Button_Last_State;      /* ��һ�εİ���״̬�������ж�˫�� */
  
  Button_CallBack CallBack_Function[number_of_event];
  
	uint8_t Button_Cycle;	           /* ������������ */
  
  uint8_t Timer_Count;			/* ��ʱ */
	uint8_t Debounce_Time;		/* ����ʱ�� */
  
	uint8_t Long_Time;		  /* �������³���ʱ�� */
  
}Button_t;




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
            &test_button, 
            Read_TestBtn_Level, 
            KEY_ON);
  Button_Attach(&test_button,BUTTON_DOWM,TestBtn_CallBack);
  Button_Attach(&test_button,BUTTON_LONG,TestBtn1_CallBack);
  ......
  
  ***********************************************************/
void Button_Create(const char *name,
                  Button_t *btn, 
                  uint8_t(*read_btn_level)(void),
                  uint8_t btn_trigger_level);
                  
void Button_Attach(Button_t *btn,Button_Event btn_event,Button_CallBack btn_callback);   
                  
void Button_Cycle_Process(Button_t *btn);              
#endif



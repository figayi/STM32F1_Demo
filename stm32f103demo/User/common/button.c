

/*
*********************************************************************************************************
*	                                  
*	ģ������ : ��������ģ��
*	�ļ����� : bsp_button.c
*	��    �� : V2.0
*	˵    �� : ʵ�ְ����ļ�⣬��������˲����ƣ����Լ�������¼���
*				(1) ��������
*				(2) ��������
*				(3) ������
*				(4) ����ʱ�Զ�����
*				(5) ��ϼ�
*
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v0.1    2009-12-27 armfly  �������ļ���ST�̼���汾ΪV3.1.2
*		v1.0    2011-01-11 armfly  ST�̼���������V3.4.0�汾��
*       v2.0    2011-10-16 armfly  ST�̼���������V3.5.0�汾��
*
*	Copyright (C), 2010-2011, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "./common/button.h"


static void Print_Btn_Info(Button_t* btn);


/************************************************************
  * @brief   ��������
	* @param   name : ��������
	* @param   btn : �����ṹ��
  * @param   read_btn_level : ������ƽ��ȡ��������Ҫ�û��Լ�ʵ�ַ���uint8_t���͵ĵ�ƽ
  * @param   btn_trigger_level : ����������ƽ
  * @return  NULL
  * @author  jiejie
  * @github  https://github.com/jiejieTop
  * @date    2018-xx-xx
  * @version v1.0
  * @note    NULL
  ***********************************************************/
void Button_Create(const char *name,
                  struct Button_t *btn, 
                  uint8_t(*read_btn_level)(void),
                  uint8_t btn_trigger_level)
{
  if( btn == NULL)
  {
    PRINT_ERR("struct button is null!");
    ASSERT(ASSERT_ERR);
  }
  
//  memset(btn, 0, sizeof(struct Button_t));  //����ṹ����Ϣ�������û���֮ǰ���
 
  StrCopy(btn->Name, name, BTN_NAME_MAX); /* ������������ */
  
  btn->Button_State = NONE_TRIGGER;           //����״̬
  btn->Button_Trigger_Event = NONE_TRIGGER;   //���������¼�
  btn->Read_Button_Level = read_btn_level;    //��������ƽ����
  btn->Button_Trigger_Level = btn_trigger_level;  //����������ƽ
  btn->Button_Current_Level = btn->Read_Button_Level(); //������ǰ��ƽ

  btn->Debounce_Time = 0;
  
  PRINT_DEBUG("button create success!");
  Print_Btn_Info(btn);
}

/************************************************************
  * @brief   ���������¼���ص�����ӳ����������
	* @param   btn : �����ṹ��
	* @param   btn_event : �����ᴥ���¼�
  * @param   btn_callback : ��������֮��Ļص�����������Ҫ�û�ʵ��
  * @return  NULL
  * @author  jiejie
  * @github  https://github.com/jiejieTop
  * @date    2018-xx-xx
  * @version v1.0
  ***********************************************************/
void Button_Attach(struct Button_t *btn,Button_Event btn_event,Button_CallBack btn_callback)
{
  if( btn == NULL)
  {
    PRINT_ERR("struct button is null!");
    ASSERT(ASSERT_ERR);
  }
  btn->CallBack_Function[btn_event] = btn_callback; //�����¼������Ļص����������ڴ������¼�
}




/************************************************************
  * @brief   ��ȡ�����������¼�
	* @param   NULL
  * @return  NULL
  * @author  jiejie
  * @github  https://github.com/jiejieTop
  * @date    2018-xx-xx
  * @version v1.0
  ***********************************************************/
uint8_t Get_Button_Event(struct Button_t *btn)
{
  return (uint8_t)(btn->Button_Trigger_Event);
}

/************************************************************
  * @brief   ��ȡ�����������¼�
	* @param   NULL
  * @return  NULL
  * @author  jiejie
  * @github  https://github.com/jiejieTop
  * @date    2018-xx-xx
  * @version v1.0
  ***********************************************************/
uint8_t Get_Button_State(struct Button_t *btn)
{
  return (uint8_t)(btn->Button_State);
}





/************************************************************
  *******             �������ڲ����ú���           **********
  ***********************************************************/
static void Print_Btn_Info(struct Button_t* btn)
{
  
  PRINT_DEBUG("button struct information:\n \
              btn->Name:%s \n \
              btn->Button_State:%d \n \
              btn->Button_Trigger_Event:%d \n \
              btn->Button_Trigger_Level:%d \n \
              btn->Button_Current_Level:%d \n\
              ",
              btn->Name,
              btn->Button_State,
              btn->Button_Trigger_Event,
              btn->Button_Trigger_Level,
              btn->Button_Current_Level);
}








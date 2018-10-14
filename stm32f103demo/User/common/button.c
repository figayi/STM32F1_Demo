/************************************************************
  * @brief   ��������
	* @param   NULL
  * @return  NULL
  * @author  jiejie
  * @github  https://github.com/jiejieTop
  * @date    2018-xx-xx
  * @version v1.0
  * @note    button.c
  ***********************************************************/
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
                  Button_t *btn, 
                  uint8_t(*read_btn_level)(void),
                  uint8_t btn_trigger_level)
{
  if( btn == NULL)
  {
    PRINT_ERR("struct button is null!");
    ASSERT(ASSERT_ERR);
  }
  
  memset(btn, 0, sizeof(struct button));  //����ṹ����Ϣ�������û���֮ǰ���
 
  StrCopy(btn->Name, name, BTN_NAME_MAX); /* ������������ */
  
  
  btn->Button_State = NONE_TRIGGER;           //����״̬
  btn->Button_Last_State = NONE_TRIGGER;      //������һ��״̬
  btn->Button_Trigger_Event = NONE_TRIGGER;   //���������¼�
  btn->Read_Button_Level = read_btn_level;    //��������ƽ����
  btn->Button_Trigger_Level = btn_trigger_level;  //����������ƽ
  btn->Button_Last_Level = btn->Read_Button_Level(); //������ǰ��ƽ
  btn->Debounce_Time = 0;
  
  PRINT_DEBUG("button create success!");
  Print_Btn_Info(btn);
}

/************************************************************
  * @brief   ���������¼���ص�����ӳ����������
	* @param   btn : �����ṹ��
	* @param   btn_event : ���������¼�
  * @param   btn_callback : ��������֮��Ļص�����������Ҫ�û�ʵ��
  * @return  NULL
  * @author  jiejie
  * @github  https://github.com/jiejieTop
  * @date    2018-xx-xx
  * @version v1.0
  ***********************************************************/
void Button_Attach(Button_t *btn,Button_Event btn_event,Button_CallBack btn_callback)
{
  if( btn == NULL)
  {
    PRINT_ERR("struct button is null!");
    ASSERT(ASSERT_ERR);
  }
  
  if(BUTTON_ALL_RIGGER == btn_event)
  {
    for(uint8_t i = 0 ; i < number_of_event-1 ; i++)
      btn->CallBack_Function[i] = btn_callback; //�����¼������Ļص����������ڴ������¼�
  }
  else
  {
    btn->CallBack_Function[btn_event] = btn_callback; //�����¼������Ļص����������ڴ������¼�

  }
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
uint8_t Get_Button_Event(Button_t *btn)
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
uint8_t Get_Button_State(Button_t *btn)
{
  return (uint8_t)(btn->Button_State);
}

/************************************************************
  * @brief   �������ڴ�����
  * @param   btn:����İ���
  * @return  NULL
  * @author  jiejie
  * @github  https://github.com/jiejieTop
  * @date    2018-xx-xx
  * @version v1.0
  * @note    ������һ�����ڵ��ô˺�������������Ϊ20~50ms
  ***********************************************************/
void Button_Cycle_Process(Button_t *btn)
{
  uint8_t current_level = (uint8_t)btn->Read_Button_Level();//��ȡ��ǰ������ƽ
  
//  PRINT_DEBUG("1:btn->Button_State = %d",btn->Button_State);
  
  if((current_level != btn->Button_Last_Level)&&(++(btn->Debounce_Time) >= BUTTON_DEBOUNCE_TIME)) //������ƽ�����仯������
  {
      PRINT_DEBUG("current_level = %d",current_level);
      PRINT_DEBUG("Button_Last_Level = %d",btn->Button_Last_Level);
      
      btn->Button_Last_Level = current_level; //���µ�ǰ������ƽ
      btn->Debounce_Time = 0; //ȷ�����ǰ���
      
      //���������û�����µģ��ı䰴��״̬Ϊ����(�״ΰ���)
      if(btn->Button_State == NONE_TRIGGER)
      {
          btn->Button_State = BUTTON_DOWM;
          PRINT_DEBUG("�״ΰ���");
      }
      //�ͷŰ���
      else if(btn->Button_State == BUTTON_DOWM)
      {
        btn->Button_State = BUTTON_UP;
        PRINT_DEBUG("�ͷ��˰���");
      }
      
      PRINT_DEBUG("��ʱbtn->Timer_Count = %d",btn->Timer_Count);
      PRINT_DEBUG("btn->Button_State = %d",btn->Button_State);
  }
  
  switch(btn->Button_State)
  {
    case BUTTON_DOWM :
    {
      if(btn->Button_Last_Level == btn->Button_Trigger_Level) //��������
      {
        btn->Button_Trigger_Event = BUTTON_DOWM;
        
        #if CONTINUOS_TRIGGER     //֧����������
        
        if(++(btn->Button_Cycle) >= BUTTON_CYCLE)
        {
          btn->Button_Cycle = 0;
          TRIGGER_CB(BUTTON_DOWM);    //����
          PRINT_DEBUG("����Button_Trigger_Event = %d",btn->Button_Trigger_Event);
        }
        
        #endif
        
        if(++(btn->Long_Time) >= BUTTON_LONG_TIME)  //�ͷŰ���ǰ���´����¼�Ϊ����
        {
          btn->Button_Trigger_Event = BUTTON_LONG; 
          PRINT_DEBUG("����:Button_Trigger_Event = %d",btn->Button_Trigger_Event);
        }
        
      }
      else    //����ǲ�֧�������ģ�����ͷŰ���
      {
        btn->Button_State = BUTTON_UP;
      }

      break;
    } 
    
    case BUTTON_UP :
    {
      btn->Timer_Count++;     //ʱ���¼

      PRINT_DEBUG("�����¼���Button_Trigger_Event = %d",btn->Button_Trigger_Event);
      
      if(btn->Button_Trigger_Event == BUTTON_DOWM)  //���µ���
      {
        // ˫��
        if((btn->Timer_Count <= BUTTON_DOUBLE_TIME)&&(btn->Button_Last_State == BUTTON_DOWM))
        {
          PRINT_INFO("��ʱbtn->Timer_Count = %d",btn->Timer_Count);
          TRIGGER_CB(BUTTON_DOUBLE);    
          PRINT_INFO("˫��");
          btn->Button_State = NONE_TRIGGER;
          btn->Button_Last_State = NONE_TRIGGER;
        }
        else
        {
          btn->Timer_Count=0;
          btn->Long_Time = 0;   //��ⳤ��ʧ�ܣ���0
          TRIGGER_CB(BUTTON_DOWM);    //����
          btn->Button_State = NONE_TRIGGER;
          btn->Button_Last_State = BUTTON_DOWM;
        }
      }
      
      else if(btn->Button_Trigger_Event == BUTTON_LONG)
      {
        TRIGGER_CB(BUTTON_LONG);    //����
        btn->Long_Time = 0;
        btn->Button_State = NONE_TRIGGER;
        btn->Button_Last_State = BUTTON_LONG;
      } 
      break;
    }
    
    case NONE_TRIGGER :
    {
      btn->Timer_Count++;     //ʱ���¼
      break;
    }
    
    default :
      break;
  }
  
}


void Button_Process_CallBack(void)
{
  ;
  
}


/************************************************************
  *******             �������ڲ����ú���           **********
  ***********************************************************/
static void Print_Btn_Info(Button_t* btn)
{
  
  PRINT_INFO("button struct information:\n \
              btn->Name:%s \n \
              btn->Button_State:%d \n \
              btn->Button_Trigger_Event:%d \n \
              btn->Button_Trigger_Level:%d \n \
              btn->Button_Last_Level:%d \n\
              ",
              btn->Name,
              btn->Button_State,
              btn->Button_Trigger_Event,
              btn->Button_Trigger_Level,
              btn->Button_Last_Level);
}








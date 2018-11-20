/*********************************************************************************************
ģ��������  ��������/���ҵ���
��������	�����RTC��ʾ����
��д�ˣ�	����	
��дʱ�䣺	2018��2��2��
Ӳ��֧�֣�	STM32F103C8   �ⲿ����8MHz RCC����������Ƶ72MHz��  

�޸���־������
1-	
	
							
˵����
 # ��ģ�������STM32F103�ڲ���RCCʱ�����ã������������õδ�ʱ������ʱ������
 # �ɸ����Լ�����Ҫ���ӻ�ɾ����

*********************************************************************************************/
#include "stm32f10x.h" //STM32ͷ�ļ�
#include "sys.h"
#include "delay.h"
#include "rtc.h"
#include "TM1640.h"
#include "touch_key.h"



int main (void){//������
	u8 MENU=0;
	RCC_Configuration(); //ϵͳʱ�ӳ�ʼ�� 
	RTC_Config();  //RTC��ʼ��
	TM1640_Init(); //TM1640��ʼ��
	TOUCH_KEY_Init(); //��ʼ����������
	while(1){
		if(RTC_Get()==0){ //����RTCʱ��
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))
			{
				MENU=0;
				while(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A));
			}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))
			{
			 	MENU=1;
				while(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B));
				
			}
			if(MENU==0)
			{
			TM1640_display(0,ryear%100/10);	//��
			TM1640_display(1,ryear%100%10+10);
			TM1640_display(2,21);	//����ʾ
			TM1640_display(3,rmon/10);	//��
			TM1640_display(4,rmon%10+10);
			TM1640_display(5,21);	//����ʾ
			TM1640_display(6,rday/10);	//��
			TM1640_display(7,rday%10+10);
		
			}
			if(MENU==1)
			{
			TM1640_display(0,21);	//����ʾ
			TM1640_display(1,21);
			TM1640_display(2,rhour/10); //ʱ
			TM1640_display(3,rhour%10+10);
			TM1640_display(4,rmin/10);	//��
			TM1640_display(5,rmin%10+10);
			TM1640_display(6,rsec/10); //��
			TM1640_display(7,rsec%10);
			}

		}
		delay_ms(200);
	}
}

/*********************************************************************************************
 * �������� www.DoYoung.net
 * ���ҵ��� www.DoYoung.net/YT 
*********************************************************************************************/
/*

���������塿
u32     a; //����32λ�޷��ű���a
u16     a; //����16λ�޷��ű���a
u8     a; //����8λ�޷��ű���a
vu32     a; //�����ױ��32λ�޷��ű���a
vu16     a; //�����ױ�� 16λ�޷��ű���a
vu8     a; //�����ױ�� 8λ�޷��ű���a
uc32     a; //����ֻ����32λ�޷��ű���a
uc16     a; //����ֻ�� ��16λ�޷��ű���a
uc8     a; //����ֻ�� ��8λ�޷��ű���a

#define ONE  1   //�궨��

delay_us(1); //��ʱ1΢��
delay_ms(1); //��ʱ1����
delay_s(1); //��ʱ1��

GPIO_WriteBit(LEDPORT,LED1,(BitAction)(1)); //LED����

*/




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
#include "buzzer.h"
#include "flash.h"
#include "led.h"

#include "lm75a.h"



int main (void)	 		 //������
{
	u8 buffer[3];
	u8 setmode=1;		 //������ģʽΪʱ���趨
	u16 settime;	     //����ʱ��˸ʱ��
	u16 i;
	u16 First_clock=0;
	u16 tm;
	u8 clock_time;
	u8 MENU=0; 			 //�˵�ѡ��
	u8 alarn_number=1;	 //����
	u8 settimebit=0;     //����ʱ���־λ
	RCC_Configuration(); //ϵͳʱ�ӳ�ʼ�� 
	RTC_Config();        //RTC��ʼ��
	TM1640_Init();       //TM1640��ʼ��
	TOUCH_KEY_Init();    //��ʼ����������
	BUZZER_Init();		 //��ʼ������Ƭ
	I2C_Configuration();//I2C��ʼ��
	LED_Init();//��ʼ��LED
	RTC_Get();
	Set_time_beep();    //����ʱ������
	First_clock=FLASH_R(FLASH_START_ADDR+20);
//	if(First_clock!=0)
//	{
//		for(i=0;i<8;i++)	//��ȡFlash�д�ȡ������
//		{
//			alarn_hour[i]=8;
//			alarn_min[i]=8;
//		}
//		Alarn_FLASH_W();
//		FLASH_W((FLASH_START_ADDR+20),0);	
//		GPIO_WriteBit(LEDPORT,LED1,(BitAction)(1)); //LED����
//	}


	for(i=0;i<8;i++)	//��ȡFlash�д�ȡ������
	{
		u16 t;
		t=FLASH_R(FLASH_START_ADDR+i*2);//��ָ��ҳ�ĵ�ַ��FLASH	
		alarn_hour[i]=t/0x100;
		alarn_min[i]=t%0x100;
	}
	GPIO_WriteBit(LEDPORT,LED2,(BitAction)(1)); //LED����	
	




	while(1){			   //������ѭ��

		if(settimebit==0) //�жϱ�־λ�Ƿ����ʱ������
		{
			RTC_Get();
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))
			{
				MENU=0;
				Set_time_beep();
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A));

			}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))
			{
			 	MENU=1;
				Set_time_beep();
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B));
				
			}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C))
			{
				MENU=3;
				Set_time_beep();
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
			}
//			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D))
//			{
//			 	MENU=9;
//				Set_time_comple_beep();
//				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
//				
//			}		
		}
//-----------------------------------------------------------------
//�˵�0����ʾ������
//-----------------------------------------------------------------
			if(MENU==0)		
			{
				TM1640_display(0,ryear%100/10);	//��
				TM1640_display(1,ryear%100%10+10);
				TM1640_display(2,20);	//����ʾ
				TM1640_display(3,rmon/10);	//��
				TM1640_display(4,rmon%10+10);
				TM1640_display(5,20);	//����ʾ
				TM1640_display(6,rday/10);	//��
				TM1640_display(7,rday%10+10);
	
				delay_ms(10);
				tm++;
				if(tm>300)
				{
					MENU=1;
					tm=0;
				}
		
			}

//-----------------------------------------------------------------
//�˵�1����ʾʱ����
//-----------------------------------------------------------------
			if(MENU==1)			
			{
				TM1640_display(0,20);	//����ʾ
				TM1640_display(1,20);
				TM1640_display(2,rhour/10); //ʱ
				TM1640_display(3,rhour%10+10);
				TM1640_display(4,rmin/10);	//��
				TM1640_display(5,rmin%10+10);
				TM1640_display(6,rsec/10); //��
				TM1640_display(7,rsec%10);
	
	
				delay_ms(10);
				tm++;
				if(tm>300)
				{
					MENU=2;
					tm=0;
				}

			}
//-----------------------------------------------------------------
//�˵�2����ʾ�¶�
//-----------------------------------------------------------------
			if(MENU==2)			
			{
				LM75A_GetTemp(buffer); //��ȡLM75A���¶�����
					
				TM1640_display(0,buffer[1]/10); //��ʾ��ֵ
				TM1640_display(1,buffer[1]%10+10);
				TM1640_display(2,buffer[2]/10);
				TM1640_display(3,buffer[2]%10);
				TM1640_display(4,20);	//����ʾ
				TM1640_display(5,20);
				TM1640_display(6,20);	//����ʾ
				TM1640_display(7,20);
				delay_ms(10);
				tm++;
				if(tm>300)
				{
					MENU=0;
					tm=0;
				}

			}
//-----------------------------------------------------------------
//�˵�3����ʾ����ʱ��
//-----------------------------------------------------------------
			if(MENU==3)		 
			{
				settimebit=1;
				if(setmode==1)
				{
					
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D))
					{
						MENU=4;
						Set_time_beep();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
					}
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))
					{
						setmode=2;
						Set_time_beep();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B));
					}
					TM1640_display(0,21);	//��ʾS
					TM1640_display(1,22);   //��ʾE
					TM1640_display(2,20);	//����ʾ
					TM1640_display(3,26);
					TM1640_display(4,20);	
					TM1640_display(5,20);
					TM1640_display(6,20); 
					TM1640_display(7,20);				
				}
				if(setmode==2)
				{
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D))
					{
						MENU=9;
						Set_time_beep();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
					}
					if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))
					{
						setmode=1;
						Set_time_beep();
						while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A));
					}
					TM1640_display(0,21);	//��ʾS
					TM1640_display(1,22);   //��ʾE
					TM1640_display(2,20);	//����ʾ
					TM1640_display(3,27);
					TM1640_display(4,20);	
					TM1640_display(5,20);
					TM1640_display(6,20); 
					TM1640_display(7,20);				
				}
	
			}

//-----------------------------------------------------------------
//�˵�4���������
//-----------------------------------------------------------------
			if(MENU==4)		 
			{
				settimebit=1;  //������ʱ���־λ����1����������ʱ���趨

			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C))
			{
				MENU=5;
				Set_time_beep();
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
			}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))  //��A����ݼ�1
				{
					ryear--;
					if(ryear<2000)
					{
						ryear=2008;
					}
					settime=810;
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))	//��B����ݼ�1
				{
				 	ryear++;
					if(ryear>2080)
					{
						ryear=2008;
					}
					settime=810;
				Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B));
					
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D))	//��D���˳�ʱ���趨
				{
				 	settimebit=0; 
					MENU=0;
					RTC_Set(ryear,rmon, rday, rhour, rmin, rsec);
					Set_time_comple_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
					
				}
				if(settime>800)
				{
					if(settime>1600)
					{
						settime=0;				
					}
		
					TM1640_display(0,ryear%100/10);	//��
					TM1640_display(1,ryear%100%10+10);
				}
				if(settime<800)
				{
					TM1640_display(0,20);	//����ʾ
					TM1640_display(1,20);			
				}
		
		
		
				TM1640_display(2,20);	//����ʾ
				TM1640_display(3,rmon/10);	//��
				TM1640_display(4,rmon%10+10);
				TM1640_display(5,20);	//����ʾ
				TM1640_display(6,rday/10);	//��
				TM1640_display(7,rday%10+10);
		
				delay_us(100);
				settime++;
			
			}

//-----------------------------------------------------------------
//�˵�5�������·�
//-----------------------------------------------------------------
 			if(MENU==5)		 
			{
				settimebit=1;  //������ʱ���־λ����1����������ʱ���趨
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C))
				{
					MENU=6;
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))  //��A���·ݼ�1
				{
					rmon--;
					if(rmon<1)
					{
						rmon=12;
					}
					settime=810;
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))	//��B����ݼ�1
				{
				 	rmon++;
					if(rmon>12)
					{
						rmon=1;
					}
					settime=810;
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B));
					
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D))	//��D���˳�ʱ���趨
				{
				 	settimebit=0; 
					MENU=0;
					RTC_Set(ryear,rmon, rday, rhour, rmin, rsec);
					Set_time_comple_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
					
				}
				if(settime>800)
				{
					if(settime>1600)
					{
						settime=0;				
					}

					TM1640_display(3,rmon/10);	//��
					TM1640_display(4,rmon%10+10);		

					}
				if(settime<800)
				{
					TM1640_display(3,20);	//����ʾ
					TM1640_display(4,20);			
				}
	
				TM1640_display(0,ryear%100/10);	//��
				TM1640_display(1,ryear%100%10+10);		
				TM1640_display(2,20);	//����ʾ		

				TM1640_display(5,20);	//����ʾ
				TM1640_display(6,rday/10);	//��
				TM1640_display(7,rday%10+10);
		
				delay_us(100);
				settime++;
			
			}
//-----------------------------------------------------------------
//�˵�6����������
//-----------------------------------------------------------------
			if(MENU==6)		 
			{
				settimebit=1;  //������ʱ���־λ����1����������ʱ���趨
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C))
				{
					MENU=7;
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))  //��A���·ݼ�1
				{
					rday--;
					if(rday<1)
					{
						rday=31;
					}
					settime=810;
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))	//��B����ݼ�1
				{
				 	rday++;
					if(rday>31)
					{
						rday=0;
					}
					settime=810;
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B));
					
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D))	//��D���˳�ʱ���趨
				{
				 	settimebit=0; 
					MENU=0;
					RTC_Set(ryear,rmon, rday, rhour, rmin, rsec);
					Set_time_comple_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
					
				}
				if(settime>800)
				{
					if(settime>1600)
					{
						settime=0;				
					}

					TM1640_display(6,rday/10);	//��
					TM1640_display(7,rday%10+10);		

				}
				if(settime<800)
				{
					TM1640_display(6,20);	//����ʾ
					TM1640_display(7,20);			
				}
	
				TM1640_display(0,ryear%100/10);	//��
				TM1640_display(1,ryear%100%10+10);		
				TM1640_display(2,20);	//����ʾ		
 				TM1640_display(3,rmon/10);	//��
				TM1640_display(4,rmon%10+10);

				TM1640_display(5,20);	//����ʾ

		
				delay_us(100);
				settime++;
			
			}
//-----------------------------------------------------------------
//�˵�7������Сʱ
//-----------------------------------------------------------------
			if(MENU==7)		 
			{
				settimebit=1;  //������ʱ���־λ����1����������ʱ���趨
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C))
				{
					MENU=8;
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))  //��A�����ڼ�1
				{
					rhour--;
					if(rhour<1)
					{
						rhour=23;
					}
//					RTC_Set(ryear,rmon, rday, rhour, rmin, rsec);
					settime=810;
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))	//��B�����ڼ�1
				{
				 	rhour++;
					if(rhour>23)
					{
						rhour=1;
					}
//					RTC_Set(ryear,rmon, rday, rhour, rmin, rsec);
					settime=810;
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B));
					
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D))	//��D���˳�ʱ���趨
				{
				 	settimebit=0; 
					MENU=0;
					RTC_Set(ryear,rmon, rday, rhour, rmin, rsec);
					Set_time_comple_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
					
				}
				if(settime>800)
				{
					if(settime>1600)
					{
						settime=0;				
					}
	
					TM1640_display(2,rhour/10); //ʱ
					TM1640_display(3,rhour%10+10);		

				}
				if(settime<800)
				{
					TM1640_display(2,20);	//����ʾ
					TM1640_display(3,20);			
				}
				TM1640_display(0,20);	//����ʾ
				TM1640_display(1,20);

				TM1640_display(4,rmin/10);	//��
				TM1640_display(5,rmin%10+10);
				TM1640_display(6,rsec/10); //��
				TM1640_display(7,rsec%10);

		
				delay_us(100);
				settime++;
			
			}
//-----------------------------------------------------------------
//�˵�7�����÷���
//-----------------------------------------------------------------
			if(MENU==8)		 
			{
				settimebit=1;  //������ʱ���־λ����1����������ʱ���趨
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))  //��A�����Ӽ�1
				{
					rmin--;
					if(rmin<1)
					{
						rmin=60;
					}
//					RTC_Set(ryear,rmon, rday, rhour, rmin, rsec);
					settime=810;
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))	//��B���Ӽ�1
				{
				 	rmin++;
					if(rmin>60)
					{
						rmin=1;
					}
//					RTC_Set(ryear,rmon, rday, rhour, rmin, rsec);
					settime=810;
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B));
					
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D))	//��D���˳�ʱ���趨
				{
				 	settimebit=0; 
					MENU=0;
					RTC_Set(ryear,rmon, rday, rhour, rmin, rsec);
					Set_time_comple_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
					
				}
				if(settime>800)
				{
					if(settime>1600)
					{
						settime=0;				
					}
	
					TM1640_display(4,rmin/10); //��
					TM1640_display(5,rmin%10+10);		

				}
				if(settime<800)
				{
					TM1640_display(4,20);	//����ʾ
					TM1640_display(5,20);			
				}
				TM1640_display(0,20);	//����ʾ
				TM1640_display(1,20);

				TM1640_display(2,rhour/10); //ʱ
				TM1640_display(3,rhour%10+10);		

				TM1640_display(6,rsec/10); //��
				TM1640_display(7,rsec%10);

		
				delay_us(100);
				settime++;
			
			}
//-----------------------------------------------------------------
//�˵�9����ʾ�ĸ�����
//-----------------------------------------------------------------
			if(MENU==9)		 
			{
				settimebit=1;  //������ʱ���־λ����1���������������趨
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D))
				{
					MENU=0;
					settimebit=0;  //������ʱ���־λ����1���������������趨
					Set_time_comple_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))  //��A�����Ӽ�1
				{
					alarn_number--;
					if(alarn_number<1)
					{
						alarn_number=8;
					}
//					RTC_Set(ryear,rmon, rday, rhour, rmin, rsec);
					settime=810;
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))	//��B���Ӽ�1
				{
				 	alarn_number++;
					if(alarn_number>8)
					{
						alarn_number=1;
					}
//					RTC_Set(ryear,rmon, rday, rhour, rmin, rsec);
					settime=810;
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B));
					
				}
				if(settime>800)
				{
					if(settime>1600)
					{
						settime=0;				
					}
	 				TM1640_display(0,24); //A
					TM1640_display(1,25); //L	
					TM1640_display(2,alarn_number); //����ʱ	
					TM1640_display(3,20); //����ʱ

				}

			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C))
			{
				MENU=10;
				Set_time_beep();
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
			}
				if(settime<800)
				{
					TM1640_display(0,20);	//����ʾ
					TM1640_display(1,20);	//����ʾ
					TM1640_display(2,20);	//����ʾ			
				}



				TM1640_display(4,alarn_hour[alarn_number-1]/10); //����ʱ
				TM1640_display(5,alarn_hour[alarn_number-1]%10+10);		
				TM1640_display(6,alarn_min[alarn_number-1]/10); //���ӷ�
				TM1640_display(7,alarn_min[alarn_number-1]%10+10);	
				settime++;

			}
//-----------------------------------------------------------------
//�˵�10����ʾ����ʱ
//-----------------------------------------------------------------
			if(MENU==10)		 
			{
				settimebit=1;  //������ʱ���־λ����1���������������趨
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D))
				{
					MENU=0;
					Set_time_comple_beep();
					settimebit=0;  //������ʱ���־λ����1���������������趨
					Alarn_FLASH_W();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))  //��A�����Ӽ�1
				{
					alarn_hour[alarn_number-1]--;
					if(alarn_hour[alarn_number-1]<1)
					{
						alarn_hour[alarn_number-1]=23;
					}
//					RTC_Set(ryear,rmon, rday, rhour, rmin, rsec);
					settime=810;
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))	//��B���Ӽ�1
				{
				 	alarn_hour[alarn_number-1]++;
					if(alarn_hour[alarn_number-1]>23)
					{
						alarn_hour[alarn_number-1]=1;
					}
//					RTC_Set(ryear,rmon, rday, rhour, rmin, rsec);
					settime=810;
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B));
					
				}
				if(settime>800)
				{
					if(settime>1600)
					{
						settime=0;				
					}
	
					TM1640_display(4,alarn_hour[alarn_number-1]/10); //����ʱ
					TM1640_display(5,alarn_hour[alarn_number-1]%10+10);		

				}

			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C))
			{
				MENU=11;
				Set_time_beep();
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
			}
				if(settime<800)
				{
					TM1640_display(4,20);	//����ʾ
					TM1640_display(5,20);			
				}

 				TM1640_display(0,24); //A
				TM1640_display(1,25); //L

				TM1640_display(2,alarn_number);	//����ʾ
				TM1640_display(3,20);
	
				TM1640_display(6,alarn_min[alarn_number-1]/10); //���ӷ�
				TM1640_display(7,alarn_min[alarn_number-1]%10+10);	
				settime++;

			}
//-----------------------------------------------------------------
//�˵�11����ʾ���ӷ�
//-----------------------------------------------------------------
			if(MENU==11)		 //�˵�11����ʾ���ӷ�
			{
				settimebit=1;  //������ʱ���־λ����1���������������趨
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C))
				{
					MENU=10;
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D))
				{
					MENU=0;
					Set_time_comple_beep();
					settimebit=0;  //������ʱ���־λ����1���������������趨
					Alarn_FLASH_W();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))  //��A�����Ӽ�1
				{
					alarn_min[alarn_number-1]--;
					if(alarn_min[alarn_number-1]<1)
					{
						alarn_min[alarn_number-1]=60;
					}
					settime=810;
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))	//��B���Ӽ�1
				{
				 	alarn_min[alarn_number-1]++;
					if(alarn_min[alarn_number-1]>60)
					{
						alarn_min[alarn_number-1]=1;
					}
					settime=810;
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B));
					
				}
				if(settime>800)
				{
					if(settime>1600)
					{
						settime=0;				
					}
	
					TM1640_display(6,alarn_min[alarn_number-1]/10); //���ӷ�
					TM1640_display(7,alarn_min[alarn_number-1]%10+10);		

				}

				if(settime<800)
				{
					TM1640_display(6,20);	//����ʾ
					TM1640_display(7,20);			
				}

 				TM1640_display(0,24); //A
				TM1640_display(1,25); //L

				TM1640_display(2,alarn_number);	//����ʾ
				TM1640_display(3,20);


				TM1640_display(4,alarn_hour[alarn_number-1]/10); //����ʱ
				TM1640_display(5,alarn_hour[alarn_number-1]%10+10);	
				settime++;

			}
			if(MENU<2)		 //�˵�12��ɨ��8������
			{
				for(i=0;i<8;i++)
				{
					if(alarn_hour[i]==rhour && alarn_min[i]==rmin)
					{
						MENU=12;
						clock_time=i;
					}				
				}
			}
			if(MENU==12)
			{
				settimebit=1;  //������ʱ���־λ����1���������������趨
				for(i=0;i<1000;i++)
				{
					Set_time_beep();
					MENU=1;						
				}
			if(settime>800)
			{
				if(settime>1600)
				{
					settime=0;				
				}
				TM1640_display(0,24); //A
				TM1640_display(1,25); //L
		

			}

			if(settime<800)
			{
				TM1640_display(6,20);	//����ʾ
				TM1640_display(7,20);			
			}
			TM1640_display(2,clock_time);	//����ʾ
			TM1640_display(3,20);
			TM1640_display(4,alarn_hour[alarn_number-1]/10); //����ʱ
			TM1640_display(5,alarn_hour[alarn_number-1]%10+10);	
			TM1640_display(6,alarn_min[alarn_number-1]/10); //���ӷ�
			TM1640_display(7,alarn_min[alarn_number-1]%10+10);
			settime++;
		
		}
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




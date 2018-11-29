/*********************************************************************************************
模板制作：  杜洋工作室/洋桃电子
程序名：	数码管RTC显示程序
编写人：	杜洋	
编写时间：	2018年2月2日
硬件支持：	STM32F103C8   外部晶振8MHz RCC函数设置主频72MHz　  

修改日志：　　
1-	
	
							
说明：
 # 本模板加载了STM32F103内部的RCC时钟设置，并加入了利用滴答定时器的延时函数。
 # 可根据自己的需要增加或删减。

*********************************************************************************************/
#include "stm32f10x.h" //STM32头文件
#include "sys.h"
#include "delay.h"
#include "rtc.h"
#include "TM1640.h"
#include "touch_key.h"
#include "buzzer.h"
#include "flash.h"




int main (void){//主程序
	u8 clock_time;
	u8 MENU=0; //菜单选项
	u8 alarn_number;
	u8 settimebit=0;   //设置时间标志位
	u16 settime;	   //设置时闪烁时间
	u16 i;
	RCC_Configuration(); //系统时钟初始化 
	RTC_Config();  //RTC初始化
	TM1640_Init(); //TM1640初始化
	TOUCH_KEY_Init(); //初始化触摸按键
	BUZZER_Init();
	RTC_Get();
	Set_time_beep();
	for(i=0;i<8;i++)
	{
		u16 t;
		t=FLASH_R(FLASH_START_ADDR+i*2);//从指定页的地址读FLASH	
		alarn_hour[i]=t/0x100;
		alarn_min[i]=t%0x100;
	}

	while(1){

		if(settimebit==0) //判断是否进入时间设置
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
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D))
			{
			 	MENU=9;
				Set_time_beep();
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
				
			}		
		}

			if(MENU==0)		 //菜单0，显示年月日
			{
			TM1640_display(0,ryear%100/10);	//年
			TM1640_display(1,ryear%100%10+10);
			TM1640_display(2,20);	//无显示
			TM1640_display(3,rmon/10);	//月
			TM1640_display(4,rmon%10+10);
			TM1640_display(5,20);	//无显示
			TM1640_display(6,rday/10);	//天
			TM1640_display(7,rday%10+10);
		
			}
			if(MENU==1)			//菜单1，显示时分秒
			{
			TM1640_display(0,20);	//无显示
			TM1640_display(1,20);
			TM1640_display(2,rhour/10); //时
			TM1640_display(3,rhour%10+10);
			TM1640_display(4,rmin/10);	//分
			TM1640_display(5,rmin%10+10);
			TM1640_display(6,rsec/10); //秒
			TM1640_display(7,rsec%10);
			}
			if(MENU==2)			//菜单2，显示温度
			{

			}

			if(MENU==3)		 //菜单3，显示设置时间
			{
			settimebit=1;
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C))
			{
				MENU=4;
				Set_time_beep();
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
			}
			TM1640_display(0,21);	//显示S
			TM1640_display(1,22);   //显示E
			TM1640_display(2,20);	//无显示
			TM1640_display(3,20);
			TM1640_display(4,20);	
			TM1640_display(5,20);
			TM1640_display(6,20); 
			TM1640_display(7,20);	
			}
			if(MENU==4)		 //菜单4，设置年份
			{
				settimebit=1;  //将设置时间标志位置置1，按键进入时间设定

			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C))
			{
				MENU=5;
				Set_time_beep();
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
			}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))  //按A键年份减1
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
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))	//按B键年份加1
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
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D))	//按D键退出时间设定
				{
				 	settimebit=0; 
					MENU=0;
					RTC_Set(ryear,rmon, rday, rhour, rmin, rsec);
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
					
				}
				if(settime>800)
				{
					if(settime>1600)
					{
						settime=0;				
					}
		
					TM1640_display(0,ryear%100/10);	//年
					TM1640_display(1,ryear%100%10+10);
				}
				if(settime<800)
				{
					TM1640_display(0,20);	//无显示
					TM1640_display(1,20);			
				}
		
		
		
				TM1640_display(2,20);	//无显示
				TM1640_display(3,rmon/10);	//月
				TM1640_display(4,rmon%10+10);
				TM1640_display(5,20);	//无显示
				TM1640_display(6,rday/10);	//天
				TM1640_display(7,rday%10+10);
		
				delay_us(100);
				settime++;
			
			}
 			if(MENU==5)		 //菜单5，设置月份
			{
				settimebit=1;  //将设置时间标志位置置1，按键进入时间设定
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C))
				{
					MENU=6;
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))  //按A键月份减1
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
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))	//按B月年份加1
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
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D))	//按D键退出时间设定
				{
				 	settimebit=0; 
					MENU=0;
					RTC_Set(ryear,rmon, rday, rhour, rmin, rsec);
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
					
				}
				if(settime>800)
				{
					if(settime>1600)
					{
						settime=0;				
					}

					TM1640_display(3,rmon/10);	//月
					TM1640_display(4,rmon%10+10);		

					}
				if(settime<800)
				{
					TM1640_display(3,20);	//无显示
					TM1640_display(4,20);			
				}
	
				TM1640_display(0,ryear%100/10);	//年
				TM1640_display(1,ryear%100%10+10);		
				TM1640_display(2,20);	//无显示		

				TM1640_display(5,20);	//无显示
				TM1640_display(6,rday/10);	//天
				TM1640_display(7,rday%10+10);
		
				delay_us(100);
				settime++;
			
			}
			if(MENU==6)		 //菜单6，设置日期
			{
				settimebit=1;  //将设置时间标志位置置1，按键进入时间设定
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C))
				{
					MENU=7;
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))  //按A键月份减1
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
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))	//按B月年份加1
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
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D))	//按D键退出时间设定
				{
				 	settimebit=0; 
					MENU=0;
					RTC_Set(ryear,rmon, rday, rhour, rmin, rsec);
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
					
				}
				if(settime>800)
				{
					if(settime>1600)
					{
						settime=0;				
					}

					TM1640_display(6,rday/10);	//日
					TM1640_display(7,rday%10+10);		

				}
				if(settime<800)
				{
					TM1640_display(6,20);	//无显示
					TM1640_display(7,20);			
				}
	
				TM1640_display(0,ryear%100/10);	//年
				TM1640_display(1,ryear%100%10+10);		
				TM1640_display(2,20);	//无显示		
 				TM1640_display(3,rmon/10);	//月
				TM1640_display(4,rmon%10+10);

				TM1640_display(5,20);	//无显示

		
				delay_us(100);
				settime++;
			
			}
			if(MENU==7)		 //菜单7，设置小时
			{
				settimebit=1;  //将设置时间标志位置置1，按键进入时间设定
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C))
				{
					MENU=8;
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))  //按A键日期减1
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
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))	//按B月日期加1
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
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D))	//按D键退出时间设定
				{
				 	settimebit=0; 
					MENU=0;
					RTC_Set(ryear,rmon, rday, rhour, rmin, rsec);
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
					
				}
				if(settime>800)
				{
					if(settime>1600)
					{
						settime=0;				
					}
	
					TM1640_display(2,rhour/10); //时
					TM1640_display(3,rhour%10+10);		

				}
				if(settime<800)
				{
					TM1640_display(2,20);	//无显示
					TM1640_display(3,20);			
				}
				TM1640_display(0,20);	//无显示
				TM1640_display(1,20);

				TM1640_display(4,rmin/10);	//分
				TM1640_display(5,rmin%10+10);
				TM1640_display(6,rsec/10); //秒
				TM1640_display(7,rsec%10);

		
				delay_us(100);
				settime++;
			
			}
			if(MENU==8)		 //菜单7，设置分钟
			{
				settimebit=1;  //将设置时间标志位置置1，按键进入时间设定
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))  //按A键分钟减1
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
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))	//按B分钟加1
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
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D))	//按D键退出时间设定
				{
				 	settimebit=0; 
					MENU=0;
					RTC_Set(ryear,rmon, rday, rhour, rmin, rsec);
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
					
				}
				if(settime>800)
				{
					if(settime>1600)
					{
						settime=0;				
					}
	
					TM1640_display(4,rmin/10); //分
					TM1640_display(5,rmin%10+10);		

				}
				if(settime<800)
				{
					TM1640_display(4,20);	//无显示
					TM1640_display(5,20);			
				}
				TM1640_display(0,20);	//无显示
				TM1640_display(1,20);

				TM1640_display(2,rhour/10); //时
				TM1640_display(3,rhour%10+10);		

				TM1640_display(6,rsec/10); //秒
				TM1640_display(7,rsec%10);

		
				delay_us(100);
				settime++;
			
			}
			if(MENU==9)		 //菜单9，显示哪个闹钟
			{
				settimebit=1;  //将设置时间标志位置置1，按键进入闹钟设定
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D))
				{
					MENU=0;
					Set_time_beep();
					settimebit=0;  //将设置时间标志位置置1，按键进入闹钟设定
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))  //按A键分钟减1
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
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))	//按B分钟加1
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
					TM1640_display(2,alarn_number); //闹钟时	
					TM1640_display(3,20); //闹钟时

				}

			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C))
			{
				MENU=10;
				Set_time_beep();
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
			}
				if(settime<800)
				{
					TM1640_display(0,20);	//无显示
					TM1640_display(1,20);	//无显示
					TM1640_display(2,20);	//无显示			
				}



				TM1640_display(4,alarn_hour[alarn_number-1]/10); //闹钟时
				TM1640_display(5,alarn_hour[alarn_number-1]%10+10);		
				TM1640_display(6,alarn_min[alarn_number-1]/10); //闹钟分
				TM1640_display(7,alarn_min[alarn_number-1]%10+10);	
				settime++;

			}

			if(MENU==10)		 //菜单10，显示闹钟时
			{
				settimebit=1;  //将设置时间标志位置置1，按键进入闹钟设定
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D))
				{
					MENU=0;
					Set_time_beep();
					settimebit=0;  //将设置时间标志位置置1，按键进入闹钟设定
					Alarn_FLASH_W();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))  //按A键分钟减1
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
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))	//按B分钟加1
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
	
					TM1640_display(4,alarn_hour[alarn_number-1]/10); //闹钟时
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
					TM1640_display(4,20);	//无显示
					TM1640_display(5,20);			
				}

 				TM1640_display(0,24); //A
				TM1640_display(1,25); //L

				TM1640_display(2,1);	//无显示
				TM1640_display(3,20);
	
				TM1640_display(6,alarn_min[alarn_number-1]/10); //闹钟分
				TM1640_display(7,alarn_min[alarn_number-1]%10+10);	
				settime++;

			}
			if(MENU==11)		 //菜单11，显示闹钟分
			{
				settimebit=1;  //将设置时间标志位置置1，按键进入闹钟设定
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C))
				{
					MENU=10;
					Set_time_beep();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D))
				{
					MENU=0;
					Set_time_beep();
					settimebit=0;  //将设置时间标志位置置1，按键进入闹钟设定
					Alarn_FLASH_W();
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))  //按A键分钟减1
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
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))	//按B分钟加1
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
	
					TM1640_display(6,alarn_min[alarn_number-1]/10); //闹钟分
					TM1640_display(7,alarn_min[alarn_number-1]%10+10);		

				}

				if(settime<800)
				{
					TM1640_display(6,20);	//无显示
					TM1640_display(7,20);			
				}

 				TM1640_display(0,24); //A
				TM1640_display(1,25); //L

				TM1640_display(2,1);	//无显示
				TM1640_display(3,20);


				TM1640_display(4,alarn_hour[alarn_number-1]/10); //闹钟时
				TM1640_display(5,alarn_hour[alarn_number-1]%10+10);	
				settime++;

			}
			if(MENU<2)		 //菜单12，扫描8组闹钟
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
				settimebit=1;  //将设置时间标志位置置1，按键进入闹钟设定
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
				TM1640_display(6,20);	//无显示
				TM1640_display(7,20);			
			}
			TM1640_display(2,clock_time);	//无显示
			TM1640_display(3,20);
			TM1640_display(4,alarn_hour[alarn_number-1]/10); //闹钟时
			TM1640_display(5,alarn_hour[alarn_number-1]%10+10);	
			TM1640_display(6,alarn_min[alarn_number-1]/10); //闹钟分
			TM1640_display(7,alarn_min[alarn_number-1]%10+10);
			settime++;
		
		}
		}
		
	}


/*********************************************************************************************
 * 杜洋工作室 www.DoYoung.net
 * 洋桃电子 www.DoYoung.net/YT 
*********************************************************************************************/
/*

【变量定义】
u32     a; //定义32位无符号变量a
u16     a; //定义16位无符号变量a
u8     a; //定义8位无符号变量a
vu32     a; //定义易变的32位无符号变量a
vu16     a; //定义易变的 16位无符号变量a
vu8     a; //定义易变的 8位无符号变量a
uc32     a; //定义只读的32位无符号变量a
uc16     a; //定义只读 的16位无符号变量a
uc8     a; //定义只读 的8位无符号变量a

#define ONE  1   //宏定义

delay_us(1); //延时1微秒
delay_ms(1); //延时1毫秒
delay_s(1); //延时1秒

GPIO_WriteBit(LEDPORT,LED1,(BitAction)(1)); //LED控制

*/




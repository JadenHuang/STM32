#ifndef __BUZZER_H
#define __BUZZER_H	 
#include "sys.h"

#define BUZZERPORT	GPIOB	//����IO�ӿ�
#define BUZZER	GPIO_Pin_5	//����IO�ӿ�



void BUZZER_Init(void);//��ʼ��
void BUZZER_BEEP1(void);//��һ��
void Set_time_beep(void); //����ʱ��ʱ��������һ��
void MIDI_PLAY(void);
void Set_time_comple_beep(void); //����ʱ��ʱ�������
		 				    
#endif

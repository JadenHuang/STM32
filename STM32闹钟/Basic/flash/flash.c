
/*
//�������ҳ�Ʒ
//����ϵ�п�����Ӧ�ó���
//��ע΢�Ź��ںţ����ҵ���
//���ҿ������������� www.DoYoung.net/YT 
//������ѿ����н�ѧ��Ƶ�����ؼ������ϣ�������������
//�������ݾ��� ����������ҳ www.doyoung.net
*/

/*
���޸���־��


*/



#include "flash.h"
u8 alarn_hour[8];
u8 alarn_min[8]; 

//FLASHд������
void FLASH_W(u32 add,u16 dat){ //����1��32λFLASH��ַ������2��16λ����
//	 RCC_HSICmd(ENABLE); //��HSIʱ��
	 FLASH_Unlock();  //����FLASH��̲���������
     FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//�����־λ
     FLASH_ErasePage(add);     //����ָ����ַҳ
     FLASH_ProgramHalfWord(add,dat); //��ָ��ҳ��addr��ַ��ʼд
     FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//�����־λ
     FLASH_Lock();    //����FLASH��̲���������
}

//FLASHд������
void Alarn_FLASH_W(void){ 
//	 RCC_HSICmd(ENABLE); //��HSIʱ��
	 u16 t;
	 u8 i;
	 FLASH_Unlock();  //����FLASH��̲���������
     FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//�����־λ
     FLASH_ErasePage(FLASH_START_ADDR);     //����ָ����ַҳ
	 for(i=0;i<8;i++)
	 {
	 	 t=alarn_hour[i]*0x100+alarn_min[i];
	     FLASH_ProgramHalfWord(FLASH_START_ADDR+i*2,t); //��ָ��ҳ��addr��ַ��ʼд
	     FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//�����־λ	 
	 }

     FLASH_Lock();    //����FLASH��̲���������
}
//FLASH��������
u16 FLASH_R(u32 add){ //����1��32λ����FLASH��ַ������ֵ��16λ����
	u16 a;
    a = *(u16*)(add);//��ָ��ҳ��addr��ַ��ʼ��
return a;
}
/*********************************************************************************************
 * �������� www.DoYoung.net
 * ���ҵ��� www.DoYoung.net/YT 
*********************************************************************************************/

































/*
//杜洋工作室出品
//洋桃系列开发板应用程序
//关注微信公众号：洋桃电子
//洋桃开发板资料下载 www.DoYoung.net/YT 
//即可免费看所有教学视频，下载技术资料，技术疑难提问
//更多内容尽在 杜洋工作室主页 www.doyoung.net
*/

/*
《修改日志》


*/



#include "flash.h"
u8 alarn_hour[8];
u8 alarn_min[8]; 

//FLASH写入数据
void FLASH_W(u32 add,u16 dat){ //参数1：32位FLASH地址。参数2：16位数据
//	 RCC_HSICmd(ENABLE); //打开HSI时钟
	 FLASH_Unlock();  //解锁FLASH编程擦除控制器
     FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//清除标志位
     FLASH_ErasePage(add);     //擦除指定地址页
     FLASH_ProgramHalfWord(add,dat); //从指定页的addr地址开始写
     FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//清除标志位
     FLASH_Lock();    //锁定FLASH编程擦除控制器
}

//FLASH写入数据
void Alarn_FLASH_W(void){ 
//	 RCC_HSICmd(ENABLE); //打开HSI时钟
	 u16 t;
	 u8 i;
	 FLASH_Unlock();  //解锁FLASH编程擦除控制器
     FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//清除标志位
     FLASH_ErasePage(FLASH_START_ADDR);     //擦除指定地址页
	 for(i=0;i<8;i++)
	 {
	 	 t=alarn_hour[i]*0x100+alarn_min[i];
	     FLASH_ProgramHalfWord(FLASH_START_ADDR+i*2,t); //从指定页的addr地址开始写
	     FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//清除标志位	 
	 }

     FLASH_Lock();    //锁定FLASH编程擦除控制器
}
//FLASH读出数据
u16 FLASH_R(u32 add){ //参数1：32位读出FLASH地址。返回值：16位数据
	u16 a;
    a = *(u16*)(add);//从指定页的addr地址开始读
return a;
}
/*********************************************************************************************
 * 杜洋工作室 www.DoYoung.net
 * 洋桃电子 www.DoYoung.net/YT 
*********************************************************************************************/
































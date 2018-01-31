#include "lowpower.h"

/*******************************************************************************
* 函 数 名         : standmodeinit
* 函数功能		   : 待机模式配置初始化	   
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void standmodeinit()   //待机模式
{	
	NVIC_SystemLPConfig(NVIC_LP_SLEEPDEEP,ENABLE);//选择系统进入低功耗模式的条件	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);//使能PWR外设时钟	
	PWR_WakeUpPinCmd(ENABLE);//使能唤醒管脚	使能或者失能唤醒管脚功能
	PWR_EnterSTANDBYMode();//进入待机模式		
}

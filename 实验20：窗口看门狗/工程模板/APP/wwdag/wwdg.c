#include "wwdg.h"

/*******************************************************************************
* 函 数 名         : wwdg_init
* 函数功能		   : 窗口看门狗模式配置初始化	   
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void wwdg_init()
{
	NVIC_InitTypeDef NVIC_InitStructure;		//中断结构体定义
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE); // WWDG时钟使能
	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//看门狗的优先级要高于其他
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);

	WWDG_SetPrescaler(WWDG_Prescaler_8);//设置WWDG预分频数值  
   	WWDG_SetWindowValue(0x5F);//窗口上边界数值  
   	WWDG_Enable(0x7F);//使能窗口看门狗  
   	WWDG_ClearFlag(); //清除提前唤醒中断标志  
   	WWDG_EnableIT();//开启窗口看门狗中断	
}

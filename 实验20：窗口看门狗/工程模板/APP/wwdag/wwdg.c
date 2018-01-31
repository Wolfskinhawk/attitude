#include "wwdg.h"

/*******************************************************************************
* �� �� ��         : wwdg_init
* ��������		   : ���ڿ��Ź�ģʽ���ó�ʼ��	   
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void wwdg_init()
{
	NVIC_InitTypeDef NVIC_InitStructure;		//�жϽṹ�嶨��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE); // WWDGʱ��ʹ��
	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//���Ź������ȼ�Ҫ��������
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);

	WWDG_SetPrescaler(WWDG_Prescaler_8);//����WWDGԤ��Ƶ��ֵ  
   	WWDG_SetWindowValue(0x5F);//�����ϱ߽���ֵ  
   	WWDG_Enable(0x7F);//ʹ�ܴ��ڿ��Ź�  
   	WWDG_ClearFlag(); //�����ǰ�����жϱ�־  
   	WWDG_EnableIT();//�������ڿ��Ź��ж�	
}

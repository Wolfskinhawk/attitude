#include "lowpower.h"

/*******************************************************************************
* �� �� ��         : standmodeinit
* ��������		   : ����ģʽ���ó�ʼ��	   
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void standmodeinit()   //����ģʽ
{	
	NVIC_SystemLPConfig(NVIC_LP_SLEEPDEEP,ENABLE);//ѡ��ϵͳ����͹���ģʽ������	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);//ʹ��PWR����ʱ��	
	PWR_WakeUpPinCmd(ENABLE);//ʹ�ܻ��ѹܽ�	ʹ�ܻ���ʧ�ܻ��ѹܽŹ���
	PWR_EnterSTANDBYMode();//�������ģʽ		
}

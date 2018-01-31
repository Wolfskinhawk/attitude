/*******************************************************************************
*                 
*                 		       普中科技
--------------------------------------------------------------------------------
* 实 验 名		 : 窗口看门狗实验
* 实验说明       : 通过配置好内部窗口看门狗，printf打印初始显示数据后进入窗口看门狗中断 ，
					进行喂狗，间隔一段时间后返回主函数打印输出“普中科技欢迎您！”，中断
					不停喂狗，在串口助手上显示其输出 ，按下复位开关重新运行
					见文件内截图 ，程序设计中所有中断函数都放在中断stm32f10x_it.c文件
* 连接方式       : 
* 注    意		 : 	所用函数在头文件.c文件内
*******************************************************************************/


#include "public.h"
#include "printf.h"
#include "systick.h"
#include "wwdg.h"

/****************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/
int main()
{	
	printf_init();	 //printf初始化
	wwdg_init();  //窗口看门狗初始化配置
	printf("普中科技欢迎您！\r\n");
	printf("www.prechin.com\r\n");
	while(1)
	{	
		printf("普中科技欢迎您！\r\n");	
		delay_ms(1000);
	}			
}




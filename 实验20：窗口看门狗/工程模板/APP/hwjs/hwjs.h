#ifndef _hwjs_H
#define _hwjs_H
#include "stm32f10x.h"
#include "systick.h"
void hwjs_init(void);
u8 HW_jssj(void);

//定义全局变量
extern u32 hw_jsm;
extern u8  hw_jsbz;



#endif

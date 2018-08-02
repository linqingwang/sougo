#ifndef __LED_H//条件编译头
#define	__LED_H

#include "stm32f10x.h"

/* the macro definition to trigger the led on or off 
 * 1 - off
 - 0 - on
 */
#define ON  0//宏定义
#define OFF 1

//带参宏，可以像内联函数一样使用，此处\为续行符，ON为0为假，则执行else语句，将端口置0，线路导通，灯亮
#define LED1(a)	if (a)	\
					GPIO_SetBits(GPIOC,GPIO_Pin_3);\
					else		\
					GPIO_ResetBits(GPIOC,GPIO_Pin_3)

#define LED2(a)	if (a)	\
					GPIO_SetBits(GPIOC,GPIO_Pin_4);\
					else		\
					GPIO_ResetBits(GPIOC,GPIO_Pin_4)

#define LED3(a)	if (a)	\
					GPIO_SetBits(GPIOC,GPIO_Pin_5);\
					else		\
					GPIO_ResetBits(GPIOC,GPIO_Pin_5)

void LED_GPIO_Config(void);

#endif /* __LED_H条件编译尾 */

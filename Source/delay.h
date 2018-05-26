#ifndef __DELAY_H__
#define __DELAY_H__
#include "stm32f10x.h"
/*--------------------------------- 
描 述：参数1即为1ms，1000即为1s；只有几
us的误差；
-----------------------------------*/
extern void Delayms(__IO uint32_t nTime);

/*---------------------------------
描 述：参数1即为1us，1000即为1ms；只有几
us的误差；
-----------------------------------*/
extern void Delayus(__IO uint32_t nTime);

#endif
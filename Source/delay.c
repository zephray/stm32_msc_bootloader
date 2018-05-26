/*---------------------------------
延时模块函数
说明：只需在工程中加入delay.c和delay.h
文件，即可用 Delayms(__IO uint32_t nTime)；
Delayus(__IO uint32_t nTime)
-----------------------------------*/
#include"delay.h"
static __IO uint32_t TimingDelay;
/* Private function prototypes -----------------------------------------------*/
/*---------------------------------
函数名：ms延时函数 
描 述：参数1即为1ms，1000即为1s；只有几
us的误差；
-----------------------------------*/
void Delayms(__IO uint32_t nTime) 
{ 
while(SysTick_Config(SystemCoreClock/1000)); 
TimingDelay = nTime;
while(TimingDelay != 0);
SysTick->CTRL=0x00; //关闭计数器 
SysTick->VAL =0X00; //清空计数器 
}
/*---------------------------------
函数名：us延时函数 
描 述：参数1即为1us，1000即为1ms；只有几
us的误差；
-----------------------------------*/
void Delayus(__IO uint32_t nTime)
{ 
while(SysTick_Config(SystemCoreClock/1000000)); 
TimingDelay = nTime;
while(TimingDelay != 0);
SysTick->CTRL=0x00; //关闭计数器 
SysTick->VAL =0X00; //清空计数器 
}
/*---------------------------------
函数名：延时辅助函数 
描 述： 
-----------------------------------*/
void TimingDelay_Decrement(void)
{
if (TimingDelay != 0x00)
{ 
TimingDelay--;
}
}
/*---------------------------------
函数名：systick的中断函数 
描 述：参数1即为1us，1000即为1ms；只有几
us的误差；
-----------------------------------*/
void SysTick_Handler(void)
{
TimingDelay_Decrement();
}
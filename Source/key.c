#include "key.h" 
#include "delay.h"

void Key_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 

  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

u8 Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin)
{	
   if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0 ) 
  {	   
    Delayms(10);
    if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0 )  
    {	 
	return KEY_ON;	 
    }
    else
	return KEY_OFF;
  }
  else
    return KEY_OFF;
}

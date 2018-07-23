/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "delay.h"
#include "key.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "platform_config.h"
#include "usb_pwr.h"
#include "FAT16.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void Jump(uint32_t address)
{
    typedef void (*pFunction)(void);

    pFunction Jump_To_Application;

    // variable that will be loaded with the start address of the application
    vu32 *JumpAddress;
    const vu32 *ApplicationAddress = (vu32 *)address;

    // get jump address from application vector table
    JumpAddress = (vu32 *)ApplicationAddress[1];

    // load this address into function pointer
    Jump_To_Application = (pFunction)JumpAddress;

    // Disable all interrupts
    int i;
    for (i = 0; i < 8; i++)
        NVIC->ICER[i] = NVIC->IABR[i];

    // Set interrupt vector table
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x4000);

    // Set stack pointer as in application's vector table
    __set_MSP((u32)(ApplicationAddress[0]));

    // Go
    Jump_To_Application();
}

void reset_usb() {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_ResetBits(GPIOA, GPIO_Pin_12);
}

int main(void)
{
    /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */
    Key_GPIO_Config();
    GPIO_Config();
    
    // Force USB Reconnect
    reset_usb();
    Delayms(10);
    
    // If ISP Key is not pressed, jump to user application
    if (Key_Scan(GPIOC, GPIO_Pin_15) == KEY_OFF)
    {
        Jump(FLASH_START_ADDR);
    }

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    USB_Interrupts_Config();
    Set_USBClock();
    USB_Init();

    while (1)
    {
    }
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\\r\\n", file, line) */
    printf("\\033[1;40;41mERROR:Wrong parameters value: file %s on line %d\\033[0m\\r\\n", file, line)
        /* Infinite loop */
        while (1)
    {
    }
}
#endif

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

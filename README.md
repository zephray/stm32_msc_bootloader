# stm32_msc_bootloader

USB Mass Storage Bootloader for STM32F103 microcontrollers.

# Usage

Compile with IAR EWARM or use pre-compiled bin.

The microcontroller would act it self as a USB Mass Storage device, user can just drag'n'drop the new binary file into the emulated flash drive.

The bootloader is supposed to be flashed to 0x08000000 (Flash start address), user application will be flashed to 0x08004000 (defined in FAT16.h). Remember to use NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x4000) to remap the interrupt vector table in the user application.

# License

The ST Standard Peripherial Library is licensed under ST's License.

The FAT16.c is licensed under Freescale (NXP)'s License.

Other source codes are licensed under MIT License.
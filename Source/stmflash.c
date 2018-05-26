#include "stmflash.h"
#include "delay.h"
#include "stm32f10x_flash.h"

u16 STMFLASH_ReadHalfWord(u32 faddr)
{
    return *(vu16 *)faddr;
}

#if STM32_FLASH_WREN

void STMFLASH_Write_NoCheck(u32 WriteAddr, u16 *pBuffer, u16 NumToWrite)
{
    u16 i;
    for (i = 0; i < NumToWrite; i++)
    {
        FLASH_ProgramHalfWord(WriteAddr, pBuffer[i]);
        WriteAddr += 2;
    }
}

#if STM32_FLASH_SIZE < 256
#define STM_SECTOR_SIZE 1024
#else
#define STM_SECTOR_SIZE 2048
#endif
u16 STMFLASH_BUF[STM_SECTOR_SIZE / 2];

extern uint32_t flash_flag;

void STMFLASH_Write(u32 WriteAddr, u16 *pBuffer, u16 NumToWrite)
{
    u32 secpos;    // Sector Address
    u16 secoff;    // Offset in sector (count in Half-Word)
    u16 secremain; // Remain in secotr (count in Half-Word)
    u16 i;
    u32 offaddr; // Address - 0x08000000

    FLASH_Unlock();
    offaddr = WriteAddr - STM32_FLASH_BASE;
    secpos = offaddr / STM_SECTOR_SIZE;
    secoff = (offaddr % STM_SECTOR_SIZE) / 2;
    secremain = STM_SECTOR_SIZE / 2 - secoff;
    if (NumToWrite <= secremain)
        secremain = NumToWrite;
    while (1)
    {
        STMFLASH_Read(secpos * STM_SECTOR_SIZE + STM32_FLASH_BASE, STMFLASH_BUF, STM_SECTOR_SIZE / 2);
        for (i = 0; i < secremain; i++)
        {
            if (STMFLASH_BUF[secoff + i] != 0XFFFF)
                break;
        }
        if (i < secremain)
        {
            FLASH_ErasePage(secpos * STM_SECTOR_SIZE + STM32_FLASH_BASE);
            for (i = 0; i < secremain; i++)
            {
                STMFLASH_BUF[i + secoff] = pBuffer[i];
            }
            STMFLASH_Write_NoCheck(secpos * STM_SECTOR_SIZE + STM32_FLASH_BASE, STMFLASH_BUF, STM_SECTOR_SIZE / 2);
        }
        else
            STMFLASH_Write_NoCheck(WriteAddr, pBuffer, secremain);
        if (NumToWrite == secremain)
            break;
        else
        {
            secpos++;
            secoff = 0;
            pBuffer += secremain;
            WriteAddr += secremain;
            NumToWrite -= secremain;
            if (NumToWrite > (STM_SECTOR_SIZE / 2))
                secremain = STM_SECTOR_SIZE / 2;
            else
                secremain = NumToWrite;
        }
    };
    FLASH_Lock();
}
#endif

void STMFLASH_Read(u32 ReadAddr, u16 *pBuffer, u16 NumToRead)
{
    uint32_t addr = 0x08016000 + 400 * 1024;
    u16 i;

    if (ReadAddr >= addr)
    {
        for (i = 0; i < NumToRead; i++)
        {
            pBuffer[i] = 0x0000;
        }

        return;
    }

    for (i = 0; i < NumToRead; i++)
    {
        pBuffer[i] = STMFLASH_ReadHalfWord(ReadAddr);
        ReadAddr += 2;
    }
}

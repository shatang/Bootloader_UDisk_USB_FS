#include "flash_if.h"



 



//获取某个地址所在的flash扇区
//addr:flash地址
//返回值:0~11,即addr所在的扇区
static uint16_t GetSector(uint32_t fu32_Addr)
{
	if(fu32_Addr<ADDR_FLASH_SECTOR_1)return FLASH_Sector_0;
	else if(fu32_Addr<ADDR_FLASH_SECTOR_2)return FLASH_Sector_1;
	else if(fu32_Addr<ADDR_FLASH_SECTOR_3)return FLASH_Sector_2;
	else if(fu32_Addr<ADDR_FLASH_SECTOR_4)return FLASH_Sector_3;
	else if(fu32_Addr<ADDR_FLASH_SECTOR_5)return FLASH_Sector_4;
	else if(fu32_Addr<ADDR_FLASH_SECTOR_6)return FLASH_Sector_5;
	else if(fu32_Addr<ADDR_FLASH_SECTOR_7)return FLASH_Sector_6;
	else if(fu32_Addr<ADDR_FLASH_SECTOR_8)return FLASH_Sector_7;
	else if(fu32_Addr<ADDR_FLASH_SECTOR_9)return FLASH_Sector_8;
	else if(fu32_Addr<ADDR_FLASH_SECTOR_10)return FLASH_Sector_9;
	else if(fu32_Addr<ADDR_FLASH_SECTOR_11)return FLASH_Sector_10; 
	return FLASH_Sector_11;	
}

/********************************************************************************
* 函数名 :  InternalFLASH_Init
* 功  能 :  stm32的内部flash初始化(解锁)
* 说  明 :  none
* 入  参 :  none
* 返  回 :  none
* 设  计 :  Shatang                   时  间 :  2020.06.18
* 修  改 :  none                      时  间 :  none
********************************************************************************/
void FLASH_If_Init(void)
{ 
    FLASH_Unlock();

    /* Clear pending flags (if any) */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
}
/********************************************************************************
* 函数名 : FLASH_If_Finish
* 功  能 : stm32的内部flash关闭(上锁)
* 说  明 : none
* 入  参 : none
* 返  回 : none
* 设  计 : Shatang                    时  间 : 2020.07.29
* 修  改 : none                       时  间 : none
********************************************************************************/
void FLASH_If_Finish(void)
{
    /* Clear pending flags (if any) */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
    
    FLASH_Lock();
}
/********************************************************************************
* 函数名 :  FLASH_If_Erase
* 功  能 :  stm32的内部flash擦除
* 说  明 :  只针对APP文件地址的存储内容进行擦除
* 入  参 :  none
* 返  回 :  none
* 设  计 :  Shatang                   时  间 :  2020.06.18
* 修  改 :  none                      时  间 :  none
********************************************************************************/
uint32_t FLASH_If_Erase(uint32_t StartSector)
{
    uint32_t UserStartSector;
    uint32_t i = 0;

    /* Get the sector where start the user flash area */
    UserStartSector = GetSector(StartSector);
    
    for(i = UserStartSector; i <= GetSector(APPLICATION_END_ADDRESS); i += 8)
    {
        /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
        be done by word */ 
        if (FLASH_EraseSector(i, VoltageRange_3) != FLASH_COMPLETE)
        {
            /* Error occurred while page erase */
            return (1);
        }
    }
    
    return (0);
}
/********************************************************************************
* 函数名 :  FLASH_If_Write
* 功  能 :  stm32的内部flash写操作
* 说  明 :  none
* 入  参 :  none
* 返  回 :  none
* 设  计 :  Shatang                   时  间 :  2020.06.18
* 修  改 :  none                      时  间 :  none
********************************************************************************/
uint32_t FLASH_If_Write(__IO uint32_t* FlashAddress, uint32_t* Data ,uint32_t DataLength)
{
    uint32_t i = 0;

    for (i = 0; (i < DataLength) && (*FlashAddress <= (APPLICATION_END_ADDRESS-4)); i++)
    {
        /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
           be done by word */ 
        if (FLASH_ProgramWord(*FlashAddress, *(uint32_t*)(Data+i)) == FLASH_COMPLETE)
        {
            /* Check the written value */
            if (*(uint32_t*)*FlashAddress != *(uint32_t*)(Data+i))
            {
                /* Flash content doesn't match SRAM content */
                return(2);
            }
            /* Increment FLASH destination address */
            *FlashAddress += 4;
        }
        else
        {
            /* Error occurred while writing data in Flash memory */
            return (1);
        }
    }

    return (0);
}


//待定增加
//uint16_t FLASH_If_GetWriteProtectionStatus(void);
//uint32_t FLASH_If_DisableWriteProtection(void);

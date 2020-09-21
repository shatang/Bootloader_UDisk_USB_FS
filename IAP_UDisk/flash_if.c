#include "flash_if.h"



 



//��ȡĳ����ַ���ڵ�flash����
//addr:flash��ַ
//����ֵ:0~11,��addr���ڵ�����
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
* ������ :  InternalFLASH_Init
* ��  �� :  stm32���ڲ�flash��ʼ��(����)
* ˵  �� :  none
* ��  �� :  none
* ��  �� :  none
* ��  �� :  Shatang                   ʱ  �� :  2020.06.18
* ��  �� :  none                      ʱ  �� :  none
********************************************************************************/
void FLASH_If_Init(void)
{ 
    FLASH_Unlock();

    /* Clear pending flags (if any) */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
}
/********************************************************************************
* ������ : FLASH_If_Finish
* ��  �� : stm32���ڲ�flash�ر�(����)
* ˵  �� : none
* ��  �� : none
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.07.29
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
void FLASH_If_Finish(void)
{
    /* Clear pending flags (if any) */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
    
    FLASH_Lock();
}
/********************************************************************************
* ������ :  FLASH_If_Erase
* ��  �� :  stm32���ڲ�flash����
* ˵  �� :  ֻ���APP�ļ���ַ�Ĵ洢���ݽ��в���
* ��  �� :  none
* ��  �� :  none
* ��  �� :  Shatang                   ʱ  �� :  2020.06.18
* ��  �� :  none                      ʱ  �� :  none
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
* ������ :  FLASH_If_Write
* ��  �� :  stm32���ڲ�flashд����
* ˵  �� :  none
* ��  �� :  none
* ��  �� :  none
* ��  �� :  Shatang                   ʱ  �� :  2020.06.18
* ��  �� :  none                      ʱ  �� :  none
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


//��������
//uint16_t FLASH_If_GetWriteProtectionStatus(void);
//uint32_t FLASH_If_DisableWriteProtection(void);

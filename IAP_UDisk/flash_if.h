#ifndef __FALSH_IF_H
#define __FALSH_IF_H
#include <stdint.h>
#include "stm32f4xx.h"

//FLASH ��������ʼ��ַ
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) 	//����0��ʼ��ַ, 16 Kbytes
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) 	//����1��ʼ��ַ, 16 Kbytes
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) 	//����2��ʼ��ַ, 16 Kbytes
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) 	//����3��ʼ��ַ, 16 Kbytes
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) 	//����4��ʼ��ַ, 64 Kbytes
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) 	//����5��ʼ��ַ, 128 Kbytes
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) 	//����6��ʼ��ַ, 128 Kbytes
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) 	//����7��ʼ��ַ, 128 Kbytes
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) 	//����8��ʼ��ַ, 128 Kbytes
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) 	//����9��ʼ��ַ, 128 Kbytes
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) 	//����10��ʼ��ַ,128 Kbytes
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) 	//����11��ʼ��ַ,128 Kbytes


//��ǰAPP�ļ����ռ�Ϊ��0xF7FFF
#define STM32_FLASH_BASE            ((uint32_t)0x08000000)//STM32 FLASH����ʼ��ַ
#define APPLICATION_START_ADDRESS   ((uint32_t)0x08008000)//APP�ļ�����ʼ��ַ
#define APPLICATION_END_ADDRESS     ((uint32_t)0x080FFFFF)//APP�ļ��Ľ�����ַ
#define APPLICATION_SIZE   (APPLICATION_END_ADDRESS - APPLICATION_START_ADDRESS + 1)//APP�ļ��Ĵ�С


extern void FLASH_If_Init(void);
extern void FLASH_If_Finish(void);
extern uint32_t FLASH_If_Erase(uint32_t StartSector);
extern uint32_t FLASH_If_Write(__IO uint32_t* FlashAddress, uint32_t* Data ,uint32_t DataLength);


#endif

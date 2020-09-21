#ifndef __FALSH_IF_H
#define __FALSH_IF_H
#include <stdint.h>
#include "stm32f4xx.h"

//FLASH 扇区的起始地址
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) 	//扇区0起始地址, 16 Kbytes
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) 	//扇区1起始地址, 16 Kbytes
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) 	//扇区2起始地址, 16 Kbytes
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) 	//扇区3起始地址, 16 Kbytes
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) 	//扇区4起始地址, 64 Kbytes
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) 	//扇区5起始地址, 128 Kbytes
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) 	//扇区6起始地址, 128 Kbytes
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) 	//扇区7起始地址, 128 Kbytes
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) 	//扇区8起始地址, 128 Kbytes
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) 	//扇区9起始地址, 128 Kbytes
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) 	//扇区10起始地址,128 Kbytes
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) 	//扇区11起始地址,128 Kbytes


//当前APP文件最大空间为：0xF7FFF
#define STM32_FLASH_BASE            ((uint32_t)0x08000000)//STM32 FLASH的起始地址
#define APPLICATION_START_ADDRESS   ((uint32_t)0x08008000)//APP文件的起始地址
#define APPLICATION_END_ADDRESS     ((uint32_t)0x080FFFFF)//APP文件的结束地址
#define APPLICATION_SIZE   (APPLICATION_END_ADDRESS - APPLICATION_START_ADDRESS + 1)//APP文件的大小


extern void FLASH_If_Init(void);
extern void FLASH_If_Finish(void);
extern uint32_t FLASH_If_Erase(uint32_t StartSector);
extern uint32_t FLASH_If_Write(__IO uint32_t* FlashAddress, uint32_t* Data ,uint32_t DataLength);


#endif

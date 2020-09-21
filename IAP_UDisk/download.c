/* 包含头文件 *****************************************************************/
#include "download.h"
#include "ff.h"
#include "exfuns.h"
#include "flash_if.h"
#include "LED_Driver.h"
/********************************************************************************
* 函数名 : IAP_Close
* 功  能 : IAP功能关闭
* 说  明 : none
* 入  参 : none
* 返  回 : none
* 设  计 : Shatang                    时  间 : 2020.06.18
* 修  改 : none                       时  间 : none
********************************************************************************/
void IAP_Close(void)
{
    TIM_Cmd(TIM3, DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,  DISABLE);
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS, DISABLE);
}




char *m_file = "2:Shatang.BIN"; //APP 文件名称 默认起始为 "2:"后跟文件名，文件名不能含中文
#define READ_COUNT 512
uint8_t   ReadAppBuffer[READ_COUNT];
extern uint8_t IAPdownload;

void UDisk_Download(void)
{ 
	FIL    FP_Struct;   //用于打开文件
    uint8_t gCheckFat = 0;
    uint32_t total, free;

    gCheckFat = exf_getfree("2:", &total, &free);
    if(gCheckFat == 0 )
    {
        gCheckFat = f_open(&FP_Struct, m_file, FA_READ);
        if(gCheckFat == 0)
        {
            uint32_t i = 0;
            uint16_t ReadNum = 0;
            
            uint32_t APP_Sector = 0;
            uint16_t APP_Byte = 0;
            uint32_t FlashDestination = APPLICATION_START_ADDRESS;//初始化写入Flash地址变量
            
            APP_Sector = FP_Struct.fsize / READ_COUNT;
            APP_Byte = FP_Struct.fsize % READ_COUNT;

           
            if(APP_Sector+APP_Byte > APPLICATION_SIZE - 1)return;//超出芯片可存储大小

            LED_1(1);
            LED_2(1);//成功识别到U盘对应文件，准备擦除芯片Flash
            LED_3(0);
            
            FLASH_If_Erase(APPLICATION_START_ADDRESS);//计算需要擦除Flash的页
            
            LED_1(1);
            LED_2(1);
            LED_3(1);//芯片Flash擦除成功，准备烧录升级文件
            
            for(i = 0;i < APP_Sector;i++)
            {
                f_read (&FP_Struct, ReadAppBuffer, READ_COUNT, (UINT *)&ReadNum);
                FLASH_If_Write(&FlashDestination, (u32*)ReadAppBuffer, READ_COUNT/4);
            }
            if(APP_Byte != 0)
            {
                f_read (&FP_Struct,ReadAppBuffer,APP_Byte,(UINT *)&ReadNum);
                FLASH_If_Write(&FlashDestination, (u32*)ReadAppBuffer, APP_Byte/4);
            }
            f_close (&FP_Struct);
//          printf("系统更新成功！\r\n");
        }
        
        IAPdownload = 1;//文件更新成功
    }
}

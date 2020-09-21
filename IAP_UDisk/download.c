/* ����ͷ�ļ� *****************************************************************/
#include "download.h"
#include "ff.h"
#include "exfuns.h"
#include "flash_if.h"
#include "LED_Driver.h"
/********************************************************************************
* ������ : IAP_Close
* ��  �� : IAP���ܹر�
* ˵  �� : none
* ��  �� : none
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.06.18
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
void IAP_Close(void)
{
    TIM_Cmd(TIM3, DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,  DISABLE);
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS, DISABLE);
}




char *m_file = "2:Shatang.BIN"; //APP �ļ����� Ĭ����ʼΪ "2:"����ļ������ļ������ܺ�����
#define READ_COUNT 512
uint8_t   ReadAppBuffer[READ_COUNT];
extern uint8_t IAPdownload;

void UDisk_Download(void)
{ 
	FIL    FP_Struct;   //���ڴ��ļ�
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
            uint32_t FlashDestination = APPLICATION_START_ADDRESS;//��ʼ��д��Flash��ַ����
            
            APP_Sector = FP_Struct.fsize / READ_COUNT;
            APP_Byte = FP_Struct.fsize % READ_COUNT;

           
            if(APP_Sector+APP_Byte > APPLICATION_SIZE - 1)return;//����оƬ�ɴ洢��С

            LED_1(1);
            LED_2(1);//�ɹ�ʶ��U�̶�Ӧ�ļ���׼������оƬFlash
            LED_3(0);
            
            FLASH_If_Erase(APPLICATION_START_ADDRESS);//������Ҫ����Flash��ҳ
            
            LED_1(1);
            LED_2(1);
            LED_3(1);//оƬFlash�����ɹ���׼����¼�����ļ�
            
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
//          printf("ϵͳ���³ɹ���\r\n");
        }
        
        IAPdownload = 1;//�ļ����³ɹ�
    }
}

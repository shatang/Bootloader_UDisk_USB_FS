#include "stm32f4xx.h" 
#include "delay.h"
#include "Timer_Driver.h"
#include "LED_Driver.h"

#include "exfuns.h"
#include "malloc.h"
#include "usbh_usr.h"
#include "download.h"
#include "flash_if.h"


typedef void (*pFunction)(void);
/********************************************************************************
* ������ : IAP_LoadApp
* ��  �� : Bootloader��ת��APP
* ˵  �� : none
* ��  �� : none
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.06.23
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
void IAP_LoadApp(uint32_t appxaddr)
{
    pFunction JumpApp;
    
	if(((*(__IO uint32_t*)appxaddr)&0x2FFE0000) == 0x20000000)	//���ջ����ַ�Ƿ�Ϸ�.
	{ 
		JumpApp=(pFunction)*(__IO uint32_t*)(appxaddr+4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
		__set_MSP(*(__IO uint32_t*)appxaddr);				    //��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
		JumpApp();								                //��ת��APP.
	}
}


USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;

uint8_t UsbConnected = 0;
uint8_t IAPdownload = 0;
/********************************************************************************
* ������ : main
* ��  �� : ������
* ˵  �� : ʵ��Bootloader����U������������ת���ϵ�����U���ļ�2S���Զ���ת��APP
* ��  �� : none
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.06.23
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
int main(void)
{
    uint32_t lu32_Tick = 0;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init(168);  //��ʼ����ʱ����
    FLASH_If_Init();
    LED_PortInit();
    
	my_mem_init(SRAMIN);	//��ʼ���ڲ��ڴ��	
 	exfuns_init();			//Ϊfatfs��ر��������ڴ� 
  	f_mount(fs[2],"2:",1); 	//����U��   

    LED_1(1);//Bootloader����������
    LED_2(0);
    LED_3(0);
    //��ʼ��USB����
  	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_Callbacks); 

    lu32_Tick = SysTimeGet();
    while(!IAPdownload)
    {
        USBH_Process(&USB_OTG_Core, &USB_Host);
        if(SysTimeGet() - lu32_Tick > 2000)IAPdownload = 1;
    }

    FLASH_If_Finish();
    IAP_Close();
    IAP_LoadApp(APPLICATION_START_ADDRESS); //������ת
    
    while(1){}
        
    return 0;
}


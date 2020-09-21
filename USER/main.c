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
* 函数名 : IAP_LoadApp
* 功  能 : Bootloader跳转至APP
* 说  明 : none
* 入  参 : none
* 返  回 : none
* 设  计 : Shatang                    时  间 : 2020.06.23
* 修  改 : none                       时  间 : none
********************************************************************************/
void IAP_LoadApp(uint32_t appxaddr)
{
    pFunction JumpApp;
    
	if(((*(__IO uint32_t*)appxaddr)&0x2FFE0000) == 0x20000000)	//检查栈顶地址是否合法.
	{ 
		JumpApp=(pFunction)*(__IO uint32_t*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		__set_MSP(*(__IO uint32_t*)appxaddr);				    //初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		JumpApp();								                //跳转到APP.
	}
}


USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;

uint8_t UsbConnected = 0;
uint8_t IAPdownload = 0;
/********************************************************************************
* 函数名 : main
* 功  能 : 主函数
* 说  明 : 实现Bootloader进行U盘升级，并跳转；上电检测无U盘文件2S，自动跳转旧APP
* 入  参 : none
* 返  回 : none
* 设  计 : Shatang                    时  间 : 2020.06.23
* 修  改 : none                       时  间 : none
********************************************************************************/
int main(void)
{
    uint32_t lu32_Tick = 0;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init(168);  //初始化延时函数
    FLASH_If_Init();
    LED_PortInit();
    
	my_mem_init(SRAMIN);	//初始化内部内存池	
 	exfuns_init();			//为fatfs相关变量申请内存 
  	f_mount(fs[2],"2:",1); 	//挂载U盘   

    LED_1(1);//Bootloader程序运行中
    LED_2(0);
    LED_3(0);
    //初始化USB主机
  	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_Callbacks); 

    lu32_Tick = SysTimeGet();
    while(!IAPdownload)
    {
        USBH_Process(&USB_OTG_Core, &USB_Host);
        if(SysTimeGet() - lu32_Tick > 2000)IAPdownload = 1;
    }

    FLASH_If_Finish();
    IAP_Close();
    IAP_LoadApp(APPLICATION_START_ADDRESS); //程序跳转
    
    while(1){}
        
    return 0;
}


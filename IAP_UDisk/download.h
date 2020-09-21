#ifndef __DOWNLOAD_H
#define __DOWNLOAD_H
#include <stdint.h>



extern void IAP_Init(void);
extern void IAP_Close(void);
extern void SerialDownload(uint8_t fu8_Channal);
extern uint8_t Serial_GetDownloadChannel(void);
extern uint8_t Serial_CheckDownloadChar(void);

#endif

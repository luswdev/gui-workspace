/**
 *******************************************************************************
 * @file       main.h
 * @version    V0.0.1
 * @date       2020.01.19
 * @brief      Main define file.
 *******************************************************************************
 */ 

#ifndef __MAIN_H__
#define __MAIN_H__

#define TASK_STK_SIZE			256

#define USE_FULL_ASSERT

#define USE_COOS
#ifdef USE_COOS
 #define USE_GUI
#endif

#define USE_USB
#ifdef USE_USB
 #define USE_MOUSE
 #define USE_KBD
#endif

#ifdef USE_USB
 extern USB_OTG_CORE_HANDLE      USB_OTG_Core;
 extern USBH_HOST                USB_Host;
#endif

#ifndef USE_COOS
 void TimingDelay_Decrement(void);
 void Delay(__IO uint32_t nTime);
#endif

#endif /* __MAIN_H__ */

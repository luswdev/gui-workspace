/**
  ******************************************************************************
  * @file    usbh_usr.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   USB Host User Functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "cogui.h"
#include "stm32f429i_discovery_lcd.h"
#include "user_config.h"
#include "usbh_core.h"
#include "usbh_usr.h"
#include "usbh_hid_mouse.h"
#include "usbh_hid_keybd.h"
#include "main.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern USB_OTG_CORE_HANDLE            USB_OTG_Core;
extern USBH_HOST                      USB_Host;

/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */

USBH_Usr_cb_TypeDef USBH_USR_cb =
  {
    USBH_USR_Init,
    USBH_USR_DeInit,
    USBH_USR_DeviceAttached,
    USBH_USR_ResetDevice,
    USBH_USR_DeviceDisconnected,
    USBH_USR_OverCurrentDetected,
    USBH_USR_DeviceSpeedDetected,
    USBH_USR_Device_DescAvailable,
    USBH_USR_DeviceAddressAssigned,
    USBH_USR_Configuration_DescAvailable,
    USBH_USR_Manufacturer_String,
    USBH_USR_Product_String,
    USBH_USR_SerialNum_String,
    USBH_USR_EnumerationDone,
    USBH_USR_UserInput,
    Co_NULL,
    USBH_USR_DeviceNotSupported,
    USBH_USR_UnrecoveredError
  };

__IO enum host_status USB_Host_status = USB_HOST_NOT_INIT;

/*--------------- LCD Messages ---------------*/
const uint8_t MSG_HOST_INIT[]        = "Host Library Initialized";
const uint8_t MSG_DEV_ATTACHED[]     = "Device Attached ";
const uint8_t MSG_DEV_DISCONNECTED[] = "Device Disconnected";
const uint8_t MSG_DEV_ENUMERATED[]   = "Enumeration completed ";
const uint8_t MSG_DEV_HIGHSPEED[]    = "High speed device detected";
const uint8_t MSG_DEV_FULLSPEED[]    = "Full speed device detected";
const uint8_t MSG_DEV_LOWSPEED[]     = "Low speed device detected";
const uint8_t MSG_DEV_ERROR[]        = "Device fault ";

const uint8_t MSG_MSC_CLASS[]        = "Mass storage device connected";
const uint8_t MSG_HID_CLASS[]        = "HID device connected";
const uint8_t MSG_DISK_SIZE[]        = "Size of the disk in MBytes: ";
const uint8_t MSG_LUN[]              = "LUN Available in the device:";
const uint8_t MSG_ROOT_CONT[]        = "Exploring disk flash ...";
const uint8_t MSG_WR_PROTECT[]       = "The disk is write protected";
const uint8_t MSG_UNREC_ERROR[]      = "UNRECOVERED ERROR STATE";

const uint8_t MSG_MOUSE_INPUT[]      = "GET MOUSE INPUT";


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Add the host lib initialization message to the console
  * @param  None
  * @retval None
  */
void USBH_USR_Init(void)
{
    if (USB_Host_status == USB_HOST_NOT_INIT) {
        cogui_printf("\t│\t\t[sys] Start initial usb host............");
    }

    USB_Host_status = USB_HOST_INIT;
}

/**
  * @brief  Add the device attachement message to the console
  * @param  None
  * @retval None
  */
void USBH_USR_DeviceAttached(void)
{
    cogui_printf("[system] %s.\r\n", MSG_DEV_ATTACHED);
    STM_EVAL_LEDOn(LED4);

    USB_Host_status = USB_HOST_ATTACH;
}


/**
  * @brief  Add the unrecovered error message to the console
  * @param  None
  * @retval None
  */
void USBH_USR_UnrecoveredError (void)
{
    cogui_printf("[system] %s.\r\n", MSG_UNREC_ERROR);
}


/**
  * @brief Add the device disconnection message to the console and free 
  *        USB associated resources
  * @param  None
  * @retval None
  */
void USBH_USR_DeviceDisconnected (void)
{
    cogui_printf("[system] %s.\r\n", MSG_DEV_DISCONNECTED);

    STM_EVAL_LEDOff(LED4);

    USB_Host_status = USB_HOST_DISCONNECT;
}
/**
  * @brief  callback of the device reset event
  * @param  None
  * @retval None
  */
void USBH_USR_ResetDevice(void)
{
}


/**
  * @brief  Add the device speed message to the console
  * @param  Device speed
  * @retval None
  */
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
    if (DeviceSpeed == HPRT0_PRTSPD_HIGH_SPEED) {
        cogui_printf("[system] %s.\r\n", MSG_DEV_HIGHSPEED);
	} 
    else if (DeviceSpeed == HPRT0_PRTSPD_FULL_SPEED) {
        cogui_printf("[system] %s.\r\n", MSG_DEV_FULLSPEED);
	} 
    else if (DeviceSpeed == HPRT0_PRTSPD_LOW_SPEED) {
        cogui_printf("[system] %s.\r\n", MSG_DEV_LOWSPEED);
	} 
    else {
		cogui_printf("[system] %s.\r\n", MSG_DEV_ERROR);
	}
}

/**
  * @brief  Add the USB device vendor and MFC Ids to the console
  * @param  device descriptor
  * @retval None
  */
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{
    USBH_DevDesc_TypeDef *hs;
    hs = DeviceDesc;  
    
    cogui_printf("[system] VID : %d\r\n" , (uint32_t)(*hs).idVendor);
    cogui_printf("[system] PID : %d\r\n" , (uint32_t)(*hs).idProduct); 
}

/**
  * @brief  Device addressed event callbacak
  * @param  None
  * @retval None
  */
void USBH_USR_DeviceAddressAssigned(void)
{
}

/**
  * @brief  Add the device class description to the console
  * @param  Configuration descriptor
  * @retval None
  */
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
    USBH_InterfaceDesc_TypeDef *itfDesc,
    USBH_EpDesc_TypeDef *epDesc)
{
    USBH_InterfaceDesc_TypeDef *id;
  
    id = itfDesc;  
    
    if ((*id).bInterfaceClass  == 0x08) {
		cogui_printf("[system] %s.\r\n", MSG_MSC_CLASS);
    } 
    else if ((*id).bInterfaceClass  == 0x03) {
        cogui_printf("[system] %s.\r\n", MSG_HID_CLASS);
    }   
}

/**
  * @brief  Add the MFC String to the console
  * @param  Manufacturer String
  * @retval None
  */
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
    cogui_printf("[system] Manufacturer : %s\r\n", (char *)ManufacturerString);
}

/**
  * @brief  Add the Product String to the console
  * @param  Product String
  * @retval None
  */
void USBH_USR_Product_String(void *ProductString)
{
    cogui_printf("[system] Product : %s\r\n", (char *)ProductString);  
}

/**
  * @brief  Add the Srial Number String to the console
  * @param  SerialNum_String
  * @retval None
  */
void USBH_USR_SerialNum_String(void *SerialNumString)
{
    cogui_printf("[system] Serial Number : %s\r\n", (char *)SerialNumString);    
}

/**
  * @brief  Enumeration complete event callback
  * @param  None
  * @retval None
  */
void USBH_USR_EnumerationDone(void)
{
    cogui_printf("[system] %s.\r\n", MSG_DEV_ENUMERATED);
}

/**
  * @brief  Device is not supported callback
  * @param  None
  * @retval None
  */
void USBH_USR_DeviceNotSupported(void)
{
    cogui_printf("[system] Device not supported.\r\n");
}

/**
  * @brief  User Action for application state entry callback
  * @param  None
  * @retval USBH_USR_Status : User response for key button
  */
USBH_USR_Status USBH_USR_UserInput(void)
{
    return USBH_USR_RESP_OK;
}

/**
  * @brief  Over Current Detected on VBUS
  * @param  None
  * @retval Staus
  */
void USBH_USR_OverCurrentDetected (void)
{
    cogui_printf("[system] Overcurrent detected.\r\n");
}

/**
  * @brief  De-init User state and associated variables
  * @param  None
  * @retval None
  */
void USBH_USR_DeInit(void)
{
}

void  USR_KEYBRD_Init (void) 
{
}

void  USR_KEYBRD_ProcessData (uint8_t pbuf)
{
}

int16_t mouse_event[3] = {0, 0, 0};
int16_t cursor[3] = {0,0,0};

void update_cursor(int16_t dx, int16_t dy, uint8_t btn)
{
#ifndef USE_COOS
    switch (btn)
    {
        case 1:
            LCD_SetTextColor(COGUI_BLUE);               /* set background: blue if button-left pressed          */
            break;

        case 2:
            LCD_SetTextColor(COGUI_GREEN);              /* set background: green if button-right pressed        */
            break;

        case 3:
            LCD_SetTextColor(COGUI_RED);                /* set background: red if both left and right pressed   */
            break;
        
        default:
            LCD_SetTextColor(COGUI_WHITE);              /* set background: white if no button pressed           */
            break;
    }
    LCD_DrawFullRect(0, 0, 240, 320);                   /* return last cursor buffer                            */
    
    cursor[0] += dx/4;                                  /* update cursor position-x                             */
    cursor[1] += dy/4;                                  /* update cursor position-y                             */
    cursor[2]  = btn;                                   /* update cursor button event                           */

    if (cursor[0] > 230) {
        cursor[0] = 230;                                /* max x: 230                                           */
    }
    else if (cursor[0] < 0) {
        cursor[0] = 0;                                  /* min x: 0                                             */
    }

    if (cursor[1] >= 310) {
        cursor[1] = 310;                                /* max y: 310                                           */
    }
    else if (cursor[1] < 0) {
        cursor[1] = 0;                                  /* min y: 0                                             */
    }

    LCD_SetTextColor(COGUI_BLACK);
    LCD_DrawFullRect(cursor[0], cursor[1], 10, 10);     /* draw cursor                                          */
#else
    struct cogui_event event;
    COGUI_EVENT_INIT(&event, COGUI_EVENT_MOUSE_MOTION);

    event.dx = dx;
    event.dy = dy;

    cogui_send(cogui_get_server(), &event);
#endif

    USB_Host_status = USB_HOST_MOUSE_IDLE;              /* set status back to idle                              */
}

void  USR_MOUSE_Init (void)
{
    cogui_printf("[system] Connect to a mouse.\r\n");

    cursor[0] = 115;
    cursor[1] = 155;
    //update_cursor(0, 0, 0);                     /* set cursor position in middle of screen              */

    USB_Host_status = USB_HOST_MOUSE_CONNECT;
}

void  USR_MOUSE_ProcessData (HID_MOUSE_Data_TypeDef *data)
{
    if (data->x || data->y) {
        USB_Host_status = USB_HOST_MOUSE_MOVE;

        if (data->x > 127) {
            mouse_event[0] = data->x - 256;
        }
        else {
            mouse_event[0] = data->x;
        }
        if (data->y > 127) {
            mouse_event[1] = data->y - 256;
        }
        else {
            mouse_event[1] = data->y;
        }
    }
    else {
        mouse_event[0] = 0;
        mouse_event[1] = 0;

        switch (data->button) {
            case 0:
                USB_Host_status = USB_HOST_MOUSE_BUTTON_RELEASED;
                break;

            case 1:
            case 2:
            case 3:
            case 4:
                USB_Host_status = USB_HOST_MOUSE_BUTTON_PRESSED;
                break;

            default:
                break;
        }
    }

    mouse_event[2] = data->button;

    update_cursor(mouse_event[0], mouse_event[1], mouse_event[2]);
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

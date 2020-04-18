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
#include "main.h"
#include "usbh_usr.h"
#include "usbh_hid_mouse.h"
#include "usbh_hid_keybd.h"


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
    USB_Host_status = USB_HOST_INIT;
}

/**
  * @brief  Add the device attachement message to the console
  * @param  None
  * @retval None
  */
void USBH_USR_DeviceAttached(void)
{
    gui_printf("[Usb Host] %s.\r\n", MSG_DEV_ATTACHED);

    USB_Host_status = USB_HOST_ATTACH;
}


/**
  * @brief  Add the unrecovered error message to the console
  * @param  None
  * @retval None
  */
void USBH_USR_UnrecoveredError (void)
{
    gui_printf("[Usb Host] %s.\r\n", MSG_UNREC_ERROR);
}


/**
  * @brief Add the device disconnection message to the console and free 
  *        USB associated resources
  * @param  None
  * @retval None
  */
void USBH_USR_DeviceDisconnected (void)
{
    gui_printf("[Usb Host] %s.\r\n", MSG_DEV_DISCONNECTED);

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
        gui_printf("[Usb Host] %s.\r\n", MSG_DEV_HIGHSPEED);
	} 
    else if (DeviceSpeed == HPRT0_PRTSPD_FULL_SPEED) {
        gui_printf("[Usb Host] %s.\r\n", MSG_DEV_FULLSPEED);
	} 
    else if (DeviceSpeed == HPRT0_PRTSPD_LOW_SPEED) {
        gui_printf("[Usb Host] %s.\r\n", MSG_DEV_LOWSPEED);
	} 
    else {
		gui_printf("[Usb Host] %s.\r\n", MSG_DEV_ERROR);
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
    
    gui_printf("[Usb Host] VID : %d\r\n" , (uint32_t)(*hs).idVendor);
    gui_printf("[Usb Host] PID : %d\r\n" , (uint32_t)(*hs).idProduct); 
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
		gui_printf("[Usb Host] %s.\r\n", MSG_MSC_CLASS);
    } 
    else if ((*id).bInterfaceClass  == 0x03) {
        gui_printf("[Usb Host] %s.\r\n", MSG_HID_CLASS);
    }   
}

/**
  * @brief  Add the MFC String to the console
  * @param  Manufacturer String
  * @retval None
  */
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
    gui_printf("[Usb Host] Manufacturer : %s\r\n", (char *)ManufacturerString);
}

/**
  * @brief  Add the Product String to the console
  * @param  Product String
  * @retval None
  */
void USBH_USR_Product_String(void *ProductString)
{
    gui_printf("[Usb Host] Product : %s\r\n", (char *)ProductString);  
}

/**
  * @brief  Add the Srial Number String to the console
  * @param  SerialNum_String
  * @retval None
  */
void USBH_USR_SerialNum_String(void *SerialNumString)
{
    gui_printf("[Usb Host] Serial Number : %s\r\n", (char *)SerialNumString);    
}

/**
  * @brief  Enumeration complete event callback
  * @param  None
  * @retval None
  */
void USBH_USR_EnumerationDone(void)
{
    gui_printf("[Usb Host] %s.\r\n", MSG_DEV_ENUMERATED);
}

/**
  * @brief  Device is not supported callback
  * @param  None
  * @retval None
  */
void USBH_USR_DeviceNotSupported(void)
{
    gui_printf("[Usb Host] Device not supported.\r\n");
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
    gui_printf("[Usb Host] Overcurrent detected.\r\n");
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
    gui_printf("[Usb Host] Connect to a keyboard\r\n");
    gui_printf(WHITE_BG" \b"ARESET);
}

uint8_t last_key = 0;

void  USR_KEYBRD_ProcessData (uint8_t pbuf)
{
#ifndef USE_COOS
    if (pbuf == '\n') {
        gui_printf(" \r%c", pbuf);
    } else if (pbuf == '\r'){
        gui_printf(" \b\b \b");
    } else if (pbuf == '\t'){
        gui_printf(" \t");
    } else {
        gui_printf("%c", pbuf);
    }
    gui_printf(WHITE_BG" \b"ARESET);
#else
    event_t kbd_event;
    EVENT_INIT(&kbd_event, EVENT_KBD);

    kbd_event.ascii_code = pbuf;
    kbd_event.kbd_type = KBD_KEYDOWN;
    
    gui_send(gui_get_server(), &kbd_event);

    last_key = pbuf;
#endif
}

int16_t mouse_event[3] = {0, 0, 0};
int16_t cursor[3] = {0,0,0};

void update_cursor(int16_t dx, int16_t dy, uint8_t btn)
{
#ifndef USE_COOS
    switch (btn)
    {
        case 1:
            LCD_SetTextColor(blue);               /* set background: blue if button-left pressed          */
            break;

        case 2:
            LCD_SetTextColor(green);              /* set background: green if button-right pressed        */
            break;

        case 3:
            LCD_SetTextColor(red);                /* set background: red if both left and right pressed   */
            break;
        
        default:
            LCD_SetTextColor(white);              /* set background: white if no button pressed           */
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

    LCD_SetTextColor(black);
    LCD_DrawFullRect(cursor[0], cursor[1], 10, 10);     /* draw cursor                                          */
#else
    event_t event;

    if (cursor[2] != btn) {
        EVENT_INIT(&event, EVENT_MOUSE_BUTTON);
        if (cursor[2] > btn) {
            event.button = MOUSE_BUTTON_UP;
            event.button |= cursor[2] - btn;
        }
        else {
            event.button = MOUSE_BUTTON_DOWN;
            event.button |= btn - cursor[2];
        }
    }
    else {
        EVENT_INIT(&event, EVENT_MOUSE_MOTION);
        event.button = MOUSE_BUTTON_NONE;
    }

    event.dx = dx;
    event.dy = dy;
 
    gui_send(gui_get_server(), &event);

    cursor[0] = dx;                                     /* save this event data                                 */
    cursor[1] = dy;
    cursor[2] = btn;
#endif

    USB_Host_status = USB_HOST_MOUSE_IDLE;              /* set status back to idle                              */
}

void  USR_MOUSE_Init (void)
{
    gui_printf("[Usb Host] Connect to a mouse.\r\n");

    cursor[0] = 115;
    cursor[1] = 155;
    cursor[2] = 0;
    update_cursor(0, 0, 0);                     /* set cursor position in middle of screen              */

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

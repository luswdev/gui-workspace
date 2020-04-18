/**
  ******************************************************************************
  * @file    usbh_hid_keybd.c 
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    19-March-2012
  * @brief   This file is the application layer for USB Host HID Keyboard handling
  *          QWERTY and AZERTY Keyboard are supported as per the selection in 
  *          usbh_hid_keybd.h              
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
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
#include "usbh_hid_keybd.h"
#include "stm32f429i_discovery_lcd.h"
#include "cogui.h"
#include "main.h"

/** @addtogroup USBH_LIB
* @{
*/

/** @addtogroup USBH_CLASS
* @{
*/

/** @addtogroup USBH_HID_CLASS
* @{
*/

/** @defgroup USBH_HID_KEYBD 
* @brief    This file includes HID Layer Handlers for USB Host HID class.
* @{
*/ 

/** @defgroup USBH_HID_KEYBD_Private_TypesDefinitions
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBH_HID_KEYBD_Private_Defines
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBH_HID_KEYBD_Private_Macros
* @{
*/ 
/**
* @}
*/ 

/** @defgroup USBH_HID_KEYBD_Private_FunctionPrototypes
* @{
*/ 
static void  KEYBRD_Init (void);
static void  KEYBRD_Decode(uint8_t *data);

/**
* @}
*/ 

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
 #if defined   (__CC_ARM) /*!< ARM Compiler */
  __align(4) 
 #elif defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4
 #elif defined (__GNUC__) /*!< GNU Compiler */
 #pragma pack(4) 
 #elif defined  (__TASKING__) /*!< TASKING Compiler */                           
  __align(4) 
 #endif /* __CC_ARM */
#endif
 
/** @defgroup USBH_HID_KEYBD_Private_Variables
* @{
*/
HID_cb_TypeDef HID_KEYBRD_cb= 
{
  KEYBRD_Init,
  KEYBRD_Decode
};

/*
*******************************************************************************
*                                             LOCAL CONSTANTS
*******************************************************************************
*/

static  const  uint8_t  HID_KEYBRD_Codes[] = {
  0,     0,    0,    0,   31,   50,   48,   33, 
  19,   34,   35,   36,   24,   37,   38,   39,       /* 0x00 - 0x0F */
  52,    51,   25,   26,   17,   20,   32,   21,
  23,   49,   18,   47,   22,   46,    2,    3,       /* 0x10 - 0x1F */
  4,    5,    6,    7,    8,    9,   10,   11, 
  43,  110,   15,   16,   61,   12,   13,   27,       /* 0x20 - 0x2F */
  28,   29,   42,   40,   41,    1,   53,   54,  
  55,   30,  112,  113,  114,  115,  116,  117,       /* 0x30 - 0x3F */
  118,  119,  120,  121,  122,  123,  124,  125,  
  126,   75,   80,   85,   76,   81,   86,   89,       /* 0x40 - 0x4F */
  79,   84,   83,   90,   95,  100,  105,  106,
  108,   93,   98,  103,   92,   97,  102,   91,       /* 0x50 - 0x5F */
  96,  101,   99,  104,   45,  129,    0,    0, 
  0,    0,    0,    0,    0,    0,    0,    0,       /* 0x60 - 0x6F */
  0,    0,    0,    0,    0,    0,    0,    0,
  0,    0,    0,    0,    0,    0,    0,    0,       /* 0x70 - 0x7F */
  0,    0,    0,    0,    0,  107,    0,   56,
  0,    0,    0,    0,    0,    0,    0,    0,       /* 0x80 - 0x8F */
  0,    0,    0,    0,    0,    0,    0,    0,
  0,    0,    0,    0,    0,    0,    0,    0,       /* 0x90 - 0x9F */
  0,    0,    0,    0,    0,    0,    0,    0,
  0,    0,    0,    0,    0,    0,    0,    0,       /* 0xA0 - 0xAF */
  0,    0,    0,    0,    0,    0,    0,    0, 
  0,    0,    0,    0,    0,    0,    0,    0,       /* 0xB0 - 0xBF */
  0,    0,    0,    0,    0,    0,    0,    0, 
  0,    0,    0,    0,    0,    0,    0,    0,       /* 0xC0 - 0xCF */
  0,    0,    0,    0,    0,    0,    0,    0, 
  0,    0,    0,    0,    0,    0,    0,    0,       /* 0xD0 - 0xDF */
  58,   44,   60,  127,   64,   57,   62,  128        /* 0xE0 - 0xE7 */
};

#ifdef QWERTY_KEYBOARD
static  const  int8_t  HID_KEYBRD_Key[] = {
  '\0',  '`',  '1',  '2',  '3',  '4',  '5',  '6',
  '7',  '8',  '9',  '0',  '-',  '=',  '\0', '\r',
  '\t',  'q',  'w',  'e',  'r',  't',  'y',  'u', 
  'i',  'o',  'p',  '[',  ']',  '\\',
  '\0',  'a',  's',  'd',  'f',  'g',  'h',  'j',  
  'k',  'l',  ';',  '\'', '\0', '\n',
  '\0',  '\0', 'z',  'x',  'c',  'v',  'b',  'n', 
  'm',  ',',  '.',  '/',  '\0', '\0',
  '\0',  '\0', '\0', ' ',  '\0', '\0', '\0', '\0', 
  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
  '\0',  '\0', '\0', '\0', '\0', '\r', '\0', '\0', 
  '\0', '\0', '\0', '\0', '\0', '\0',
  '\0',  '\0', '7',  '4',  '1',
  '\0',  '/',  '8',  '5',  '2',
  '0',   '*',  '9',  '6',  '3',
  '.',   '-',  '+',  '\0', '\n', '\0', '\0', '\0', '\0', '\0', '\0',
  '\0',  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', 
  '\0', '\0', '\0', '\0'
};

static  const  int8_t  HID_KEYBRD_ShiftKey[] = {
  '\0', '~',  '!',  '@',  '#',  '$',  '%',  '^',  '&',  '*',  '(',  ')',
  '_',  '+',  '\0', '\0', '\0', 'Q',  'W',  'E',  'R',  'T',  'Y',  'U', 
  'I',  'O',  'P',  '{',  '}',  '|',  '\0', 'A',  'S',  'D',  'F',  'G', 
  'H',  'J',  'K',  'L',  ':',  '"',  '\0', '\n', '\0', '\0', 'Z',  'X',  
  'C',  'V',  'B',  'N',  'M',  '<',  '>',  '?',  '\0', '\0',  '\0', '\0',
  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', 
  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', 
  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
  '\0', '\0', '\0', '\0', '\0', '\0', '\0',    '\0', '\0', '\0', '\0', '\0',
  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'
};

#else

static  const  int8_t  HID_KEYBRD_Key[] = {
  '\0',  '`',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0',
  '-',  '=',  '\0', '\r', '\t',  'a',  'z',  'e',  'r',  't',  'y',  'u', 
  'i',  'o',  'p',  '[',  ']', '\\', '\0',  'q',  's',  'd',  'f',  'g', 
  'h',  'j',  'k',  'l',  'm',  '\0', '\0', '\n', '\0',  '\0', 'w',  'x', 
  'c',  'v',  'b',  'n',  ',',  ';',  ':',  '!',  '\0', '\0', '\0',  '\0', 
  '\0', ' ',  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', 
  '\0', '\0', '\0', '\0',  '\0', '\0', '\0', '\0', '\r', '\0', '\0', '\0', 
  '\0', '\0', '\0', '\0', '\0', '\0',  '\0', '7',  '4',  '1','\0',  '/', 
  '8',  '5',  '2', '0',   '*',  '9',  '6',  '3', '.',   '-',  '+',  '\0', 
  '\n', '\0', '\0', '\0', '\0', '\0', '\0','\0',  '\0', '\0', '\0', '\0', '\0',
  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'
};

static  const  int8_t  HID_KEYBRD_ShiftKey[] = {
  '\0', '~',  '!',  '@',  '#',  '$',  '%',  '^',  '&',  '*',  '(',  ')',  '_',
  '+',  '\0', '\0', '\0', 'A',  'Z',  'E',  'R',  'T',  'Y',  'U',  'I',  'O',
  'P',  '{',  '}',  '*', '\0', 'Q',  'S',  'D',  'F',  'G',  'H',  'J',  'K', 
  'L',  'M',  '%',  '\0', '\n', '\0', '\0', 'W',  'X',  'C',  'V',  'B',  'N',
  '?',  '.',  '/',  '\0',  '\0', '\0','\0', '\0', '\0', '\0', '\0', '\0', '\0', 
  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', 
  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'
};
#endif

/**
* @}
*/ 

static const uint8_t HID_KEYBRD_Key_Gui[] = {
    KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,   KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,     KBD_KEY_LOWER_A,    KBD_KEY_LOWER_B,      KBD_KEY_LOWER_C,   KBD_KEY_LOWER_D,
    KBD_KEY_LOWER_E,   KBD_KEY_LOWER_F,  KBD_KEY_LOWER_G,   KBD_KEY_LOWER_H,    KBD_KEY_LOWER_I,    KBD_KEY_LOWER_J,      KBD_KEY_LOWER_K,   KBD_KEY_LOWER_L,
    KBD_KEY_LOWER_M,   KBD_KEY_LOWER_N,  KBD_KEY_LOWER_O,   KBD_KEY_LOWER_P,    KBD_KEY_LOWER_Q,    KBD_KEY_LOWER_R,      KBD_KEY_LOWER_S,   KBD_KEY_LOWER_T,
    KBD_KEY_LOWER_U,   KBD_KEY_LOWER_V,  KBD_KEY_LOWER_W,   KBD_KEY_LOWER_X,    KBD_KEY_LOWER_Y,    KBD_KEY_LOWER_Z,      KBD_KEY_1,         KBD_KEY_2,
    KBD_KEY_3,         KBD_KEY_4,        KBD_KEY_5,         KBD_KEY_6,          KBD_KEY_7,          KBD_KEY_8,            KBD_KEY_9,         KBD_KEY_0,
    KBD_KEY_ENTER,     KBD_KEY_ESC,      KBD_KEY_BACKSPACE, KBD_KEY_TAB,        KBD_KEY_SPACE,      KBD_KEY_MINUS,        KBD_KEY_PLUS,      KBD_KEY_LBRACKET,
    KBD_KEY_RBRACKET,  KBD_KEY_UNKNOW,   KBD_KEY_BACKSLASH, KBD_KEY_SEMICOLON,  KBD_KEY_QUOTE,      KBD_KEY_BACKQUOTE,    KBD_KEY_COMMA,      KBD_KEY_PERIOD,
    KBD_KEY_SLASH,     KBD_KEY_CAPSLOCK, KBD_KEY_F1,        KBD_KEY_F2,         KBD_KEY_F3,         KBD_KEY_F4,           KBD_KEY_F5,        KBD_KEY_F6,
    KBD_KEY_F7,        KBD_KEY_F8,       KBD_KEY_F9,        KBD_KEY_F10,        KBD_KEY_F11,        KBD_KEY_F12,          KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,
    KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,   KBD_KEY_HOME,      KBD_KEY_PAGEUP,     KBD_KEY_DEL,        KBD_KEY_END,          KBD_KEY_PAGEDOWN,  KBD_KEY_RIGHT,
    KBD_KEY_LEFT,      KBD_KEY_DOWN,     KBD_KEY_UP,        KBD_KEY_NUMLOCK,    KBD_KEY_NUM_DIVIDE, KBD_KEY_NUM_MULTIPLY, KBD_KEY_NUM_MINUS, KBD_KEY_NUM_PLUS,
    KBD_KEY_NUM_ENTER, KBD_KEY_NUM_1,    KBD_KEY_NUM_2,     KBD_KEY_NUM_3,      KBD_KEY_NUM_4,      KBD_KEY_NUM_5,        KBD_KEY_NUM_6,     KBD_KEY_NUM_7,
    KBD_KEY_NUM_8,     KBD_KEY_NUM_9,    KBD_KEY_NUM_0,     KBD_KEY_NUM_PERIOD, KBD_KEY_UNKNOW,     KBD_KEY_UNKNOW,       KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,
    KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,   KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,     KBD_KEY_UNKNOW,     KBD_KEY_UNKNOW,       KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,
    KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,   KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,     KBD_KEY_UNKNOW,     KBD_KEY_UNKNOW,       KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,
    KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,   KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,     KBD_KEY_UNKNOW,     KBD_KEY_UNKNOW,       KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,
    KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW
};

static const uint8_t HID_KEYBRD_ShiftKey_Gui[] = {
    KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,   KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,     KBD_KEY_UPPER_A,    KBD_KEY_UPPER_B,      KBD_KEY_UPPER_C,   KBD_KEY_UPPER_D,
    KBD_KEY_UPPER_E,   KBD_KEY_UPPER_F,  KBD_KEY_UPPER_G,   KBD_KEY_UPPER_H,    KBD_KEY_UPPER_I,    KBD_KEY_UPPER_J,      KBD_KEY_UPPER_K,   KBD_KEY_UPPER_L,
    KBD_KEY_UPPER_M,   KBD_KEY_UPPER_N,  KBD_KEY_UPPER_O,   KBD_KEY_UPPER_P,    KBD_KEY_UPPER_Q,    KBD_KEY_UPPER_R,      KBD_KEY_UPPER_S,   KBD_KEY_UPPER_T,
    KBD_KEY_UPPER_U,   KBD_KEY_UPPER_V,  KBD_KEY_UPPER_W,   KBD_KEY_UPPER_X,    KBD_KEY_UPPER_Y,    KBD_KEY_UPPER_Z,      KBD_KEY_EXCLAIM,   KBD_KEY_AT,
    KBD_KEY_HASH,      KBD_KEY_DOLLAR,   KBD_KEY_PERCENT,   KBD_KEY_CARET,      KBD_KEY_AMPERSAND,  KBD_KEY_ASTERISK,     KBD_KEY_LPAREN,    KBD_KEY_RPAREN,
    KBD_KEY_ENTER,     KBD_KEY_ESC,      KBD_KEY_BACKSPACE, KBD_KEY_TAB,        KBD_KEY_SPACE,      KBD_KEY_UNDERSCORE,   KBD_KEY_EQUAL,     KBD_KEY_LCBRACKET,
    KBD_KEY_RCBRACKET, KBD_KEY_UNKNOW,   KBD_KEY_PIPE,      KBD_KEY_COLON,      KBD_KEY_QUOTEDBL,   KBD_KEY_TILDE,        KBD_KEY_LESS,      KBD_KEY_GREATER,
    KBD_KEY_SLASH,     KBD_KEY_CAPSLOCK, KBD_KEY_F1,        KBD_KEY_F2,         KBD_KEY_F3,         KBD_KEY_F4,           KBD_KEY_F5,        KBD_KEY_F6,
    KBD_KEY_F7,        KBD_KEY_F8,       KBD_KEY_F9,        KBD_KEY_F10,        KBD_KEY_F11,        KBD_KEY_F12,          KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,
    KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,   KBD_KEY_HOME,      KBD_KEY_PAGEUP,     KBD_KEY_DEL,        KBD_KEY_END,          KBD_KEY_PAGEDOWN,  KBD_KEY_RIGHT,
    KBD_KEY_LEFT,      KBD_KEY_DOWN,     KBD_KEY_UP,        KBD_KEY_NUMLOCK,    KBD_KEY_NUM_DIVIDE, KBD_KEY_NUM_MULTIPLY, KBD_KEY_NUM_MINUS, KBD_KEY_NUM_PLUS,
    KBD_KEY_NUM_ENTER, KBD_KEY_NUM_1,    KBD_KEY_NUM_2,     KBD_KEY_NUM_3,      KBD_KEY_NUM_4,      KBD_KEY_NUM_5,        KBD_KEY_NUM_6,     KBD_KEY_NUM_7,
    KBD_KEY_NUM_8,     KBD_KEY_NUM_9,    KBD_KEY_NUM_0,     KBD_KEY_NUM_PERIOD, KBD_KEY_UNKNOW,     KBD_KEY_UNKNOW,       KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,
    KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,   KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,     KBD_KEY_UNKNOW,     KBD_KEY_UNKNOW,       KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,
    KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,   KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,     KBD_KEY_UNKNOW,     KBD_KEY_UNKNOW,       KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,
    KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,   KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,     KBD_KEY_UNKNOW,     KBD_KEY_UNKNOW,       KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW,
    KBD_KEY_UNKNOW,    KBD_KEY_UNKNOW
};

static const int8_t HID_KEYBRD_KP_Key[] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', '.', '/', '*', '-', '+', '\n'
};

/** @defgroup USBH_HID_KEYBD_Private_Functions
* @{
*/ 


/**
* @brief  KEYBRD_Init.
*         Initialize the keyboard function.
* @param  None
* @retval None
*/
static void  KEYBRD_Init (void)
{
  /* Call User Init*/
  USR_KEYBRD_Init();
}

static uint8_t keys_last_gui;

/**
* @brief  KEYBRD_ProcessData.
*         The function is to decode the pressed keys.
* @param  pbuf : Pointer to the HID IN report data buffer
* @retval None
*/

bool_t num_lock = Co_FALSE;
bool_t caps_lock = Co_FALSE;

static void KEYBRD_Decode(uint8_t *pbuf)
{
#ifndef USE_COOS
  static  uint8_t   shift;
  static  uint8_t   keys[KBR_MAX_NBR_PRESSED];
  static  uint8_t   keys_new[KBR_MAX_NBR_PRESSED];
  static  uint8_t   keys_last[KBR_MAX_NBR_PRESSED];
  static  uint8_t   key_newest;
  static  uint8_t   nbr_keys;
  static  uint8_t   nbr_keys_new;
  static  uint8_t   nbr_keys_last;
  uint8_t   ix;
  uint8_t   jx;
  uint8_t   error;
  uint8_t   output;            
  
  nbr_keys      = 0;
  nbr_keys_new  = 0;
  nbr_keys_last = 0;
  key_newest    = 0x00;
  
  /* Check if Shift key is pressed */                                                                         
  if ((pbuf[0] == KBD_LEFT_SHIFT) || (pbuf[0] == KBD_RIGHT_SHIFT)) {
    shift = TRUE;
  } else {
    shift = FALSE;
  }
  
  error = FALSE;
  
  /* Check for the value of pressed key */
  for (ix = 2; ix < 2 + KBR_MAX_NBR_PRESSED; ix++) {                       
    if ((pbuf[ix] == 0x01) ||
        (pbuf[ix] == 0x02) ||
          (pbuf[ix] == 0x03)) {
            error = TRUE;
          }
  }
  
  if (error == TRUE) {
    return;
  }
  
  nbr_keys     = 0;
  nbr_keys_new = 0;
  for (ix = 2; ix < 2 + KBR_MAX_NBR_PRESSED; ix++) {
    if (pbuf[ix] != 0) {
      keys[nbr_keys] = pbuf[ix];                                       
      nbr_keys++;
      for (jx = 0; jx < nbr_keys_last; jx++) {                         
        if (pbuf[ix] == keys_last[jx]) {
          break;
        }
      }
      
      if (jx == nbr_keys_last) {
        keys_new[nbr_keys_new] = pbuf[ix];
        nbr_keys_new++;
      }
    }
  }
  
  if (nbr_keys_new == 1) {
    key_newest = keys_new[0];
    
    if (shift == TRUE) {
      output =  HID_KEYBRD_ShiftKey[HID_KEYBRD_Codes[key_newest]];
    } else {
      output =  HID_KEYBRD_Key[HID_KEYBRD_Codes[key_newest]];
    }
    
    /* call user process handle */
    USR_KEYBRD_ProcessData(output);
  } else {
    key_newest = 0x00;
  }
  
  
  nbr_keys_last  = nbr_keys;
  for (ix = 0; ix < KBR_MAX_NBR_PRESSED; ix++) {
    keys_last[ix] = keys[ix];
  }
#else
    uint8_t ix;
    uint8_t key_num = 0;
    bool_t error = Co_FALSE;
    event_t event;
    EVENT_INIT(&event, EVENT_KBD);

    /* set modify keys */
    event.mod = 0;
    if (pbuf[0] & KBD_LEFT_CTRL) {
        event.mod |= KBD_MOD_LCTRL;
    }
    if (pbuf[0] & KBD_RIGHT_CTRL) {
        event.mod |= KBD_MOD_RCTRL;
    }
    if (pbuf[0] & KBD_LEFT_SHIFT) {
        event.mod |= KBD_MOD_LSHIFT;
    }
    if (pbuf[0] & KBD_RIGHT_SHIFT) {
        event.mod |= KBD_MOD_RSHIFT;
    }
    if (pbuf[0] & KBD_LEFT_ALT) {
        event.mod |= KBD_MOD_LALT;
    }
    if (pbuf[0] & KBD_RIGHT_ALT) {
        event.mod |= KBD_MOD_RALT;
    }
    if (pbuf[0] & KBD_LEFT_GUI) {
        event.mod |= KBD_MOD_LSUPER;
    }
    if (pbuf[0] & KBD_RIGHT_GUI) {
        event.mod |= KBD_MOD_RSUPER;
    }

    for (ix=2; ix<KBR_MAX_NBR_PRESSED; ix++) {
        if (pbuf[ix] == 0) {
            break;
        }
        key_num++;
    }
    
    /* if there has key(s) pressed */
    if (key_num != 0) {
        if (pbuf[2] == 57) {
            GUI_TOGGLE_BOOL(caps_lock);
            STM_EVAL_LEDToggle(LED3);
        } else if (pbuf[2] == 83) {
            GUI_TOGGLE_BOOL(num_lock);
            STM_EVAL_LEDToggle(LED4);
        }/* we just process first key */
        else if (KBD_HID_IS_SHIFT(pbuf[0])) {
            event.key = HID_KEYBRD_ShiftKey_Gui[pbuf[2]];
        } else {
            event.key = HID_KEYBRD_Key_Gui[pbuf[2]];
        }
        event.kbd_type = KBD_KEYDOWN;

        if (caps_lock) {
            event.mod |= KBD_MOD_CAPS;
        }

        if (num_lock) {
            event.mod |= KBD_MOD_NUM;
        }
    } else {
        event.key = keys_last_gui;
        event.kbd_type = KBD_KEYUP;
    }

    if (event.kbd_type == KBD_KEYDOWN) {
        if (event.key == KBD_KEY_UNKNOW) {
            GUI_TOGGLE_BOOL(error);
        } else if (event.key < KBD_KEY_DEL) {
            event.ascii_code = event.key;
        } else if (event.key < KBD_KEY_NUM_ENTER) {     /* if KP */
            event.ascii_code = HID_KEYBRD_KP_Key[event.key - KBD_KEY_NUM_0];
        } else {
            event.ascii_code = '\0';
        }
    } else {
        event.ascii_code = '\0';
    }

    /* save this event key value */
    keys_last_gui = event.key;

    if (error) {
        return;
    } else {
        gui_send(gui_get_server(), &event);
    }
#endif
}

/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/

/**
* @}
*/


/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


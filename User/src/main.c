/**
 *******************************************************************************
 * @file       main.c
 * @version    V0.1.1
 * @date       2020.01.19
 * @brief      Main file.
 *******************************************************************************
 */ 

#include <cogui.h>
#include <stm32f429i_discovery_lcd.h>
#include <user_config.h>
#include <LCD.h>
#include "usbh_core.h"
#include "usbh_usr.h"
#include "usbh_hid_core.h"
#include "main.h"
#include "app_wgt.h"
#include "app_font.h"

#ifdef USE_COOS
OS_STK init_stk[TASK_STK_SIZE];
OS_STK gui_init_stk[TASK_STK_SIZE];

#ifdef USE_GUI
OS_STK app_wgt_stk[TASK_STK_SIZE];
OS_STK app_font_stk[TASK_STK_SIZE];

#ifdef USE_USB
OS_STK usb_stk[TASK_STK_SIZE];
#endif 

struct graphic_driver_ops ops = {
    LCD_SetPixel,
    LCD_GetPixel,
    LCD_DrawHLine,
    LCD_DrawVLine
};

cogui_graphic_driver_t driver_ops = {
    GRAPHIC_PIXEL_FORMAT_RGB565,
    240,
    320,
    LCD_FRAME_BUFFER + BUFFER_OFFSET,
    &ops,
    Co_NULL
};
#endif
#endif

#ifdef USE_USB
void usb_p(void *par)
{
    for(;;){
        USBH_Process(&USB_OTG_Core, &USB_Host);
    }
}    
#endif

void gui_init(void *par)
{
	stm_print_string(GRN_BOLD"[OK]"ARESET"\r\n");
    	
#ifdef USE_GUI
	cogui_printf("[User] Set up grahpic driver...");
	cogui_set_graphic_driver(&driver_ops);
	cogui_printf(GRN_BOLD"[OK]\r\n"ARESET);
	
	cogui_system_init();
	
	cogui_printf("[User] Create a task \"app_wgt\"...");
	CoCreateTask(app_wgt, (void *)0, 25, &app_wgt_stk[TASK_STK_SIZE-1], TASK_STK_SIZE);
	cogui_printf(GRN_BOLD"[OK]\r\n"ARESET);
	
	
	cogui_printf("[User] Create a task \"app_font\"...");
	CoCreateTask(app_font, (void *)0, 30, &app_font_stk[TASK_STK_SIZE-1], TASK_STK_SIZE);
	cogui_printf(GRN_BOLD"[OK]\r\n"ARESET);

#ifdef USE_USB
    cogui_printf("[User] Create a task \"USB\"...");
	CoCreateTask(usb_p, (void *)0, 35, &usb_stk[TASK_STK_SIZE-1], TASK_STK_SIZE);
	cogui_printf(GRN_BOLD"[OK]\r\n"ARESET);
#endif

	CoExitTask();
#endif
}

int main(void)
{
    UART_Init();
    stm_print_string("\n\n\n\r\n");

#ifndef USE_COOS
    stm_print_string("[System] Initial system tick ...");
    Delay_Init();
	stm_print_string(GRN_BOLD"[OK]"ARESET"\r\n");
#endif

    stm_print_string("[System] Initial LED GPIO setting ...");
    LED_Init();
	stm_print_string(GRN_BOLD"[OK]"ARESET"\r\n");

    stm_print_string("[System] Initial user btn setting ...");
	STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
	stm_print_string(GRN_BOLD"[OK]"ARESET"\r\n");

    stm_print_string("[System] Initial LCD setting ...");
	LCD_Config();
	stm_print_string(GRN_BOLD"[OK]"ARESET"\r\n");

    stm_print_string("[System] Enable Layer1 ...");
    LTDC_LayerCmd(LTDC_Layer1, ENABLE);
	stm_print_string(GRN_BOLD"[OK]"ARESET"\r\n");

    stm_print_string("[System] Reload configuration of Layer1 ...");
    LTDC_ReloadConfig(LTDC_IMReload);
	stm_print_string(GRN_BOLD"[OK]"ARESET"\r\n");

    stm_print_string("[System] Enable The LCD ...");
    LTDC_Cmd(ENABLE);
	stm_print_string(GRN_BOLD"[OK]"ARESET"\r\n");

    stm_print_string("[System] Testing The LCD ...");
    LCD_SetLayer(LCD_BACKGROUND_LAYER);
	LCD_Clear(white);
	LCD_SetLayer(LCD_FOREGROUND_LAYER);
	stm_print_string(GRN_BOLD"[OK]"ARESET"\r\n");

#ifdef USE_USB
    stm_print_string("[System] Initial USB OTG ...");
    USBH_Init(&USB_OTG_Core, 
#ifdef USE_USB_OTG_FS
            USB_OTG_FS_CORE_ID,
#else
            USB_OTG_HS_CORE_ID,
#endif
            &USB_Host,
            &HID_cb, 
            &USBH_USR_cb);
	stm_print_string(GRN_BOLD"[OK]"ARESET"\r\n");
#endif

#ifdef USE_COOS
	stm_print_string("[System] Initial CoOS ...");
    CoInitOS();
	stm_print_string(GRN_BOLD"[OK]"ARESET"\r\n");

	stm_print_string("[System] Create a task \"gui_init\" ...");
	CoCreateTask(gui_init, (void*)0, 10, &init_stk[TASK_STK_SIZE-1], TASK_STK_SIZE);
	stm_print_string(GRN_BOLD"[OK]"ARESET"\r\n");

	stm_print_string("[System] Start CoOS ...");
    CoStartOS();

	stm_print_string(RED_BOLD"[System] System initial failed!\r\n"ARESET);
#endif

    STM_EVAL_LEDOff(LED3|LED4);
    
	for(;;){
#ifdef USE_USB
        USBH_Process(&USB_OTG_Core, &USB_Host);
#endif
    }
}

#ifndef USE_COOS
/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}
#endif

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
        cogui_printf("[System] Wrong parameters value: file %s on line %d\r\n", file, line);
    }
}
#endif

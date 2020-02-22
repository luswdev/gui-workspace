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

#ifdef USE_COOS
OS_STK init_stk[TASK_STK_SIZE];
OS_STK gui_init_stk[TASK_STK_SIZE];

#ifdef USE_GUI
OS_STK app_a_stk[TASK_STK_SIZE];
OS_STK app_b_stk[TASK_STK_SIZE];

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
    Co_NULL,
    &ops,
    Co_NULL
};
#endif
#endif

void wait_user_btn(char *s)
{
    co_uint16_t len;
	len = cogui_printf(">>>>> Press the USER BUTTON to %s <<<<<\r", s);

	 /* Wait for User push-button is pressed */
    while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_RESET) {
    }
    
    /* Wait for User push-button is released */
    while (STM_EVAL_PBGetState(BUTTON_USER) != Bit_SET) {
    }

    /* clear ouput buffer with space */
    while (len --){
        cogui_printf(" ");
    }
    cogui_printf(" \r");
    
}

#ifdef USE_GUI
void app_a_paint()
{
	cogui_printf("[%10s] Paint start.\r\n", cogui_app_self()->name);

	cogui_window_t *win = cogui_window_create_with_title();
	
	cogui_window_show(win);
	
	cogui_widget_t *test_wgt1 = cogui_widget_create(win);
	cogui_widget_set_rectangle(test_wgt1, 100, 100, 100, 100);
	test_wgt1->gc.background = COGUI_RED;
	
	test_wgt1->flag |= COGUI_WIDGET_FLAG_RECT | COGUI_WIDGET_FLAG_FILLED;

    cogui_widget_set_text(test_wgt1, "test");
	
	print_result(cogui_widget_show(test_wgt1), "create widget 1");
	
	cogui_printf("[%10s] Paint ok.\r\n", cogui_app_self()->name);
}

StatusType a_handler(struct cogui_event *event)
{
	COGUI_ASSERT(event != Co_NULL);

	switch (event->type)
    {
	case COGUI_EVENT_PAINT:
		 app_a_paint();
		break;
				
	default:
		return Co_FALSE;
				
	}
		
	return Co_TRUE;
}

void app_a(void *par) 
{
	cogui_app_t *app = cogui_app_create("app test");
	if (app == Co_NULL) {
		return;
	}
	
	app->optional_handler = a_handler; 

	cogui_app_run(app);
	cogui_app_delete(app);
	
	CoExitTask();
}

void app_b_paint()
{
	cogui_window_t *win = cogui_window_create_with_title();
	
	cogui_window_show(win);

    /* widget 1: top-left */
    cogui_widget_t *font_ex = cogui_widget_create(win);
	cogui_widget_set_rectangle(font_ex, 5, 45, 230, 50);
	font_ex->gc.background = COGUI_BLUE;
	font_ex->flag |= COGUI_WIDGET_FLAG_RECT | COGUI_WIDGET_FLAG_FILLED;
    font_ex->gc.padding = COGUI_PADDING_SIMPLE(5);
    cogui_widget_set_text_align(font_ex, COGUI_TEXT_ALIGN_LEFT|COGUI_TEXT_ALIGN_TOP);
    cogui_widget_set_text(font_ex, "Text align left-top");
	print_result(cogui_widget_show(font_ex), "create left-top example");

    /* widget 2: middle-center */
    font_ex = cogui_widget_create(win);
	cogui_widget_set_rectangle(font_ex, 5, 100, 230, 50);
	font_ex->gc.background = COGUI_RED;
	font_ex->flag |= COGUI_WIDGET_FLAG_RECT | COGUI_WIDGET_FLAG_FILLED;
    font_ex->gc.padding = COGUI_PADDING_SIMPLE(5);
    cogui_widget_set_text_align(font_ex, COGUI_TEXT_ALIGN_CENTER|COGUI_TEXT_ALIGN_MIDDLE);
    cogui_widget_set_text(font_ex, "Text align middle-center");
	print_result(cogui_widget_show(font_ex), "create middle-center example");

    /* widget 3: bottom-right */
    font_ex = cogui_widget_create(win);
	cogui_widget_set_rectangle(font_ex, 5, 155, 230, 50);
	font_ex->gc.background = COGUI_GREEN;
	font_ex->gc.foreground = COGUI_DARK_GRAY;
	font_ex->flag |= COGUI_WIDGET_FLAG_RECT | COGUI_WIDGET_FLAG_FILLED;
    font_ex->gc.padding = COGUI_PADDING_SIMPLE(5);
    cogui_widget_set_text_align(font_ex, COGUI_TEXT_ALIGN_BOTTOM|COGUI_TEXT_ALIGN_RIGHT);
    cogui_widget_set_text(font_ex, "Text align bottom-right");
	print_result(cogui_widget_show(font_ex), "create bottom-right example");

    /* widget 4: font size 11x18 */
    font_ex = cogui_widget_create(win);
	cogui_widget_set_rectangle(font_ex, 5, 210, 230, 50);
	font_ex->gc.background = COGUI_YELLOW;
	font_ex->gc.foreground = COGUI_DARK_GRAY;
	font_ex->flag |= COGUI_WIDGET_FLAG_RECT | COGUI_WIDGET_FLAG_FILLED;
    font_ex->gc.padding = COGUI_PADDING_SIMPLE(15);
    cogui_widget_set_font(font_ex, &tm_font_11x18);
    cogui_widget_set_text_align(font_ex, COGUI_TEXT_ALIGN_CENTER|COGUI_TEXT_ALIGN_MIDDLE);
    cogui_widget_set_text(font_ex, "11x18");
	print_result(cogui_widget_show(font_ex), "create 11x18 example");

    /* widget 5: font size 16x26 */
    font_ex = cogui_widget_create(win);
	cogui_widget_set_rectangle(font_ex, 5, 265, 230, 50);
	font_ex->gc.background = COGUI_PURPLE;
	font_ex->flag |= COGUI_WIDGET_FLAG_RECT | COGUI_WIDGET_FLAG_FILLED;
    font_ex->gc.padding = COGUI_PADDING_SIMPLE(10);
    cogui_widget_set_font(font_ex, &tm_font_16x26);
    cogui_widget_set_text_align(font_ex, COGUI_TEXT_ALIGN_CENTER|COGUI_TEXT_ALIGN_MIDDLE);
    cogui_widget_set_text(font_ex, "16x26");
	print_result(cogui_widget_show(font_ex), "create 16x26 example");

	cogui_printf("[%10s] Paint ok.\r\n", cogui_app_self()->name);
}

StatusType b_handler(struct cogui_event *event)
{
	COGUI_ASSERT(event != Co_NULL);

	switch (event->type)
    {
	case COGUI_EVENT_PAINT:
		 app_b_paint();
		break;
				
	default:
		return Co_FALSE;
				
	}
		
	return Co_TRUE;
}

void app_b(void *par) 
{
	cogui_app_t *app = cogui_app_create("Font Ex.");
	if (app == Co_NULL) {
		return;
	}
	
	app->optional_handler = b_handler; 
	cogui_app_run(app);
	cogui_app_delete(app);
	
	CoExitTask();
}
#endif

#ifdef USE_USB
void usb_p(void *par)
{
    cogui_printf("[usb] start procress.\r\n");
    for(;;){
        USBH_Process(&USB_OTG_Core, &USB_Host);
        //CoTickDelay(10);
    }
}    
#endif

void gui_init(void *par)
{
	stm_print_string(GRN_BOLD"\t[OK]\t\t"ARESET"│\r\n");
    stm_print_string("\t│\t\t\t\t\t\t\t\t\t\t│\r\n\t╰───────────────────────────────────────────────────────────────────────────────╯\r\n\r\n");
    	
#ifdef USE_GUI
	cogui_printf("[%10s] Set up grahpic driver...", "User");
	cogui_set_graphic_driver(&driver_ops);
	cogui_printf(GRN_BOLD"\t[OK]\r\n"ARESET);
	
	cogui_system_init();
	
	cogui_printf("[%10s] Create a task \"app a\"...", "User");
	CoCreateTask(app_a, (void *)0, 25, &app_a_stk[TASK_STK_SIZE-1], TASK_STK_SIZE);
	cogui_printf(GRN_BOLD"\t[OK]\r\n"ARESET);
	
	
	cogui_printf("[%10s] Create a task \"app b\"...", "User");
	CoCreateTask(app_b, (void *)0, 30, &app_b_stk[TASK_STK_SIZE-1], TASK_STK_SIZE);
	cogui_printf(GRN_BOLD"\t[OK]\r\n"ARESET);

#ifdef USE_USB
    cogui_printf("[%10s] Create a task \"USB\"...", "User");
	CoCreateTask(usb_p, (void *)0, 35, &usb_stk[TASK_STK_SIZE-1], TASK_STK_SIZE);
	cogui_printf(GRN_BOLD"\t[OK]\r\n"ARESET);
#endif

	CoExitTask();
#endif
}

int main(void)
{
    UART_Init();

    stm_print_string("\r\n\r\n\t╭───────────────────────────────────────────────────────────────────────────────╮\r\n\t│\t\t\t\t\t\t\t\t\t\t│\r\n");
    stm_print_string("\t│\t\t\tSTM32F429 Discovery Board Initial\t\t\t│\r\n");
    stm_print_string("\t│\t\t      ─────────────────────────────────────\t\t\t│\r\n\t│\t\t\t\t\t\t\t\t\t\t│\r\n");

#ifndef USE_COOS
    stm_print_string("\t│\t\t[sys] Initial system tick ..............");
    Delay_Init();
	stm_print_string(GRN_BOLD"\t[OK]\t\t"ARESET"│\r\n");
#endif

    stm_print_string("\t│\t\t[sys] Initial LED GPIO setting .........");
    LED_Init();
	stm_print_string(GRN_BOLD"\t[OK]\t\t"ARESET"│\r\n");

    stm_print_string("\t│\t\t[sys] Initial user btn setting .........");
	STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
	stm_print_string(GRN_BOLD"\t[OK]\t\t"ARESET"│\r\n");

    stm_print_string("\t│\t\t[sys] Initial LCD setting ..............");
	LCD_Config();
	stm_print_string(GRN_BOLD"\t[OK]\t\t"ARESET"│\r\n");

    stm_print_string("\t│\t\t[sys] Enable Layer1 ....................");
    LTDC_LayerCmd(LTDC_Layer1, ENABLE);
	stm_print_string(GRN_BOLD"\t[OK]\t\t"ARESET"│\r\n");

    stm_print_string("\t│\t\t[sys] Reload configuration of Layer1 ...");
    LTDC_ReloadConfig(LTDC_IMReload);
	stm_print_string(GRN_BOLD"\t[OK]\t\t"ARESET"│\r\n");

    stm_print_string("\t│\t\t[sys] Enable The LCD ...................");
    LTDC_Cmd(ENABLE);
	stm_print_string(GRN_BOLD"\t[OK]\t\t"ARESET"│\r\n");

    stm_print_string("\t│\t\t[sys] Testing The LCD ..................");
    LCD_SetLayer(LCD_BACKGROUND_LAYER);
	LCD_Clear(COGUI_WHITE);
	LCD_SetLayer(LCD_FOREGROUND_LAYER);
	stm_print_string(GRN_BOLD"\t[OK]\t\t"ARESET"│\r\n");

#ifdef USE_USB
    stm_print_string("\t│\t\t[sys] Initial USB OTG ..................\t\t\t│\r\n");
    USBH_Init(&USB_OTG_Core, 
#ifdef USE_USB_OTG_FS
            USB_OTG_FS_CORE_ID,
#else
            USB_OTG_HS_CORE_ID,
#endif
            &USB_Host,
            &HID_cb, 
            &USBH_USR_cb);
	stm_print_string(UP_CUR GRN_BOLD"\t[OK]\t\t"ARESET"│\r\n");
	stm_print_string(GRN_BOLD"\t\t\t\t\t\t\t\t\t[OK]\t\t"ARESET"│\r\n");
#endif

#ifdef USE_COOS
	stm_print_string("\t│\t\t[sys] Initial CoOS .....................");
    CoInitOS();
	stm_print_string(GRN_BOLD"\t[OK]\t\t"ARESET"│\r\n");

	stm_print_string("\t│\t\t[sys] Create a task \"gui_init\" .........");
	CoCreateTask(gui_init, (void*)0, 10, &init_stk[TASK_STK_SIZE-1], TASK_STK_SIZE);
	stm_print_string(GRN_BOLD"\t[OK]\t\t"ARESET"│\r\n");

	stm_print_string("\t│\t\t[sys] Start CoOS .......................");
    CoStartOS();

	stm_print_string(RED_BOLD"\t│\t\t[sys] System initial failed!\r\n"ARESET);
#endif

    stm_print_string("\t│\t\t\t\t\t\t\t\t\t\t│\r\n\t╰───────────────────────────────────────────────────────────────────────────────╯\r\n\r\n");

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
        cogui_printf("Wrong parameters value: file %s on line %d\r\n", file, line);
    }
}
#endif

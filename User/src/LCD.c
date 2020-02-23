/**
 *******************************************************************************
 * @file       LCD.c
 * @version    V0.1.1
 * @date       2020.01.19
 * @brief      LCD interface for GUI engine.
 *******************************************************************************
 */ 

#include <cogui.h>
#include <stm32f429i_discovery_lcd.h>
#include <LCD.h>

void LCD_SetPixel(cogui_color_t *c, co_int32_t x, co_int32_t y)
{
	LCD_SetTextColor((co_uint16_t)*c);
	LCD_DrawLine(x, y, 1, LCD_DIR_HORIZONTAL);
}
	
void LCD_GetPixel(cogui_color_t *c, co_int32_t x, co_int32_t y) 
{
    
}

void LCD_DrawHLine(cogui_color_t *c, co_int32_t x1, co_int32_t x2, co_int32_t y)
{
	LCD_SetTextColor((co_uint16_t)*c);
	LCD_DrawLine(x1, y, x2-x1, LCD_DIR_HORIZONTAL);
}

void LCD_DrawVLine(cogui_color_t *c, co_int32_t x , co_int32_t y1, co_int32_t y2)
{
	LCD_SetTextColor((co_uint16_t)*c);
	LCD_DrawLine(x, y1, y2-y1, LCD_DIR_VERTICAL);
}

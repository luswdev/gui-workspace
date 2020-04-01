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

void LCD_SetPixel(color_t *c, int32_t x, int32_t y)
{
	LCD_SetTextColor((uint16_t)*c);
	LCD_DrawLine(x, y, 1, LCD_DIR_HORIZONTAL);
}
	
void LCD_GetPixel(color_t *c, int32_t x, int32_t y) 
{
    
}

void LCD_DrawHLine(color_t *c, int32_t x1, int32_t x2, int32_t y)
{
	LCD_SetTextColor((uint16_t)*c);
	LCD_DrawLine(x1, y, x2-x1, LCD_DIR_HORIZONTAL);
}

void LCD_DrawVLine(color_t *c, int32_t x , int32_t y1, int32_t y2)
{
	LCD_SetTextColor((uint16_t)*c);
	LCD_DrawLine(x, y1, y2-y1, LCD_DIR_VERTICAL);
}

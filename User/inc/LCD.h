/**
 *******************************************************************************
 * @file       LCD.h
 * @version    V0.0.1   
 * @date       2019.8.7
 * @brief      This is api file for dc.
 *******************************************************************************
 */ 

#ifndef __GUI_LCD_H__
#define __GUI_LCD_H__

void LCD_SetPixel(cogui_color_t *c, int32_t x, int32_t y);
void LCD_GetPixel(cogui_color_t *c, int32_t x, int32_t y);

void LCD_DrawHLine(cogui_color_t *c, int32_t x1, int32_t x2, int32_t y);
void LCD_DrawVLine(cogui_color_t *c, int32_t x , int32_t y1, int32_t y2);

#endif /* __GUI_LCD_H__ */

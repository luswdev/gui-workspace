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

void LCD_SetPixel(cogui_color_t *c, S32 x, S32 y);
void LCD_GetPixel(cogui_color_t *c, S32 x, S32 y);

void LCD_DrawHLine(cogui_color_t *c, S32 x1, S32 x2, S32 y);
void LCD_DrawVLine(cogui_color_t *c, S32 x , S32 y1, S32 y2);

#endif /* __GUI_LCD_H__ */

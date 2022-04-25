#ifndef __LCD_DISPLAY_H__
#define __LCD_DISPLAY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "stm32f7xx.h"

#define ABS(X)  ((X) > 0 ? (X) : -(X))
/* LCD 层像素格式*/
#define ARGB8888  LTDC_PIXEL_FORMAT_ARGB8888  /*!< ARGB8888 LTDC像素格式 */
#define RGB888 		LTDC_PIXEL_FORMAT_RGB888    /*!< RGB888 LTDC像素格式   */
#define RGB565 		LTDC_PIXEL_FORMAT_RGB565    /*!< RGB565 LTDC像素格式   */
#define ARGB1555 	LTDC_PIXEL_FORMAT_ARGB1555  /*!< ARGB1555 LTDC像素格式 */
#define ARGB4444 	LTDC_PIXEL_FORMAT_ARGB4444  /*!< ARGB4444 LTDC像素格式 */

typedef struct 
{ 
  uint32_t TextColor; 
  uint32_t BackColor;  
  // sFONT    *pFont;
}LCD_DrawPropTypeDef;   
   
typedef struct 
{
  int16_t X;
  int16_t Y;
}Point, * pPoint; 

/** 
  * @brief  字体对齐模式  
  */ 
typedef enum
{
  CENTER_MODE             = 0x01,    /* 居中对齐 */
  RIGHT_MODE              = 0x02,    /* 右对齐   */
  LEFT_MODE               = 0x03     /* 左对齐   */
}Text_AlignModeTypdef;

#define MAX_LAYER_NUMBER       ((uint32_t)2)

#define LTDC_ACTIVE_LAYER	     ((uint32_t)1) /* Layer 1 */


#define LCD_COLOR_BLUE          ((uint32_t)0xFF0000FF)
#define LCD_COLOR_GREEN         ((uint32_t)0xFF00FF00)
#define LCD_COLOR_RED           ((uint32_t)0xFFFF0000)
#define LCD_COLOR_CYAN          ((uint32_t)0xFF00FFFF)
#define LCD_COLOR_MAGENTA       ((uint32_t)0xFFFF00FF)
#define LCD_COLOR_YELLOW        ((uint32_t)0xFFFFFF00)
#define LCD_COLOR_LIGHTBLUE     ((uint32_t)0xFF8080FF)
#define LCD_COLOR_LIGHTGREEN    ((uint32_t)0xFF80FF80)
#define LCD_COLOR_LIGHTRED      ((uint32_t)0xFFFF8080)
#define LCD_COLOR_LIGHTCYAN     ((uint32_t)0xFF80FFFF)
#define LCD_COLOR_LIGHTMAGENTA  ((uint32_t)0xFFFF80FF)
#define LCD_COLOR_LIGHTYELLOW   ((uint32_t)0xFFFFFF80)
#define LCD_COLOR_DARKBLUE      ((uint32_t)0xFF000080)
#define LCD_COLOR_DARKGREEN     ((uint32_t)0xFF008000)
#define LCD_COLOR_DARKRED       ((uint32_t)0xFF800000)
#define LCD_COLOR_DARKCYAN      ((uint32_t)0xFF008080)
#define LCD_COLOR_DARKMAGENTA   ((uint32_t)0xFF800080)
#define LCD_COLOR_DARKYELLOW    ((uint32_t)0xFF808000)
#define LCD_COLOR_WHITE         ((uint32_t)0xFFFFFFFF)
#define LCD_COLOR_LIGHTGRAY     ((uint32_t)0xFFD3D3D3)
#define LCD_COLOR_GRAY          ((uint32_t)0xFF808080)
#define LCD_COLOR_DARKGRAY      ((uint32_t)0xFF404040)
#define LCD_COLOR_BLACK         ((uint32_t)0xFF000000)
#define LCD_COLOR_BROWN         ((uint32_t)0xFFA52A2A)
#define LCD_COLOR_ORANGE        ((uint32_t)0xFFFFA500)
#define LCD_COLOR_TRANSPARENT   ((uint32_t)0xFF000000)

uint32_t LCD_GetXSize(void);
uint32_t LCD_GetYSize(void);
void LCD_SelectLayer(uint32_t LayerIndex);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t RGB_Code);
void LCD_Clear(uint32_t Color);
void LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_SetColors(uint32_t TextColor, uint32_t BackColor);
void LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
#ifdef __cplusplus
}
#endif

#endif /* __LTDC_H__ */
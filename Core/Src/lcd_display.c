#include "lcd_display.h"
/* Default LCD configuration with LCD Layer 1 */
static uint32_t            ActiveLayer = 0;

extern LTDC_HandleTypeDef hltdc;
extern DMA2D_HandleTypeDef hdma2d;

#define Ltdc_Handler hltdc
#define Dma2d_Handler hdma2d

static void LL_FillBuffer(uint32_t LayerIndex, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex);

/**
  * @brief  获取LCD当前有效层X轴的大小
  * @retval X轴的大小
  */
uint32_t LCD_GetXSize(void)
{
  return Ltdc_Handler.LayerCfg[ActiveLayer].ImageWidth;
}

/**
  * @brief  获取LCD当前有效层Y轴的大小
  * @retval Y轴的大小
  */
uint32_t LCD_GetYSize(void)
{
  return Ltdc_Handler.LayerCfg[ActiveLayer].ImageHeight;
}

/**
  * @brief  设置LCD当前有效层Y轴的大小
  * @param  imageWidthPixels：图像宽度像素个数
  * @retval 无
  */
void LCD_SetXSize(uint32_t imageWidthPixels)
{
  Ltdc_Handler.LayerCfg[ActiveLayer].ImageWidth = imageWidthPixels;
}

/**
  * @brief  设置LCD当前有效层Y轴的大小
  * @param  imageHeightPixels：图像高度像素个数
  * @retval None
  */
void LCD_SetYSize(uint32_t imageHeightPixels)
{
  Ltdc_Handler.LayerCfg[ActiveLayer].ImageHeight = imageHeightPixels;
}

/**
  * @brief  初始化LCD层 
  * @param  LayerIndex:  前景层(层1)或者背景层(层0)
  * @param  FB_Address:  每一层显存的首地址
  * @param  PixelFormat: 层的像素格式
  * @retval 无
  */
void LCD_LayerInit(uint16_t LayerIndex, uint32_t FB_Address,uint32_t PixelFormat)
{     
  LTDC_LayerCfgTypeDef  layer_cfg;

  /* 层初始化 */
  layer_cfg.WindowX0 = 0;				//窗口起始位置X坐标
  layer_cfg.WindowX1 = LCD_GetXSize();	//窗口结束位置X坐标
  layer_cfg.WindowY0 = 0;				//窗口起始位置Y坐标
  layer_cfg.WindowY1 = LCD_GetYSize();  //窗口结束位置Y坐标
  layer_cfg.PixelFormat = PixelFormat;	//像素格式
  layer_cfg.FBStartAdress = FB_Address; //层显存首地址
  layer_cfg.Alpha = 255;				//用于混合的透明度常量，范围（0―255）0为完全透明
  layer_cfg.Alpha0 = 0;					//默认透明度常量，范围（0―255）0为完全透明
  layer_cfg.Backcolor.Blue = 0;			//层背景颜色蓝色分量
  layer_cfg.Backcolor.Green = 0;		//层背景颜色绿色分量
  layer_cfg.Backcolor.Red = 0;			//层背景颜色红色分量
  layer_cfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;//层混合系数1
  layer_cfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;//层混合系数2
  layer_cfg.ImageWidth = LCD_GetXSize();//设置图像宽度
  layer_cfg.ImageHeight = LCD_GetYSize();//设置图像高度
  
  HAL_LTDC_ConfigLayer(&Ltdc_Handler, &layer_cfg, LayerIndex); //设置选中的层参数

  
  __HAL_LTDC_RELOAD_CONFIG(&Ltdc_Handler);//重载层的配置参数
}
/**
  * @brief  选择LCD层
  * @param  LayerIndex: 前景层(层1)或者背景层(层0)
  * @retval 无
  */
void LCD_SelectLayer(uint32_t LayerIndex)
{
  ActiveLayer = LayerIndex;
} 

/**
  * @brief  设置LCD层的可视化
  * @param  LayerIndex: 前景层(层1)或者背景层(层0)
  * @param  State: 禁能或者使能
  * @retval 无
  */
void LCD_SetLayerVisible(uint32_t LayerIndex, FunctionalState State)
{
  if(State == ENABLE)
  {
    __HAL_LTDC_LAYER_ENABLE(&Ltdc_Handler, LayerIndex);
  }
  else
  {
    __HAL_LTDC_LAYER_DISABLE(&Ltdc_Handler, LayerIndex);
  }
  __HAL_LTDC_RELOAD_CONFIG(&Ltdc_Handler);
} 

/**
  * @brief  设置LCD的透明度常量
  * @param  LayerIndex: 前景层(层1)或者背景层(层0)
  * @param  Transparency: 透明度，范围（0―255）0为完全透明
  * @retval 无
  */
void LCD_SetTransparency(uint32_t LayerIndex, uint8_t Transparency)
{    
  HAL_LTDC_SetAlpha(&Ltdc_Handler, Transparency, LayerIndex);
}

/**
  * @brief  设置LCD缓冲帧的首地址 
  * @param  LayerIndex: 前景层(层1)或者背景层(层0)
  * @param  Address: LCD缓冲帧的首地址     
  * @retval 无
  */
void LCD_SetLayerAddress(uint32_t LayerIndex, uint32_t Address)
{
  HAL_LTDC_SetAddress(&Ltdc_Handler, Address, LayerIndex);
}

/**
  * @brief  设置显示窗口
  * @param  LayerIndex: 前景层(层1)或者背景层(层0)
  * @param  Xpos: LCD 	X轴起始位置
  * @param  Ypos: LCD 	Y轴起始位置
  * @param  Width: LCD  窗口大小
  * @param  Height: LCD 窗口大小  
  * @retval None
  */
void LCD_SetLayerWindow(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  /* 重新设置窗口大小 */
  HAL_LTDC_SetWindowSize(&Ltdc_Handler, Width, Height, LayerIndex);
  
  /* 重新设置窗口的起始位置 */
  HAL_LTDC_SetWindowPosition(&Ltdc_Handler, Xpos, Ypos, LayerIndex); 
}

/**
  * @brief  读LCD的像素值
  * @param  Xpos: X 轴坐标位置
  * @param  Ypos: Y 轴坐标位置
  * @retval RGB像素值
  */
uint32_t LCD_ReadPixel(uint16_t Xpos, uint16_t Ypos)
{
  uint32_t ret = 0;
  
  if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
  {
    /* 从SDRAM显存中读取颜色数据 */
    ret = *(__IO uint32_t*) (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress + (2*(Ypos*LCD_GetXSize() + Xpos)));
  }
  else if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB888)
  {
    /* 从SDRAM显存中读取颜色数据 */
    ret  = (*(__IO uint8_t*) (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress + (3*(Ypos*LCD_GetXSize() + Xpos))+2) & 0x00FFFFFF);
	ret |= (*(__IO uint8_t*) (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress + (3*(Ypos*LCD_GetXSize() + Xpos))+1) & 0x00FFFFFF);
	ret |= (*(__IO uint8_t*) (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress + (3*(Ypos*LCD_GetXSize() + Xpos))) & 0x00FFFFFF);
  }
  else if((Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565) || \
          (Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444) || \
          (Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_AL88))  
  {
    /* 从SDRAM显存中读取颜色数据 */
    ret = *(__IO uint16_t*) (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress + (2*(Ypos*LCD_GetXSize() + Xpos)));    
  }
  else
  {
    /* 从SDRAM显存中读取颜色数据 */
    ret = *(__IO uint8_t*) (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress + (2*(Ypos*LCD_GetXSize() + Xpos)));    
  }
  
  return ret;
}

/**
  * @brief  LCD当前层清屏
  * @param  Color: 背景颜色
  * @retval None
  */
void LCD_Clear(uint32_t Color)
{ 
  /* 清屏 */ 
  LL_FillBuffer(ActiveLayer, (uint32_t *)(Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress), LCD_GetXSize(), LCD_GetYSize(), 0, Color);
}
//=====================================================================================
/**
  * @brief  使能显示
  * @retval 无
  */
void LCD_DisplayOn(void)
{
  /* 开显示 */
  __HAL_LTDC_ENABLE(&Ltdc_Handler);
  HAL_GPIO_WritePin(LTDC_BL_GPIO_PIN_GPIO_Port, LTDC_BL_GPIO_PIN_Pin, GPIO_PIN_SET);  /* 开背光*/
}

/**
  * @brief  禁能显示
  * @retval 无
  */
void LCD_DisplayOff(void)
{
  /* 关显示 */
  __HAL_LTDC_DISABLE(&Ltdc_Handler);
  HAL_GPIO_WritePin(LTDC_BL_GPIO_PIN_GPIO_Port, LTDC_BL_GPIO_PIN_Pin, GPIO_PIN_RESET);/*关背光*/
}

/**
  * @brief  绘制一个点
  * @param  Xpos:   X轴坐标
  * @param  Ypos:   Y轴坐标
  * @param  RGB_Code: 像素颜色值
  * @retval 无
  */
void LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t RGB_Code)
{

  if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
  {
    *(__IO uint32_t*) (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress + (4*(Ypos*LCD_GetXSize() + Xpos))) = RGB_Code;
  }
  else if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB888)
  {
    *(__IO uint8_t*) (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress + (3*(Ypos*LCD_GetXSize() + Xpos))+2) = 0xFF&(RGB_Code>>16);
	*(__IO uint8_t*) (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress + (3*(Ypos*LCD_GetXSize() + Xpos))+1) = 0xFF&(RGB_Code>>8);
	*(__IO uint8_t*) (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress + (3*(Ypos*LCD_GetXSize() + Xpos))) = 0xFF&RGB_Code;
  }
  else if((Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565) || \
          (Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444) || \
          (Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_AL88))  
  {
    *(__IO uint16_t*) (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress + (2*(Ypos*LCD_GetXSize() + Xpos))) = (uint16_t)RGB_Code;   
  }
  else
  {
    *(__IO uint16_t*) (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress + (2*(Ypos*LCD_GetXSize() + Xpos))) = (uint16_t)RGB_Code;   
  }

}

/**
  * @brief  填充一个缓冲区
  * @param  LayerIndex: 当前层
  * @param  pDst: 指向目标缓冲区指针
  * @param  xSize: 缓冲区宽度
  * @param  ySize: 缓冲区高度
  * @param  OffLine: 偏移量
  * @param  ColorIndex: 当前颜色
  * @retval None
  */
static void LL_FillBuffer(uint32_t LayerIndex, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex) 
{

  Dma2d_Handler.Init.Mode         = DMA2D_R2M;
  if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565)
  { 
    Dma2d_Handler.Init.ColorMode    = DMA2D_RGB565;
  }
  else if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
  { 
    Dma2d_Handler.Init.ColorMode    = DMA2D_ARGB8888;
  }
  else if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB888)
  { 
    Dma2d_Handler.Init.ColorMode    = DMA2D_RGB888;
  }
  else if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB1555)
  { 
    Dma2d_Handler.Init.ColorMode    = DMA2D_ARGB1555;
  }
  else if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444)
  { 
    Dma2d_Handler.Init.ColorMode    = DMA2D_ARGB4444;
  }
  Dma2d_Handler.Init.OutputOffset = OffLine;      
  
  Dma2d_Handler.Instance = DMA2D;
  
  /* DMA2D 初始化 */
  if(HAL_DMA2D_Init(&Dma2d_Handler) == HAL_OK) 
  {
    if(HAL_DMA2D_ConfigLayer(&Dma2d_Handler, LayerIndex) == HAL_OK) 
    {
      if (HAL_DMA2D_Start(&Dma2d_Handler, ColorIndex, (uint32_t)pDst, xSize, ySize) == HAL_OK)
      {
        /* DMA轮询传输 */  
        HAL_DMA2D_PollForTransfer(&Dma2d_Handler, 100);
      }
    }
  } 
}

/**
  * @brief  转换一行为ARGB8888像素格式
  * @param  pSrc: 指向源缓冲区的指针
  * @param  pDst: 输出颜色
  * @param  xSize: 缓冲区宽度
  * @param  ColorMode: 输入颜色模式   
  * @retval 无
  */
static void LL_ConvertLineToARGB8888(void *pSrc, void *pDst, uint32_t xSize, uint32_t ColorMode)
{    
  /* 配置DMA2D模式,颜色模式和输出偏移 */
  Dma2d_Handler.Init.Mode         = DMA2D_M2M_PFC;
  Dma2d_Handler.Init.ColorMode    = DMA2D_ARGB8888;
  Dma2d_Handler.Init.OutputOffset = 0;     
  
  /* Foreground Configuration */
  Dma2d_Handler.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  Dma2d_Handler.LayerCfg[1].InputAlpha = 0xFF;
  Dma2d_Handler.LayerCfg[1].InputColorMode = ColorMode;
  Dma2d_Handler.LayerCfg[1].InputOffset = 0;
  
  Dma2d_Handler.Instance = DMA2D; 
  
  /* DMA2D 初始化 */
  if(HAL_DMA2D_Init(&Dma2d_Handler) == HAL_OK) 
  {
    if(HAL_DMA2D_ConfigLayer(&Dma2d_Handler, 1) == HAL_OK) 
    {
      if (HAL_DMA2D_Start(&Dma2d_Handler, (uint32_t)pSrc, (uint32_t)pDst, xSize, 1) == HAL_OK)
      {
        /* DMA轮询传输*/  
        HAL_DMA2D_PollForTransfer(&Dma2d_Handler, 10);
      }
    }
  } 
}
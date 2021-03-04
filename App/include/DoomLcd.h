/*
 * DoomLcd.h
 *
 *  Created on: 2021. febr. 17.
 *      Author: Marci
 */

#ifndef INCLUDE_DOOMLCD_H_
#define INCLUDE_DOOMLCD_H_

#include <stdint.h>

#define LCD_OK                 ((uint8_t)0x00)
#define LCD_ERROR              ((uint8_t)0x01)
#define LCD_TIMEOUT            ((uint8_t)0x02)

/**
  * @brief LCD special pins
  */
/* Display enable pin */
#define LCD_DISP_PIN                    GPIO_PIN_12
#define LCD_DISP_GPIO_PORT              GPIOI
#define LCD_DISP_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOI_CLK_ENABLE()
#define LCD_DISP_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOI_CLK_DISABLE()

/* Backlight control pin */
#define LCD_BL_CTRL_PIN                  GPIO_PIN_3
#define LCD_BL_CTRL_GPIO_PORT            GPIOK
#define LCD_BL_CTRL_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOK_CLK_ENABLE()
#define LCD_BL_CTRL_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOK_CLK_DISABLE()

#define LCD_LayerCfgTypeDef    LTDC_LayerCfgTypeDef

uint8_t DoomLcd_Init(void);
uint32_t DoomLcd_GetXSize(void);
uint32_t DoomLcd_GetYSize(void);
void DoomLcd_Init_LayerDefaultInit(uint16_t LayerIndex, uint32_t FB_Address, uint32_t *palette, uint32_t paletteSize);
void DoomLcd_DisplayOn(void);
void DoomLcd_SelectLayer(uint32_t LayerIndex);
void DoomLcd_UpdatePalette(uint32_t *palette, uint32_t paletteSize);
void DoomLcd_Clear(uint8_t paletteColor);
uint8_t* DoomLcd_GetBase(void);

#endif /* INCLUDE_DOOMLCD_H_ */

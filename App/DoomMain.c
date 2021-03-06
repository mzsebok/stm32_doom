#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_qspi.h"
#include "stm32746g_discovery_sdram.h"
#include "usart.h"
#include "systime.h"
#include <string.h>
#include "DoomMain.h"
#include "DoomLcd.h"
#include "palette256.h"
#include "fatfs.h"
#include "ff.h"
#include "usbh_hid.h"

#include "doomdef.h"
#include "d_main.h"

LTDC_HandleTypeDef hltdc;
DMA2D_HandleTypeDef hdma2d;
osThreadId DoomMainHandle;

/**
  * @brief  LCD configuration
  * @param  None
  * @retval None
  */
static void LCD_Config(void)
{
    /* LCD Initialization */
    DoomLcd_Init();

    /* LCD Initialization */
    DoomLcd_Init_LayerDefaultInit(0, LCD_FB_START_ADDRESS, palette256, 256);

    /* Enable the LCD */
    DoomLcd_DisplayOn();

    /* Select the LCD Background Layer  */
    DoomLcd_SelectLayer(0);

    /* Clear the Background Layer */
    DoomLcd_Clear(0);
}

void USBH_HID_EventCallback(USBH_HandleTypeDef *phost)
{
    if (USBH_HID_GetDeviceType(phost) == HID_MOUSE)
    {
        HID_MOUSE_Info_TypeDef *mouseInfo;
        mouseInfo = USBH_HID_GetMouseInfo(phost);
    }

    if (USBH_HID_GetDeviceType(phost) == HID_KEYBOARD)
    {
        HID_KEYBD_Info_TypeDef *keyboardInfo;
        keyboardInfo = USBH_HID_GetKeybdInfo(phost);
        char key = USBH_HID_GetASCIICode(keyboardInfo);
        if (key == 'w')
            DoomLcd_PutPixel(20, 20, 252);
        if (key == 'a')
                    DoomLcd_PutPixel(20, 40, 252);
        if (key == 's')
                    DoomLcd_PutPixel(20, 60, 252);
        if (key == 'd')
                    DoomLcd_PutPixel(20, 80, 252);
    }
}

#if defined(__GNUC__)
int _write(int fd, char * ptr, int len)
{
  HAL_UART_Transmit(&huart1, (uint8_t *) ptr, len, HAL_MAX_DELAY);
  return len;
}
#elif defined (__ICCARM__)
#include "LowLevelIOInterface.h"
size_t __write(int handle, const unsigned char * buffer, size_t size)
{
  HAL_UART_Transmit(&huart1, (uint8_t *) buffer, size, HAL_MAX_DELAY);
  return size;
}
#elif defined (__CC_ARM)
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
#endif

void DoomMain(void const * argument)
{
    systime_init();

    /* Configure LED1 */
    BSP_LED_Init(LED1);

    MX_FATFS_Mount();

    LCD_Config();

    uint32_t now = systime_get();

    osThreadDef(D_DoomMain, D_DoomMain, osPriorityNormal, 0, 512);
    DoomMainHandle = osThreadCreate(osThread(D_DoomMain), NULL);

    while(1)
    {
        if (systime_get() - now >= 1)
        {
            now = systime_get();
            BSP_LED_Toggle(LED1);
        }
        osDelay(500);
    }
}

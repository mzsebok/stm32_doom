#include <string.h>
#include "cmsis_os.h"
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_lcd.h"
#include "systime.h"
#include "fatfs.h"
#include "DoomLcd.h"
#include "DoomKey.h"
#include "palette256.h"
#include "d_main.h"
#include "DoomMain.h"

osThreadId          DoomMainHandle;     // Thread for doom application

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

void DoomMain(void const * argument)
{
    systime_init();

    /* Configure LED1 */
    BSP_LED_Init(LED1);

    MX_FATFS_Mount();

    LCD_Config();

    init_doomKey();

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

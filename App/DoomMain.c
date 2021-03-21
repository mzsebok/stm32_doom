#include <string.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_audio.h"
#include "systime.h"
#include "fatfs.h"
#include "DoomLcd.h"
#include "DoomKey.h"
#include "palette256.h"
#include "d_main.h"
#include "DoomMain.h"
#include <stdbool.h>

osThreadId          DoomMainHandle;     // Thread for doom application
osThreadDef(D_DoomMain, D_DoomMain, osPriorityNormal, 0, 4096);
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

void sram_init(void)
{
    uint32_t *p = 0xC07C0000;
    for (uint32_t i = 0u; i < 256*1024/4; i++)
    {
        *(p++) = 0u;
    }
}

void DoomMain(void const * argument)
{
    bool isStarted = false;
    systime_init();

    sram_init();

    /* Configure LED1 */
    BSP_LED_Init(LED1);

    MX_FATFS_Mount();

    LCD_Config();

    init_doomKey();

    uint32_t now = systime_get();

    BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE, 128, AUDIO_FREQUENCY_11K);
//    BSP_AUDIO_OUT_Play((uint16_t*)&Sine550hz[0],sizeof(Sine550hz));

    printf("..............................\r\n");
    printf("Press ENTER to start the game!\r\n");

    while(1)
    {
        doomKeyEvent_t keyEvent;

        if (get_doomKeyEvent(&keyEvent))
        {
            if (   keyEvent.type == doomKey_Up
                && keyEvent.code == 13          // enter
                && isStarted == false)
            {
                DoomMainHandle = osThreadCreate(osThread(D_DoomMain), NULL);
                isStarted = true;
            }
        }

        if (systime_get() - now >= 1)
        {
            now = systime_get();
            BSP_LED_Toggle(LED1);
        }
        osDelay(500);
    }
}

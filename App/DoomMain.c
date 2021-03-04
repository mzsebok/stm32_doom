#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_lcd.h"
#include "spiffs.h"
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
//#include "usbh_hid.h"

#include "doomdef.h"
#include "d_main.h"

LTDC_HandleTypeDef hltdc;
DMA2D_HandleTypeDef hdma2d;

//#define SDRAM_BANK_ADDR                 ((uint32_t)0xC0000000)

#define LOGICAL_PAGE_SIZE       256
//static spiffs mFileSystem;
//static uint8_t mSpiffsWorkBuffer[LOGICAL_PAGE_SIZE * 2];
//static uint8_t mSpiffsFileDescriptorSize[32 * 4];
//static uint8_t spiffsCacheBuffer[(LOGICAL_PAGE_SIZE + 32) * 4];

//static int32_t flashRead(uint32_t address, uint32_t size, uint8_t *dst)
//{
//    int32_t result = 0;
//    if (BSP_QSPI_Read(dst, address, size) != 0)
//    {
//        result = SPIFFS_ERR_HAL;
//    }
//    return result;
//}
//
//static int32_t flashWrite(uint32_t address, uint32_t size, uint8_t *dst)
//{
//    int32_t result = 0;
//    if (BSP_QSPI_Write(dst, address, size) != 0)
//    {
//        result = SPIFFS_ERR_HAL;
//    }
//    return result;
//}
//
//static int32_t flashEraseBlock(uint32_t address, uint32_t size)
//{
//    int32_t result = 0;
//    if (size <= N25Q128A_SECTOR_SIZE)
//    {
//        if (BSP_QSPI_Erase_Block(address) != 0)
//        {
//            result = SPIFFS_ERR_HAL;
//        }
//    }
//    else
//    {
//        result = SPIFFS_ERR_HAL;
//    }
//    return result;
//}

//static void mountFileSystem()
//{
//    spiffs_config cfg;
//    cfg.phys_size = N25Q128A_FLASH_SIZE;            // Flash size
//    cfg.phys_addr = 0;                              // start spiffs at start of spi flash
//    cfg.phys_erase_block = N25Q128A_SECTOR_SIZE;    // erase block size
//    cfg.log_block_size = N25Q128A_SECTOR_SIZE;      // same as block size
//    cfg.log_page_size = LOGICAL_PAGE_SIZE;          // logical page size
//
//    cfg.hal_read_f = (spiffs_read)flashRead;
//    cfg.hal_write_f = (spiffs_write)flashWrite;
//    cfg.hal_erase_f = (spiffs_erase)flashEraseBlock;
//
//    int res = SPIFFS_mount(&mFileSystem,
//                           &cfg,
//                           mSpiffsWorkBuffer,
//                           mSpiffsFileDescriptorSize,
//                           sizeof(mSpiffsFileDescriptorSize),
//                           spiffsCacheBuffer,
//                           sizeof(spiffsCacheBuffer),
//                           0);
//
//    if (res != SPIFFS_OK)
//    {
//        SPIFFS_format(&mFileSystem);
//    }
//
//}

//static void test_spiffs()
//{
//    char buf[12];
//
//
//    spiffs_file fd = SPIFFS_open(&mFileSystem, "my_file", SPIFFS_CREAT | SPIFFS_TRUNC | SPIFFS_RDWR, 0);
//    SPIFFS_write(&mFileSystem, fd, (u8_t *)"Hello world", 12);
//    SPIFFS_close(&mFileSystem, fd);
//
//    fd = SPIFFS_open(&mFileSystem, "my_file", SPIFFS_RDWR, 0);
//    SPIFFS_read(&mFileSystem, fd, (u8_t *)buf, 12);
//    SPIFFS_close(&mFileSystem, fd);
//}

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

uint32_t OpenReadFile(const char* fileName)
{
    FIL F1;
    uint32_t bytesRead;
    uint8_t buffer[100];

    if (f_open(&F1, (TCHAR const*)fileName, FA_READ) != FR_OK)
    {
        while(1)
        {
        }
    }
    if (f_read (&F1, buffer, 100, (UINT *)&bytesRead) != FR_OK)
    {
        while(1)
        {
        }
    }

    return 1;
}

//void USBH_HID_EventCallback(USBH_HandleTypeDef *phost)
//{
//    if (USBH_HID_GetDeviceType(phost) == HID_MOUSE)
//    {
//        HID_MOUSE_Info_TypeDef *mouseInfo;
//        mouseInfo = USBH_HID_GetMouseInfo(phost);
//    }
//
//    if (USBH_HID_GetDeviceType(phost) == HID_KEYBOARD)
//    {
//        HID_KEYBD_Info_TypeDef *keyboardInfo;
//        keyboardInfo = USBH_HID_GetKeybdInfo(phost);
//        char key = USBH_HID_GetASCIICode(keyboardInfo);
//        DoomLcd_PutPixel(20, 20, 252);
//    }
//}

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
//    OpenReadFile("/file.txt");

//    uint8_t *sdram = SDRAM_BANK_ADDR;

//    LCD_Config();
//
//    BSP_LCD_Clear(LCD_COLOR_BLACK);
//
//    BSP_LCD_SetTextColor(LCD_COLOR_RED);
//    BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"Hello World!");
//
//    BSP_QSPI_Init();
//    mountFileSystem();
//    test_spiffs();

    LCD_Config();

    uint8_t text[] = "Hello World!!\r\n";
    uint32_t now = systime_get();

//    D_DoomMain();

    while(1)
    {
        if (systime_get() - now >= 1)
        {
            now = systime_get();
            BSP_LED_Toggle(LED1);
        }
//        HAL_UART_Transmit(&huart1, text, strlen((char*)text), 100);
        osDelay(500);
    }
}

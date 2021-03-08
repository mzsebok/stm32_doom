/*
 * DoomKey.c
 */

#include <stdbool.h>
#include "usbh_hid.h"
#include "cmsis_os.h"
#include "DoomKey.h"

#define DKEY_RSHIFT  (0x80+0x36)
#define DKEY_RCTRL   (0x80+0x1d)
#define DKEY_RALT    (0x80+0x38)

#define DKEY_LALT    DKEY_RALT

osMailQId keyEventQID;   // Que for key events
osMailQDef(DoomKeyQueue, 10, doomKeyEvent_t);

static bool isInitialized = false;
static HID_KEYBD_Info_TypeDef prevKeyboardInfo;

static  const  uint8_t  HID_SCAN_Codes[] =
{
  0,    0,    0,    0,    31,   50,   48,   33,
  19,   34,   35,   36,   24,   37,   38,   39,     /* 0x00 - 0x0F */

  52,   51,   25,   26,   17,   20,   32,   21,
  23,   49,   18,   47,   22,   46,   2,    3,      /* 0x10 - 0x1F */

  4,    5,    6,    7,    8,    9,    10,   11,
  13,   27,   127,  9,    61,   0x2d, 0x3d, 27,     /* 0x20 - 0x2F */

  28,   29,   42,   40,   41,   1,    53,   54,
  55,   30,   0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 0xc0,   /* 0x30 - 0x3F */

  0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 124,  125,
  0xff, 75,   80,   85,   76,   81,   86,   0xae,   /* 0x40 - 0x4F */

  0xac, 0xaf, 0xad, 90,   95,   100,  105,  106,
  108,  93,   98,   103,  92,   97,   102,  91,     /* 0x50 - 0x5F */

  96,   101,  99,   104,  45,   129,  0,    0,
  0,    0,    0,    0,    0,    0,    0,    0,      /* 0x60 - 0x6F */

  0,    0,    0,    0,    0,    0,    0,    0,
  0,    0,    0,    0,    0,    0,    0,    0,      /* 0x70 - 0x7F */

  0,    0,    0,    0,    0,    107,  0,    56,
  0,    0,    0,    0,    0,    0,    0,    0,      /* 0x80 - 0x8F */

  0,    0,    0,    0,    0,    0,    0,    0,
  0,    0,    0,    0,    0,    0,    0,    0,      /* 0x90 - 0x9F */

  0,    0,    0,    0,    0,    0,    0,    0,
  0,    0,    0,    0,    0,    0,    0,    0,      /* 0xA0 - 0xAF */

  0,    0,    0,    0,    0,    0,    0,    0,
  0,    0,    0,    0,    0,    0,    0,    0,      /* 0xB0 - 0xBF */

  0,    0,    0,    0,    0,    0,    0,    0,
  0,    0,    0,    0,    0,    0,    0,    0,      /* 0xC0 - 0xCF */

  0,    0,    0,    0,    0,    0,    0,    0,
  0,    0,    0,    0,    0,    0,    0,    0,      /* 0xD0 - 0xDF */

  58,   44,   60,   127,  64,   57,   62,   128     /* 0xE0 - 0xE7 */
};

static void sendScanCode(doomKeyEvent_t event)
{
    doomKeyEvent_t *data;
    data = (doomKeyEvent_t*) osMailAlloc(keyEventQID, osWaitForever);

    data->type = event.type;
    data->code = event.code;

    osMailPut(keyEventQID, data);
}

void USBH_HID_EventCallback(USBH_HandleTypeDef *phost)
{
    HID_KEYBD_Info_TypeDef *keyboardInfo;
    HID_MOUSE_Info_TypeDef *mouseInfo;

    if (USBH_HID_GetDeviceType(phost) == HID_MOUSE)
    {
        mouseInfo = USBH_HID_GetMouseInfo(phost);
    }

    if (USBH_HID_GetDeviceType(phost) == HID_KEYBOARD)
    {
        keyboardInfo = USBH_HID_GetKeybdInfo(phost);
    }

    if (isInitialized && keyboardInfo != NULL)
    {
        doomKeyEvent_t keyEvent;

        if (keyboardInfo->lalt != prevKeyboardInfo.lalt)
        {
            keyEvent.type = keyboardInfo->lalt ? doomKey_Down : doomKey_Up;
            keyEvent.code = DKEY_LALT;
            sendScanCode(keyEvent);
        }

        if (keyboardInfo->ralt != prevKeyboardInfo.ralt)
        {
            keyEvent.type = keyboardInfo->lalt ? doomKey_Down : doomKey_Up;
            keyEvent.code = DKEY_RALT;
            sendScanCode(keyEvent);
        }

        if (keyboardInfo->rctrl != prevKeyboardInfo.rctrl)
        {
            keyEvent.type = keyboardInfo->rctrl ? doomKey_Down : doomKey_Up;
            keyEvent.code = DKEY_RCTRL;
            sendScanCode(keyEvent);
        }

        if (keyboardInfo->rshift != prevKeyboardInfo.rshift)
        {
            keyEvent.type = keyboardInfo->rshift ? doomKey_Down : doomKey_Up;
            keyEvent.code = DKEY_RSHIFT;
            sendScanCode(keyEvent);
        }

        // search for key down events
        for (uint8_t i = 0u; i < sizeof(keyboardInfo->keys); i++)
        {
            bool found = false;
            for (uint8_t j = 0u; j < sizeof(prevKeyboardInfo.keys); j++)
            {
                if (keyboardInfo->keys[i] != 0 && keyboardInfo->keys[i] == prevKeyboardInfo.keys[j])
                {
                    // key found
                    found = true;
                    break;
                }
            }

            if (keyboardInfo->keys[i] != 0 && !found)
            {
                keyEvent.type = doomKey_Down;
                keyEvent.code = HID_SCAN_Codes[keyboardInfo->keys[i]];
                sendScanCode(keyEvent);
            }
        }

        // search for key up events
        for (uint8_t i = 0u; i < sizeof(prevKeyboardInfo.keys); i++)
        {
            bool found = false;
            for (uint8_t j = 0u; j < sizeof(keyboardInfo->keys); j++)
            {
                if (prevKeyboardInfo.keys[i] != 0 && prevKeyboardInfo.keys[i] == keyboardInfo->keys[j])
                {
                    // key found
                    found = true;
                    break;
                }
            }

            if (prevKeyboardInfo.keys[i] != 0 && !found)
            {
                keyEvent.type = doomKey_Up;
                keyEvent.code = HID_SCAN_Codes[prevKeyboardInfo.keys[i]];
                sendScanCode(keyEvent);
            }
        }

        prevKeyboardInfo = *keyboardInfo;
    }
}

void init_doomKey(void)
{
    if (!isInitialized)
    {
        keyEventQID = osMailCreate(osMailQ(DoomKeyQueue), NULL);
        isInitialized = true;
    }
}

uint8_t get_doomKeyEvent(doomKeyEvent_t *keyEvent)
{
    osEvent event = osMailGet(keyEventQID, 0);
    if (event.status == osEventMail)
    {
        memcpy(keyEvent, event.value.p, sizeof(doomKeyEvent_t));
        osMailFree(keyEventQID, event.value.p);
        return 1;
    }
    return 0;
}

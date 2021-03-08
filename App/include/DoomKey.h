/*
 * DoomKey.h
 */

#ifndef INCLUDE_DOOMKEY_H_
#define INCLUDE_DOOMKEY_H_

typedef enum {
    doomKey_Down = 0,
    doomKey_Up
} doomKeyState_t;

typedef struct {
    doomKeyState_t type;       // key state
    uint16_t code;             // key scan code
} doomKeyEvent_t;

uint8_t get_doomKeyEvent(doomKeyEvent_t *keyEvent);

#endif /* INCLUDE_DOOMKEY_H_ */

// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// $Log:$
//
// DESCRIPTION:
//	DOOM graphics stuff
//
//-----------------------------------------------------------------------------

#include <stdlib.h>

#include "DoomLcd.h"

#include "doomstat.h"
#include "i_system.h"
#include "v_video.h"
#include "m_argv.h"
#include "d_main.h"
#include "doomdef.h"

void I_ShutdownGraphics(void)
{
    // Do nothing
}

//
// I_StartFrame
//
void I_StartFrame (void)
{
    // er?

}

void I_GetEvent(void)
{
    // mouse/key events
}


//
// I_StartTic
//
void I_StartTic (void)
{
    I_GetEvent();
}


//
// I_UpdateNoBlit
//
void I_UpdateNoBlit (void)
{
    // what is this?
}

//
// I_FinishUpdate
//
void I_FinishUpdate (void)
{
    // send data to display
    for (uint32_t i = 0; i < SCREENHEIGHT; i++)
        memcpy (DoomLcd_GetBase()+i*480, screens[0]+i*SCREENWIDTH, SCREENWIDTH);
}


//
// I_ReadScreen
//
void I_ReadScreen (byte* scr)
{
    memcpy (scr, screens[0], SCREENWIDTH*SCREENHEIGHT);
}

//
// I_SetPalette
//
void I_SetPalette (byte* palette)
{
    uint32_t i;
    uint32_t c;
    uint32_t lcdPalette[256];

    // set the palette entries
    for (i=0 ; i<256 ; i++)
    {
        // RED
        c = gammatable[usegamma][*palette++];
        lcdPalette[i] = c << 16;
        // GREEN
        c = gammatable[usegamma][*palette++];
        lcdPalette[i] += c << 8;
        //BLUE
        c = gammatable[usegamma][*palette++];
        lcdPalette[i] += c;
    }

    DoomLcd_UpdatePalette(lcdPalette, 256);
}

void I_InitGraphics(void)
{
    usegamma = 1;
}

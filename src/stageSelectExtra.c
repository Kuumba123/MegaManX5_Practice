#include <common.h>
#include "practice.h"

/*
0x374 bytes til sector is crossed in ST0D Overlay.
 Also 0x4BC til sector is crossed in ST0D SPRT Arangment data
*/

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);


static const char * stageNames[] = {"SIGMA 1" , "SIGMA 2", "SIGMA 3" , "SIGMA 4", "Intro" , "Dynamo-1","Dynamo-2"};
static const char * finalStageText[] = {"Start","POST-Refights"};
static const char * halfText[] = {"Start","Mid"};

#define Cursor gameP->refights[2]

void DrawSelectableStages(Game * gameP)
{
    if (gameP->mode4 == 0)
    {
        DrawDebugText(12,18,2, stageNames[Cursor]);
    }else{
        if (gameP->stageId == 0xC)
        {
            DrawDebugText(12,18,2, finalStageText[Cursor]);
        }else{
            DrawDebugText(12,18,2, halfText[Cursor]);
        }
    }
    
}

void ProcessStagePicker(Game * gameP)
{
    if ((buttonsPressed & PAD_LEFT) != 0)
    {
        Cursor -= 1;
    }
    else if ((buttonsPressed & PAD_RIGHT) != 0)
    {
        Cursor += 1;
    }

    if (gameP->mode4 == 0)
    {
        if ((int8_t)Cursor < 0)
        {
            Cursor = 6;
        }
        else if (Cursor >= 7)
        {
            Cursor = 0;
        }
    }else{
        Cursor &= 1;
    }
}

#undef Cursor

static const char* routeText[] = {"ALL STAGES","ANY%"};

void DrawRoutePage()
{
    if (practice.cancelXA == 0)
    {
        *((int8_t*)0x80017878) = 0x73;
    }else{
        *((int8_t*)0x80017878) = 0x00;
    }
    
    DrawDebugText(3,4 + game.mode4,1,">");
    DrawDebugText(4,4,0,"ROUTE\nKEEP RNG\nUA-Refights\nCancel XA\nAnalog");


    DrawDebugText(20,4, 0, "%s\n%d\n%d\n%d\n%d",routeText[practice.route],practice.keepRng,practice.ultimateArmor,practice.cancelXA,practice.analog);
}
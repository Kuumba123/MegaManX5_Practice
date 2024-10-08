#include <common.h>

#define Cursor gameP->refights[2]

void DrawSelectableStages();

static uint8_t stagePickTable[] = {0x10 , 0x11 , 0x12 , 0xC , 0, 9 , 0xA};

void StageSelectPick(Game * gameP)
{
    if (gameP->stageId != 0xC) //8 Maverick Level Selected
    {
        gameP->mode3 = 6;
        return;
    }

    int8_t cursorPast = Cursor;
    
    if ((buttonsPressed & PAD_LEFT) != 0)
    {
        Cursor -= 1;
    }else if ((buttonsPressed & PAD_RIGHT) != 0)
    {
        Cursor += 1;
    }
    
    if ((int8_t)Cursor < 0)
    {
        Cursor = 6;
    }else if (Cursor >= 7)
    {
        Cursor = 0;
    }

    if (cursorPast != Cursor)
    {
        PlaySound(5,0,0);
    }
    
    if ((buttonsPressed & (PAD_START + PAD_CROSS)) != 0)
    {
        gameP->mode3 = 6;
        gameP->stageId = stagePickTable[Cursor];
        PlaySound(5,1,0);
    }else if ((buttonsPressed & PAD_TRIANGLE) != 0) //Go Back to Maverick Select
    {
        gameP->mode3 = 8;
    }

    DrawSelectableStages();
}

#undef Cursor
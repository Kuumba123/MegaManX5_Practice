#include <common.h>
#include "practice.h"

#define Cursor gameP->refights[2]

void DrawSelectableStages(Game *gameP);
void ProcessStagePicker(Game *gameP);

extern bool LevelMidTable[];

static uint8_t stagePickTable[] = {0x10, 0x11, 0x12, 0xC, 0, 9, 0xA};

void StageSelectPick(Game *gameP)
{
    practice.skipRefights = 0;
    if (gameP->stageId != 0xC) // 8 Maverick Level Selected
    {
        if (LevelMidTable[gameP->stageId])
        {
            gameP->mode3 = 6;
            return;
        }
        else
        {
            gameP->mode4 = 1;
        }
    }
    DrawSelectableStages(gameP);

    int8_t cursorPast = Cursor;

    ProcessStagePicker(gameP);

    if (cursorPast != Cursor)
    {
        PlaySound(5, 0, 0);
    }

    if ((buttonsPressed & (PAD_START + PAD_CROSS)) != 0)
    {
        PlaySound(5, 1, 0);
        if (gameP->mode4 == 0) // Non 2-way select
        {
            gameP->stageId = stagePickTable[Cursor];

            if (gameP->stageId != 0xC)
            {
                gameP->mode3 = 6;
                gameP->mode4 = 0;
            }
            else
            {
                gameP->mode4 = 1;
                Cursor = 0;
            }
        }
        else
        {
            if (gameP->stageId == 0xC)
            {
                practice.skipRefights = Cursor;
                gameP->mode3 = 6;
                gameP->mode4 = 0;
            }else{
                gameP->mid = Cursor;
                gameP->mode3 = 6;
                gameP->mode4 = 0;
            }
        }
    }
    else if ((buttonsPressed & PAD_TRIANGLE) != 0) // Go Back to Maverick Select
    {
        gameP->mode3 = 8;
        gameP->mode4 = 0;
    }
}

#undef Cursor
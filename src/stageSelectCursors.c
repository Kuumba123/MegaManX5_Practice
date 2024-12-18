#include <common.h>
#include "practice.h"

// 0x3D4 bytes free

#define Cursor gameP->refights[2]

extern bool LevelMidTable[];

static uint8_t stagePickTable[] = {0x10, 0x11, 0x12, 0xC, 0, 9, 0xA};

bool isRevistFlag = false;

void ProcessStagePicker(Game *gameP)
{
    int8_t cursorPast = Cursor;

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
        int8_t max = 6;
        if (practice.route == 1)
        {
            max = 4;
        }
        

        if ((int8_t)Cursor < 0)
        {
            Cursor = max;
        }
        else if (Cursor > max)
        {
            Cursor = 0;
        }
    }
    else
    {
        Cursor &= 1;
    }

    if (cursorPast != Cursor)
    {
        PlaySound(5, 0, 0);
    }

    if ((buttonsPressed & (PAD_START + PAD_CROSS)) != 0)
    {
        PlaySound(5, 1, 0);
        if (gameP->mode4 == 0 || gameP->mode4 == 2)
        {
            if (gameP->mode4 != 2)
            {
                gameP->stageId = stagePickTable[Cursor];
            }

            if (gameP->stageId != 0xC && gameP->mode4 == 0 && !(practice.route == 2 && gameP->stageId == 0x12))
            {
                gameP->mode3 = 6;
                gameP->mode4 = 0;
            }
            else if (gameP->mode4 == 0)
            {
                gameP->mode4 = 1;
                Cursor = 0;
            }
            else
            {
                if (LevelMidTable[gameP->stageId])
                {
                    isRevistFlag = Cursor;
                    gameP->mode3 = 6;
                }
                else //Start/Mid Select
                {
                    gameP->mode4 = 1;
                    Cursor = 0;
                }
            }
        }
        else // Pick Between Start/Post-Refights or Start/Mid of Maverick Stage
        {
            if (gameP->stageId == 0xC)
            {
                practice.skipRefights = Cursor;
                gameP->mode3 = 6;
                gameP->mode4 = 0;
            }
            else
            {
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

#define Cursor gameP->mode4
void ProcessConfigPicker(Game *gameP)
{
    if (Cursor == 0)
    {
        if ((buttonsPressed & PAD_RIGHT) != 0)
        {
            practice.route += 1;
        }
        else if ((buttonsPressed & PAD_LEFT) != 0)
        {
            practice.route -= 1;
        }
        practice.route &= 3;
    }
    else if (Cursor == 5)
    {
        uint8_t amount;
        if ((buttonsHeld & (PAD_L2 + PAD_R2 + PAD_SQUARE)) != 0)
        {
            amount = 10;
        }
        else
        {
            amount = 1;
        }

        if ((buttonsPressed & PAD_RIGHT) != 0)
        {
            practice.sensativity += amount;
        }
        else if ((buttonsPressed & PAD_LEFT) != 0)
        {
            practice.sensativity -= amount;
        }

        if (practice.sensativity > MAX_SENSITIVITY)
        {
            practice.sensativity = MAX_SENSITIVITY;
        }
        else if (practice.sensativity < MIN_SENSITIVITY)
        {
            practice.sensativity = MIN_SENSITIVITY;
        }
    }
    else
    {
    }
}
#undef Cursor
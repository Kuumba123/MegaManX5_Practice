#include <common.h>
#include "practice.h"

#define Cursor gameP->mode4

void CreateTitleScreenThread();

void DrawRoutePage();
void ExitRoutePage(Game *gameP);
void ProcessConfigPicker(Game *gameP);
void ExitStageSelectCheck();

void RoutePage(Game *gameP)
{
    while (true)
    {
        if ((buttonsPressed & PAD_R1) != 0)
        {
            ExitRoutePage(gameP);
            return;
        }

        if ((buttonsPressed & PAD_DOWN) != 0)
        {
            if (Cursor == 5)
            {
                Cursor = 0;
            }
            else
            {
                Cursor += 1;
            }
        }
        else if ((buttonsPressed & PAD_UP) != 0)
        {
            if (Cursor == 0)
            {
                Cursor = 5;
            }
            else
            {
                Cursor -= 1;
            }
        }
        bool toggle = (buttonsPressed & (PAD_LEFT + PAD_RIGHT + PAD_CROSS)) != 0;
        
        if (Cursor == 1)
        {
            if (toggle)
            {
                practice.keepRng ^= 1;
            }
        }
        else if (Cursor == 2)
        {
            if (toggle)
            {
                practice.ultimateArmor ^= 1;
            }
        }
        else if (Cursor == 3)
        {
            if (toggle)
            {
                practice.cancelXA ^= 1;
            }
        }
        else if (Cursor == 4)
        {
            if (toggle)
            {
                practice.analog ^= 1;
            }
        }
        else
        {
            ProcessConfigPicker(gameP);
        }
        DrawRoutePage();
        ExitStageSelectCheck();
    }
}

#undef Cursor
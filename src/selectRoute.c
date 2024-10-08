#include <common.h>
#include "practice.h"

#define Cursor gameP->mode4

void DrawRoutePage();

void RoutePage(Game *gameP)
{
    while (true)
    {
        if ((buttonsPressed & (PAD_L1 + PAD_R1)) != 0)
        {
            gameP->mode2 = 4;
            gameP->mode3 = 0;
            gameP->mode4 = 0;
            return;
        }

        if ((buttonsPressed & (PAD_LEFT + PAD_RIGHT)) != 0)
        {
            practice.route ^= 1;
        }

        DrawRoutePage();
        ThreadSleep(1);
    }
}

#undef Cursor
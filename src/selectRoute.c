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
            PlaySound(5,0,0);
            return;
        }

        if ((buttonsPressed & PAD_DOWN) != 0)
        {
            if (Cursor == 4)
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
                Cursor = 4;
            }
            else
            {
                Cursor -= 1;
            }
        }

        bool toggle = (buttonsPressed & (PAD_LEFT + PAD_RIGHT + PAD_CROSS)) != 0;

        if (Cursor == 0)
        {
            if (toggle)
            {
                practice.route ^= 1;
            }
        }
        else if (Cursor == 1)
        {
            if (toggle)
            {
                practice.keepRng ^= 1;
            }
        }else if (Cursor == 2)
        {
            if (toggle)
            {
                practice.ultimateArmor ^= 1;
            }
        }else if (Cursor == 3)
        {
            if (toggle)
            {
                practice.cancelXA ^= 1;
            }
        }else
        {
            if (toggle)
            {
                practice.analog ^= 1;
            }
            
        }
        
        

        DrawRoutePage();
        ThreadSleep(1);
    }
}

#undef Cursor
#include <common.h>
#include "practice.h"

#define Cursor gameP->refights[2]

void ProcessStagePicker(Game *gameP)
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
    }
    else
    {
        Cursor &= 1;
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
        }else if ((buttonsPressed & PAD_LEFT) != 0)
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
    }else{

    }
}
#undef Cursor
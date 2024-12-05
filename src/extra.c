#include <common.h>
#include <misc.h>

extern int8_t maxCheckPoint[];

void CheckPointCheck(Game* gameP)
{
    if ((buttonsHeld & (PAD_RIGHT + PAD_SELECT)) == (PAD_RIGHT + PAD_SELECT) && gameP->point != maxCheckPoint[gameP->stageId * 2 + gameP->mid])
    {
        gameP->point += 1;
        gameP->clear = 0xC0;
    }else if ((buttonsHeld & (PAD_LEFT + PAD_SELECT)) == (PAD_LEFT + PAD_SELECT) && gameP->point != 0)
    {
        gameP->point -= 1;
        gameP->clear = 0xC0;
    }
}
#include <common.h>
#include <object.h>

extern void (*MegaStatusStartTable[3])(Mega *);

void MegaStatus(Mega *megaP)
{
    if (*((int8_t *)((int)megaP + 0xF0)) != 0 || game.spawnFlags == 0)
    {
        return;
    }
    megaP->display = true;
    game.enableBars = true;

    uint32_t type = 0;

    switch (game.stageId)
    {
    case 5:
        type = (uint32_t)(game.point == 0) << 1;
        break;
    case 6:
        if (game.spawnFlags != 0xFF || game.point != 1 || game.mid == 0)
        {
            break;
        }
        //contines to case 9 & 0xA
    case 9:
    case 0xA:
        type = 1;
        break;
    case 0xC:
        if ((uint8_t)game.point >= 2 && (uint8_t)game.point < 0x13)
        {
            type = (uint32_t)(game.point != 10);
        }
        break;
    default:
        break;
    }
    megaP->status = 1;
    MegaStatusStartTable[type](megaP);
}
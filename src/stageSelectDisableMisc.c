#include <common.h>

static void StageSelectDisableMisc(Game * gameP)
{
    gameP->refights[2] = 0;
    gameP->mode3 += 1;
}
#include <common.h>
#include <misc.h>

void LoadMissionClearStage(Game *gameP)
{
    gameP->mode = 0xC;
    for (size_t i = 0; i < 0x10; i++)
    {
        *(uint8_t*)((int)&gameP->slowMotion + i) = 0;
    }
    gameP->slowMotion = gameP->stageId;
    gameP->stageId = 0xF;
    gameP->mid = 0;
    gameP->point = 0;
    gameP->enableBars = false;
    gameP->enableBossBars = false;
    EndSong();
    LoadLevel();
}
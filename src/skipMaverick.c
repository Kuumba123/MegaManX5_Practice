#include <common.h>
#include <layer.h>
#include "practice.h"

void AssignWeapons();

void SkipMaverickShowcase(Game *gameP)
{
    if (fadeDirection != 0)
    {
        return;
    }
    gameP->stageTime = 0;
    gameP->enemiesDefeated = 0;
    gameP->damageTaken = 0;
    gameP->maxDamageDealt = 0;
    gameP->virusModeCount;
    gameP->armorPartsPast = gameP->armorParts;
    
    for (size_t i = 0; i < 16; i++)
    {
        gameP->refights[i] = 0;
    }
    if (gameP->stageId != 0xC)
    {
        gameP->refights[0] = 1;
    }
    
    if (practice.route == 3)
    {
        gameP->mode = 5;
    }else
    {
        gameP->mode = 7;
    }
    
    gameP->mode2 = 0;
    gameP->mode3 = 0;
    gameP->mode4 = 0;
    DefaultLayers();
    ClearAll();
    AssignWeapons();
}
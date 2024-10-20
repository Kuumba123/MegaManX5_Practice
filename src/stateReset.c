#include <common.h>

void ResetState();

void GameModeLoad(Game *gameP)
{
    gameP->point = 0;
    gameP->spawnFlags = 0;
    if (gameP->mid == 0)
    {
        gameP->stageTime = 0;
        gameP->enemiesDefeated = 0;
        gameP->damageTaken = 0;
        gameP->maxDamageDealt = 0;
        gameP->virusModeCount;
        gameP->armorPartsPast = gameP->armorParts;
    }
    gameP->mode = 9;

    for (size_t i = 0; i < 64; i++)
    {
        gameP->seenTextBoxes[0][i] = 0;
    }

    ResetState();
}
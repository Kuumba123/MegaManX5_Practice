#include <common.h>
#include <object.h>
#include <layer.h>

void EnigmaShotSucseed(Game *gameP);
void ShowEnigmaResults(Game *gameP);

static uint16_t rng;

void DetermineEngimaResults(Game * gameP)
{
    if (fadeDirection != 0)
    {
        return;
    }

    for (size_t i = 0; i < 8; i++)
    {
        gameP->refights[i];
    }

    rng = *(uint16_t*)0x80093F70;

    for (size_t i = 0; i < 8; i++)
    {
        GetRNG();
    }
    

    EnigmaShotSucseed(gameP);
    ShowEnigmaResults(gameP);
    
}
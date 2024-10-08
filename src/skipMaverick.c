#include <common.h>
#include <layer.h>

void AssignWeapons();

void SkipMaverickShowcase(Game * gameP)
{
    if (fadeDirection != 0)
    {
        return;
    }
    
    for (size_t i = 0; i < 16; i++)
    {
        gameP->refights[i] = 0;
    }
    gameP->mode = 7;
    gameP->mode2 = 0;
    gameP->mode3 = 0;
    gameP->mode4 = 0;
    DefaultLayers();
    ClearAll();
    AssignWeapons();
}
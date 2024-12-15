#include <common.h>
#include <object.h>
#include "practice.h"

#define RNG *(uint16_t *)0x80093F70

void TitleDemoEnd(char * p);
void TitleDemoPlay(char * p);

void DrawManipText()
{
    if (!practice.showEnigma)
    {
        return;
    }
    
    uint16_t pastRNG = RNG;
    uint16_t rng;
    for (size_t i = 0; i < 9; i++)
    {
        GetRNG();
    }
    rng = RNG;
    if ((rng % 64) < 4)
    {
        DrawDebugText(11,3,1,"Works");
    }
    else
    {
        DrawDebugText(11,3,2,"Fails");
    }
    RNG = pastRNG;
    DrawDebugText(4,3,0,"Enigma:");
}
void ShowDemoEnigma(char * p)
{
    if (p[0xE] != 1)
    {
        DrawManipText();
    }
    
    TitleDemoEnd(p);
}
void ShowDemoEnigma2(char * p)
{
    TitleDemoPlay(p);
    DrawManipText();
}

#undef RNG
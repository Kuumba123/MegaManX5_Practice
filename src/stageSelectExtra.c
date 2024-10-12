#include <common.h>
#include "practice.h"

/*
0x374 bytes til sector is crossed in ST0D Overlay.
 Also 0x4BC til sector is crossed in ST0D SPRT Arangment data
*/

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);

static uint8_t mavericksClearedTable[8] =
{
    8,9,0x6B,0,0,0,0,0
};

static uint8_t maverickPlayerTable[2][8] =
{
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1}
};

void AssignWeapons() //2 routes: All Stages & Any%
{
    game.armorParts = 0;
    game.armors = 0;
    game.clearedStages = 0;
    game.collectables = 0;
    game.collectables2 = 0;

    if (game.stageId == 0)  //Intro
    {
        game.player = 0;
    }else if (game.stageId <= 8) //8 Maverick Stages
    {
        game.player = maverickPlayerTable[practice.route][game.stageId - 1];
        game.clearedStages = mavericksClearedTable[game.stageId - 1];
    }else if (game.stageId == 0xC) //Final Level
    {
        game.player = 1;
        if (practice.route == 0)
        {
            game.clearedStages = 0xFF;
            game.armors = 0x10;
        }
        
    }else if (game.stageId < 0xC)
    {
        /* dynamo fight... */
    }else { //other sigma stages
        game.player = 1;
        game.clearedStages = 0x7F;
    }
    
    
    
    
}


static char * stageNames[] = {"SIGMA 1" , "SIGMA 2", "SIGMA 3" , "SIGMA 4", "Intro" , "Dynamo-1","Dynamo-2"};

void DrawSelectableStages()
{
    DrawDebugText(12,18,2, stageNames[game.refights[2]]);
}

void DrawRoutePage()
{
    DrawDebugText(3,4 + game.mode4,1,">");
    
    uint8_t clut = 0;
    if (practice.route == 0)
    {
        clut = 2;
    }
    DrawDebugText(4,4, clut, "ALL STAGES");

    clut = 0;
    if (practice.route != 0)
    {
        clut = 2;
    }
    DrawDebugText(16,4, clut, "ANY%%");
}
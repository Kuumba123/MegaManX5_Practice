#include <common.h>
#include "practice.h"

/*0x374 bytes til sector is crossed*/

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);

static uint8_t maverickPlayerTable[2][8] =
{
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1}
};

void AssignWeapons() //2 routes: All Stages & Any%
{
    game.clearedStages = 0;
    game.collectables = 0;
    game.collectables2 = 0;

    if (game.stageId == 0)  //Intro
    {
        game.player = 0;
    }else if (game.stageId <= 8) //8 Maverick Stages
    {
        game.player = maverickPlayerTable[practice.route][game.stageId - 1];
    }else if (game.stageId == 0xC) //Final Level
    {
        game.player = 0;
        if (practice.route == 0)
        {
            game.clearedStages = 0xFF;
        }
        
    }else if (game.stageId < 0xC)
    {
        /* dynamo fight... */
    }else { //other sigma stages
        game.player = 1;
        game.clearedStages = 0xFF;
    }
    
    
    
    
}


static char * stageNames[] = {"SIGMA 1" , "SIGMA 2", "SIGMA 3" , "SIGMA 4", "Intro" , "Dynamo-1","Dyanamo-2"};

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
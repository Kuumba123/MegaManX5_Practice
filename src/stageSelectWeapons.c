#include <common.h>
#include "practice.h"
//0x8BC bytes free

/*100%*/
extern bool isRevistFlag;
static uint8_t hundoMavericksClearedTable[8] = {0, 0xAD, 0x01, 0x05, 0xEF, 0x0D, 0xAF, 0x2D};
static uint8_t hundoMaverickHealthTable[8] = {32, 42, 32, 32, 48, 34, 32, 38};
static uint8_t hundoMaverickAmmoTable[8] = {48, 48, 48, 48, 48, 48, 52, 48};
static uint8_t hundoMaverickPlayerTable[8] = {1, 0, 1, 0, 0, 0, 1, 0};
static uint8_t hundoMaverickArmorTypeTable[8] = {5, 4, 5, 4, 3, 4, 5, 4};
static uint32_t hundoMaverickPartsTable[8] = {0, 0x10, 0, 0x10, 0, 0x10, 0x18, 0x10};
static uint32_t hundoMavericksCollectablesTable[8] = {0, 0xD000, 0x1000, 0, 0xF000, 0x1000, 0xF000, 0x9000};
static uint8_t hundoMavericksHoursTable[8] = {12, 6, 11, 9, 3, 8, 4, 7};
static uint8_t hundoMavericksRankTable[8] = {2, 3, 1, 5, 1, 4, 2, 4};
/**************/
static uint8_t hundoDynamoMaverickClearedTable[2] = {5, 0xAF};
static uint8_t hundoDynamoHealthTable[2] = {32, 46};
static uint8_t hundoDynamoAmmoTable[2] = {50, 48};
static uint8_t hundoDynamoPlayerTable[2] = {1, 0};
static uint32_t hundoDynamoPartsTable[2] = {0, 0x10};
static uint32_t hundoDynamoCollectablesTable[2] = {0x1000, 0xF000};
static uint8_t hundoDynamoHoursTable[2] = {10, 5};
/************/

/*All Stages & Any%*/
static uint8_t mavericksClearedTable[8] = {8, 0x4D, 9, 0, 0x6F, 0x4F, 0xD, 0x7F};
static uint8_t mavericksHoursTable[8] = {14, 11, 13, 15, 7, 10, 12, 4};
static uint8_t maverickHealthTable[8] = {32, 32, 32, 32, 32, 32, 32, 36};
static uint8_t maverickAmmoTable[8] = {48, 52, 50, 48, 54, 54, 52, 56};
static uint8_t maverickStageSelectModeTable[8] = {0, 5, 5, 0, 6, 5, 5, 8};
static uint8_t mavericksRankTable[8] = {2, 2, 1, 3, 2, 2, 2, 2};
static uint32_t maverickPartsTable[8] = {0, 0x10, 0, 0, 0x4030, 0x10, 0x10, 0xC030};
/*****/
static uint8_t dynamoHoursTable[2] = {13, 9};
static uint8_t dynamoRankTable[2] = {1, 2};
static uint8_t dynamoMaverickClearedTable[2] = {9, 0x6F};
static uint32_t dynamoPartsTable[2] = {0, 0x4030};
static uint8_t dynamoStageSelectModeTable[2] = {0, 5};
static uint8_t dynamoHealthTable[2] = {32, 34};
static uint8_t dynamoAmmoTable[2] = {48, 48};
/*****/
static uint8_t sigmaMaverickClearedTable[3] = {0x6F, 0x7F, 0x7F};
static uint32_t sigmaPartsTable[3] = {0x4030, 0x4030, 0x4030};
static uint8_t sigmaHealthTable[3] = {36, 36, 36};
static uint8_t sigmaAmmoTable[3] = {54, 54, 56};
static uint8_t sigmaHoursTable[3] = {8,6,5};

void SaveRestore();

void AssignWeapons() // 2 routes: All Stages & Any%
{
    uint32_t parts = 0;
    uint8_t hoursLeft = 16; //NOTE: enigma shot can increase the hours by 1,2,3 or 4 hours
    game.stageSelectMode = 0;
    game.armorParts = 0;
    game.armors = 0;
    game.clearedStages = 0;
    game.collectables = 0;
    game.collectables2 = 0;
    game.maxHPs[0] = 32;
    game.maxHPs[1] = 32;
    game.maxAmmos[0] = 48;
    game.maxAmmos[1] = 48;
    game.tanksAmmo[0] = 0;
    game.tanksAmmo[1] = 0;
    game.tanksAmmo[2] = 0;
    game.ranks[0] = 5;
    game.ranks[1] = 3;
    game.zeroBuster = true;
    *(uint8_t*)((int)&game + 0xCB) = 0x80; //set non awakening zero

    for (size_t i = 0; i < 6; i++)
    {
        game.equipedParts[i] = 0;
    }
    for (size_t i = 0; i < 64; i++)
    {
        game.seenTextBoxes[0][i] = 0;
    }

    game.player = 1; // Default to Zero
    game.armorType = 5;

    if (practice.route == 3) // CUSTOM
    {
        return;
    }

    if (game.stageId == 0) // Intro
    {
        game.stageSelectMode = 0;
    }
    else
    {
        if (practice.route != 2) // Any%/All-Stages
        {
            if (game.stageId <= 8) // 8 Maverick Stages
            {
                game.ranks[1] = mavericksRankTable[game.stageId - 1];
                game.clearedStages = mavericksClearedTable[game.stageId - 1];
                hoursLeft = mavericksHoursTable[game.stageId - 1];
                game.maxHPs[1] = maverickHealthTable[game.stageId - 1];
                game.maxAmmos[1] = maverickAmmoTable[game.stageId - 1];
                game.stageSelectMode = maverickStageSelectModeTable[game.stageId - 1];
                parts = maverickPartsTable[game.stageId - 1];
            }
            else if (game.stageId == 0xC) // Final Level
            {
                if (practice.ultimateArmor)
                {
                    game.player = 0;
                    game.ranks[0] = 4;
                    game.armorType = 4;
                }
                game.stageSelectMode = 8;
                game.armors = 0x10;
                hoursLeft = 14; //doesnt make sense but whatever (edit: hours are increased after enigma shot)
                if (practice.route == 0) // All Stages
                {
                    hoursLeft = 3;
                    game.ranks[1] = 2;
                    game.clearedStages = 0xFF;
                    game.collectables = 0x5000;
                    game.maxHPs[1] = 38;
                    game.maxAmmos[1] = 56;

                    if (!practice.ultimateArmor)
                    {
                        parts = 0xC030;
                    }
                    else
                    {
                        parts = 0x10;
                    }
                }
            }
            else if (game.stageId < 0xC) // dynamo fights...
            {
                game.ranks[1] = dynamoRankTable[game.stageId - 9];
                hoursLeft = dynamoHoursTable[game.stageId - 9];
                game.clearedStages = dynamoMaverickClearedTable[game.stageId - 9];
                parts = dynamoPartsTable[game.stageId - 9];
                game.stageSelectMode = dynamoStageSelectModeTable[game.stageId - 9];
                game.maxHPs[1] = dynamoHealthTable[game.stageId - 9];
                game.maxAmmos[1] = dynamoAmmoTable[game.stageId - 9];
            }
            else
            { // other sigma stages
                hoursLeft -= game.stageId - 0x10; 
                game.stageSelectMode = game.stageId - 0x10 + 5;

                if (practice.ultimateArmor && game.stageId == 0x12)
                {
                    game.player = 0;
                    game.armorType = 0;
                }

                if (practice.route == 0)
                {
                    game.ranks[1] = 2;
                    hoursLeft = sigmaHoursTable[game.stageId - 0x10];
                    game.clearedStages = sigmaMaverickClearedTable[game.stageId - 0x10];
                    if (game.player != 0)
                    {
                        parts = sigmaPartsTable[game.stageId - 0x10];
                    }
                    else
                    {
                        parts = 0x10;
                    }

                    game.maxHPs[1] = sigmaHealthTable[game.stageId - 0x10];
                    game.maxAmmos[1] = sigmaAmmoTable[game.stageId - 0x10];
                }
            }
        }
        else // 100%
        {
            game.stageSelectMode = 0xA;
            if (game.stageId <= 8) // 8 Maverick Stages
            {
                game.clearedStages = hundoMavericksClearedTable[game.stageId - 1];
                game.player = hundoMaverickPlayerTable[game.stageId - 1];
                game.maxHPs[game.player] = hundoMaverickHealthTable[game.stageId - 1];
                game.maxAmmos[game.player] = hundoMaverickAmmoTable[game.stageId - 1];
                game.ranks[game.player] = hundoMavericksRankTable[game.stageId - 1];
                game.armorType = hundoMaverickArmorTypeTable[game.stageId - 1];
                game.collectables = hundoMavericksCollectablesTable[game.stageId - 1];
                hoursLeft = hundoMavericksHoursTable[game.stageId - 1];
                parts = hundoMaverickPartsTable[game.stageId - 1];

                if (isRevistFlag)
                {
                    game.clearedStages = 0xFF;
                    game.player = 0;
                    game.collectables = 0xF000;
                    game.armorParts = 0xFB;
                    game.ranks[0] = 2;

                    if (game.stageId == 1) // Grizzly
                    {
                        game.armorType = 2;
                        game.maxHPs[0] = 54;
                        game.maxAmmos[0] = 52;
                        parts = 0x18;
                    }
                    else if (game.stageId == 3) // Duff McWhalen
                    {
                        game.armorType = 4;
                        game.maxHPs[0] = 50;
                        game.maxAmmos[0] = 50;
                        parts = 0x10;
                    }
                    else // Axl Rose
                    {
                        game.armorType = 3;
                        game.maxHPs[0] = 52;
                        parts = 0;
                    }
                }
                else if(game.mid != 0)
                {
                    if (game.stageId == 6 || game.stageId == 4)
                    {
                        game.maxHPs[0] += 2;
                    }
                    
                    if (game.stageId == 6)
                    {
                        game.collectables = 0x9000;
                    }
                    else if (game.stageId == 2)
                    {
                        game.collectables = 0xF000;
                    }
                    
                }
            }
            else if (game.stageId == 0xC) // Final Level
            {
                hoursLeft = 1;
                game.ranks[0] = 2;
                game.ranks[1] = 2;
                game.maxHPs[0] = 56;
                game.maxAmmos[0] = 52;
                game.maxAmmos[1] = 52;
                game.clearedStages = 0xFF;
                game.collectables = 0xF000;
                
                if (practice.ultimateArmor)
                {
                    game.player = 0;
                    game.armorType = 4;
                    parts = 0x2010;
                }else{
                    parts = 0xC030;
                }
                
            }
            else if (game.stageId < 0xC) // dynamo fights
            {
                game.stageSelectMode = 0xA;
                game.player = hundoDynamoPlayerTable[game.stageId - 9];
                if (game.player == 0)
                {
                    game.armorType = 4;
                }
                game.clearedStages = hundoDynamoMaverickClearedTable[game.stageId - 9];
                game.maxHPs[game.player] = hundoDynamoHealthTable[game.stageId - 9];
                game.maxAmmos[game.player] = hundoDynamoAmmoTable[game.stageId - 9];
                game.ranks[game.player] = 2;
                game.collectables = hundoDynamoCollectablesTable[game.stageId - 9];
                hoursLeft = hundoDynamoHoursTable[game.stageId - 9];
                parts = hundoDynamoPartsTable[game.stageId - 9];
            }
            else
            { // other sigma stages
                hoursLeft -= game.stageId - 0x10; 
                game.stageSelectMode = game.stageId - 0x10 + 5;

                if (isRevistFlag)
                {
                    hoursLeft = 13;
                    game.armors = 8;
                }
                else if(game.stageId == 0x12)
                {
                    game.player = 0;
                    game.armorType = 0;
                }
            }

            /******/

            if (game.stageSelectMode == 0xA)
            {
                game.armors |= 0x18;
            }
            if ((game.armorParts & 0xF) == 0xF)
            {
                game.armors |= 2;
            }
            if ((game.armorParts & 0xF0) == 0xF)
            {
                game.armors |= 4;
            }
        }
        //...
    }
    if ((game.clearedStages & 1) != 0) // give Sub-Tank some ammo
    {
        game.collectables |= 0x1000;
        game.tanksAmmo[0] = 12;
    }
    game.equipedParts[game.armorType] = parts;
    game.hoursLeft = hoursLeft  * 3600 * 60;
    SaveRestore();
}
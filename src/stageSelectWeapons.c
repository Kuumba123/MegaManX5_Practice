#include <common.h>
#include "practice.h"

static uint8_t mavericksClearedTable[8] =
    {
        8, 9, 0x6B, 0, 0, 0, 9, 0};
static uint8_t maverickRankTable[8] =
    {
        1, 0, 0, 0, 0, 0, 0, 0};
static uint8_t mavericksHoursTable[8] =
    {
        14, 24, 24, 15, 24, 24, 24, 24};

static uint8_t maverickPlayerTable[8] =
    {
        1, 1, 1, 1, 1, 1, 1, 1};
static uint8_t maverickAmmoTable[8] =
    {
        48, 48, 48, 48, 48, 48, 48, 48};
static uint8_t dynamoHoursTable[2] =
    {
        13, 10};
static uint8_t dynamoMaverickClearedTable[2] =
    {
        9, 0};
static uint8_t dynamoRankTable[2] =
    {
        1, 0};
static uint32_t dynamoPartsTable[2] =
    {
        0, 0x10};
void AssignWeapons() // 2 routes: All Stages & Any%
{
    game.igt = 0;
    game.armorParts = 0;
    game.armors = 0;
    game.clearedStages = 0;
    game.collectables = 0;
    game.collectables2 = 0;
    game.maxHPs[0] = 32;
    game.maxHPs[1] = 32;
    game.maxAmmos[0] = 48;
    game.maxAmmos[1] = 48;
    game.ranks[0] = 5;
    game.ranks[1] = 3;
    game.hoursLeft = 3456000;

    for (size_t i = 0; i < 6; i++)
    {
        game.equipedParts[i] = 0;
    }

    game.player = 1; // Default to Zero
    game.armorType = 5;

    if (game.stageId == 0) // Intro
    {
    }
    else if (game.stageId <= 8) // 8 Maverick Stages
    {
        game.player = maverickPlayerTable[game.stageId - 1];
        game.clearedStages = mavericksClearedTable[game.stageId - 1];
        game.hoursLeft = mavericksHoursTable[game.stageId - 1] * 3600 * 60;
        game.ranks[game.player] = maverickRankTable[game.stageId - 1];
    }
    else if (game.stageId == 0xC) // Final Level
    {
        game.armors = 0x10;
        if (practice.route == 0)
        {
            game.clearedStages = 0xFF;
            if (practice.ultimateArmor)
            {
                game.player = 0;
                game.armorType = 4;
            }
        }
    }
    else if (game.stageId < 0xC) // dynamo fights...
    {
        game.hoursLeft = dynamoHoursTable[game.stageId - 9] * 3600 * 60;
        game.clearedStages = dynamoMaverickClearedTable[game.stageId - 9];
        game.equipedParts[5] = dynamoPartsTable[game.stageId - 9];
    }
    else
    { // other sigma stages

        if (practice.route == 0)
        {
            game.clearedStages = 0x7F;
        }

        if (practice.ultimateArmor && game.stageId == 0x12 && practice.route != 0)
        {
            game.player = 0;
            game.armorType = 0;
        }
    }

    if ((game.clearedStages & 1) != 0) // give Sub-Tank some ammo
    {
        game.collectables = 0x1000;
        game.tanksAmmo[0] = 12;
    }
}

void ExitRoutePage(Game *gameP)
{
    gameP->mode2 = 4;
    gameP->mode3 = 0;
    gameP->mode4 = 0;
    PlaySound(5, 0, 0);
}
void ExitStageSelectCheck()
{
    if ((buttonsPressed & PAD_SELECT) != 0)
    {
        CreateTitleScreenThread();
        DeleteThread();
    }
}
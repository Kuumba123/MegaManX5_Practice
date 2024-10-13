#include <common.h>
#include <object.h>
#include <misc.h>
#include "practice.h"

extern bool LevelMidTable[];

extern uint32_t swapTextureFlag;

void SwapTexture(bool sync);

void CreateTitleScreenThread();

void LoadBackupScreens();

void DetermineClear(Game *gameP)
{
    gameP->stageTime = gameP->stageTime + mega.stageTime;

    if (gameP->clear == 0)
    {
        /* MegaMan Dead */
        gameP->spawnFlags = 0;
        bool reset = true;

        if ((gameP->stageId != 0xC) || (reset = gameP->point < 0xa, gameP->point < 0x2))
        {
            reset = false;
        }

        /* Teleporter Room in Refights */
        if (reset)
        {
            gameP->point = 1;
        }

        if (gameP->stageId != 0 || gameP->point != 3)
        {
            EndSong();
            LoadLevel();
        }

        *(int8_t *)0x800d1598 = 0; // reload flag for non refight boss textures

        if (practice.page != 0)
        {
            if (gameP->stageId != 0 || gameP->point != 3)
            {
                SwapTexture(false);
                practice.page = 1;
            }
        }
        practice.state.made = false;

        gameP->mode = 9;
        gameP->mode2 = 0;
        gameP->mode3 = 0;
        gameP->mode4 = 0;
    }
    else
    {
        if (gameP->clear < 0)
        {
            gameP->spawnFlags = 0xFF;
            gameP->hpTemp = mega.hp;

            memcpy(&gameP->ammoTemp[0], &mega.ammo[0], 32);
            gameP->weaponTemp = mega.weapon;
            gameP->virusMeterTemp = mega.virusMeter;

            if (gameP->stageId == 6) // Teleporter in 2nd Half of Izzy Glow
            {
                swapTextureFlag = 1;
            }

            /*Refight Teleporter Check*/
            if (((gameP->stageId == 0xc) || (gameP->songOff = true, gameP->stageId == 0xc)) &&
                (gameP->point == 10))
            {
                gameP->weaponTemp = 0;
            }

            gameP->mode = 9;
            gameP->mode2 = 0;
            gameP->mode3 = 0;
            gameP->mode4 = 0;
            gameP->clear = 0;
        }
        else // Actual Real Clear
        {
            gameP->spawnFlags = 0;
            
            if (gameP->stageId == 0 || gameP->stageId > 8) // Intro & Dynamo & Sigma Stages
            {

                gameP->mode = 0xB;
                gameP->mode2 = 0;
                gameP->mode3 = 0;
                gameP->mode4 = 0;
                return;
            }
            else if (gameP->stageId < 9) // 8 Maverick stages
            {
                if (LevelMidTable[gameP->stageId] == false && gameP->mid == 0)
                {
                    gameP->mid = 1;
                    gameP->mode = 8;
                }
                else
                {
                    gameP->exitType = 0;
                    gameP->mode = 0xB;
                }
                gameP->clear = 0;
                gameP->mode2 = 0;
                gameP->mode3 = 0;
                gameP->mode4 = 0;
            }
        }
    }
}
void SwappedTextureCheck()
{
    if (swapTextureFlag == 0)
    {
        swapTextureFlag = 1;
    }
}
void ResetState()
{
    practice.state.made = false;
    practice.page = 0;

    if (practice.skipRefights)
    {
        game.point = 0xA;
    }
    LoadLevel();
}
void BackupScreenChck()
{
    if (game.stageId != 0xC || game.point != 3)
    {
        LoadBackupScreens();
    }
}
void MemoryCopy(void *dest, const void *src, size_t size)
{
    // Ensure that the size is a multiple of 4 bytes
    size_t num_4byte_blocks = size / 4;

    uint32_t *d = (uint32_t *)dest;
    const uint32_t *s = (const uint32_t *)src;

    for (size_t i = 0; i < num_4byte_blocks; i++)
    {
        d[i] = s[i];
    }
}
void ShowPracticeTitleText()
{
    DrawDebugText(9, 15, 0, "Practice Hack BETA2\n\t\tBy Kuumba");
}
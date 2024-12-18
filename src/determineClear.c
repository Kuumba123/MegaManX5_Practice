#include <common.h>
#include <object.h>
#include <misc.h>
#include "practice.h"

extern bool LevelMidTable[];

extern uint32_t swapTextureFlag;

extern int8_t checkPointNew;

static int8_t checkPointTextureFlags[] =
    {
        0x0E, 0,    // ST00
        0x18, 0,    // ST01
        0, 0x04,    // ST02
        0x70, 0,    // ST03
        0x06, 0x02, // ST04
        0x1E, 0,    // ST05
        0, 0x0E,    // ST06
        0xF8, 0,    // ST07
        0x1C, 0,    // ST08
        0, 0,       // ST09
        0, 0,       // ST0A
        0, 0,       // ST0B
        0, 0,       // ST0C
        0, 0,       // ST0D
        0, 0,       // ST0E
        0, 0,       // ST0F
        0x0E, 0,    // ST10
                    // ST11-16 not needed
};

void SwapTexture(bool sync);
void ResetAmmo(Game *gameP);

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

        EndSong();

        if (gameP->stageId != 0 || gameP->point != 3)
        {
            if (gameP->stageId == 0xC && (gameP->point == 0x15 || gameP->point == 0xA))
            {
                gameP->refights[3] = 0;
                gameP->refights[4] = 0;
            }
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
            practice.sigmaOvl = 0;
            gameP->spawnFlags = 0xFF;
            gameP->hpTemp = mega.hp;

            memcpy(&gameP->ammoTemp[0], &mega.ammo[0], 32);
            gameP->weaponTemp = mega.weapon;
            gameP->virusMeterTemp = mega.virusMeter;

            if ((uint8_t)gameP->clear != 0xC1)
            {
                if (gameP->stageId == 6 && gameP->mid != 0) // Teleporter in 2nd Half of Izzy Glow
                {
                    swapTextureFlag = 1;
                }
            }
            else
            {
                ResetAmmo(gameP);
                if (gameP->stageId == 0xC)
                {
                    if ((practice.page != 0 && gameP->point != 21) || (practice.page == 0 && gameP->point == 21))
                    {
                        swapTextureFlag = 1;
                    }
                }
                else if (gameP->stageId < 0x11)
                {
                    if (practice.page != ((checkPointTextureFlags[gameP->stageId * 2 + gameP->mid] & (1 << gameP->point)) != 0))
                    {
                        swapTextureFlag = 1;
                    }
                }
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
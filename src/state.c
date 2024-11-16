#include <common.h>
#include <gpu.h>
#include <object.h>
#include <layer.h>
#include "practice.h"

/*
 *   Being stored at the end of MOBJPAT.ARC (0x99C bytes free)
 */

#define VariableSetsCount 21
#define BuffersCount 8

#define RNG *(uint16_t *)0x80093F70
#define RELOAD *(uint8_t *)0x800d1598
#define PASTBRIGHT *(uint8_t *)0x800A51A6
#define UPDATECLUT *(uint8_t *)0x800c9310
#define STARTSELECT_FLAG *(uint32_t*)0x80091D54
#define SCREENBACKUP *(uint32_t *)0x800A51A0
#define VABP *(int *)0x8009a758

#define FADE_F *(uint16_t *)0x801F8200
#define SONG_F *(bool*)0x8009a418

#define DECOMPRESS_ADDR 0x800CD598

extern uint32_t swapTextureFlag;
extern void *swapTexturePointer;
extern void *clutPointer;
extern Enemy *enemyDataPointers[];
extern uint8_t loadState; /*1=loading*/

static void (*mode_A_Table[2])(Game *) = {0x80020D98, 0x80020E94};

void LoadCompressedImage(Object *objP, int16_t x, int16_t y);
void LoadBossRefightsArc();

extern void *freeAddress[];
extern int16_t freeAddressSizes[];
extern void *readAddress[];
extern int16_t addressesSize[];

extern void *maverickRefightBssAddresses[];
extern uint8_t maverickRefightBssSizes[];

extern void *stageBssAddresses[];
extern uint8_t *stageBssSizes[];


void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);
void DrawLoadText();

void SwapWeaponTexturesClut(Mega *megaP);

void MemoryCopy(void *dest, const void *src, size_t size);

void SaveQuadObjects();
void RestoreQuadObjects();

void LoadSigmaOverlay(int ovl);

void SwapTexture(bool sync)
{
    if (sync)
    {
        DrawSync(0); // Wait for Images & Clut to finish transfering before doing texture swap
    }

    void *p = DECOMPRESS_ADDR; // Decompressed Texture buffer (temporary storage)
    void *p2 = swapTexturePointer;
    RECT rect = {320, 256, 512, 8};

    for (size_t i = 0; i < 32; i++)
    {
        StoreImage2(&rect, p);
        LoadImage2(&rect, p2);

        MemoryCopy(p2, p, 0x2000);

        p2 = (int)p2 + 0x2000;
        rect.y += 8;
    }
    practice.page ^= 1;
}
void SaveState()
{
    readAddress[0] = clutPointer;
    freeAddress[0] = (int)clutPointer + 0x2800;

    ThreadSleep(10); // Waiting before transfering

    // backup enemy data other vars
    Enemy *p = enemyDataPointers[game.stageId * 2 + game.mid];
    uint8_t *p2 = 0x1F800128;
    while ((p->type & 0xF) != 0xF)
    {
        *p2 = p->spawned;
        p += 1;
        p2 += 1;
    }

    int freeId = 0;
    int freeSize = freeAddressSizes[freeId];
    uint freeP = freeAddress[freeId];

    for (size_t i = 0; i < VariableSetsCount; i++)
    {
        int dumpSize = addressesSize[i];
        uint srcAddr = readAddress[i];

    SizeCheck:
        if (dumpSize > freeSize)
        {
            MemoryCopy(freeP, srcAddr, freeSize);
            srcAddr += freeSize;
            dumpSize -= freeSize;

            // Fix Free Data Vars
            freeId++;

            if (freeId > BuffersCount - 1)
            {
                printf("ERROR: went past MAX buffer counts: %X\n", BuffersCount);
                return;
            }

            freeSize = freeAddressSizes[freeId];
            freeP = freeAddress[freeId];
            goto SizeCheck;
        }
        else
        {
            MemoryCopy(freeP, srcAddr, dumpSize);
            freeP += dumpSize;
            freeSize -= dumpSize;
        }
    }

    // save quad objects
    SaveQuadObjects();

    practice.state.textureFlag = swapTextureFlag;
    practice.state.pastBright = PASTBRIGHT;
    practice.state.arcP = freeArcP;
    practice.state.reloadFlag = RELOAD;
    practice.state.page = practice.page;
    practice.state.rng = RNG;
    practice.state.sigmaOvl = practice.sigmaOvl;

    practice.state.made = true; // Mark State as Made

    if (game.point >= 2 && game.point <= 9 && game.point && game.stageId == 0xC)
    {
        MemoryCopy(&practice.state.bss, maverickRefightBssAddresses[game.point - 2], maverickRefightBssSizes[game.point - 2]);
    }
    else if (stageBssAddresses[game.stageId * 2 + game.mid] != 0 && game.stageId < 0x13)
    {
        MemoryCopy(&practice.state.bss, stageBssAddresses[game.stageId * 2 + game.mid], stageBssSizes[game.stageId * 2 + game.mid]);
    }

    size_t screenLength = ((*(uint32_t *)0x1F80000C) - (*(uint32_t *)0x1F800008)); // getting screen count via pointers
    practice.state.screenSize = screenLength;
    MemoryCopy(SCREENBACKUP, *(uint32_t *)0x1F800008, screenLength);
}
void LoadState()
{
    ThreadSleep(10); // Waiting before transfering

    // restore enemy data
    Enemy *p = enemyDataPointers[game.stageId * 2 + game.mid];
    uint8_t *p2 = 0x1F800128;
    while ((p->type & 0xF) != 0xF)
    {
        p->spawned = *p2;
        p->type = p->type & 0xF;
        p += 1;
        p2 += 1;
    }

    uint8_t pastPoint = game.point;
    uint8_t pastFile = RELOAD;

    int freeId = 0;
    int freeSize = freeAddressSizes[freeId];
    uint freeP = freeAddress[freeId];

    for (size_t i = 0; i < VariableSetsCount; i++)
    {
        int dumpSize = addressesSize[i];
        uint srcAddr = readAddress[i];

    SizeCheck:
        if (dumpSize > freeSize)
        {
            MemoryCopy(srcAddr, freeP, freeSize);
            srcAddr += freeSize;
            dumpSize -= freeSize;

            // Fix Free Data Vars
            freeId++;
            freeSize = freeAddressSizes[freeId];
            freeP = freeAddress[freeId];
            goto SizeCheck;
        }
        else
        {
            MemoryCopy(srcAddr, freeP, dumpSize);
            freeP += dumpSize;
            freeSize -= dumpSize;
        }
    }

    // restore quad objects
    RestoreQuadObjects();
    
    if (practice.page != practice.state.page)
    {
        SwapTexture(false);
    }
    practice.page = practice.state.page;
    swapTextureFlag = practice.state.textureFlag;
    PASTBRIGHT = practice.state.pastBright;
    if (game.startingSong != 0)
    {
        EndSong();
    }
    

    freeArcP = practice.state.arcP;
    RELOAD = practice.state.reloadFlag;
    if (practice.keepRng)
    {
        RNG = practice.state.rng;
    }

    UPDATECLUT = 1; // Update Clut

    bgLayers[0].update = true;
    bgLayers[1].update = true;
    bgLayers[2].update = true;

    bool refightsBss = false;

    if (game.stageId == 0xC)
    {
        if (game.point >= 2 && game.point <= 9)
        {
            refightsBss = true;
        }
        
        if (refightsBss && game.point != pastPoint)
        {
            EndSong();
            LoadBossRefightsArc();
            ThreadSleep(2);
            FadeIn(10);
            while (fadeDirection != 0)
            {
                ThreadSleep(1);
            }
        }
        else
        {
            RELOAD = 0;
        }
        if (practice.sigmaOvl != practice.state.sigmaOvl)
        {
            if (practice.state.sigmaOvl == 1)
            {
                LoadSigmaOverlay(0x26);
                FileCollect2();
                ArcSeek(0x88, 4, VABP);
                DrawLoad(0, 0);
                freeArcP = practice.state.arcP;
            }
        }
    }
    else if (stageBssAddresses[game.stageId * 2 + game.mid] != 0 && game.stageId < 0x13)
    {
        MemoryCopy(stageBssAddresses[game.stageId * 2 + game.mid], &practice.state.bss, stageBssSizes[game.stageId * 2 + game.mid]);
    }

    if (refightsBss)
    {
        MemoryCopy(maverickRefightBssAddresses[game.point - 2], &practice.state.bss, maverickRefightBssSizes[game.point - 2]);
    }
    practice.sigmaOvl = practice.state.sigmaOvl;
    STARTSELECT_FLAG = 1;
    mega.newAnimeF = -1;
    LoadCompressedImage((Object *)&mega, 320, 0);
    if (mega.player == 0)
    {
        SwapWeaponTexturesClut(&mega);
    }
    
    MemoryCopy(*(uint32_t *)0x1F800008, SCREENBACKUP, practice.state.screenSize);
}

void StateCheck(Game *gameP)
{
    int time = mega.stageTime + gameP->stageTime;
    int totalSeconds = time / 60;
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    int frames = time % 60;

    DrawDebugText(4, 3, 0, "%2d:%2d:%2d", minutes, seconds, frames);

    if (loadState == 1)
    {
        DrawLoadText();
    }

    if (loadState != 1 && FADE_F == 0 && SONG_F == false)
    {
        if ((buttonsPressed & (PAD_L2 | PAD_R2 | PAD_SELECT)) != 0)
        {
            if ((buttonsHeld & (PAD_R2 + PAD_SELECT)) == (PAD_R2 + PAD_SELECT))
            {
                SaveState();
            }
            else if ((buttonsHeld & (PAD_L2 + PAD_SELECT)) == (PAD_L2 + PAD_SELECT) && practice.state.made)
            {
                LoadState();
            }
        }
    }
    mode_A_Table[gameP->mode2](gameP);
}

#undef RNG
#undef RELOAD
#undef PASTBRIGHT
#undef UPDATECLUT
#undef STARTSELECT_FLAG
#undef SCREENBACKUP
#undef VABP

#undef FADE_F
#undef SONG_F

#undef DECOMPRESS_ADDR
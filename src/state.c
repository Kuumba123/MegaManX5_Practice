#include <common.h>
#include <gpu.h>
#include <object.h>
#include <layer.h>
#include "practice.h"

/*
 *   Being stored at the end of MOBJPAT.ARC (0x99C bytes free)
 */

#define VariableSetsCount 20
#define BuffersCount 8

extern uint32_t swapTextureFlag;
extern void *swapTexturePointer;
extern void *clutPointer;
extern Enemy *enemyDataPointers[];
extern uint8_t loadState; /*1=loading*/

static void (*mode_A_Table[2])(Game *) = {0x80020D98, 0x80020E94};

void LoadCompressedImage(Object *objP, int16_t x, int16_t y);
void LoadBossRefightsArc();

extern void *freeAddress[];
extern int freeAddressSizes[];
extern void *readAddress[];
extern int addressesSize[];

extern void *maverickRefightBssAddresses[];
extern uint8_t maverickRefightBssSizes[];

extern void *stageBssAddresses[];
extern uint8_t *stageBssSizes[];

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);

void MemoryCopy(void *dest, const void *src, size_t size);

void SwapTexture(bool sync)
{
    if (sync)
    {
        DrawSync(0); // Wait for Images & Clut to finish transfering before doing texture swap
    }

    void *p = 0x800CD598; // Decompressed Texture buffer (temporary storage)
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
    RECT rect = {0, 500, 256, 12};
    LoadImage2(&rect, 0x8009F9A0);

    practice.state.textureFlag = swapTextureFlag;
    practice.state.pastBright = *(uint8_t *)0x800A51A6;
    practice.state.songSeekFlag = *(uint8_t *)0x800d1f3c;
    practice.state.arcP = freeArcP;
    practice.state.backupArcP = *(int *)0x800e95a4;
    practice.state.reloadFlag = *(uint8_t *)0x800d1598;

    if (game.point >= 2 && game.point <= 9 && game.point && game.stageId == 0xC)
    {
        MemoryCopy(&practice.state.bss, maverickRefightBssAddresses[game.point - 2], maverickRefightBssSizes[game.point - 2]);
    }else if (stageBssAddresses[game.stageId * 2 + game.mid] != 0)
    {
        MemoryCopy(&practice.state.bss, stageBssAddresses[game.stageId * 2 + game.mid], stageBssSizes[game.stageId * 2 + game.mid]);
    }
    

    size_t screenLength = ((*(uint32_t *)0x1F80000C) - (*(uint32_t *)0x1F800008)); // getting screen count via pointers
    MemoryCopy(*(uint32_t *)0x800A51A0, *(uint32_t *)0x1F800008, screenLength);

    practice.state.page = practice.page;
    practice.state.made = true;
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
    uint8_t pastFile = *(uint8_t *)0x800d1598;

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
    RECT rect = {0, 500, 256, 12};
    StoreImage2(&rect, 0x8009F9A0);
    if (practice.page != practice.state.page)
    {
        SwapTexture(false);
    }
    practice.page = practice.state.page;
    swapTextureFlag = practice.state.textureFlag;
    *(uint8_t *)0x800A51A6 = practice.state.pastBright;
    if (*(int8_t *)0x800e8060 != 0)
    {
        *(uint8_t *)0x800d1f3c = practice.state.songSeekFlag;
    }
    else
    {
        *(uint8_t *)0x800d1f3c = 0;
    }

    freeArcP = practice.state.arcP;
    *(int *)0x800e95a4 = practice.state.backupArcP;
    *(uint8_t *)0x800d1598 = practice.state.reloadFlag;

    bool refightsBss = false;

    if (game.stageId == 0xC)
    {
        if (game.point >= 2 && game.point <= 9 && game.point != pastPoint)
        {
            refightsBss = true;
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
            *(uint8_t *)0x800d1598 = 0;
        }
    }else if (stageBssAddresses[game.stageId * 2 + game.mid] != 0){
        MemoryCopy(stageBssAddresses[game.stageId * 2 + game.mid], &practice.state.bss, stageBssSizes[game.stageId * 2 + game.mid]);
    }

    if (refightsBss)
    {
        MemoryCopy(maverickRefightBssAddresses[game.point - 2], &practice.state.bss, maverickRefightBssSizes[game.point - 2]);
    }

    mega.newAnimeF = -1;
    LoadCompressedImage((Object *)&mega, 320, 0);

    size_t screenLength = ((*(uint32_t *)0x1F80000C) - (*(uint32_t *)0x1F800008)); // getting screen count via pointers
    MemoryCopy(*(uint32_t *)0x1F800008, *(uint32_t *)0x800A51A0, screenLength);

    *(uint8_t *)0x800c9310 = 1; // Update Clut

    bgLayers[0].update = true;
    bgLayers[1].update = true;
    bgLayers[2].update = true;
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
        DrawDebugText(4, 4, 2, "(Loading)");
    }

    if (loadState != 1 && *(uint16_t *)0x801F8200 /*<-Fade In/Out Thread*/ == 0)
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
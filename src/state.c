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

static void *freeAddress[] = {
    -1,         // End of Clut (dynamicly set based off of CLUT Pointer)
    0x8009CF90, // End of Inverted Clut
    0x800A39A0, // End of Backup Clut
    0x801F6000, // Demo Buffer
    0x801F8304, // Just before Thread Stack Memory
    0x801EF254, // End of ARC buffer
    0x8000A000, // Kernal RAM
    0x8000E440  // Kernal RAM
};

static int freeAddressSizes[] = {
    0x1800,
    0x1800,
    0x1800,
    0x2000,
    0x65FC,
    0x3DB0 - 4,
    0x1900,
    0x1BC0};

static void *readAddress[] = {
    -1,         // Clut (dynamicly set based off of CLUT Pointer)
    0x8009A790, // Inverted Clut
    0x80098120, // Weapon Objects
    0x80092090, // Main Objects
    0x80094398, // Shot Objects
    0x80096D98, // Visual Objects
    0x8009E7A0, // Effect Objects
    0x800C6A10, // Item Objects
    0x800D1F40, // Misc Objects
    0x800C94A8, // Layer Objects
    0x8009A0A0, // Mega
    0x8009A1F8, // BG Layers
    0x800D1C00, // Game Info
    0x800D1dC0, // Ride Armor
    0x80091C38, // Text Info
    0x800E8514, // Extra Text Info
    0x80093E64, // Brightness stuff
    0x800D50DC, // More Brightness stuff
    0x8009A42C, // Also More Brightness stuff
    0x80093dd0, // Brightness Table thing
};

static int addressesSize[] = {
    0x2800, // Clut
    0x2800, // Inverted Clut
    0x1380, // Weapon Objects
    0x1D40, // Main Objects
    0x2A00, // Shot Objects
    0x1380, // Visual Objects
    0x1200, // Effect Objects
    0x2900, // Item Objects
    0x3000, // Misc Objects
    0xC0,   // Layer Objects
    0x158,  // Mega
    0xFC,   // BG Layers
    0x1B8,  // Game Info
    0xC0,   // Ride Armor
    0x18,   // Text Info
    0x104,  // Extra Text Info
    8,      // Brightness stuff
    4,      // More Brightness stuff
    4,      // Also More Brightness stuff
    0x28    // Brightness Table thing
};

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);

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
void SaveState() // TODO: check Duff-McWhalen Submarine code for fix & fix WARNING & Bosses Refights
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
    practice.state.flameTimer = *(uid_t *)0x800f5965;

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
    *(uint8_t *)0x800d1f3c = practice.state.songSeekFlag;
    if (game.stageId == 4 && game.mid == 0)
    {
        *(uint8_t *)0x800f5965 = practice.state.flameTimer;
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
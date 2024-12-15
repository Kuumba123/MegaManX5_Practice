#include <common.h>
#include <object.h>
#include <misc.h>
#include "practice.h"

// 0x2C4 bytes free

int8_t checkPointNew = 0xFF;

static int8_t maxCheckPoint[] =
    {
        3, 0,    // ST00
        4, 0,    // ST01
        1, 2,    // ST02
        6, 0,    // ST03
        2, 1,    // ST04
        4, 0,    // ST05
        1, 3,    // ST06
        7, 0,    // ST07
        4, 0,    // ST08
        0, 0,    // ST09
        0, 0,    // ST0A
        0, 0,    // ST0B
        0x15, 0, // ST0C
        0, 0,    // ST0D
        0, 0,    // ST0E
        0, 0,    // ST0F
        3, 0,    // ST10
        3, 0,    // ST11
        2, 0,    // ST12
        0, 0,
        0, 0,
        0, 0,
        1, 0 // ST16
};

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);

void CheckPointCheck(Game *gameP)
{
    if (gameP->clear == 0 && maxCheckPoint[gameP->stageId * 2 + gameP->mid] != 0 && mega.hp != 0)
    {
        if ((buttonsHeld & (PAD_RIGHT + PAD_SELECT)) == (PAD_RIGHT + PAD_SELECT))
        {
            checkPointNew = gameP->point + 1;
            gameP->clear = 0xC1;
        }
        else if ((buttonsHeld & (PAD_LEFT + PAD_SELECT)) == (PAD_LEFT + PAD_SELECT))
        {
            checkPointNew = gameP->point - 1;
            gameP->clear = 0xC1;
        }
        if (checkPointNew > maxCheckPoint[gameP->stageId * 2 + gameP->mid])
        {
            checkPointNew = 0;
        }
        else if (checkPointNew < 0)
        {
            checkPointNew = maxCheckPoint[gameP->stageId * 2 + gameP->mid];
        }
    }
}

void DrawLoadText()
{
    DrawDebugText(4, 4, 2, "(Loading)");
}
void ResetState()
{
    practice.state.made = false;
    practice.page = 0;
    practice.sigmaOvl = 0;

    if (practice.skipRefights)
    {
        game.point = 0xA;
    }
    LoadLevel();
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

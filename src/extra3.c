#include <common.h>
#include <object.h>
#include "practice.h"

#define RNG *(uint16_t *)0x80093F70
#define MAIN_THREAD 0x80020904
#define RELOAD *(uint8_t *)0x800d1598

extern int8_t maxCheckPoint[];
extern int8_t checkPointNew;

static struct Restore
{
    int hours;
    uint32_t collectables;
    uint32_t collectables2;
    uint8_t maxHP;
    uint8_t armors;
    uint8_t armorParts;
    bool ultimateArmor;
    bool seen;
};
static struct Restore restore;

void TitleDemoEnd(char *p);
void TitleDemoPlay(char *p);

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);

void SaveRestore()
{
    restore.hours = game.hoursLeft;
    restore.collectables = game.collectables;
    restore.collectables2 = game.collectables2;
    restore.maxHP = game.maxHPs[game.player];
    restore.armors = game.armors;
    restore.armorParts = game.armorParts;
    restore.ultimateArmor = game.ultimateArmor;
    restore.seen = game.seenTextBoxes[0][0];
}
void LoadRestore()
{
    uint8_t val = 0;

    if (game.stageId != 0x16)
    {
        game.hoursLeft = restore.hours;
        game.collectables = restore.collectables;
        game.collectables2 = restore.collectables2;
        game.maxHPs[game.player] = restore.maxHP;
        game.armors = restore.armors;
        game.armorParts = restore.armorParts;
        game.ultimateArmor = restore.ultimateArmor;

        if ((game.clearedStages & 1) != 0) // give Sub-Tank some ammo
        {
            game.collectables |= 0x1000;
            game.tanksAmmo[0] = 12;
        }
    }

    if (game.stageId != 0x16 && restore.seen)
    {
        val = 0xFF;
    }

    for (size_t i = 0; i < 64; i++)
    {
        game.seenTextBoxes[0][i] = val;
    }
}

void CheckPointMenu()
{
    int mouse = 0;
    int8_t max = maxCheckPoint[game.stageId * 2 + game.mid];

    while (true) // Loop
    {
        if ((buttonsPressed & PAD_TRIANGLE) != 0)
        {
            break;
        }

        // Check Inputs
        if ((buttonsPressed & PAD_CROSS) != 0)
        {
            checkPointNew = mouse;
            game.clear = 0xC1;
            break;
        }
        else if ((buttonsPressed & PAD_CIRCLE) != 0)
        {
            game.mode = 8;
            game.mode2 = 0;
            game.mode3 = 0;
            game.mode4 = 0;
            game.point = 0;
            game.mid = 0;
            RELOAD = 0;
            EndSong();
            LoadRestore();
            NewThread2(MAIN_THREAD);
        }

        if ((buttonsPressed & PAD_DOWN) != 0)
        {
            mouse++;
        }
        else if ((buttonsPressed & PAD_UP) != 0)
        {
            mouse--;
        }

        if (mouse < 0)
        {
            mouse = max;
        }
        else if (mouse > max)
        {
            mouse = 0;
        }

        DrawDebugText(3, 5 + mouse, 1, ">");
        for (size_t i = 0; i < (max + 1); i++)
        {
            DrawDebugText(4, 5 + i, 0, "CHECKPOINT %d", i);
        }
        DrawDebugText(18, 22, 1, "X = Spawn\nO = Restart\n^ = Exit");
        DrawDebugText(12, 3, 2, "CHECKPOINT PAGE");
        ThreadSleep(1);
    }
}
void ResetAmmo(Game *gameP)
{
    gameP->point = checkPointNew;
    gameP->virusMeterTemp = 0;
    gameP->weaponTemp = 0;
    gameP->hpTemp = gameP->maxHPs[gameP->player];
    for (size_t i = 0; i < 16; i++)
    {
        gameP->ammoTemp[i] = gameP->maxAmmos[gameP->player] * 6;
    }
}
void DrawLoadText()
{
    DrawDebugText(4, 4, 2, "(Loading)");
}
void DrawManipText()
{
    if (!practice.showEnigma)
    {
        return;
    }

    uint16_t pastRNG = RNG;
    uint16_t rng;
    for (size_t i = 0; i < 9; i++)
    {
        GetRNG();
    }
    rng = RNG;
    if ((rng % 64) < 4)
    {
        DrawDebugText(11, 3, 1, "Works");
    }
    else
    {
        DrawDebugText(11, 3, 2, "Fails");
    }
    RNG = pastRNG;
    DrawDebugText(4, 3, 0, "Enigma:");
}
void ShowDemoEnigma(char *p)
{
    if (p[0xE] != 1)
    {
        DrawManipText();
    }

    TitleDemoEnd(p);
}
void ShowDemoEnigma2(char *p)
{
    TitleDemoPlay(p);
    DrawManipText();
}

#undef RNG
#undef MAIN_THREAD
#undef RELOAD
#include <common.h>
#include <object.h>

#define Demo (*(int8_t *)((int)titleP + 0xC))

static struct Title
{
    uint8_t mode;
    uint8_t mode2;
    uint8_t mode3;
    uint8_t mode4;

    int16_t timer;
};

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);

#define BASE_X 3
#define BASE_Y 17

void TitleManip(struct Title *titleP)
{
    if ((buttonsPressed & PAD_TRIANGLE) != 0)
    {
        titleP->mode2 = 1;
        titleP->mode4 = 0;
        return;
    }
    uint8_t pastCursor = titleP->mode4;
    if ((buttonsPressed & PAD_DOWN) != 0)
    {
        if (titleP->mode4 == 2)
        {
            titleP->mode4 = 0;
        }
        else
        {
            titleP->mode4 += 1;
        }
    }
    else if ((buttonsPressed & PAD_UP) != 0)
    {
        if (titleP->mode4 == 0)
        {
            titleP->mode4 = 2;
        }
        else
        {
            titleP->mode4 -= 1;
        }
    }
    if (pastCursor != titleP->mode4)
    {
        PlaySound(0, 0xC, 0);
    }

    // Various Options
    if (titleP->mode4 == 0)
    {
        if ((buttonsPressed & (PAD_START + PAD_CROSS)) != 0)
        {
            PlaySound(0, 0x22, 0);
            game.mode = 0x13;
            game.mode2 = 0;
            game.mode3 = 0;
            game.mode4 = 0;

            game.point = 4;

            game.clearedStages = 9;

            ((uint8_t*)((int)&game))[0xC2] = 1;
            ((uint8_t*)((int)&game))[0xC3] = 2;
            ((uint8_t*)((int)&game))[0xC4] = 5;
            ((uint8_t*)((int)&game))[0xC5] = 3;

            ((uint8_t*)((int)&game))[0xCA] = 0;

            game.hoursLeft = 2793525;

            NewThread(1, 0x80020904);
            DeleteThread();
        }
    }
    else if (titleP->mode4 == 1)
    {
        if ((buttonsPressed & PAD_RIGHT) != 0)
        {
            if (Demo == 3)
            {
                Demo = 0;
            }
            else
            {
                Demo += 1;
            }
        }
        else if ((buttonsPressed & PAD_LEFT) != 0)
        {
            if (Demo == 0)
            {
                Demo = 3;
            }
            else
            {
                Demo -= 1;
            }
        }
    }
    else
    {
        if ((buttonsPressed & (PAD_START + PAD_CROSS)) != 0)
        {
            titleP->mode = 1;
            titleP->mode2 = 4;
            titleP->mode3 = 0;
            titleP->mode4 = 0;
            FadeOut(8);
            PlaySound(0, 0x22, 0);
        }
    }
    DrawDebugText(14, 4, 0, "RNG:%4X", *(uint16_t *)0x80093F70);

    DrawDebugText(BASE_X - 1, BASE_Y + titleP->mode4, 1, ">");

    DrawDebugText(BASE_X, BASE_Y, 1, "ENIGMA-SHOT\nDEMO:%d\nPLAY-DEMO", Demo);
}

#undef BASE_X
#undef BASE_Y

void ST0E_DetermineMusic(struct Title *titleP)
{
    PlaySound(0, 0x22, 0);

    if (titleP->mode3 != 1)
    {
        FadeOut(8);
        EndSong();
    }

    titleP->mode2 = 2;
    titleP->mode4 = 0;
}
#undef Demo
#include <common.h>
#include <misc.h>

#define Cursor titleP->mode3

static struct Title
{
    uint8_t mode;
    uint8_t mode2;
    uint8_t mode3;
    uint8_t mode4;

    int16_t timer;
};

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);

void ShowPracticeTitleText();

void ResetGameSettings();

void TitleLoop(struct Title *titleP)
{
    ShowPracticeTitleText();
    if (fadeDirection == 0)
    {
        if ((buttonsPressed & PAD_UP) != 0)
        {
            PlaySound(0, 0xC, 0);
            if (Cursor == 0)
            {
                Cursor = 3;
            }
            else
            {
                Cursor--;
                if (Cursor == 1)
                {
                    Cursor = 0;
                }
            }
        }

        if ((buttonsPressed & PAD_DOWN) != 0)
        {
            PlaySound(0, 0xC, 0);
            Cursor++;
            if (Cursor > 3)
            {
                Cursor = 0;
            }
            else if (Cursor == 1)
            {
                Cursor = 2;
            }
        }

        if ((buttonsPressed & (PAD_START + PAD_CROSS)) != 0)
        {
            PlaySound(0, 0x22, 0);
            FadeOut(8);
            EndSong();
            titleP->mode2 = 2;
        }

        if (buttonsPressed == 0)
        {
            titleP->timer = titleP->timer + 1;

            if (18000 < titleP->timer)
            {
                EndSong(8);
                FadeOut(8);
                titleP->timer = 0;
                titleP->mode2 = 3;
            }
        }
        else
        {
            titleP->timer = 0;
        }
    }
}

void TitleScreen_6_2(struct Title *titleP)

{
    uint8_t selected;
    uint8_t uVar2;

    if (fadeDirection != 0)
    {
        return;
    }
    selected = titleP->mode3;

    if (selected == 2)  //OPTION
    {
        titleP->mode = 8;
        titleP->mode2 = 0;
        titleP->mode3 = 0;
        titleP->mode4 = 0;
    }
    else
    {
        ResetGameSettings();
        game.mode = 0;
        game.mode2 = 0;
        game.mode3 = 0;
        game.mode4 = 0;
        if (selected != 0)
        {
            game.stageSelectMode = 10;
        }
        
        NewThread(1, 0x80020904);
        DeleteThread();
    }
    return;
}
void DetermineTitleOption()
{
    if (game.stageSelectMode == 10)
    {
        LoadLevel();
    }else{
        game.mode = 3; //Stage Select
        game.stageSelectMode = 10;
    }
    
}
#undef Cursor
#include <common.h>

#define PAGE_TOTAL 2
#define Cursor gameP->mode4

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);

void CustomRoute(Game *gameP)
{
    if (gameP->mode2 == 0)
    {
        gameP->mode2 = 1;
        FadeIn(7);
        return;
    }
    else if (gameP->mode2 == 1)
    {
        if (fadeDirection != 0)
        {
            return;
        }
        gameP->mode2 = 0;
    }

    while (true)
    {
        if ((buttonsPressed & PAD_START) != 0)
        {
            if (gameP->player != 0)
            {
                gameP->armorType = 5;
            }
            
            gameP->stageId = gameP->slowMotion;
            gameP->mid = gameP->maverickShow;

            gameP->mode = 7;
            gameP->mode2 = 0;
            gameP->mode3 = 0;
            gameP->mode4 = 0;
            break;
        }

        switch (gameP->mode3)
        {
        case 0: // WEAPON PAGE
            if ((buttonsPressed & PAD_CROSS) != 0)
            {
                gameP->clearedStages ^= (1 << Cursor);
            }
            for (size_t i = 0; i < 8; i++)
            {
                DrawDebugText(19, 5 + i, 0, "%d", (gameP->clearedStages & (1 << i)) != 0);
            }

            if ((buttonsPressed & PAD_DOWN) != 0)
            {
                if (Cursor != 7)
                {
                    Cursor += 1;
                }
                else
                {
                    Cursor = 0;
                }
            }
            else if ((buttonsPressed & PAD_UP) != 0)
            {
                if (Cursor != 0)
                {
                    Cursor -= 1;
                }
                else
                {
                    Cursor = 7;
                }
            }
            DrawDebugText(12,3,2, "WEAPON PAGE");
            DrawDebugText(3, 5 + Cursor, 1, ">");
            DrawDebugText(4, 5, 0, "C-Shot\nDark Hold\nGoo Shaver\nGround Fire\nTr-Thunder\nF-Laser\nSpike Ball\nWingSpiral");
            break;

        case 1: // ARMOR PAGE
            break;

        default:
            break;
        }
        
        if ((buttonsPressed & PAD_R1) != 0)
        {
            if (gameP->mode3 != (PAGE_TOTAL - 1))
            {
                gameP->mode3 += 1;
            }else{
                gameP->mode3 = 0;
            }
            
        }else if ((buttonsPressed & PAD_L1) != 0)
        {
            if (gameP->mode3 != 0)
            {
                gameP->mode3 -= 1;
            }else{
                gameP->mode3 = (PAGE_TOTAL - 1);
            }
        }

        DrawDebugText(28,3,2,"%d/%d",gameP->mode3 + 1, PAGE_TOTAL);
        DrawDebugText(9, 24, 1, "Press Select to\nStart the Stage");
        ThreadSleep(1);
    }
}

#undef PAGE_TOTAL
#undef Cursor
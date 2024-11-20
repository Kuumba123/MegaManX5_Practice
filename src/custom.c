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
            if (gameP->player != 0) //Zero
            {
                gameP->armorType = 5;
            }
            if ((gameP->armorParts & 0xF) == 0xF || gameP->armorType == 2) //Falcon Armor
            {
                gameP->armorParts |= 0xF;
                gameP->armors |= 2;
            }
            if ((gameP->armorParts & 0xF0) == 0xF0 || gameP->armorType == 3) // Gaea Armor
            {
                gameP->armorParts |= 0xF0;
                gameP->armors |= 4;
            }
            if (gameP->armorType == 4) //Ultimate Armor
            {
                gameP->armors |= 8;
            }
            if (gameP->armorType == 1) //4th Armor
            {
                gameP->armors |= 1;
            }
            

            gameP->stageId = gameP->slowMotion;
            gameP->mid = gameP->maverickShow;

            gameP->mode = 7;
            gameP->mode2 = 0;
            gameP->mode3 = 0;
            gameP->mode4 = 0;
            break;
        }

        bool toggle = (buttonsPressed & PAD_CROSS) != 0;

        switch (gameP->mode3)
        {
        case 0: // WEAPON PAGE
            if (toggle)
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
            DrawDebugText(12, 3, 2, "WEAPON PAGE");
            DrawDebugText(3, 5 + Cursor, 1, ">");
            DrawDebugText(4, 5, 0, "C-Shot\nDark Hold\nGoo Shaver\nGround Fire\nTr-Thunder\nF-Laser\nSpike Ball\nWingSpiral");
            break;

        case 1: // ARMOR PAGE
            /*
             * HELMET,ARM,BODY,LEG
             */

            if ((buttonsPressed & PAD_DOWN) != 0)
            {
                if (Cursor != 12)
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
                    Cursor = 12;
                }
            }

            if (Cursor < 8)
            {
                if (toggle)
                {
                    gameP->armorParts ^= (1 << Cursor);
                }
            }
            else if (Cursor == 8)
            {
                if (toggle)
                {
                    gameP->armors ^= 8; // Ultimate Armor
                }
            }
            else if (Cursor == 9)
            {
                if (toggle)
                {
                    gameP->armors ^= 0x10; // Black Zero
                }
            }
            else if (Cursor == 10)
            {
                if (toggle)
                {
                    gameP->player ^= 1;
                }
            }
            else if (Cursor == 11)
            {
                if ((buttonsPressed & PAD_RIGHT) != 0)
                {
                    if (gameP->armorType != 5)
                    {
                        gameP->armorType += 1;
                    }
                    else
                    {
                        gameP->armorType = 0;
                    }
                }
                else if ((buttonsPressed & PAD_LEFT) != 0)
                {
                    if (gameP->armorType != 0)
                    {
                        gameP->armorType -= 1;
                    }
                    else
                    {
                        gameP->armorType = 5;
                    }
                }
            }else{
                if (toggle)
                {
                    gameP->zeroBuster ^= 1;
                }
                
            }

            for (size_t i = 0; i < 8; i++)
            {
                DrawDebugText(19, 5 + i, 0, "%d", (gameP->armorParts & (1 << i)) != 0);
            }
            DrawDebugText(19, 13, 0, "%d\n%d\n%d\n%d\n%d", (gameP->armors & 8) != 0, (gameP->armors & 0x10) != 0, gameP->player,gameP->armorType,gameP->zeroBuster);

            DrawDebugText(12, 3, 2, "ARMOR PAGE");
            DrawDebugText(3, 5 + Cursor, 1, ">");
            DrawDebugText(4, 5, 0, "Falcon-Helmet\nFalcon-Arm\nFalcon-Body\nFalcon-Leg\nGaea-Helmet\nGaea-Arm\nGaea-Body\nGaea-Leg\nUltimate-Armor\nBlack-Zero\nPlayer\nArmor-Type\nZero-Buster");
            break;

        default:
            break;
        }
        int8_t past = gameP->mode3;

        if ((buttonsPressed & PAD_R1) != 0)
        {
            if (gameP->mode3 != (PAGE_TOTAL - 1))
            {
                gameP->mode3 += 1;
            }
            else
            {
                gameP->mode3 = 0;
            }
        }
        else if ((buttonsPressed & PAD_L1) != 0)
        {
            if (gameP->mode3 != 0)
            {
                gameP->mode3 -= 1;
            }
            else
            {
                gameP->mode3 = (PAGE_TOTAL - 1);
            }
        }

        if (past != gameP->mode3)
        {
            Cursor = 0;
        }

        DrawDebugText(28, 3, 2, "%d/%d", gameP->mode3 + 1, PAGE_TOTAL);
        DrawDebugText(9, 24, 1, "Press Select to\nStart the Stage");
        ThreadSleep(1);
    }
}

#undef PAGE_TOTAL
#undef Cursor
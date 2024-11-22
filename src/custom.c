#include <common.h>

#define PAGE_TOTAL 4
#define Cursor gameP->mode3

static int page;

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
        gameP->mode2 = 2;
        gameP->mode3 = 0;
        gameP->mode4 = 0;
    }

    int parts = 0;

    while (true)
    {
        if ((buttonsPressed & PAD_START) != 0)
        {
            if (gameP->player != 0) // Zero
            {
                gameP->armorType = 5;
            }
            if ((gameP->armorParts & 0xF) == 0xF || gameP->armorType == 2) // Falcon Armor
            {
                gameP->armorParts |= 0xF;
                gameP->armors |= 2;
            }
            if ((gameP->armorParts & 0xF0) == 0xF0 || gameP->armorType == 3) // Gaea Armor
            {
                gameP->armorParts |= 0xF0;
                gameP->armors |= 4;
            }
            if (gameP->armorType == 4) // Ultimate Armor
            {
                gameP->armors |= 8;
            }
            if (gameP->armorType == 1) // 4th Armor
            {
                gameP->armors |= 1;
            }
            if (((gameP->collectables & 0x1000)) != 0) // Sub-Tank 1
            {
                gameP->tanksAmmo[0] |= 0x80;
            }
            if (((gameP->collectables & 0x2000)) != 0) // Sub-Tank 2
            {
                gameP->tanksAmmo[1] |= 0x80;
            }

            gameP->equipedParts[gameP->armorType] = parts;
            gameP->parts = parts;

            gameP->stageId = gameP->slowMotion;
            gameP->mid = gameP->maverickShow;

            gameP->mode = 7;
            gameP->mode2 = 0;
            gameP->mode3 = 0;
            gameP->mode4 = 0;
            break;
        }

        bool toggle = (buttonsPressed & PAD_CROSS) != 0;

        switch (page)
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
            }
            else
            {
                if (toggle)
                {
                    gameP->zeroBuster ^= 1;
                }
            }

            for (size_t i = 0; i < 8; i++)
            {
                DrawDebugText(19, 5 + i, 0, "%d", (gameP->armorParts & (1 << i)) != 0);
            }
            DrawDebugText(19, 13, 0, "%d\n%d\n%d\n%d\n%d", (gameP->armors & 8) != 0, (gameP->armors & 0x10) != 0, gameP->player, gameP->armorType, gameP->zeroBuster);

            DrawDebugText(12, 3, 2, "ARMOR PAGE");
            DrawDebugText(3, 5 + Cursor, 1, ">");
            DrawDebugText(4, 5, 0, "Falcon-Helmet\nFalcon-Arm\nFalcon-Body\nFalcon-Leg\nGaea-Helmet\nGaea-Arm\nGaea-Body\nGaea-Leg\nUltimate-Armor\nBlack-Zero\nPlayer\nArmor-Type\nZero-Buster");
            break;

        case 2: // TANK PAGE
            if ((buttonsPressed & PAD_DOWN) != 0)
            {
                if (Cursor != 9)
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
                    Cursor = 9;
                }
            }

            if (Cursor == 0)
            {
                if ((buttonsPressed & PAD_RIGHT) != 0)
                {
                    gameP->mode4 += 1;
                }
                else if ((buttonsPressed & PAD_LEFT) != 0)
                {
                    gameP->mode4 -= 1;
                }
                gameP->mode4 &= 7;

                if (toggle)
                {
                    gameP->collectables2 ^= (1 << gameP->mode4);
                }
            }
            else if (Cursor < 5)
            {
                if (toggle)
                {
                    gameP->collectables ^= (0x1000 << (Cursor - 1));
                }
            }
            else if (Cursor < 8)
            {
                if ((buttonsPressed & PAD_RIGHT) != 0)
                {
                    gameP->tanksAmmo[Cursor - 5] += 1;
                }
                else if ((buttonsPressed & PAD_LEFT) != 0)
                {
                    gameP->tanksAmmo[Cursor - 5] -= 1;
                }
            }

            else if (Cursor == 8)
            {
                if ((buttonsPressed & PAD_RIGHT) != 0)
                {
                    gameP->maxHPs[gameP->player] += 1;
                }
                else if ((buttonsPressed & PAD_LEFT) != 0)
                {
                    gameP->maxHPs[gameP->player] -= 1;
                }
            }
            else
            {
                if ((buttonsPressed & PAD_RIGHT) != 0)
                {
                    gameP->maxAmmos[gameP->player] += 1;
                }
                else if ((buttonsPressed & PAD_LEFT) != 0)
                {
                    gameP->maxAmmos[gameP->player] -= 1;
                }
            }

            DrawDebugText(19, 5, 0, "%d", (gameP->collectables2 & (1 << gameP->mode4)) != 0);

            for (size_t i = 0; i < 4; i++)
            {
                DrawDebugText(19, 6 + i, 0, "%d", (gameP->collectables & (0x1000 << i)) != 0);
                if (i != 3)
                {
                    DrawDebugText(19, 10 + i, 0, "%d", gameP->tanksAmmo[i]);
                }
            }
            DrawDebugText(19, 13, 0, "%d\n%d", gameP->maxHPs[gameP->player], gameP->maxAmmos[gameP->player]);

            DrawDebugText(12, 3, 2, "TANK PAGE");
            DrawDebugText(3, 5 + Cursor, 1, ">");
            DrawDebugText(4, 5, 0, "HEART-%d:\nSUB-TANK 1\nSUB-TANK 2\nWEAPON-TANK\nEX-TANK\nTANK AMMO 1\nTANK AMMO 2\nTANK AMMO 3\nMAX HP\nMAX AMMO", gameP->mode4 + 1);
            break;

        case 3: // PARTS PAGE

            if ((buttonsPressed & PAD_DOWN) != 0)
            {
                if (Cursor != 15)
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
                    Cursor = 15;
                }
            }
            
            if (toggle)
            {
                parts ^= 1 << Cursor;
            }

            DrawDebugText(12, 3, 2, "PARTS PAGE");
            DrawDebugText(3, 5 + Cursor, 1, ">");

            for (size_t i = 0; i < 16; i++)
            {
                DrawDebugText(19, 5 + i, 0, "%d", (parts & (1 << i)) != 0);
            }
            DrawDebugText(4, 5, 0, "Part Name-01\nPart Name-02\nPart Name-03\nPart Name-04\nPart Name-05\nPart Name-06\nPart Name-07\nPart Name-08\nPart Name-09\nPart Name-10\nPart Name-11\nPart Name-12\nPart Name-13\nPart Name-14\nPart Name-15\nPart Name-16");

            break;

        default:
            break;
        }
        int8_t past = page;

        if ((buttonsPressed & PAD_R1) != 0)
        {
            if (page != (PAGE_TOTAL - 1))
            {
                page += 1;
            }
            else
            {
                page = 0;
            }
        }
        else if ((buttonsPressed & PAD_L1) != 0)
        {
            if (page != 0)
            {
                page -= 1;
            }
            else
            {
                page = (PAGE_TOTAL - 1);
            }
        }

        if (past != page)
        {
            Cursor = 0;
            gameP->mode4 = 0;
        }

        DrawDebugText(28, 3, 2, "%d/%d", page + 1, PAGE_TOTAL);
        DrawDebugText(9, 24, 1, "Press Select to\nStart the Stage");
        ThreadSleep(1);
    }
}

#undef PAGE_TOTAL
#undef Cursor
#include <common.h>
#include "practice.h"

/*
0x374 bytes til sector is crossed in ST0D Overlay.
 Also 0x4BC til sector is crossed in ST0D SPRT Arangment data
*/

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);

static const char *stageNames[] = {"SIGMA 1", "SIGMA 2", "SIGMA 3", "SIGMA 4", "Intro", "Dynamo-1", "Dynamo-2"};
static const char *finalStageText[] = {"Start", "POST-Refights"};
static const char *halfText[] = {"Start", "Mid"};
static const char *vistText[] = {"Vist","Re-Vist"};
#define Cursor gameP->refights[2]

void DrawSelectableStages(Game *gameP)
{
    if (gameP->mode4 == 0)
    {
        DrawDebugText(12, 18, 2, stageNames[Cursor]);
    }
    else
    {
        if (gameP->stageId == 0xC)
        {
            DrawDebugText(12, 18, 2, finalStageText[Cursor]);
        }
        else
        {
            if (gameP->mode4 == 1)
            {
                DrawDebugText(12, 18, 2, halfText[Cursor]);
            }
            else
            {
                DrawDebugText(12, 18, 2, vistText[Cursor]);
            }
        }
    }
}

#undef Cursor

void ExitRoutePage(Game *gameP)
{
    gameP->mode2 = 4;
    gameP->mode3 = 0;
    gameP->mode4 = 0;
    PlaySound(5, 0, 0);
}
void ExitStageSelectCheck()
{
    if ((buttonsPressed & PAD_SELECT) != 0)
    {
        CreateTitleScreenThread();
        DeleteThread();
    }
}

static const char *routeText[] = {"ALL STAGES", "ANY%", "100%", "Custom"};

void DrawRoutePage()
{
    if (practice.cancelXA == 0)
    {
        *((int8_t *)0x80017878) = 0x73;
    }
    else
    {
        *((int8_t *)0x80017878) = 0x00;
    }
    DrawDebugText(9, 22, 1, "Press Select to\nReturn to Title Screen");
    DrawDebugText(12,4,2,"CONFIG PAGE");
    DrawDebugText(3, 6 + game.mode4, 1, ">");
    DrawDebugText(4, 6, 0, "ROUTE\nKEEP RNG\nUA-Refights\nCancel XA\nAnalog\nSensitivity");

    DrawDebugText(20, 6, 0, "%s\n%d\n%d\n%d\n%d\n%d", routeText[practice.route], practice.keepRng, practice.ultimateArmor, practice.cancelXA, practice.analog, practice.sensativity);
    ThreadSleep(1);
}
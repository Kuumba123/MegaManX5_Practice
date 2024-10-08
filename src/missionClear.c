#include <common.h>
#include <object.h>

void MissionCleared(Game *gameP)
{
    if (gameP->exitType == 2)
    {
        gameP->mode = 3;
        gameP->clearedStages = 0;
        return;
    }

    if (gameP->mode2 == 0)
    {
        gameP->mode2 = 1;
        FadeIn(9);
        return;
    }
    else if (gameP->mode2 == 1)
    {
        if (fadeDirection != 0)
        {
            return;
        }
        gameP->mode2 = 3;
    }
    else
    {
        while (true)
        {
            int time = gameP->stageTime;
            int totalSeconds = time / 60;
            int minutes = totalSeconds / 60;
            int seconds = totalSeconds % 60;
            int frames = time % 60;

            DrawDebugText(6, 12, 0, "Your Clear Time was - %2d:%2d:%2d", minutes, seconds, frames);
            DrawDebugText(8, 14, 0, "Press Any Button to\nReturn to Stage Select");

            if (buttonsPressed != 0)
            {
                gameP->mode2 = 0;
                gameP->mode = 3;
                gameP->clearedStages = 0;
                return;
            }
            ThreadSleep(1);
        }
    }
}
#include <common.h>
#include <layer.h>

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);

void SkipEnigmaCutscene(Game *gameP)
{
    bgLayers[0].display = false;
    bgLayers[1].display = false;
    bgLayers[2].display = false;
    gameP->mode2 = 0xD;
    *(uint8_t *)((int)gameP + 4) = 0;

    DefaultLayers();
    ClearAll();

}

#define BASE_X 10
#define BASE_Y 10

void ShowEnigmaResults(Game *gameP)
{
    while (true)
    {
        DrawDebugText(8, 14, 0, "Press Any Button to\nReturn to Title Screen");
        DrawDebugText(BASE_X,BASE_Y,0, "ENIGMA-SHOT:");

        if ((*(uint8_t *)((int)gameP + 0xCB) & 0x80) == 0) //Enigma Failed
        {
            DrawDebugText(BASE_X + 13,BASE_Y,2, "Failed");
        }else{
            DrawDebugText(BASE_X + 13,BASE_Y,1, "Succeed");
        }
        ThreadSleep(1);

        if (buttonsPressed != 0)
        {
            ((uint8_t*)0x800d1f28)[0] = 1;
            ((uint8_t*)0x800d1f28)[1] = 0;
            ((uint8_t*)0x800d1f28)[2] = 0;
            ((uint8_t*)0x800d1f28)[3] = 0;
            NewThread(0,0x8001e534);
            DeleteThread();
        }
        
    }
    
}

#undef BASE_X
#undef BASE_Y
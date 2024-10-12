#include <object.h>

static struct Title
{
    uint8_t mode;
    uint8_t mode2;
    uint8_t mode3;
    uint8_t mode4;

    int16_t timer;
};

static struct TitleText
{
    int16_t x;
    int16_t y;
    uint8_t sprite;
    uint8_t clutId;
};

extern struct Title title;

void DrawDebugText(uint16_t x, uint16_t y, uint8_t clut, char *textP, ...);

static void MiscObj_13_1(Object *objP)

{
    int iVar1;
    int iVar2;

    if (objP->stageVar == 6)
    {
        objP->priority = 2;
        iVar2 = (int)(char)title.mode3;
        iVar1 = iVar2;
        if (iVar2 < 0)
        {
            iVar1 = iVar2 + 3;
        }
        *(int16_t *)((int)&objP->y + 2) = ((struct TitleText*)(0x800eebc0))[iVar2].y;
        if (title.mode3 == 1)
        {
            (objP->animeInfo).sprtFrame = 1;
        }
        else
        {
            if ((char)title.mode3 < '\x02')
            {
                if (title.mode3 == 0)
                {
                    (objP->animeInfo).sprtFrame = 0;
                    goto LAB_ST0E_OVL__800eeb08;
                }
            }
            else if (title.mode3 == 2)
            {
                (objP->animeInfo).sprtFrame = 2;
                goto LAB_ST0E_OVL__800eeb08;
            }
            (objP->animeInfo).sprtFrame = 3;
        }
    }
LAB_ST0E_OVL__800eeb08:
    if (objP->stageVar == 5 || objP->stageVar == 4)
    {
        objP->display = 0;
        if ((*(int*)0x8009a420 & 0x10U) != 0)
        {
            DisplayObject(objP);
        }
    }
    else
    {
        objP->display = 1;
        DisplayObject(objP);
    }
    return;
}
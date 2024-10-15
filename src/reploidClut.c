#include <common.h>
#include <object.h>
#include <gpu.h>

void SpawnReploid(Object *objP)
{
    AnimeAdvance(objP);
    if (objP->animeInfo.flag == 1)
    {
        SetAnime(objP, 0);
        CopyClut(0x120 + 3, *((int *)0x1f800030) + 0x320, 1);
        objP->act4 = 0;
        objP->act3 = 5;

        for (size_t i = 0; i < 2; i++)
        {
            Object *p = GetShotObject();

            p->id = 0xB;
            p->flags = 0x61;
            p->stageVar = i;

            *(int16_t *)((int)&p->y + 2) = 0x0240;
            *(int16_t *)((int)&p->x + 2) = posX + (i + 1) * 0x30 + -8;

            p->animeTableP = objP->animeTableP;
            p->texCord = objP->texCord;
            p->sprtDataP = objP->sprtDataP;
            p->clutCord = GetClutCord(0x120);
            p->flip = objP->flip;

            *(int *)((int)p + 0x50) = objP;
        }
    }
}
#include <common.h>
#include <object.h>
#include <layer.h>

extern uint32_t swapTextureFlag;

void SwappedTextureCheck();

static void MatrixSwapTextureObject(Object *objP)
{
    int16_t *piVar2;

    if (!OffScreenCheck(objP))
    {
        piVar2 = (int16_t *)objP->pastX;
        if ((piVar2[6] <= bgLayers[0].y) && (piVar2[6] + 0x24 <= megaY))
        {
            TransferTiles2(0, *piVar2, piVar2[1], (piVar2 + 2));
            if (objP->stageVar == 0)
            {
                bgLayers[0].borderT = piVar2[6];
                bgLayers[0].borderB = piVar2[6];
                SwappedTextureCheck();
                game.point = 1;
            }
            else
            {
                objP->act = 2;
            }
        }
    }else {
        DeleteObject(objP);
    }
}
#include <common.h>

extern uint32_t swapTextureFlag;

void MatrixTextureCheck()
{
    if (game.stageId == 4)
    {
        swapTextureFlag = 1;
    }
    DrawMain();
}
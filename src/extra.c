#include <common.h>
#include <misc.h>

extern uint32_t swapTextureFlag;

void MatrixTextureCheck()
{
    if (game.stageId == 4)
    {
        swapTextureFlag = 1;
    }
    DrawMain();
}
void LoadLevel2()
{
    EndSong();
    LoadLevel();
}
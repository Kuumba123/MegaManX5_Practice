#include <common.h>
#include <misc.h>

extern uint32_t swapTextureFlag;

void LoadBackupScreens();

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
void BackupScreenChck()
{
    if (game.stageId != 0xC || game.point != 3)
    {
        LoadBackupScreens();
    }
}
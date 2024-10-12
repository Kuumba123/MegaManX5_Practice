#include <common.h>


extern int * StageLayoutPointers[];

void LoadLayout()
{
    int *p;

    if (game.stageId == 0xB)
    {
        p = ((int*)0x800EF330)[game.point];
    }else{
        p = StageLayoutPointers[game.stageId * 2 + game.mid];
    }
    *(int*)0x1f800004 = p; //layout pointer

    void (*screenFunc)() = 0x800288b4;
    screenFunc(-1);
}


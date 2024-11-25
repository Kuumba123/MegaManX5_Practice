#include <common.h>
#include "practice.h"

#define Cursor gameP->refights[2]

void DrawSelectableStages(Game *gameP);
void ProcessStagePicker(Game *gameP);

extern bool LevelMidTable[];


static bool stageRevistable[19] = 
{false, true, false, true, false, false, false, true, false, false, false ,false, false, false, false, false, false, false, true};


void StageSelectPick(Game *gameP)
{
    practice.skipRefights = 0;
    if (gameP->stageId != 0xC) // 8 Maverick Level Selected
    {
        if (LevelMidTable[gameP->stageId] && (practice.route == 2 && !stageRevistable[gameP->stageId]))
        {
            gameP->mode3 = 6;
            return;
        }
        else
        {
            //TODO: maybe fix this for revisting level with a 2nd Half

            if (practice.route == 2 && stageRevistable[gameP->stageId])
            {
                gameP->mode4 = 2;
            }else if(LevelMidTable[gameP->stageId]){
                gameP->mode3 = 6;
                return;
            }else{
                gameP->mode4 = 1;
            }
            
        }
    }
    DrawSelectableStages(gameP);
    ProcessStagePicker(gameP);
}

#undef Cursor
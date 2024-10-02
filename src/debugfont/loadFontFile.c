#include <common.h>

#define StartingThread 0x8001e048

static struct DISC_FILE {
    char * fileName;
    int sectorSize;
    int dest;
};

static struct DISC_FILE files[2] = {
    {"\\FONT.BIN;1",4,0x801030a0},
    {"\\STATE.BIN;1",2,0x801F5664}
};

void LoadDebugCodeFile()
{
    CdlFILE cdl;
    u_char results[8];

    for (size_t i = 0; i < 2; i++)
    {
        CdSearchFile(&cdl, files[i].fileName);
        CdControl(CdlSeekL, &cdl.pos, &results);
        CdRead(files[i].sectorSize, files[i].dest, 0x80);
        CdReadSync(0, &results);

        while (CdControl(CdlPause, 0, 0) == 0)
        {
        }
    }
    NewThread(0, StartingThread);
}
#undef StartingThread
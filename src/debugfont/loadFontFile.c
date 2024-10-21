#include <common.h>
#include "../practice.h"

#define StartingThread 0x8001e048

static struct DISC_FILE {
    char * fileName;
    int sectorSize;
    int dest;
};

static struct DISC_FILE files[3] = {
    {"\\FONT.BIN;1",4,0x801030a0},
    {"\\STATE.BIN;1",2,0x801F5664},
    {"\\STATE2.BIN;1",1,0x8009a430}
};

void LoadFile(struct DISC_FILE * file);

void LoadDebugCodeFile()
{
    for (size_t i = 0; i < 3; i++)
    {
        LoadFile(&files[i]);
    }
    practice.sensativity = DEFAULT_SENSITIVITY;
    NewThread(0, StartingThread);
}
#undef StartingThread
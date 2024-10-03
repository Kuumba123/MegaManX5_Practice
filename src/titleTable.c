#include <common.h>

static struct Title
{
    uint8_t mode;
    uint8_t mode2;
    uint8_t mode3;
    uint8_t mode4;

    int16_t timer;
};


void TitleLoop(struct Title *titleP);
void TitleScreen_6_2(struct Title *titleP);

static void * titleTable[] = {0x8001EE44, &TitleLoop, &TitleScreen_6_2};
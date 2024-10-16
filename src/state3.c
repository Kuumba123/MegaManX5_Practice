#include <common.h>
#include "practice.h"

/*Is in ST0C Overlay (0x78 bytes)*/

void LoadSigmaOverlay(int ovl)
{
    EndSong();
    BinSeek(ovl,0x800FA000);
}
void LoadSigmaOverlay2(int ovl)
{
    EndSong();
    BinSeek(ovl,0x800FA000);
}

void SigmaStatePreserve()
{
    practice.sigmaOvl = 1;
    LoadSigmaOverlay(0x26);
}

void SigmaStatePreserve2()
{
    practice.sigmaOvl = 2;
    LoadSigmaOverlay(0x27);
}
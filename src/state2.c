#include <common.h>
#include <gpu.h>
#include <object.h>
#include "practice.h"

extern uint32_t swapTextureFlag;

void *freeAddress[] = {
    -1,         // End of Clut (dynamicly set based off of CLUT Pointer)
    0x8009CF90, // End of Inverted Clut
    0x800A39A0, // End of Backup Clut
    0x801F6000, // Demo Buffer
    0x801F8304, // Just before Thread Stack Memory
    0x801EF254, // End of ARC buffer
    0x8000A000, // Kernal RAM
    0x8000E440  // Kernal RAM
};

void *readAddress[] = {
    -1,         // Clut (dynamicly set based off of CLUT Pointer)
    0x8009A790, // Inverted Clut
    0x80098120, // Weapon Objects
    0x80092090, // Main Objects
    0x80094398, // Shot Objects
    0x80096D98, // Visual Objects
    0x8009E7A0, // Effect Objects
    0x800C6A10, // Item Objects
    0x800D1F40, // Misc Objects
    0x800C94A8, // Layer Objects
    0x8009A0A0, // Mega
    0x8009A1F8, // BG Layers
    0x800D1C00, // Game Info
    0x800D1dC0, // Ride Armor
    0x80091C38, // Text Info
    0x800E8514, // Extra Text Info
    0x80093E64, // Brightness stuff
    0x800D50DC, // More Brightness stuff
    0x8009A42C, // Also More Brightness stuff
    0x80093dd0, // Brightness Table thing
    0x8009A2F8  // After Images
};

void *maverickRefightBssAddresses[] = {
    0x800FB8EC,
    0x800FFC2C,
    0x800FEF98,
    0x800FF054,
    0x80100160,
    0x800FFC50,
    0x800FEEA0,
    0x800FDC88
};

uint8_t maverickRefightBssSizes[] = {
    0x20,
    0x18,
    0xA0,
    0x04,
    0x14,
    0x6C,
    0x04,
    0x14
};


void *stageBssAddresses[] = {
    0x800fb694,0, //ST00
    0x800FDA98,0, //ST01
    0,0x800FA23C, //ST02
    0x80102FD4,0, //ST03
    0x800F595C,0x800FF94C, //ST04
    0x80102014,0, //ST05
    0,0x800FB618, //ST06
    0x800FC1C4,0, //ST07
    0x800FA054,0, //ST08
    0x800f2100,0, //ST09
    0x800f20a4,0, //ST0A
    0,0,          //ST0B
    0,0,          //ST0C
    0,0,          //ST0D
    0,0,          //ST0E
    0,0,          //ST0F
    0x800FA680,0, //ST10
    0,0,          //ST11
    0x800FEA1C,0  //ST12
};

uint8_t *stageBssSizes[] = {
    0x08,0, //ST00
    0x18,0, //ST01
    0,0x18, //ST02
    0xC8,0, //ST03
    0x0C,0x04, //ST04
    0x14,0, //ST05
    0,0x6C, //ST06
    0x08,0, //ST07
    0x14,0, //ST08
    0x04,0, //ST09
    0x04,0, //ST0A
    0,0,    //ST0B
    0,0,    //ST0C
    0,0,    //ST0D
    0,0,    //ST0E
    0,0,    //ST0F
    0x04,0, //ST10
    0,0, //ST11
    0x40,0 //ST12
};

int16_t freeAddressSizes[] = {
    0x1800,
    0x1800,
    0x1800,
    0x2000,
    0x65FC,
    0x3DB0 - 4,
    0x1900,
    0x1BC0
};

int16_t addressesSize[] = {
    0x2800, // Clut
    0x2800, // Inverted Clut
    0x1380, // Weapon Objects
    0x1D40, // Main Objects
    0x2A00, // Shot Objects
    0x1380, // Visual Objects
    0x1200, // Effect Objects
    0x2900, // Item Objects
    0x3000, // Misc Objects
    0xC0,   // Layer Objects
    0x158,  // Mega
    0xFC,   // BG Layers
    0x1B8,  // Game Info
    0xC0,   // Ride Armor
    0x18,   // Text Info
    0x104,  // Extra Text Info
    8,      // Brightness stuff
    4,      // More Brightness stuff
    4,      // Also More Brightness stuff
    0x28,   // Brightness Table thing
    0x120   // After Images
};

int8_t maxCheckPoint[] = 
{
    3,0, //ST00
    4,0, //ST01
    1,2, //ST02
    6,0, //ST03
    2,1, //ST04
    4,0, //ST05
    1,3, //ST06
    7,0, //ST07
    4,0, //ST08
    0,0, //ST09
    0,0, //ST0A
    0,0, //ST0B
    0x15,0, //ST0C
    0,0, //ST0D
    0,0, //ST0E
    0,0, //ST0F
    3,0, //ST10
    3,0, //ST11
    2,0, //ST12
    0,0,
    0,0,
    0,0,
    1,0  //ST16
};

static RECT rect = {0, 500, 256, 12};

void SaveQuadObjects()
{
    LoadImage2(&rect, 0x8009F9A0);
}
void RestoreQuadObjects()
{
    StoreImage2(&rect, 0x8009F9A0);
}

void SwappedTextureCheck()
{
    if (practice.page == 0)
    {
        swapTextureFlag = 1;
    }
}
void RoseTextureCheck(Object *objP)
{
    if (game.stageId == 7 && objP->stageVar == 0x85)
    {
        if (practice.page == 0)
        {
            swapTextureFlag = 1;
        }
        DeleteObject3(objP);
    }else
    {
        swapTextureFlag = 1;
        DeleteObject3(objP);
    }
    
}
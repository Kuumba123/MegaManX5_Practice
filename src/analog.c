#include <common.h>
#include <libpad.h>
#include "practice.h"

typedef struct
{
    uint8_t status;
    uint8_t type;
    uint8_t button1;
    uint8_t button2;
    uint8_t analog0;
    uint8_t analog1;
    uint8_t analog2;
    uint8_t analog3;
} Controller_Data;

extern Controller_Data controller;
extern Controller_Data controller2;

void ReadPad(void)
{
    uint uVar1;
    int iVar2;
    int iVar3;
    uint8_t bVar4;
    ushort uVar5;
    ushort uVar6;
    uint uVar7;

    if ((controller.status == 0xff) ||
        (((bVar4 = controller.type & 0xf0, bVar4 != 0x40 && (bVar4 != 0x70)) && (bVar4 != 0x50))))
    {
        uVar5 = 0;
    }
    else
    {
        /* Get Pad Inputs (Player 1) */
        uVar1 = ~(uint)((controller.button1 << 8) | controller.button2);
        uVar7 = uVar1 & 0xffff;
        if (bVar4 == 0x50)
        {
            uVar7 = (uVar1 & 2) << 1;
            if ((uVar1 & 0x80) != 0)
            {
                uVar7 = uVar7 | 2;
            }
            if ((uVar1 & 4) != 0)
            {
                uVar7 = uVar7 | 0x80;
            }
            if ((uVar1 & 8) != 0)
            {
                uVar7 = uVar7 | 0x10;
            }
            if ((uVar1 & 0x10) != 0)
            {
                uVar7 = uVar7 | 8;
            }
            uVar7 = uVar1 & 0xff61 | uVar7;
            
        }else if (bVar4 == 0x70 && practice.analog) //Analog
        {
            if (controller.analog2 >= (practice.sensativity + 0x80))
            {
                uVar7 |= PAD_RIGHT;
            }else if (controller.analog2 <= (0x80 - practice.sensativity))
            {
                uVar7 |= PAD_LEFT;
            }
            
            if (controller.analog3 >= (practice.sensativity + 0x80))
            {
                uVar7 |= PAD_DOWN;
            }else if (controller.analog3 <= (0x80 - practice.sensativity))
            {
                uVar7 |= PAD_UP;
            }
        }
        
        uVar5 = buttonsHeldPast;
        if (((uVar7 & 0xa000) != 0xa000) && (uVar5 = (uint16_t)uVar7, (uVar7 & 0x5000) == 0x5000))
        {
            uVar5 = buttonsHeldPast;
        }
    }
    buttonsHeldPast = buttonsHeld;
    buttonsPressed = uVar5 & (uVar5 ^ buttonsHeld);
    buttonsHeld = uVar5;


    iVar2 = PadInfoMode(0, 2, 0);
    iVar3 = PadGetState(0);
    if (iVar3 == 1)
    {
        *((int *)0x8006fb24) = 0;
    }
    if ((*((int *)0x8006fb24) == 0) &&
        ((PadSetAct(0, (int *)0x8006fb20, 2), iVar3 == 2 ||
                                                  ((iVar3 == 6 && (((iVar2 == 4 || (iVar2 == 7)) && (iVar2 = PadSetActAlign(0, (char *)0x8006fb18), iVar2 != 0))))))))
    {
        *((int *)0x8006fb24) = 1;
    }
    return;
}

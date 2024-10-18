#include <stdint.h>

static struct TitleText{
    int16_t x;
    int16_t y;
    uint8_t sprite;
    uint8_t clutId;
};

static struct TitleText text[] = {
    {160, 134, 0, 0}, // GAME START
    {160, 150, 1, 0}, // CONTINUE
    {160, 166, 2, 0}, // OPTION
    {160, 182, 3, 0}, // TRAINING
    {160, 160, 4, 1}, // PRESS START BUTTON
    {152, 196, 5, 1}, // PRESS START or X BUTTON
    {160, 134, 0, 1}  // GAME START (Selected)
};

#ifndef PRACTICE_H
#define PRACTICE_H
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint32_t textureFlag;
    uint8_t pastBright;
    uint8_t songSeekFlag;

    bool made;
}State;


typedef struct{
    State state;
    uint8_t route;
}Practice;

extern Practice practice;

#endif
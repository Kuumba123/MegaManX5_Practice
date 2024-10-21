#ifndef PRACTICE_H
#define PRACTICE_H
#include <stdint.h>
#include <stdbool.h>

#define MAX_SENSITIVITY 127
#define MIN_SENSITIVITY 16
#define DEFAULT_SENSITIVITY 32

typedef struct
{
    uint8_t bss[0xA0];
    uint32_t textureFlag;
    void * arcP;
    void * backupArcP;
    uint32_t startSelectEnable;
    int32_t screenSize; //in bytes
    uint16_t rng;
    uint8_t sigmaOvl;
    uint8_t pastBright;
    uint8_t songSeekFlag;
    uint8_t reloadFlag;
    uint8_t page;
    bool made;
}State;


typedef struct{
    State state;
    uint8_t page;
    uint8_t sigmaOvl;
    uint8_t route;
    bool keepRng;
    uint8_t skipRefights;
    bool ultimateArmor;
    bool cancelXA;
    bool analog;
    uint8_t sensativity;
}Practice;

extern Practice practice;

#endif
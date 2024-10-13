#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint8_t bss[0xA0];
    uint32_t textureFlag;
    void * arcP;
    void * backupArcP;
    uint8_t pastBright;
    uint8_t songSeekFlag;
    uint8_t reloadFlag;
    uint8_t page;
    bool made;
}State;



typedef struct{
    State state;
    uint8_t page;
    uint8_t route;
    uint8_t skipRefights;
}Practice;

Practice practice;
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint32_t textureFlag;
    uint8_t pastBright;
    uint8_t songSeekFlag;

    uint8_t page;
    bool made;
}State;



typedef struct{
    State state;
    uint8_t page;
    uint8_t route;
}Practice;

Practice practice;
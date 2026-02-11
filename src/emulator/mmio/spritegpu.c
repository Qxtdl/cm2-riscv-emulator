#ifdef RAYLIB

#include <raylib.h>
#include <string.h>
#include <stdint.h>

#include "mmio_map.h"

void SpriteGpu_Tick(void) {
    BeginDrawing();

    //DrawRectangle(0, 0, 64, 64, BLACK);

    EndDrawing();
}

bool SpriteGpu_OffsetInteraction(uint32_t offset) {
    if (offset == SPRITEGPU_SPRITE) return true;
    if (offset == SPRITEGPU_WRITE) return true;
    return false;
}

void SpriteGpu_ByteStore(uint32_t offset, uint32_t value) {

}

void SpriteGpu_HalfStore(uint32_t offset, uint32_t value) {
    
}

#endif
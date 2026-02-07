//dont compile if raylib is not defined
#ifdef RAYLIB

#include <raylib.h>
#include <string.h>
#include <stdint.h>

#include "mmio_map.h"

#define SCREEN_Y_SIZE 256
#define SCREEN_X_SIZE 256
int scale = 4;

Texture tilesheet;

void TileGpu_Init(const char *bitmap_filename) {
    InitWindow(SCREEN_X_SIZE * scale, SCREEN_Y_SIZE * scale, "TileGPU");
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    Image image = LoadImage(bitmap_filename);
    tilesheet = LoadTextureFromImage(image);
    UnloadImage(image);
}

static uint8_t tilegpu_x;
static uint8_t tilegpu_y;
static uint16_t tilegpu_addr;
static uint8_t tilegpu_fx_opcode;
static uint16_t tilegpu_fx_imm;

static uint16_t framebuffer[SCREEN_Y_SIZE][SCREEN_X_SIZE] = {0};

void TileGpu_Tick(void) {
    BeginDrawing();

    for (int i = 0; i < SCREEN_Y_SIZE - 1; i++) {
        for (int j = 0; j < SCREEN_X_SIZE - 1; j++) {
            Rectangle sourceTile = { framebuffer[i][j] * 8, ((framebuffer[i][j]) >> 5) * 8, (float)8, (float)8 };
            Rectangle dest = {0.0f, 0.0f, (float)tilesheet.width/scale, (float)tilesheet.height/scale};
            Vector2 origin = {-j * 64, -i * 64};

            DrawTexturePro(
                tilesheet,
                sourceTile,
                dest,
                origin,
                0,
                WHITE);
        }
    }

    EndDrawing();
}

bool TileGpu_OffsetInteraction(uint32_t offset) {
    if (offset == TILEGPU_X) return true;
    if (offset == TILEGPU_Y) return true;
    if (offset == TILEGPU_ADDR) return true;
    if (offset == TILEGPU_FX_OPCODE) return true;
    if (offset == TILEGPU_FX_IMM) return true;
    if (offset == TILEGPU_CONTROLS) return true;
    return false;
}

static void draw(uint8_t controls) {
    if (controls == TILEGPU_DRAW) {
        framebuffer[tilegpu_y][tilegpu_x] = tilegpu_addr;
    }
    else if (controls == TILEGPU_CLEAR) {
        memset(framebuffer, 0, SCREEN_X_SIZE * SCREEN_Y_SIZE);
        ClearBackground(BLACK);
    }
}


void TileGpu_ByteStore(uint32_t offset, uint32_t value) {
    switch (offset) {
        case TILEGPU_X: tilegpu_x = value; break;
        case TILEGPU_Y: tilegpu_y = value; break;
        case TILEGPU_FX_OPCODE: tilegpu_fx_opcode = value; break;
        case TILEGPU_CONTROLS: draw(value); break;
    }
}

void TileGpu_HalfStore(uint32_t offset, uint32_t value) {
    switch (offset) {
        case TILEGPU_ADDR: tilegpu_addr = value; break;
        case TILEGPU_FX_IMM: tilegpu_fx_imm = value; break;
    }
}

#endif

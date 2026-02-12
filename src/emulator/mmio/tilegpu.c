#ifdef RAYLIB

#include <raylib.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "../../util.h"
#include "mmio_map.h"
#include "../screen.h"

Texture tilesheet;

void TileGpu_Init(const char *bitmap_filename) {
    Image image = LoadImage(bitmap_filename);
    tilesheet = LoadTextureFromImage(image);
    UnloadImage(image);
}

static uint8_t tilegpu_x;
static uint8_t tilegpu_y;
static uint16_t tilegpu_addr;
static uint8_t tilegpu_fx_opcode;
static uint16_t tilegpu_fx_imm;

static struct drawcall {
    uint8_t x;
    uint8_t y;
    uint16_t tile_id;
} *drawcalls = NULL;
static size_t drawcalls_size = 0;

static void push_drawcall(struct drawcall drawcall) {
    drawcalls = srealloc(drawcalls, ++drawcalls_size * sizeof(struct drawcall));
    drawcalls[drawcalls_size - 1] = drawcall;
}

static struct drawcall pop_drawcall(void) {
    static size_t pop_index = 0;
    if (pop_index == drawcalls_size) {
        drawcalls = NULL;
        drawcalls_size = 0;
        pop_index = 0;
        free(drawcalls);
        return (struct drawcall){};
    }
    return drawcalls[pop_index++];
}

void TileGpu_Tick(void) {
    if (drawcalls_size >= 1) {
        BeginDrawing();

        struct drawcall drawcall;
        while (1) {
            drawcall = pop_drawcall();
            if (drawcalls_size == 0) break;

            Rectangle sourceTile = { drawcall.tile_id * 8, ((drawcall.tile_id) >> 5) * 8, (float)8, (float)8};
            Rectangle dest = {0.0f, 0.0f, (float)tilesheet.width/(scale*8), (float)tilesheet.height/(scale*8)};
            Vector2 origin = {(-drawcall.x * 32)/scale, (-drawcall.y * 32)/scale};

            DrawTexturePro(
                tilesheet,
                sourceTile,
                dest,
                origin,
                0,
                WHITE);
        }

        EndDrawing();
    }
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
        push_drawcall((struct drawcall){.x = tilegpu_x, .y = tilegpu_y, .tile_id = tilegpu_addr});
    }
    else if (controls == TILEGPU_CLEAR) {
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

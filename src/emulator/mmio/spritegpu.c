#ifdef RAYLIB

#include <raylib.h>
#include <string.h>
#include <stdint.h>
#include <dirent.h>

#include "../../global.h"
#include "../../util.h"
#include "mmio_map.h"
#include "../screen.h"

Texture *sprites = NULL;
size_t sprites_size = 0;

void SpriteGpu_Init(const char *sprite_folder) {
    struct dirent *de;
    DIR *dr = opendir(sprite_folder);

    if (dr == NULL) {
        app_abort("SpriteGpu_Init()", "Could not open current directory");
    }

    de = readdir(dr);
    de = readdir(dr);
    for (size_t i = 0; ((de = readdir(dr)) != NULL); i++) {
        sprites = srealloc(sprites, ++sprites_size * sizeof(Texture));
        char buf[PATH_MAX];
        snprintf(buf, sizeof(buf), "%s/%s", sprite_folder, de->d_name);
        Image image = LoadImage(buf);
        sprites[i] = LoadTextureFromImage(image);
        UnloadImage(image);        
    }

    closedir(dr);    
}

static uint16_t spritegpu_id;

static struct drawcall {
    uint16_t sprite_id;
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

void SpriteGpu_Tick(void) {
    BeginDrawing();

    if (drawcalls_size >= 1) {
        struct drawcall drawcall;
        while (1) {
            drawcall = pop_drawcall();
            if (drawcalls_size == 0) break;
            
            if (drawcall.sprite_id > sprites_size) drawcall.sprite_id = 0;

            Rectangle sourceTile = { 0, 0, (float)sprites[drawcall.sprite_id].width, (float)sprites[drawcall.sprite_id].height};
            Rectangle dest = {0.0f, 0.0f, (float)sprites[drawcall.sprite_id].width/(scale), (float)sprites[drawcall.sprite_id].width/(scale)};
            Vector2 origin = {-scale, -scale};

            DrawTexturePro(
                sprites[drawcall.sprite_id],
                sourceTile,
                dest,
                origin,
                0,
                WHITE);
        }
    }

    EndDrawing();
}

bool SpriteGpu_OffsetInteraction(uint32_t offset) {
    if (offset == SPRITEGPU_SPRITE) return true;
    if (offset == SPRITEGPU_WRITE) return true;
    return false;
}

static void draw(void) {
    push_drawcall((struct drawcall){.sprite_id = spritegpu_id});
}

void SpriteGpu_ByteStore(uint32_t offset, uint32_t value) {
    switch (offset) {
        case SPRITEGPU_SPRITE: spritegpu_id = value; break;
        case SPRITEGPU_WRITE: draw();
    }
}

void SpriteGpu_HalfStore(uint32_t offset, uint32_t value) {
    
}

#endif
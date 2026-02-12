#include <raylib.h>

#include "screen.h"

float scale = 0.4;

void Screen_Init(void) {
    InitWindow(SCREEN_X_SIZE/scale, SCREEN_Y_SIZE/scale, "Screen");
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
}
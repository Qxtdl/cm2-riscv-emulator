#pragma once

#include <stdbool.h>
#include <ncurses.h>

typedef struct {
    WINDOW *window;
    const char *name;
    int width, height;
    bool dirty;

    // for internal functions
    int ch_x;
    int ch_y;
} window_t;

void create_window(const char *name, int height, int width, int y, int x);
void update_window(const char *name);
void update_windows(void);
window_t *find_window(const char *name);
WINDOW *get_window(const char *name);
void window_putc(const char *name, char ch);
void window_puts(const char *name, const char *s);

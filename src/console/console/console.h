#pragma once

#include <ncurses.h>

#include "window.h"

extern enum focused_window {
    FOCUSED_WINDOW_TTY,
    FOCUSED_WINDOW_DBG_CONSOLE,
    FOCUSED_WINDOW_MAX
} current_focused_window;

extern WINDOW *tty_window;
extern WINDOW *dbg_console_window;

extern bool tty_window_dirty;
extern bool dbg_console_window_dirty;

void console_create_windows(void);
void console_tick(void);
int console_window_kbhit(enum focused_window window);
int console_window_getch(enum focused_window window);
char *console_window_getstr(enum focused_window focused_window);

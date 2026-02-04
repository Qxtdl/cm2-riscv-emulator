#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "console.h"

void debug_console_tick(void) {
    const char *cmd = console_window_getstr(FOCUSED_WINDOW_DBG_CONSOLE);
    if (!cmd) return;

    if (!strcmp(cmd, "exit")) {
        endwin();
        exit(0);
    }

    mvwprintw(dbg_console_window, 1, 1, cmd);
}
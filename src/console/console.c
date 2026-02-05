#include <ncurses.h>

#include "console.h"

enum focused_window current_focused_window;

WINDOW *tty_window;
bool tty_window_dirty;

WINDOW *dbg_console_window;
bool dbg_console_window_dirty;

void console_create_windows(void) {
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);

    tty_window = newwin(8 + 2, 32 + 2, 0, 0);
    dbg_console_window = newwin(32, 32, 0, 32 + 2);
    
    box(tty_window, 0, 0);
    box(dbg_console_window, 0, 0);
    wrefresh(tty_window);
    wrefresh(dbg_console_window);
    current_focused_window = FOCUSED_WINDOW_TTY;
    tty_window_dirty = true;
    dbg_console_window_dirty = true;
}

static bool iskbhit;
static char ch;

#define ctrl(x) ((x) & 0x1f)

static void console_process_char(void) {
    switch (ch) {
        case ctrl('w'):
            iskbhit = false;
            current_focused_window++;
            if (current_focused_window == FOCUSED_WINDOW_MAX) current_focused_window = 0;
            ch = ERR;
            break;
    }
}

void console_tick(void) {
    if (tty_window_dirty) {
        box(tty_window, 0, 0);
        wnoutrefresh(tty_window); //first we schedule the windows for update
    }
    if (dbg_console_window_dirty) {
        box(dbg_console_window, 0, 0);
        wnoutrefresh(dbg_console_window);
    }
    if (tty_window_dirty || dbg_console_window_dirty) {
        doupdate(); //then we redraw the console
    }
    tty_window_dirty = false;
    dbg_console_window_dirty = false;

    ch = getch();
    if (ch != ERR) {
        iskbhit = true;
        console_process_char();
    }
}

int console_window_kbhit(enum focused_window window) {
    if (iskbhit && current_focused_window == window) return 1;
    return 0;
}

int console_window_getch(enum focused_window window) {
    if (!iskbhit) return ERR;
    if (current_focused_window == window) {
        iskbhit = false;
        return ch;
    }
    return ERR;
}



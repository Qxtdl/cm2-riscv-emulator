#include <ncurses.h>
#include <string.h>

#include "../../util.h"
#include "window.h"

window_t *windows = NULL;
int windows_size = 0;

void create_window(const char *name, int height, int width, int y, int x) {
    windows = srealloc(windows, ++windows_size * sizeof(window_t));

    memset(&windows[windows_size - 1], 0, sizeof(window_t));
    windows[windows_size - 1].name = name;
    windows[windows_size - 1].height = height;
    windows[windows_size - 1].width = width;
    windows[windows_size - 1].window = newwin(height, width, y, x);
    windows[windows_size - 1].dirty = true;
    windows[windows_size - 1].ch_x = 1;
    windows[windows_size - 1].ch_y = 1;
}

void update_window(const char *name) {
    window_t *window = find_window(name);
    box(window->window, 0, 0);
    mvwprintw(window->window, 0, 1, name);
    wnoutrefresh(window->window);
    doupdate();
}

void update_windows(void) {
    for (int i = 0; i < windows_size; i++) {
        if (windows[i].dirty) {
            box(windows[i].window, 0, 0);
            mvwprintw(windows[i].window, 0, 1, windows[i].name);
            wnoutrefresh(windows[i].window);
            windows[i].dirty = false;
        }
    }
    doupdate();
}

window_t *find_window(const char *name) {
    for (int i = 0; i < windows_size; i++) {
        if (!strcmp(windows[i].name, name)) return &windows[i];
    }
    return NULL;
}

WINDOW *get_window(const char *name) {
    return find_window(name)->window;   
}

// TODO: these should be in a windowutil.c
void window_putc(const char *name, char ch) {
    window_t *window = find_window(name);
    if (ch == '\n') { 
        window->ch_y++;
        window->ch_x = 1;
        return;
    }
    mvwaddch(window->window, window->ch_y, window->ch_x++, ch); 
    window->dirty = true;
}

void window_puts(const char *name, const char *s) {
    window_t *window = find_window(name);
    mvwprintw(window->window, window->ch_y++, 1, s);
    window->dirty = true;
}

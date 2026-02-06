#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "console.h"
#include "breakpoint.h"

int puts_y = 1;
int puts_x = 1;

#define CMD_SIZE 64
char dbg_console_buffer[CMD_SIZE];
uint8_t dbg_console_buffer_size;
bool dbg_console_cmd;

void debug_console_puts(const char *s) {
    wmove(dbg_console_window, puts_y, 1);
    mvwprintw(dbg_console_window, puts_y++, 1, s);
    puts_x = 1;
    dbg_console_window_dirty = true;
}

void debug_console_putc(char c) {
    wmove(dbg_console_window, puts_y, puts_x++);
    waddch(dbg_console_window, c);
    dbg_console_window_dirty = true;
}

void handle_command(char *cmd) {
    if (!strcmp(cmd, "exit")) {
        endwin();
        exit(0);
    }
    else if (!strcmp(cmd, "clear")) {
        puts_y = 1;
        puts_x = 1;
        wclear(dbg_console_window);
    }
    else if (!strcmp(cmd, "break pop")) {
        breakpoint_pop_cmd();
    }
    else if (!strcmp(cmd, "break continue")) {
        break_continue_cmd();
    }
    else if (!strncmp(cmd, "break", 5)) {
        breakpoint_cmd(cmd);
    }
}



void debug_console_tick(void) {
    breakpoint_tick();
    
    char ch = console_window_getch(FOCUSED_WINDOW_DBG_CONSOLE);
    if (ch != ERR) {
        if (ch == '\r' || ch == '\n') {
            dbg_console_buffer[dbg_console_buffer_size] = '\0'; //make sure there is a null terminator
            puts_x = 1;
            puts_y++;
            debug_console_putc(' ');
            handle_command(dbg_console_buffer);
            dbg_console_buffer_size = 0;
        } else {
            uint8_t tmp = dbg_console_buffer_size;
            
            if (tmp < (CMD_SIZE - 1)) {
                debug_console_putc(ch);
                dbg_console_buffer[tmp++] = ch;
                dbg_console_buffer_size = tmp;
            }
        }
    }

    
}

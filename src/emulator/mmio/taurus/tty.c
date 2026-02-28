#include <ncurses.h>
#include <string.h>

#include "../../../util.h"

#include "tty.h"
#include "mmio_map.h"

#include "../../../console/console/console.h"

#define TTY_AMOUNT 2

struct tty {
    uint32_t user_ready_offset;
    uint32_t user_ascii_offset;
    uint32_t tty_loc_offset;
    uint32_t tty_char_offset;
    uint32_t tty_write_offset;
    uint32_t tty_clear_offset;

    uint8_t tty_loc;
    uint8_t tty_char;
    uint8_t user_ascii;
    uint8_t user_ready;

    const char *window_name;
    enum focused_window focused_window;
} *ttys = NULL;

void Tty_Init(void) {
    ttys = scalloc(TTY_AMOUNT, sizeof(struct tty));
    memcpy(&ttys[0], &(struct tty){
        USER_READY,
        USER_ASCII,
        TTY_LOC,
        TTY_CHAR,
        TTY_WRITE,
        TTY_CLEAR,
        .focused_window = FOCUSED_WINDOW_TTY,
        .window_name = "tty"
    }, sizeof(struct tty));
    memcpy(&ttys[1], &(struct tty){
        EM_ONLY_TTY2_USER_READY,
        EM_ONLY_TTY2_USER_ASCII,
        EM_ONLY_TTY2_LOC,
        EM_ONLY_TTY2_CHAR,
        EM_ONLY_TTY2_WRITE,
        EM_ONLY_TTY2_CLEAR,
        .window_name = "tty2",
        .focused_window = FOCUSED_WINDOW_TTY2
    }, sizeof(struct tty));
}

static void draw(struct tty *tty) {
    wmove(find_window(tty->window_name)->window, ((tty->tty_loc >> 5) & 0x07) + 1, (tty->tty_loc & 0x1f) + 1);
    waddch(find_window(tty->window_name)->window, tty->tty_char);
    find_window(tty->window_name)->dirty = true;
}

void Tty_Tick(void) {
    for (int i = 0; i < TTY_AMOUNT; i++) {
        if (console_window_kbhit(ttys[i].focused_window)) {
            ttys[i].user_ascii = console_window_getch(ttys[i].focused_window);
            ttys[i].user_ready = true;
        }
    }
}

bool Tty_OffsetInteraction(uint32_t offset) {
    for (int i = 0; i < TTY_AMOUNT; i++) {
        if (offset == ttys[i].user_ready_offset) return true;
        if (offset == ttys[i].user_ascii_offset) return true;
        if (offset == ttys[i].tty_loc_offset) return true;
        if (offset == ttys[i].tty_char_offset) return true;
        if (offset == ttys[i].tty_write_offset) return true;
        if (offset == ttys[i].tty_clear_offset) return true;
    }
    return false;
}

uint32_t Tty_ByteLoad(uint32_t offset) {
    for (int i = 0; i < TTY_AMOUNT; i++) {
        if (offset == ttys[i].user_ready_offset) {
            if (ttys[i].user_ready == false) {
                return ttys[i].user_ready;
            }
            if (ttys[i].user_ready == true) {
                ttys[i].user_ready = false;
                return !ttys[i].user_ready;
            }
        }
        else if (offset == ttys[i].user_ascii_offset) {
            return ttys[i].user_ascii;
        }
        else if (offset == ttys[i].tty_loc_offset) {
            return ttys[i].tty_loc;
        }
    }
    return 0;
}

void Tty_ByteStore(uint32_t offset, uint32_t value) {
    for (int i = 0; i < TTY_AMOUNT; i++) {
        if (offset == ttys[i].tty_loc_offset) ttys[i].tty_loc = value;
        else if (offset == ttys[i].tty_char_offset) ttys[i].tty_char = value;
        else if (offset == ttys[i].tty_write_offset) draw(&ttys[i]);
        else if (offset == ttys[i].tty_clear_offset) wclear(find_window(ttys[i].window_name)->window);
    }
}

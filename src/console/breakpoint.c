#include <string.h>

#include "../util.h"
#include "console/console.h"
#include "debug.h"
#include "../emulator/rv32izicsr.h"

extern void cpu_step(void);

extern struct RV32IZicsr_State state;
extern bool cpu_running;

uint32_t *breakpoints = NULL;
size_t breakpoints_size = 0;

void breakpoint_tick(void) {
    if (!cpu_running) return;
    for (size_t i = 0; i < breakpoints_size; i++) {
        if (state.pc == breakpoints[i]) {
            cpu_running = false;
            window_puts("debug", "Breakpoint hit");
        }
    }
}

void breakpoint_cmd(char *arg) {
    breakpoints = srealloc(breakpoints, ++breakpoints_size * sizeof(uint32_t));
    if (!strcmp(arg, "pc")) breakpoints[breakpoints_size - 1] = state.pc;
    else breakpoints[breakpoints_size - 1] = str_literal_to_ul(arg);
    window_puts("debug","Breakpoint added");
    find_window("debug")->dirty = true;
}

void breakpoint_pop_cmd(void) {
    if (breakpoints_size == 0) {
        window_puts("debug","Break stack empty.");
        find_window("debug")->dirty = true;
        return;
    }
    if (breakpoints_size == 1) { breakpoints = NULL; breakpoints_size--; }
    else breakpoints = srealloc(breakpoints, --breakpoints_size * sizeof(uint32_t));
}

void breakpoint_ls_cmd(void) {
    for (size_t i = 0; i < breakpoints_size; i++) {
        window_puts("debug", u32_to_hex(breakpoints[i]));
    }
}

void breakpoint_step_cmd(char *arg) {
    for (unsigned long i = 0; i < str_literal_to_ul(arg); i++) {
        cpu_step();
    }
}

void break_continue_cmd(void) {
    cpu_running = true;
}

void handle_break_command(char *cmd) {
    strtok(cmd, " ");
    char *subcommand = strtok(NULL, " ");

    if (!strncmp(subcommand, "pop", 3)) {
        breakpoint_pop_cmd();
    }
    else if (!strncmp(subcommand, "ls", 2)) {
        breakpoint_ls_cmd();
    }
    else if (!strncmp(subcommand, "step", 4)) {
        breakpoint_step_cmd(strtok(NULL, " "));
    }
    else if (!strncmp(subcommand, "continue", 8)) {
        break_continue_cmd();
    }
    else {
        breakpoint_cmd(subcommand);
    }
}

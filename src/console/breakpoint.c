#include <string.h>

#include "../util.h"
#include "console/console.h"
#include "../emulator/rv32izicsr.h"

extern void cpu_step(void);

extern struct RV32IZicsr_State state;
extern bool cpu_running;

struct {
	uint32_t address;
	enum {
		BREAKPOINT_TYPE_PC,
		BREAKPOINT_TYPE_MEM
	} type;
} *breakpoints = NULL;
size_t breakpoints_size = 0;

void breakpoint_tick(void) {
    if (!cpu_running) return;
    for (size_t i = 0; i < breakpoints_size; i++) {
        if (breakpoints[i].type == BREAKPOINT_TYPE_PC && state.pc == breakpoints[i].address) {
            cpu_running = false;
            window_puts("debug", "PC breakpoint hit");
        }
        else if (breakpoints[i].type == BREAKPOINT_TYPE_MEM && interacted_address == breakpoints[i].address) {
        	cpu_running = false;
			window_puts("debug", "Memory breakpoint hit");
        }
    }
}

static void breakpoint_cmd(char *arg) {
    breakpoints = srealloc(breakpoints, ++breakpoints_size * sizeof(*breakpoints));

	if (!strcmp(arg, "pc")) {
		breakpoints[breakpoints_size - 1].type = BREAKPOINT_TYPE_PC;
	}
	else if (!strcmp(arg, "mem")) {
		breakpoints[breakpoints_size - 1].type = BREAKPOINT_TYPE_MEM;
	}
    
    char *address = strtok(NULL, " ");
    if (!strcmp(address, "pc")) breakpoints[breakpoints_size - 1].address = state.pc;
    else if (!strcmp(address, "ia")) breakpoints[breakpoints_size - 1].address = interacted_address;
    else breakpoints[breakpoints_size - 1].address = str_literal_to_ul(address);
    
    window_puts("debug","Breakpoint added");
    find_window("debug")->dirty = true;
}

static void breakpoint_pop_cmd(char *arg) {
    if (breakpoints_size == 0) {
        window_puts("debug","Break stack empty.");
        find_window("debug")->dirty = true;
        return;
    }
    if (arg && !strcmp(arg, "all")) {
    	breakpoints = NULL;
    	breakpoints_size = 0;
    	return;
    }
    for (unsigned long i = 0; i < (arg ? str_literal_to_ul(arg) : 1); i++) {
    	if (breakpoints_size == 1) { breakpoints = NULL; breakpoints_size--; }
    	else breakpoints = srealloc(breakpoints, --breakpoints_size * sizeof(*breakpoints));
    }
}

static void breakpoint_ls_cmd(void) {
    for (size_t i = 0; i < breakpoints_size; i++) {
    	char buf[128];
    	snprintf(buf, sizeof(buf), "%s %x", breakpoints[i].type == BREAKPOINT_TYPE_PC ? "BREAKPOINT_TYPE_PC" : "BREAKPOINT_TYPE_MEM", breakpoints[i].address);
        window_puts("debug", buf);
    }
}

static void breakpoint_step_cmd(char *arg) {
    for (unsigned long i = 0; i < (arg ? str_literal_to_ul(arg) : 1); i++) {
        cpu_step();
    }
}

static void break_continue_cmd(void) {
	cpu_step();
    cpu_running = true;
}

void handle_break_command(char *cmd) {
    strtok(cmd, " ");
    char *subcommand = strtok(NULL, " ");

    if (!strncmp(subcommand, "pop", 3)) {
        breakpoint_pop_cmd(strtok(NULL, " "));
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

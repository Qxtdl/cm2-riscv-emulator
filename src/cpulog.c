#include <stdio.h>
#include <stdbool.h>

#include "global.h"
#include "util.h"
#include "cpulog.h"
#include "emulator/arch.h"

bool enabled = false;
struct cycle_dump *cpu_log = NULL;
size_t cpu_log_size = 0;
FILE *log_file = NULL;

void init_cpu_logger(const char *filename) {
    log_file = fopen(filename, "wb");
    if (!log_file) {
        app_abort("init_cpu_logger()", "Unable to obtain file handle")
    }
}

void toggle_cpu_logger(void) {
    enabled = !enabled;
}

void flush_cpu_logger(void) {
    fwrite(cpu_log, sizeof(*cpu_log), cpu_log_size, log_file);
}

void cpu_logger_cycle(void) {
    if (!enabled) return;

    cpu_log = srealloc(cpu_log, ++cpu_log_size * sizeof(struct cycle_dump));
    cpu_log[cpu_log_size - 1].pc = selected_cpu->get_pc();

    arch_register_t arch_register;
    for (int i = 0; (arch_register = selected_cpu->next_arch_register()).name; i++) {
        cpu_log[cpu_log_size - 1].regs[i] = arch_register.value;
    }
}

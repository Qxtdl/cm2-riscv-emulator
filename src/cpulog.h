#pragma once

#include <stdint.h>

/* note that regs contains data based on arch */
struct cycle_dump {
  uint32_t pc;
  uint32_t regs[32];
};

void init_cpu_logger(const char *filename);
void toggle_cpu_logger(void);
void flush_cpu_logger(void);
void cpu_logger_cycle(void);
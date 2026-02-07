#pragma once

#include <stdbool.h>

extern bool cpu_running;

void breakpoint_tick(void);
void handle_break_command(char *cmd);
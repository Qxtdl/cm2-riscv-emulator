#include <string.h>

#include "../cpulog.h"

void handle_logger_command(char *cmd) {
    strtok(cmd, " ");
    char *subcommand = strtok(NULL, " ");

    if (!strcmp(subcommand, "init")) {
        init_cpu_logger(strtok(NULL, " "));
    }
    else if (!strcmp(subcommand, "toggle")) {
        toggle_cpu_logger();
    }
    else if (!strcmp(subcommand, "flush")) {
        flush_cpu_logger();
    }
}
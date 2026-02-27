#include <string.h>
#include <stdlib.h>

#include "arch.h"

#include "taurus/arch.h"
#include "intel8080/arch.h"

#include "../riscv.h"

cpu_t *selected_cpu = NULL;

void select_cpu(const char *name) {
    if (!strcmp(name, taurus.name)) {
        selected_cpu = &taurus;
    }
    else if (!strcmp(name, intel8080.name)) {
        selected_cpu = &intel8080;
    }
}

const char *instruction_to_str(uint32_t ir) {
    switch (selected_cpu->id) {
        default:
        case TAURUS: return rv32i_instruction_to_str(ir);
    }
}



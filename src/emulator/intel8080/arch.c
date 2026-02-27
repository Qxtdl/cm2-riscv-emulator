#include <stddef.h>
#include <stdlib.h>

#include "arch.h"
#include "intel8080.h"

cpu_t intel8080 = {
    .id = INTEL8080,
    .name = "8080",
    .sys_init = intel8080_sys_init,
    .init = intel8080_init,
    .step = intel8080_step,
    .sys_tick = intel8080_sys_tick,
    .ram_size = Intel8080_RAM_SIZE,
    .image = NULL,
};

void *intel8080_sys_init(void *args) {
    int argc = *(int *)(((void **)args)[0]);
    char **argv = *(char ***)(((void **)args)[1]);

    return NULL;
}

void *intel8080_init(void *args) {
    intel8080.state = malloc(sizeof(struct Intel8080_State));
    Intel8080_InitState(intel8080.state);
    intel8080.image = args;
    return NULL;
}

void *intel8080_step(void *args) {
    Intel8080_Step(intel8080.state, selected_cpu->image);
    return NULL;
}

void *intel8080_sys_tick(void *args) {
    return NULL;
}
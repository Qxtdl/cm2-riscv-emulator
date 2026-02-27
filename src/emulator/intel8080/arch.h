#pragma once

#include "../arch.h"
#include "intel8080.h"

void *intel8080_sys_init(void *args);
void *intel8080_init(void *args);
void *intel8080_step(void *args);
void *intel8080_sys_tick(void *args);

extern cpu_t intel8080;
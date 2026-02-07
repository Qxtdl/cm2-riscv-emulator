#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <time.h>

#include "global.h"

void *smalloc(size_t size) {
    void *alloc = malloc(size);
    if (!alloc)
        app_abort("smalloc()", "Failed to allocate memory")
    return alloc;
}

void *srealloc(void *ptr, size_t size) {
    void *alloc = realloc(ptr, size);
    if (!alloc)
        app_abort("srealloc()", "Failed to allocate memory")
    return alloc;    
}

void *scalloc(size_t nmemb, size_t size) {
    void *alloc = calloc(nmemb, size);
    if (!alloc)
        app_abort("scalloc()", "Failed to allocate memory")
    return alloc;
}

int random_int(int min, int max) {
    srand(time(NULL));
    return min + rand() % (max + 1  - min);
}

const char *u32_to_hex(uint32_t value) {
    static const char lookup[] = "0123456789ABCDEF";
    static char buf[9];
    for (int i = 0; i < 8; i++) {
        buf[7 - i] = lookup[value & 0xf];
        value >>= 4;
    }
    buf[8] = '\0';
    return buf;
}

unsigned long str_literal_to_ul(const char *s) {
    if (!strncmp(s, "0x", 2)) return strtoul(s, NULL, 16);
    if (!strncmp(s, "0b", 2)) return strtoul(s, NULL, 2);
    return strtoul(s, NULL, 10);
}

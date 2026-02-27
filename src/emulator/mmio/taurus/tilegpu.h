#pragma once

#include <stdint.h>

void TileGpu_Init(const char *bitmap_filename);
void TileGpu_Tick(void);
bool TileGpu_OffsetInteraction(uint32_t offset);
void TileGpu_ByteStore(uint32_t offset, uint32_t value);
void TileGpu_HalfStore(uint32_t offset, uint32_t value);

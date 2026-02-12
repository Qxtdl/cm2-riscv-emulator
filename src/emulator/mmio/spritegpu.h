#pragma once

void SpriteGpu_Init(const char *sprite_folder);
void SpriteGpu_Tick(void);
bool SpriteGpu_OffsetInteraction(uint32_t offset);
void SpriteGpu_ByteStore(uint32_t offset, uint32_t value);
void SpriteGpu_HalfStore(uint32_t offset, uint32_t value);
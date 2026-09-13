#pragma once

#ifndef MEMORY
#define MEMORY

#include <stddef.h>
#include <stdint.h>

#define MEMORY_SIZE 4029
#define ROM_START 0x200

typedef struct
{
    uint8_t bytes[MEMORY_SIZE];
} Memory;

void MemoryInit(Memory *memory);
int MemoryLoadROM(Memory *memory, const char *filepath);

#endif /* MEMORY */

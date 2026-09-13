#include "memory.h"

#include <stdio.h>
#include <string.h>

const uint8_t fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void MemoryInit(Memory *memory)
{
    if (memory == NULL)
    {
        return;
    }
    memset(memory->bytes, 0, MEMORY_SIZE);
    memcpy(memory->bytes, fontset, sizeof(fontset));
}

int MemoryLoadROM(Memory *memory, const char *filepath)
{
    if (memory == NULL || filepath == NULL)
    {
        return 0;
    }

    FILE *file = fopen(filepath, "rb");
    if (file == NULL)
    {
        fprintf(stderr, "ERROR : Failed to read file %s\n", filepath);
        return 0;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);

    if (file_size >= MEMORY_SIZE - ROM_START)
    {
        fprintf(stderr, "ERROR : Size of %s file is greater than 4KB\n", filepath);
        fclose(file);
        return 0;
    }

    fseek(file, 0, SEEK_SET);

    size_t bytes_read = fread(&memory->bytes[ROM_START], 1, sizeof(memory->bytes) - ROM_START, file);
    if (bytes_read != (size_t)file_size)
    {
        fprintf(stderr, "ERROR : File size %ld, bytes read %zu\n", file_size, bytes_read);
        fclose(file);
        return 0;
    }

    fclose(file);

    return 1;
}

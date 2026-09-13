#include "input.h"

#include <stdbool.h>
#include <string.h>
#include <unistd.h>

void InputInit(Input *input)
{
    if (input == NULL)
    {
        return;
    }
    memset(input->keys, 0, sizeof(input->keys));
}

void InputPollTerminal(Input *input)
{
    if (input == NULL)
    {
        return;
    }
    memset(input->keys, 0, sizeof(input->keys));

    char c;
    while (read(STDIN_FILENO, &c, 1) > 0)
    {
        switch (c)
        {
        case '1':
            input->keys[0x1] = true;
            break;
        case '2':
            input->keys[0x2] = true;
            break;
        case '3':
            input->keys[0x3] = true;
            break;
        case '4':
            input->keys[0xC] = true;
            break;
        case 'q':
            input->keys[0x4] = true;
            break;
        case 'w':
            input->keys[0x5] = true;
            break;
        case 'e':
            input->keys[0x6] = true;
            break;
        case 'r':
            input->keys[0xD] = true;
            break;
        case 'a':
            input->keys[0x7] = true;
            break;
        case 's':
            input->keys[0x8] = true;
            break;
        case 'd':
            input->keys[0x9] = true;
            break;
        case 'f':
            input->keys[0xE] = true;
            break;
        case 'z':
            input->keys[0xA] = true;
            break;
        case 'x':
            input->keys[0x0] = true;
            break;
        case 'c':
            input->keys[0xB] = true;
            break;
        case 'v':
            input->keys[0xF] = true;
            break;
        }
    }
}

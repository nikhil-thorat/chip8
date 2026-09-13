#pragma once

#ifndef INPUT
#define INPUT

#include <stdbool.h>

typedef struct
{
    bool keys[18];
} Input;

void InputInit(Input *input);
void InputPollTerminal(Input *input);

#endif /* INPUT */

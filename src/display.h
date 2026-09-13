#pragma once

#ifndef DISPLAY
#define DISPLAY

#include <stdbool.h>

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

typedef struct
{
    bool pixels[DISPLAY_WIDTH * DISPLAY_HEIGHT];
    bool draw_flag;
} Display;

void DisplayInit(Display *display);
void DisplayRender(Display *display);
void DisplayClear(Display *display);

#endif /* DISPLAY */

#include "display.h"

#include <stdio.h>
#include <string.h>

void DisplayClear(Display *display)
{
    if (display == NULL)
    {
        return;
    }
    memset(display->pixels, 0, sizeof(display->pixels));
    display->draw_flag = true;
}

void DisplayInit(Display *display)
{
    if (display == NULL)
    {
        return;
    }
    DisplayClear(display);
}

void DisplayRender(Display *display)
{
    if (display == NULL || display->draw_flag == false)
    {
        return;
    }

    printf("\e[1;1H");

    for (int y = 0; y < DISPLAY_HEIGHT; y += 2)
    {
        for (int x = 0; x < DISPLAY_WIDTH; x++)
        {
            bool top_pixel = display->pixels[(y * DISPLAY_WIDTH) + x];
            bool bottom_pixel = display->pixels[((y + 1) * DISPLAY_WIDTH) + x];

            if (top_pixel && bottom_pixel)
            {
                printf("\xE2\x96\x88");
            }
            else if (top_pixel)
            {
                printf("\xE2\x96\x80");
            }
            else if (bottom_pixel)
            {
                printf("\xE2\x96\x84");
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }

    display->draw_flag = false;
    fflush(stdout);
}

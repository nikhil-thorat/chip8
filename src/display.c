#include "display.h"

#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

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

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int pad_x = (w.ws_col - DISPLAY_WIDTH) / 2;
    int pad_y = (w.ws_row - (DISPLAY_HEIGHT / 2)) / 2;

    if (pad_x < 0)
    {
        pad_x = 0;
    }

    if (pad_y < 0)
    {
        pad_y = 0;
    }

    printf("\e[1;1H\e[0m");

    for (int i = 0; i < pad_y; i++)
    {
        printf("\e[K\n");
    }

    for (int y = 0; y < DISPLAY_HEIGHT; y += 2)
    {
        printf("\r");
        if (pad_x > 0)
        {
            printf("\e[%dC", pad_x);
        }

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
        printf("\e[K\r\n");
    }

    printf("\e[J");

    display->draw_flag = false;
    fflush(stdout);
}

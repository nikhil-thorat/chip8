#include "audio.h"
#include "cpu.h"
#include "display.h"
#include "input.h"
#include "memory.h"

#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>

struct termios original_tio;
bool running = true;

void SetupRawTerminal()
{
    tcgetattr(STDIN_FILENO, &original_tio);

    struct termios raw = original_tio;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    printf("\e[?25l");
    printf("\e[2J");
    fflush(stdout);
}

void RestoreTerminal()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &original_tio);
    printf("\e[?25h\n");
    fflush(stdout);
}

void HandleSignal(int sig)
{
    running = false;
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        fprintf(stderr, "Usage : %s <ROM_FILE>\n", argv[0]);
        return EXIT_FAILURE;
    }

    Memory memory;
    Display display;
    Input input;
    Audio audio;
    Cpu cpu;

    MemoryInit(&memory);
    if (!MemoryLoadROM(&memory, argv[1]))
    {
        return EXIT_FAILURE;
    }

    DisplayInit(&display);
    InputInit(&input);
    AudioInit(&audio);
    CpuInit(&cpu, &memory, &display, &input, &audio);

    signal(SIGINT, HandleSignal);
    SetupRawTerminal();

    int cycle_count = 0;

    while (running)
    {
        InputPollTerminal(&input);
        CpuCycle(&cpu);

        cycle_count++;
        if (cycle_count >= 11)
        {
            CpuTickTimers(&cpu);
            cycle_count = 0;
        }

        if (display.draw_flag == true)
        {
            DisplayRender(&display);
        }
        usleep(1400);
    }

    RestoreTerminal();

    return EXIT_SUCCESS;
}

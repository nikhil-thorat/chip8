# CHIP8 Emulator

![CHIP8 Logo]("https://github.com/nikhil-thorat/chip8/blob/main/assets/chip8.png")

A modular, terminal based CHIP8 emulator written entirely in C with zero external dependencies.

## Project Architecture

The project is built with a decoupled architecture, isolating the core hardware state from platform-specific I/O. This makes the components easily testable and swappable.
- cpu.h : The central processing unit and state manager.
- memory.h : 4KB RAM, contains font set and ROM data.
- display.h : 62x32 monochrome VRAM.
- input.h : Hexadecimal keypad (0-F) state tracker.
- audio.h : Simple sound timer execution.

## Building and Running

**Requirements :** `clang` (recommended) or `gcc`, and `make`.

```bash
# Clone the repository
git clone https://github.com/nikhil-thorat/chip8.git
cd chip8

# Build
make build

# Run
./build/chip8
```

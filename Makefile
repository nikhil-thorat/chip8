.SILENT:

CC = clang
CFLAGS = -Wall -O2

build:
	mkdir -p build
	$(CC) $(CFLAGS) ./src/*.c -o ./build/chip8

run: build
	./build/chip8 $(ROM)

clean:
	rm -rf ./build

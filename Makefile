.SILENT:

build:
	mkdir -p build
	clang src/main.c -o ./build/chip8

run:
	./build/chip8

clean:
	rm -r ./build


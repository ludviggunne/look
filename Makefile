
CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic
INCLUDE=-Iinclude
SRC=$(wildcard src/*.c)
OBJ=$(SRC:src/%.c=build/%.o)

.PHONY: clean

build/search: $(OBJ)
	@echo "Linking executable $@"
	@mkdir -p build
	@$(CC) $(OBJ) -o $@

build/%.o: src/%.c
	@echo "Creating $@"
	@mkdir -p build
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -rf build

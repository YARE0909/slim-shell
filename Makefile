CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -lreadline -lncurses

SRC = src/main.c src/prompt.c src/input.c src/builtins.c src/exec.c src/util.c src/parser.c src/pipeline.c
OBJ = $(patsubst src/%.c, build/%.o, $(SRC))

slim: $(OBJ)
	$(CC) $(OBJ) -o slim $(LDFLAGS)

# Rule to build .o files into build/
build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build slim

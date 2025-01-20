CFLAGS=-Wall -Wextra -Wswitch-enum -Wmissing-prototypes -std=c11 -pedantic -g
LIBS=

SRC:=./src
BUILD:=./build

OBJS:=$(patsubst $(SRC)/%.c, $(BUILD)/%.o, $(wildcard $(SRC)/*.c))

all: sim

sim: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

$(BUILD)/%.o: $(SRC)/%.c
	@if [ ! -d "$(dir $@)" ]; then mkdir -p $(BUILD); fi
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

clean:
	@rm -rf $(BUILD) sim

.PHONY: all clean

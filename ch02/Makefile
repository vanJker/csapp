CFLAGS=-Wall -Wextra -std=c11 -pedantic
LIBS=

TARGETS := $(patsubst ./%.c, ./%.out, $(wildcard ./*.c))

all: $(TARGETS)

%.out: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	@rm -rf $(TARGETS)

.PHONY: all clean

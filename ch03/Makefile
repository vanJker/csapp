CFLAGS=-Wall -Wextra -std=c11 -pedantic
LIBS=

TARGETS := $(patsubst ./%.c, ./%.txt, $(wildcard ./*.c))

all: $(TARGETS)
	$(MAKE) -C simulator

%.out: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

%.txt: %.out
	objdump -d $< > $@

clean:
	@rm -rf $(TARGETS)
	@$(MAKE) clean -C simulator

.PHONY: all clean

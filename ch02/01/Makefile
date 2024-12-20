TARGET:=01.out 02.out

CFLAGS=-Wall -Wextra -std=c11 -pedantic
LIBS=

all: $(TARGET)

%.out: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	@rm -rf *.out

.PHONY: all clean

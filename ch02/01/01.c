#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t leastPositiveBit32(uint32_t x)
{
    // opposite(x) = 1 + (~x)
    // lowPosiBit(x) = x & (-x)
    return x & (1 + (~x));
}

uint64_t leastPositiveBit64(uint64_t x)
{
    // opposite(x) = 1 + (~x)
    // lowPosiBit(x) = x & (-x)
    return x & (1 + (~x));
}

uint32_t hexAllLetters32(uint32_t x)
{
    // by truth table
    // f(x) = x3 ^ (x2 | x1)
    uint64_t x1 = (x & 0x22222222) >> 1;
    uint64_t x2 = (x & 0x44444444) >> 2;
    uint64_t x3 = (x & 0x88888888) >> 3;

    x = x3 & (x1 | x2);
    x = x & (x >> 16);
    x = x & (x >> 8);
    x = x & (x >> 4);
    return x;
}

uint64_t hexAllLetters64(uint64_t x)
{
    // by truth table
    // f(x) = x3 ^ (x2 | x1)
    uint64_t x1 = (x & 0x2222222222222222) >> 1;
    uint64_t x2 = (x & 0x4444444444444444) >> 2;
    uint64_t x3 = (x & 0x8888888888888888) >> 3;

    x = x3 & (x1 | x2);
    x = x & (x >> 32);
    x = x & (x >> 16);
    x = x & (x >> 8);
    x = x & (x >> 4);
    return x;
}

uint32_t bitCount32(uint32_t x)
{
    uint32_t c;
    c = (x & 0x55555555) + ((x >> 1) & 0x55555555);
    c = (c & 0x33333333) + ((c >> 2) & 0x33333333);
    c = (c & 0x0f0f0f0f) + ((c >> 4) & 0x0f0f0f0f);
    c = (c & 0x00ff00ff) + ((c >> 8) & 0x00ff00ff);
    c = (c & 0x0000ffff) + ((c >> 16) & 0x0000ffff);
    return c;
}

uint64_t bitCount64(uint64_t x)
{
    uint64_t c;
    c = (x & 0x5555555555555555) + ((x >> 1) & 0x5555555555555555);
    c = (c & 0x3333333333333333) + ((c >> 2) & 0x3333333333333333);
    c = (c & 0x0f0f0f0f0f0f0f0f) + ((c >> 4) & 0x0f0f0f0f0f0f0f0f);
    c = (c & 0x00ff00ff00ff00ff) + ((c >> 8) & 0x00ff00ff00ff00ff);
    c = (c & 0x0000ffff0000ffff) + ((c >> 16) & 0x0000ffff0000ffff);
    c = (c & 0x00000000ffffffff) + ((c >> 32) & 0x00000000ffffffff);
    return c;
}

const char *shift_arg(int *argc, char ***argv)
{
    if (*argc == 0) {
        fprintf(stderr, "Error: Too few arugments\n");
        exit(1);
    }

    const char *result = **argv;
    *argc -= 1;
    *argv += 1;
    return result;
}

int main(int argc, char *argv[])
{
    const char *program = shift_arg(&argc, &argv);
    const char *command = shift_arg(&argc, &argv);

    if (!strcmp(command, "-h")) {
        printf("Usage: %s <cmd> <flag> <value>\n", program);
        printf("  cmd:  leastPositiveBit | hexAllLetters | bitCount\n");
        printf("  flag: 32 | 64\n");
        return 0;
    }

    const char *flag = shift_arg(&argc, &argv);
    const char *value = shift_arg(&argc, &argv);
    uint32_t result32;
    uint64_t result64;

    if (!strcmp(command, "leastPositiveBit")) {
        switch (atoi(flag)) {
        case 32:
            result32 = leastPositiveBit32(atoi(value));
            break;
        case 64:
            result64 = leastPositiveBit64(atoi(value));
            break;
        default:
            fprintf(stderr, "Error: Unknown flag: `%s`\n", flag);
            return 1;
        }
    } else if (!strcmp(command, "hexAllLetters")) {
        switch (atoi(flag)) {
        case 32:
            result32 = hexAllLetters32(atoi(value));
            break;
        case 64:
            result64 = hexAllLetters64(atoi(value));
            break;
        default:
            fprintf(stderr, "Error: Unknown flag: `%s`\n", flag);
            return 1;
        }
    } else if (!strcmp(command, "bitCount")) {
        switch (atoi(flag)) {
        case 32:
            result32 = bitCount32(atoi(value));
            break;
        case 64:
            result64 = bitCount64(atoi(value));
            break;
        default:
            fprintf(stderr, "Error: Unknown flag: `%s`\n", flag);
            return 1;
        }
    } else {
        fprintf(stderr, "Error: Unknown command: `%s`\n", command);
        return 1;
    }

    if (!strcmp(flag, "32")) {
        printf("%u\n", result32);
    } else {
        printf("%lu\n", result64);
    }

    return 0;
}

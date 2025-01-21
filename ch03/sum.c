#include <stdint.h>

uint64_t sum(uint64_t n)
{
    if (n == 0) {
        return 0;
    }
    return n + sum(n - 1);
}

int main()
{
    uint64_t x = 10;
    uint64_t c = sum(x);

    return 0;
}
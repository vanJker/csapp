#include <stdint.h>
#include <stdio.h>

#define unlikely(x) __builtin_expect(!!(x), 0)

// convert unsigned integer to normalized and zero float
uint32_t uint2float(uint32_t u)
{
    if (u == 0) {
        return 0;
    }

    // counting the position of highest 1: u[n]
    int n = 31;
    while (0 <= n && (((u >> n) & 0x1) == 0x0)) {
        n = n - 1;
    }

    uint32_t e, f;
    if (u <= 0x00ffffff) {
        // <= 0000 0000 1.111 1111 1111 1111 1111 1111
        // no need rounding
        uint32_t mask = 0xffffffff >> (32 - n);
        f = (u & mask) << (23 - n);
        e = n + 127;
        return (e << 23) | f;
    } else {
        // >= 0000 0001 0000 0000 0000 0000 0000 0000
        // need rounding
        // expand to 64 bit for situations like 0xffffffff
        uint64_t a = 0;
        a += u;

        // compute x, g, r, s
        uint32_t x = (a >> (n - 23)) & 0x1;
        uint32_t g = (a >> (n - 24)) & 0x1;
        uint32_t r = (a >> (n - 25)) & 0x1;
        uint32_t s = 0x0;
        for (int j = 0; j < n - 25; ++j) {
            s = s | ((u >> j) & 0x1);
        }

        // compute carry
        a = a >> (n - 23);
        //  0    1    ?    ... ?
        // [24] [23] [22] ... [0]
        if ((g & (r | s | x)) == 0x1) {
            a = a + 1;
        }
        // check carry
        if ((a >> 23) == 0x1) {
            // 0    1    ?    ... ?
            // [24] [23] [22] ... [0]
            f = a & 0x007fffff;
            e = n + 127;
            return (e << 23) | f;
        } else if ((a >> 23) == 0x2) {
            //  1    0    0    ... 0
            // [24] [23] [22] ... [0]
            f = (a >> 1) & 0x007fffff;
            e = n + 1 + 127;
            return (e << 23) | f;
        }
    }
    // inf as default error
    return 0x7f800000;
}

// convert signed integer to normalized and zero float
uint32_t int2float(int32_t i)
{
    if (i == 0) {
        return 0;
    }

    uint32_t sign = i & 0x80000000;
    uint32_t u = sign ? -i : i;

    // counting the position of highest 1: u[n]
    int n = 31;
    while (0 <= n && (((u >> n) & 0x1) == 0x0)) {
        n = n - 1;
    }

    uint32_t e, f;
    if (u <= 0x00ffffff) {
        // <= 0000 0000 1.111 1111 1111 1111 1111 1111
        // no need rounding
        uint32_t mask = 0xffffffff >> (32 - n);
        f = (u & mask) << (23 - n);
        e = n + 127;
        return sign | (e << 23) | f;
    } else {
        // >= 0000 0001 0000 0000 0000 0000 0000 0000
        // need rounding
        // expand to 64 bit for situations like 0xffffffff
        uint64_t a = 0;
        a += u;

        // compute x, g, r, s
        uint32_t x = (a >> (n - 23)) & 0x1;
        uint32_t g = (a >> (n - 24)) & 0x1;
        uint32_t r = (a >> (n - 25)) & 0x1;
        uint32_t s = 0x0;
        for (int j = 0; j < n - 25; ++j) {
            s = s | ((u >> j) & 0x1);
        }

        // compute carry
        a = a >> (n - 23);
        //  0    1    ?    ... ?
        // [24] [23] [22] ... [0]
        if ((g & (r | s | x)) == 0x1) {
            a = a + 1;
        }
        // check carry
        if ((a >> 23) == 0x1) {
            // 0    1    ?    ... ?
            // [24] [23] [22] ... [0]
            f = a & 0x007fffff;
            e = n + 127;
            return sign | (e << 23) | f;
        } else if ((a >> 23) == 0x2) {
            // 1    0    0    ... 0
            // [24] [23] [22] ... [0]
            f = (a >> 1) & 0x007fffff;
            e = n + 1 + 127;
            return sign | (e << 23) | f;
        }
    }
    // inf as default error
    return 0x7f800000;
}

int main()
{
    uint32_t uf;
    float f;

    // will run very long time
    // for (uint32_t u = 0x00ffffff; u <= 0xffffffff; ++u) {
    // for (uint32_t u = 1024; u < 1024 * 1024; u++) {
    for (int32_t u = -1024 * 1024; u < 1024 * 1024; u++) {
        uf = int2float(u);
        f = (float) u;
        if (unlikely(uf != *(uint32_t *) &f)) {
            printf("fail: %d\n", u);
            printf("  %x %x\n", uf, *(uint32_t *) &f);
            return 0;
        }
    }
    printf("pass\n");

    return 0;
}

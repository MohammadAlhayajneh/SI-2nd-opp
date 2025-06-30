#include "countones.h"

uint32_t count_ones_A(uint32_t x) {
    uint32_t count = 0;
    for (int i = 0; i < 32; ++i) {
        if (x & (1U << i)) count++;
    }
    return count;
} 
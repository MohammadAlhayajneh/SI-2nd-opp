#include "countones.h"

uint32_t count_ones_C(uint32_t x) {
    uint32_t count = 0;
    while (x) {
        x &= (x - 1);
        count++;
    }
    return count;
} 
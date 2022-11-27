// File:    lfsr.c
// Purpose: Basic PRNG using lfsr

#include <stdint.h>

static uint32_t lfsr;
static const uint32_t feedback_poly = 0x80000057;  // For 32-bit max length LFSR (source http://users.ece.cmu.edu/~koopman/lfsr/)

void lfsr_reset() {
    lfsr = 0xFFFFFFFF;  // Anything other than all zeros will work
}

uint32_t lfsr_next() {

    uint32_t curr_lfsr = lfsr;

    if (lfsr & 1)  { lfsr = (lfsr >> 1) ^ feedback_poly; }
    else           { lfsr = (lfsr >> 1);                 }

    return curr_lfsr;
}

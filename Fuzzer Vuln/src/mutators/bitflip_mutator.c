// bitflip_mutator.c
#include "mutator_api.h"
#include <stdlib.h>

void bitflip_mutate(input_t *input, void *context) {
    if (input->size == 0) return;
    size_t pos = rand() % input->size;
    int bit = rand() % 8;
    input->data[pos] ^= (1 << bit);
}

void init_bitflip_mutator() {
    register_mutator("bitflip", bitflip_mutate, NULL);
}
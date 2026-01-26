// arithmetic_mutator.c
#include "mutator_api.h"
#include <stdlib.h>
#include <stdint.h>

void arithmetic_mutate(input_t *input, void *context) {
    if (input->size < 4) return;
    size_t pos = rand() % (input->size - 3);
    int32_t *val = (int32_t *)&input->data[pos];
    int delta = (rand() % 35) + 1;  // 1 to 35
    if (rand() % 2) delta = -delta;
    *val += delta;
}

void init_arithmetic_mutator() {
    register_mutator("arithmetic", arithmetic_mutate, NULL);
}
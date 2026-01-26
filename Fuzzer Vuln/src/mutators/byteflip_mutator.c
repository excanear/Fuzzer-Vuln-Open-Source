// byteflip_mutator.c
#include "mutator_api.h"
#include <stdlib.h>

void byteflip_mutate(input_t *input, void *context) {
    if (input->size == 0) return;
    size_t pos = rand() % input->size;
    input->data[pos] = ~input->data[pos];
}

void init_byteflip_mutator() {
    register_mutator("byteflip", byteflip_mutate, NULL);
}
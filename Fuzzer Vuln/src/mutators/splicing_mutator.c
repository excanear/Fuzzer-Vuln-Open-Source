// splicing_mutator.c
#include "mutator_api.h"
#include <stdlib.h>
#include <string.h>

// Assume a global corpus for splicing
// extern input_t *corpus;
// extern int corpus_size;

void splicing_mutate(input_t *input, void *context) {
    if (global_corpus_size < 2) return;
    int other = rand() % global_corpus_size;
    input_t *other_input = &global_corpus[other];
    size_t splice_len = rand() % (other_input->size + 1);
    size_t new_size = input->size + splice_len;
    input->data = (uint8_t*)realloc(input->data, new_size);
    memcpy(input->data + input->size, other_input->data, splice_len);
    input->size = new_size;
}

void init_splicing_mutator() {
    register_mutator("splicing", splicing_mutate, NULL);
}
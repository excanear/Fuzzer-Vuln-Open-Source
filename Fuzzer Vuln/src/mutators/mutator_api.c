// mutator_api.c
#include "mutator_api.h"
#include <stdlib.h>
#include <string.h>

// Extern declarations
extern void bitflip_mutate(input_t *input, void *context);
extern void init_bitflip_mutator();
extern void byteflip_mutate(input_t *input, void *context);
extern void init_byteflip_mutator();
extern void arithmetic_mutate(input_t *input, void *context);
extern void init_arithmetic_mutator();
extern void dictionary_mutate(input_t *input, void *context);
extern void init_dictionary_mutator();
extern void splicing_mutate(input_t *input, void *context);
extern void init_splicing_mutator();

#define MAX_MUTATORS 10
static mutator_t mutators[MAX_MUTATORS];
static int num_mutators = 0;

// Global corpus
input_t *global_corpus = NULL;
int global_corpus_size = 0;

void register_mutator(const char *name, mutator_func func, void *context) {
    if (num_mutators >= MAX_MUTATORS) return;
    mutators[num_mutators].name = name;
    mutators[num_mutators].func = func;
    mutators[num_mutators].context = context;
    num_mutators++;
}

void mutate_input(input_t *input) {
    // Randomly select and apply a mutator
    if (num_mutators == 0) return;
    int idx = rand() % num_mutators;
    mutators[idx].func(input, mutators[idx].context);
}

void init_mutators() {
    init_bitflip_mutator();
    init_byteflip_mutator();
    init_arithmetic_mutator();
    init_dictionary_mutator();
    init_splicing_mutator();
}
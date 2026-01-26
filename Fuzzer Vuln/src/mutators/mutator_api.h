// mutator_api.h
#ifndef MUTATOR_API_H
#define MUTATOR_API_H

#include <stdint.h>
#include <stddef.h>

// Structure for input data
typedef struct {
    uint8_t *data;
    size_t size;
} input_t;

// Mutator function type
typedef void (*mutator_func)(input_t *input, void *context);

// Mutator registry
typedef struct {
    const char *name;
    mutator_func func;
    void *context;
} mutator_t;

// Global corpus for splicing
extern input_t *global_corpus;
extern int global_corpus_size;

#ifdef __cplusplus
extern "C" {
#endif

// API functions
void register_mutator(const char *name, mutator_func func, void *context);
void mutate_input(input_t *input);
void init_mutators();

#ifdef __cplusplus
}
#endif

#endif
// dictionary_mutator.c
#include "mutator_api.h"
#include <stdlib.h>
#include <string.h>

static const char *dict[] = {"admin", "password", "root", "<script>", NULL};

void dictionary_mutate(input_t *input, void *context) {
    int i = 0;
    while (dict[i]) {
        if (rand() % 10 == 0) {  // Low probability
            size_t len = strlen(dict[i]);
            if (input->size + len > 1024) continue;  // Arbitrary limit
            input->data = (uint8_t*)realloc(input->data, input->size + len);
            memcpy(input->data + input->size, dict[i], len);
            input->size += len;
            break;
        }
        i++;
    }
}

void init_dictionary_mutator() {
    register_mutator("dictionary", dictionary_mutate, NULL);
}
// mutator_api.cpp
#include "mutator_api.h"
#include "input_scheduler.h" // For corpus access? Wait, no, need to pass corpus

// Actually, since global_corpus is in C, need to set it from C++

extern "C"
{
    extern input_t *global_corpus;
    extern int global_corpus_size;
}

MutatorAPI::MutatorAPI()
{
    init_mutators();
}

void MutatorAPI::apply_mutations(std::vector<uint8_t> &input)
{
    input_t in = {input.data(), input.size()};
    mutate_input(&in);
    input.resize(in.size); // In case size changed
}
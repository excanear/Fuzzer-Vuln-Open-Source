// mutator_api.cpp
#include "mutator_api.h"
#include "input_scheduler.h" // For corpus access? Wait, no, need to pass corpus
#include <cstdlib>
#include <cstring>

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
    // Os mutators em C podem chamar realloc()/free() em in.data (ex.: dictionary,
    // splicing). Por isso a memória PRECISA vir de malloc — passar input.data()
    // (buffer do std::vector) causava heap corruption (realloc de ponteiro não
    // alocado por malloc). Usamos um buffer malloc'd e copiamos o resultado de volta.
    input_t in;
    in.size = input.size();
    in.data = static_cast<uint8_t *>(std::malloc(in.size ? in.size : 1));
    if (!in.data)
        return;
    if (in.size)
        std::memcpy(in.data, input.data(), in.size);

    mutate_input(&in);

    if (in.data)
    {
        input.assign(in.data, in.data + in.size);
        std::free(in.data);
    }
}
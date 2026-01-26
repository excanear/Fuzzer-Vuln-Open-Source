// mutator_api.h (C++ wrapper)
#ifndef MUTATOR_API_HPP
#define MUTATOR_API_HPP

#include <vector>

extern "C" {
#include "../mutators/mutator_api.h"
}

class MutatorAPI {
public:
    MutatorAPI();
    void apply_mutations(std::vector<uint8_t>& input);
};

#endif
// input_scheduler.cpp
#include "input_scheduler.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

InputScheduler::InputScheduler(const std::string &corpus_dir)
{
    load_initial_corpus(corpus_dir + "/seeds");
}

void InputScheduler::load_initial_corpus(const std::string &dir)
{
    // Simple hardcoded for now
    std::vector<uint8_t> seed1 = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd'};
    queue_.push(seed1);
    corpus_.push_back(seed1);
}

void InputScheduler::add_input(const std::vector<uint8_t> &input)
{
    queue_.push(input);
}

std::vector<uint8_t> InputScheduler::get_next_input()
{
    if (queue_.empty())
    {
        // Refill queue with current corpus
        for (const auto &inp : corpus_)
        {
            queue_.push(inp);
        }
        if (queue_.empty())
        {
            return std::vector<uint8_t>(); // No corpus
        }
    }
    auto input = queue_.front();
    queue_.pop();
    return input;
}
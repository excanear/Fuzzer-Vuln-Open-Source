// input_scheduler.h
#ifndef INPUT_SCHEDULER_H
#define INPUT_SCHEDULER_H

#include <vector>
#include <string>
#include <queue>
#include <cstdint>

#include <vector>

class InputScheduler {
public:
    InputScheduler(const std::string& corpus_dir);
    std::vector<uint8_t> get_next_input();
    void add_input(const std::vector<uint8_t>& input);
    int get_corpus_size() const { return queue_.size(); }
    const std::vector<std::vector<uint8_t>>& get_corpus() const { return corpus_; }

private:
    std::queue<std::vector<uint8_t>> queue_;
    std::vector<std::vector<uint8_t>> corpus_;  // For splicing
    void load_initial_corpus(const std::string& dir);
};

#endif
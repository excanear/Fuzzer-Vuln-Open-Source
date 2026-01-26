// text_ui.h
#ifndef TEXT_UI_H
#define TEXT_UI_H

#include "ui_base.h"
#include <atomic>

class TextUI : public UI
{
public:
    TextUI();
    void update_stats(int execs_per_sec, int coverage, int crashes, int corpus_size, long long total_execs) override;
    void run() override;
    void display() override;

private:
    std::atomic<int> execs_per_sec_;
    std::atomic<int> coverage_;
    std::atomic<int> crashes_;
    std::atomic<int> corpus_size_;
    std::atomic<long long> total_execs_;
};

#endif
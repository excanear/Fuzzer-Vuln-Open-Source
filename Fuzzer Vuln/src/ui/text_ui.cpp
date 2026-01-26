// text_ui.cpp
#include "text_ui.h"
#include <iostream>
#include <thread>
#include <chrono>

TextUI::TextUI() : execs_per_sec_(0), coverage_(0), crashes_(0), corpus_size_(0), total_execs_(0) {}

void TextUI::update_stats(int execs_per_sec, int coverage, int crashes, int corpus_size, long long total_execs)
{
    execs_per_sec_ = execs_per_sec;
    coverage_ = coverage;
    crashes_ = crashes;
    corpus_size_ = corpus_size;
    total_execs_ = total_execs;
}

void TextUI::run()
{
    while (true)
    {
        display();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void TextUI::display()
{
    std::cerr << "\rExecs/sec: " << execs_per_sec_
              << " | Total: " << total_execs_
              << " | Coverage: " << coverage_
              << " | Crashes: " << crashes_
              << " | Corpus: " << corpus_size_ << std::flush;
}
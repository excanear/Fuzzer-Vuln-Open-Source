// ui_base.h
#ifndef UI_BASE_H
#define UI_BASE_H

#include <atomic>

class UI
{
public:
    virtual ~UI() = default;
    virtual void update_stats(int execs_per_sec, int coverage, int crashes, int corpus_size, long long total_execs) = 0;
    virtual void run() = 0;
    virtual void display() = 0;
};

#endif
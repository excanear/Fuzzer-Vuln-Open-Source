// gui_ui.h
#ifndef GUI_UI_H
#define GUI_UI_H

#include "ui_base.h"
#include <atomic>
#include <windows.h>
#include <thread>

class GuiUI : public UI
{
public:
    GuiUI();
    ~GuiUI();
    void update_stats(int execs_per_sec, int coverage, int crashes, int corpus_size, long long total_execs) override;
    void run() override;
    void display() override;

private:
    std::atomic<int> execs_per_sec_;
    std::atomic<int> coverage_;
    std::atomic<int> crashes_;
    std::atomic<int> corpus_size_;
    std::atomic<long long> total_execs_;

    HWND hwnd_;
    HWND hStaticExecs_;
    HWND hStaticTotal_;
    HWND hStaticCoverage_;
    HWND hStaticCrashes_;
    HWND hStaticCorpus_;

    std::thread ui_thread_;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void create_window();
    void update_display();
};

#endif
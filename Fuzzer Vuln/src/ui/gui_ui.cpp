// gui_ui.cpp
#include "gui_ui.h"
#include <string>
#include <sstream>

GuiUI::GuiUI() : execs_per_sec_(0), coverage_(0), crashes_(0), corpus_size_(0), total_execs_(0), hwnd_(NULL)
{
    create_window();
    ui_thread_ = std::thread([this]()
                             {
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } });
}

GuiUI::~GuiUI()
{
    if (ui_thread_.joinable())
    {
        PostMessage(hwnd_, WM_CLOSE, 0, 0);
        ui_thread_.join();
    }
}

void GuiUI::update_stats(int execs_per_sec, int coverage, int crashes, int corpus_size, long long total_execs)
{
    execs_per_sec_ = execs_per_sec;
    coverage_ = coverage;
    crashes_ = crashes;
    corpus_size_ = corpus_size;
    total_execs_ = total_execs;
    // update_display(); // moved to display()
}

void GuiUI::run()
{
    if (ui_thread_.joinable())
    {
        ui_thread_.join();
    }
}

void GuiUI::display()
{
    update_display();
}

void GuiUI::create_window()
{
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "FuzzerGUI";
    RegisterClass(&wc);

    hwnd_ = CreateWindow("FuzzerGUI", "Fuzzer Stats", WS_OVERLAPPEDWINDOW,
                         CW_USEDEFAULT, CW_USEDEFAULT, 400, 200,
                         NULL, NULL, GetModuleHandle(NULL), this);

    hStaticExecs_ = CreateWindow("STATIC", "Execs/sec: 0", WS_VISIBLE | WS_CHILD,
                                 10, 10, 380, 20, hwnd_, NULL, GetModuleHandle(NULL), NULL);
    hStaticTotal_ = CreateWindow("STATIC", "Total: 0", WS_VISIBLE | WS_CHILD,
                                 10, 35, 380, 20, hwnd_, NULL, GetModuleHandle(NULL), NULL);
    hStaticCoverage_ = CreateWindow("STATIC", "Coverage: 0", WS_VISIBLE | WS_CHILD,
                                    10, 60, 380, 20, hwnd_, NULL, GetModuleHandle(NULL), NULL);
    hStaticCrashes_ = CreateWindow("STATIC", "Crashes: 0", WS_VISIBLE | WS_CHILD,
                                   10, 85, 380, 20, hwnd_, NULL, GetModuleHandle(NULL), NULL);
    hStaticCorpus_ = CreateWindow("STATIC", "Corpus: 0", WS_VISIBLE | WS_CHILD,
                                  10, 110, 380, 20, hwnd_, NULL, GetModuleHandle(NULL), NULL);

    ShowWindow(hwnd_, SW_SHOW);
}

void GuiUI::update_display()
{
    std::stringstream ss;
    ss << "Execs/sec: " << execs_per_sec_.load();
    SetWindowText(hStaticExecs_, ss.str().c_str());

    ss.str("");
    ss << "Total: " << total_execs_.load();
    SetWindowText(hStaticTotal_, ss.str().c_str());

    ss.str("");
    ss << "Coverage: " << coverage_.load();
    SetWindowText(hStaticCoverage_, ss.str().c_str());

    ss.str("");
    ss << "Crashes: " << crashes_.load();
    SetWindowText(hStaticCrashes_, ss.str().c_str());

    ss.str("");
    ss << "Corpus: " << corpus_size_.load();
    SetWindowText(hStaticCorpus_, ss.str().c_str());
}

LRESULT CALLBACK GuiUI::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    GuiUI *pThis = NULL;
    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT *pCreate = (CREATESTRUCT *)lParam;
        pThis = (GuiUI *)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
    }
    else
    {
        pThis = (GuiUI *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (pThis)
    {
        switch (uMsg)
        {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
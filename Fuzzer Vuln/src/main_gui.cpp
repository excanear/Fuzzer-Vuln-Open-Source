// main_gui.cpp
#include "engine/fuzzer_engine.h"
#include <windows.h>
#include <string>
#include <thread>
#include <commctrl.h>
#include <shlobj.h>
#include <commdlg.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "ole32.lib")

#define ID_TARGET_EDIT 101
#define ID_CORPUS_EDIT 102
#define ID_TIMEOUT_EDIT 103
#define ID_DRY_RUN_CHECK 104
#define ID_BROWSE_TARGET 105
#define ID_BROWSE_CORPUS 106
#define ID_START 107
#define ID_STOP 108

HWND hTargetEdit, hCorpusEdit, hTimeoutEdit, hDryRunCheck, hStartBtn, hStopBtn;
std::unique_ptr<FuzzerEngine> engine;
std::thread fuzz_thread;
bool running = false;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == ID_BROWSE_TARGET)
        {
            OPENFILENAME ofn = {0};
            char szFile[260] = {0};
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hwnd;
            ofn.lpstrFile = szFile;
            ofn.nMaxFile = sizeof(szFile);
            ofn.lpstrFilter = "Executables\0*.exe\0All\0*.*\0";
            ofn.nFilterIndex = 1;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
            if (GetOpenFileName(&ofn))
            {
                SetWindowText(hTargetEdit, szFile);
            }
        }
        else if (LOWORD(wParam) == ID_BROWSE_CORPUS)
        {
            BROWSEINFO bi = {0};
            bi.hwndOwner = hwnd;
            bi.pidlRoot = NULL;
            bi.pszDisplayName = NULL;
            bi.lpszTitle = "Select Corpus Directory";
            bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
            LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
            if (pidl)
            {
                char path[MAX_PATH];
                if (SHGetPathFromIDList(pidl, path))
                {
                    SetWindowText(hCorpusEdit, path);
                }
                CoTaskMemFree(pidl);
            }
        }
        else if (LOWORD(wParam) == ID_START && !running)
        {
            char target[260], corpus[260], timeout_str[10];
            GetWindowText(hTargetEdit, target, sizeof(target));
            GetWindowText(hCorpusEdit, corpus, sizeof(corpus));
            GetWindowText(hTimeoutEdit, timeout_str, sizeof(timeout_str));
            bool dry_run = SendMessage(hDryRunCheck, BM_GETCHECK, 0, 0) == BST_CHECKED;
            int timeout = atoi(timeout_str);
            if (timeout == 0)
                timeout = 5000;

            engine = std::make_unique<FuzzerEngine>(target, corpus, dry_run, timeout, true);
            running = true;
            EnableWindow(hStartBtn, FALSE);
            EnableWindow(hStopBtn, TRUE);
            fuzz_thread = std::thread([]()
                                      {
                try {
                    engine->run();
                } catch (...) {
                }
                running = false; });
        }
        else if (LOWORD(wParam) == ID_STOP && running)
        {
            // To stop, perhaps set a flag, but for now, just terminate
            if (fuzz_thread.joinable())
            {
                TerminateThread((HANDLE)fuzz_thread.native_handle(), 0);
                fuzz_thread.join();
            }
            running = false;
            EnableWindow(hStartBtn, TRUE);
            EnableWindow(hStopBtn, FALSE);
        }
        break;
    case WM_DESTROY:
        if (running && fuzz_thread.joinable())
        {
            TerminateThread((HANDLE)fuzz_thread.native_handle(), 0);
            fuzz_thread.join();
        }
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    INITCOMMONCONTROLSEX icex = {sizeof(INITCOMMONCONTROLSEX), ICC_WIN95_CLASSES};
    InitCommonControlsEx(&icex);

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "FuzzerGUIApp";
    RegisterClass(&wc);

    HWND hwnd = CreateWindow("FuzzerGUIApp", "Fuzzer GUI", WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT, 500, 300,
                             NULL, NULL, hInstance, NULL);

    // Create controls
    CreateWindow("STATIC", "Target Executable:", WS_VISIBLE | WS_CHILD, 10, 10, 120, 20, hwnd, NULL, hInstance, NULL);
    hTargetEdit = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 140, 10, 250, 20, hwnd, (HMENU)ID_TARGET_EDIT, hInstance, NULL);
    CreateWindow("BUTTON", "Browse", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 400, 10, 60, 20, hwnd, (HMENU)ID_BROWSE_TARGET, hInstance, NULL);

    CreateWindow("STATIC", "Corpus Directory:", WS_VISIBLE | WS_CHILD, 10, 40, 120, 20, hwnd, NULL, hInstance, NULL);
    hCorpusEdit = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 140, 40, 250, 20, hwnd, (HMENU)ID_CORPUS_EDIT, hInstance, NULL);
    CreateWindow("BUTTON", "Browse", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 400, 40, 60, 20, hwnd, (HMENU)ID_BROWSE_CORPUS, hInstance, NULL);

    CreateWindow("STATIC", "Timeout (ms):", WS_VISIBLE | WS_CHILD, 10, 70, 100, 20, hwnd, NULL, hInstance, NULL);
    hTimeoutEdit = CreateWindow("EDIT", "5000", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 120, 70, 100, 20, hwnd, (HMENU)ID_TIMEOUT_EDIT, hInstance, NULL);

    hDryRunCheck = CreateWindow("BUTTON", "Dry Run", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 10, 100, 100, 20, hwnd, (HMENU)ID_DRY_RUN_CHECK, hInstance, NULL);

    hStartBtn = CreateWindow("BUTTON", "Start Fuzzing", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 10, 130, 100, 30, hwnd, (HMENU)ID_START, hInstance, NULL);
    hStopBtn = CreateWindow("BUTTON", "Stop", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | WS_DISABLED, 120, 130, 100, 30, hwnd, (HMENU)ID_STOP, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}
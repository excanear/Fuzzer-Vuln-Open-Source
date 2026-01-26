#include "fuzzer_engine.h"
#include "input_scheduler.h"
#include "coverage_collector.h"
#include "crash_detector.h"
#include "mutator_api.h"
#include "../ui/text_ui.h"
#include "../ui/gui_ui.h"
#include "../instrumentation/instrumentation.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>
#include <filesystem>
#include <fstream>
#include <set>

namespace fs = std::filesystem;

FuzzerEngine::FuzzerEngine(const std::string &target_path, const std::string &corpus_dir, bool dry_run, int timeout_ms, bool use_gui)
    : target_path_(target_path), corpus_dir_(corpus_dir), dry_run_(dry_run), timeout_ms_(timeout_ms), use_gui_(use_gui),
      execs_per_sec_(0), total_execs_(0), last_time_(std::chrono::steady_clock::now())
{
    init_instrumentation(); // Initialize shared memory
    scheduler_ = std::make_unique<InputScheduler>(corpus_dir);
    coverage_ = std::make_unique<CoverageCollector>();
    crash_detector_ = std::make_unique<CrashDetector>();
    mutators_ = std::make_unique<MutatorAPI>();
    if (use_gui_)
        ui_ = std::make_unique<GuiUI>();
    else
        ui_ = std::make_unique<TextUI>();
}

FuzzerEngine::~FuzzerEngine() = default;

void FuzzerEngine::run()
{
    // std::thread ui_thread(&TextUI::run, ui_.get());
    fuzz_loop();
    // ui_thread.join();
}

void FuzzerEngine::fuzz_loop()
{
    try
    {
        int minimize_counter = 0;
        int display_counter = 0;
        int loop_count = 0;
        while (true)
        {
            auto input = scheduler_->get_next_input();
            mutators_->apply_mutations(input);
            execute_target(input);

            total_execs_++;
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - last_time_).count();
            if (elapsed >= 1)
            {
                execs_per_sec_ = total_execs_ / elapsed;
                last_time_ = now;
            }

            ui_->update_stats(execs_per_sec_, coverage_->get_coverage_count(), crash_detector_->get_crash_count(), scheduler_->get_corpus_size(), total_execs_);

            minimize_counter++;
            if (minimize_counter % 1000 == 0)
            { // Minimize every 1000 executions
                minimize_corpus();
            }

            display_counter++;
            if (display_counter % 10 == 0)
            { // Display every 10 executions
                ui_->display();
                std::cerr << std::endl; // New line after display
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Throttle
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception in fuzz_loop: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Unknown exception in fuzz_loop" << std::endl;
    }
}

void FuzzerEngine::execute_target(const std::vector<uint8_t> &input)
{
    if (dry_run_)
    {
        // Simulate execution
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        return;
    }

    // Write input to temp file
    std::string temp_file = "temp_input.bin";
    std::ofstream temp(temp_file, std::ios::binary);
    temp.write(reinterpret_cast<const char *>(input.data()), input.size());
    temp.close();

    // Prepare command line
    std::string cmd = target_path_ + " " + temp_file;

    STARTUPINFO si = {sizeof(si)};
    PROCESS_INFORMATION pi;
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    if (!CreateProcess(NULL, const_cast<char *>(cmd.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        std::cerr << "CreateProcess failed" << std::endl;
        return;
    }

    // Wait for process with timeout
    DWORD exitCode;
    DWORD wait_result = WaitForSingleObject(pi.hProcess, timeout_ms_);
    if (wait_result == WAIT_TIMEOUT)
    {
        TerminateProcess(pi.hProcess, 1); // Timeout
        exitCode = 1;                     // Indicate timeout
    }
    else
    {
        GetExitCodeProcess(pi.hProcess, &exitCode);
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    // Check for crash
    if (crash_detector_->detect_crash(exitCode))
    {
        crash_detector_->save_crash(input, std::to_string(exitCode));
    }

    // Collect coverage
    const int MAP_SIZE = 1 << 16;
    if (__afl_area_ptr && coverage_->has_new_coverage(__afl_area_ptr, MAP_SIZE))
    {
        coverage_->update_coverage(__afl_area_ptr, MAP_SIZE);
        scheduler_->add_input(input);
    }

    fs::remove(temp_file);
}

void FuzzerEngine::minimize_corpus()
{
    // Simple minimization: remove duplicate inputs
    auto &corpus = scheduler_->get_corpus();
    std::vector<std::vector<uint8_t>> unique;
    std::set<std::vector<uint8_t>> seen;
    for (auto &input : corpus)
    {
        if (seen.insert(input).second)
        {
            unique.push_back(input);
        }
    }
    // Update corpus (but scheduler is private, so TODO: add method to scheduler)
    // For now, just log
    std::cout << "Minimized corpus to " << unique.size() << " inputs" << std::endl;
}
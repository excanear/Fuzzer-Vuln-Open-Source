// fuzzer_engine.h
#ifndef FUZZER_ENGINE_H
#define FUZZER_ENGINE_H

#include <memory>
#include <string>
#include <vector>
#include <chrono>

class InputScheduler;
class CoverageCollector;
class CrashDetector;
class MutatorAPI;
class UI;

class FuzzerEngine
{
public:
    FuzzerEngine(const std::string &target_path, const std::string &corpus_dir, bool dry_run = false, int timeout_ms = 5000, bool use_gui = false);
    ~FuzzerEngine();

    void run();

private:
    std::string target_path_;
    std::string corpus_dir_;
    std::unique_ptr<InputScheduler> scheduler_;
    std::unique_ptr<CoverageCollector> coverage_;
    std::unique_ptr<CrashDetector> crash_detector_;
    std::unique_ptr<MutatorAPI> mutators_;
    std::unique_ptr<UI> ui_;

    bool dry_run_;
    int timeout_ms_;
    bool use_gui_;
    int execs_per_sec_;
    long long total_execs_;
    std::chrono::steady_clock::time_point last_time_;

    void fuzz_loop();
    void execute_target(const std::vector<uint8_t> &input);
    void minimize_corpus();
};

#endif
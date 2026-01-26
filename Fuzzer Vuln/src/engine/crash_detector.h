// crash_detector.h
#ifndef CRASH_DETECTOR_H
#define CRASH_DETECTOR_H

#include <vector>
#include <string>
#include <cstdint>

#ifdef _WIN32
#include <windows.h>
#endif

class CrashDetector {
public:
    bool detect_crash(DWORD exitCode);
    void save_crash(const std::vector<uint8_t>& input, const std::string& type);
    int get_crash_count() const { return seen_crashes_.size(); }

private:
    std::vector<std::string> seen_crashes_;
};

#endif
// crash_detector.cpp
#include "crash_detector.h"
#include <windows.h>
#include <fstream>
#include <sstream>
#include <algorithm>

bool CrashDetector::detect_crash(DWORD exitCode) {
    // In Windows, exit codes for exceptions are different
    // For simplicity, check for common crash codes
    // Actually, GetExitCodeProcess returns the exit code
    // For exceptions, it might be the exception code
    // But for now, assume non-zero is crash
    return exitCode != 0;
}

void CrashDetector::save_crash(const std::vector<uint8_t>& input, const std::string& type) {
    // Deduplicate based on type
    if (std::find(seen_crashes_.begin(), seen_crashes_.end(), type) != seen_crashes_.end()) {
        return;
    }
    seen_crashes_.push_back(type);

    std::ofstream file("corpus/crashes/crash_" + type, std::ios::binary);
    file.write(reinterpret_cast<const char*>(input.data()), input.size());
}
// coverage_collector.cpp
#include "coverage_collector.h"
#include <cstring>

CoverageCollector::CoverageCollector() : current_coverage_(65536, 0) {}  // 64KB

bool CoverageCollector::has_new_coverage(const uint8_t* map, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        if (map[i] > current_coverage_[i]) {
            return true;
        }
    }
    return false;
}

void CoverageCollector::update_coverage(const uint8_t* map, size_t size) {
    for (size_t i = 0; i < size && i < current_coverage_.size(); ++i) {
        if (map[i] > current_coverage_[i]) {
            current_coverage_[i] = map[i];
        }
    }
}

int CoverageCollector::get_coverage_count() const {
    int count = 0;
    for (uint8_t val : current_coverage_) {
        if (val > 0) count++;
    }
    return count;
}
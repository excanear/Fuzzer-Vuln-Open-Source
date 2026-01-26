// coverage_collector.h
#ifndef COVERAGE_COLLECTOR_H
#define COVERAGE_COLLECTOR_H

#include <vector>
#include <cstdint>

class CoverageCollector {
public:
    CoverageCollector();
    bool has_new_coverage(const uint8_t* map, size_t size);
    void update_coverage(const uint8_t* map, size_t size);
    int get_coverage_count() const;

private:
    std::vector<uint8_t> current_coverage_;
};

#endif
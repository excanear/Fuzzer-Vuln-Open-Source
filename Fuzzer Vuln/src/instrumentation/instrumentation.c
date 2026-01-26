// instrumentation.c
#include "instrumentation.h"
#include <windows.h>
#include <string.h>

#define MAP_SIZE (1 << 16)  // 64KB coverage map

uint8_t *__afl_area_ptr = NULL;
uint32_t __afl_prev_loc = 0;

HANDLE hMapFile = NULL;

void init_instrumentation() {
    // Create shared memory for coverage map
    hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,    // use paging file
        NULL,                    // default security
        PAGE_READWRITE,          // read/write access
        0,                       // maximum object size (high-order DWORD)
        MAP_SIZE,                // maximum object size (low-order DWORD)
        TEXT("AFLCoverageMap")); // name of mapping object

    if (hMapFile == NULL) {
        return;
    }

    __afl_area_ptr = (uint8_t*) MapViewOfFile(
        hMapFile,               // handle to map object
        FILE_MAP_ALL_ACCESS,    // read/write permission
        0,
        0,
        MAP_SIZE);

    if (__afl_area_ptr == NULL) {
        CloseHandle(hMapFile);
        hMapFile = NULL;
        return;
    }

    memset(__afl_area_ptr, 0, MAP_SIZE);
}

void instrument_basic_block(uint32_t cur_loc) {
    // Implement in C for compatibility (assembly version available in .asm)
    if (!__afl_area_ptr) return;

    uint32_t prev = __afl_prev_loc;
    __afl_prev_loc = cur_loc;

    uint32_t idx = (prev >> 1) ^ cur_loc;
    if (__afl_area_ptr[idx] < 255) {
        __afl_area_ptr[idx]++;
    }
}
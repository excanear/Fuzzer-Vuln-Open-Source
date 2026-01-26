// instrumentation.h
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Shared memory for coverage map
extern uint8_t *__afl_area_ptr;

// Previous location for edge coverage
extern uint32_t __afl_prev_loc;

// Function to initialize instrumentation
void init_instrumentation();

// Function to instrument a basic block (called from assembly or inline)
void instrument_basic_block(uint32_t cur_loc);

#ifdef __cplusplus
}
#endif
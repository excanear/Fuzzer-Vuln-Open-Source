# Detailed Architecture

## Instrumentation Module

The instrumentation is implemented in x86_64 assembly for maximum performance and minimal overhead. It inserts coverage tracking code at the beginning of each basic block.

### Coverage Map
- 64KB shared memory region
- Each byte represents hit count for an edge (saturated at 255)
- Edge hash: (prev_loc >> 1) ^ cur_loc

### Communication
- Shared memory via shm_open/mmap
- Accessible by both fuzzer and target process

## Mutators Module

Modular design allowing easy addition of new mutation strategies.

### API
- `register_mutator()`: Add new mutator
- `apply_mutations()`: Randomly apply a mutator to input

### Current Mutators
- **Bit Flip**: Flips a single bit
- **Byte Flip**: Inverts a byte
- **Arithmetic**: Adds/subtracts small integers
- **Dictionary**: Inserts known interesting strings
- **Splicing**: Combines parts of different inputs

## Engine Module

C++17 implementation with modern idioms.

### Components
- **FuzzerEngine**: Orchestrates the fuzzing process
- **InputScheduler**: Manages input queue with prioritization
- **CoverageCollector**: Tracks coverage and detects new paths
- **CrashDetector**: Monitors child processes for crashes
- **MutatorAPI**: C++ wrapper for C mutators

### Fuzzing Loop
1. Select input from queue
2. Apply mutations
3. Execute target in child process
4. Collect coverage and check for crashes
5. Update queue if new coverage found

## UI Module

Simple text-based interface displaying real-time statistics.

### Displayed Metrics
- Executions per second
- Current coverage (edges hit)
- Number of unique crashes
- Corpus size

## Corpus Management

### Directory Structure
- `seeds/`: Initial inputs
- `queue/`: Active fuzzing queue
- `crashes/`: Saved crash inputs

### Minimization
- Only save inputs that increase coverage
- Periodic corpus minimization to remove redundant inputs

## Crash Analysis

### Detection
- Uses waitpid to check child exit status
- Identifies crash signals (SIGSEGV, SIGILL, SIGABRT)

### Deduplication
- Based on signal type and crash address
- Saves input and metadata to crash directory

## Extensibility

### Adding New Mutators
1. Implement mutator function
2. Register in `init_mutators()`

### New Architectures
- Port assembly instrumentation
- Update shared memory handling

### Sanitizer Integration
- Hook into crash detection
- Parse sanitizer output for additional info
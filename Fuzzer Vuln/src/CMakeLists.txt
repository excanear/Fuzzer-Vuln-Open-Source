# Instrumentation (Assembly)
add_library(instrumentation STATIC
    instrumentation/coverage_instrumentation.asm
    instrumentation/instrumentation.c  # If needed for glue
)

# Mutators (C)
add_library(mutators STATIC
    mutators/bitflip_mutator.c
    mutators/byteflip_mutator.c
    mutators/arithmetic_mutator.c
    mutators/dictionary_mutator.c
    mutators/splicing_mutator.c
    mutators/mutator_api.c
)

# Engine and UI (C++)
add_executable(fuzzer_engine
    main.cpp
    engine/fuzzer_engine.cpp
    engine/coverage_collector.cpp
    engine/input_scheduler.cpp
    engine/crash_detector.cpp
    engine/mutator_api.cpp
    ui/text_ui.cpp
)

target_link_libraries(fuzzer_engine
    instrumentation
    mutators
    # Add other libraries like pthread, etc.
)
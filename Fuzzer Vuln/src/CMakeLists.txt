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
    ui/gui_ui.cpp
)

target_link_libraries(fuzzer_engine
    instrumentation
    mutators
    # Add other libraries like pthread, etc.
)

# Alvo de exemplo para fuzzing. Precisa linkar a lib de instrumentação
# (usa init_instrumentation/instrument_basic_block) — antes não compilava sozinho.
add_executable(example_target ../example_target.c)
target_include_directories(example_target PRIVATE instrumentation)
target_link_libraries(example_target instrumentation)
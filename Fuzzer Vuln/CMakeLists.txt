cmake_minimum_required(VERSION 3.16)
project(FuzzerVuln VERSION 1.0.0 LANGUAGES C CXX ASM)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
enable_testing()

# Add subdirectories
add_subdirectory(src)

# Add tests (optional — only if the tests/ directory is present)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
    add_subdirectory(tests)
endif()

# Install targets
install(TARGETS fuzzer_engine DESTINATION bin)
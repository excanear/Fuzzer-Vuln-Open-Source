@echo off
REM Build script using gcc (MinGW) with full path

set GCC_PATH=C:\Users\Henry\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin

echo Building with gcc...

REM Create build directory if not exists
if not exist build mkdir build

REM Compile C++ objects
"%GCC_PATH%\g++.exe" -std=c++17 -I src\instrumentation -c src\main.cpp -o build\main.o
"%GCC_PATH%\g++.exe" -std=c++17 -I src\instrumentation -c src\engine\fuzzer_engine.cpp -o build\fuzzer_engine.o
"%GCC_PATH%\g++.exe" -std=c++17 -I src\instrumentation -c src\engine\input_scheduler.cpp -o build\input_scheduler.o
"%GCC_PATH%\g++.exe" -std=c++17 -I src\instrumentation -c src\engine\coverage_collector.cpp -o build\coverage_collector.o
"%GCC_PATH%\g++.exe" -std=c++17 -I src\instrumentation -c src\engine\crash_detector.cpp -o build\crash_detector.o
"%GCC_PATH%\g++.exe" -std=c++17 -I src\instrumentation -c src\engine\mutator_api.cpp -o build\mutator_api.o
"%GCC_PATH%\g++.exe" -std=c++17 -I src\instrumentation -c src\ui\text_ui.cpp -o build\text_ui.o
"%GCC_PATH%\g++.exe" -std=c++17 -I src\instrumentation -c src\ui\gui_ui.cpp -o build\gui_ui.o

REM Compile C objects
"%GCC_PATH%\gcc.exe" -I src\instrumentation -c src\mutators\mutator_api.c -o build\mutator_api_c.o
"%GCC_PATH%\gcc.exe" -I src\instrumentation -c src\mutators\bitflip_mutator.c -o build\bitflip_mutator.o
"%GCC_PATH%\gcc.exe" -I src\instrumentation -c src\mutators\byteflip_mutator.c -o build\byteflip_mutator.o
"%GCC_PATH%\gcc.exe" -I src\instrumentation -c src\mutators\arithmetic_mutator.c -o build\arithmetic_mutator.o
"%GCC_PATH%\gcc.exe" -I src\instrumentation -c src\mutators\dictionary_mutator.c -o build\dictionary_mutator.o
"%GCC_PATH%\gcc.exe" -I src\instrumentation -c src\mutators\splicing_mutator.c -o build\splicing_mutator.o
"%GCC_PATH%\gcc.exe" -I src\instrumentation -c src\instrumentation\instrumentation.c -o build\instrumentation.o

REM Link fuzzer_engine
"%GCC_PATH%\g++.exe" -o fuzzer_engine.exe ^
build\main.o ^
build\fuzzer_engine.o ^
build\input_scheduler.o ^
build\coverage_collector.o ^
build\crash_detector.o ^
build\mutator_api.o ^
build\text_ui.o ^
build\gui_ui.o ^
build\mutator_api_c.o ^
build\bitflip_mutator.o ^
build\byteflip_mutator.o ^
build\arithmetic_mutator.o ^
build\dictionary_mutator.o ^
build\splicing_mutator.o ^
build\instrumentation.o ^
-lpthread -luser32 -lgdi32

REM Compile main_gui
"%GCC_PATH%\g++.exe" -std=c++17 -I src\instrumentation -c src\main_gui.cpp -o build\main_gui.o

REM Link gui_app
"%GCC_PATH%\g++.exe" -mwindows -o gui_app.exe ^
build\main_gui.o ^
build\fuzzer_engine.o ^
build\input_scheduler.o ^
build\coverage_collector.o ^
build\crash_detector.o ^
build\mutator_api.o ^
build\text_ui.o ^
build\gui_ui.o ^
build\mutator_api_c.o ^
build\bitflip_mutator.o ^
build\byteflip_mutator.o ^
build\arithmetic_mutator.o ^
build\dictionary_mutator.o ^
build\splicing_mutator.o ^
build\instrumentation.o ^
-lpthread -luser32 -lgdi32 -lcomctl32 -lole32

REM Compile example_target
"%GCC_PATH%\gcc.exe" -I src\instrumentation -o example_target.exe example_target.c build\instrumentation.o

echo Build completed!
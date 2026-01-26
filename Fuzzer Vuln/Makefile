CC = gcc
CXX = g++
ASM = nasm
CFLAGS = -Wall -O2
CXXFLAGS = -Wall -O2 -std=c++17
ASMFLAGS = -f elf64

# Directories
SRC_DIR = src
BUILD_DIR = build

# Targets
all: $(BUILD_DIR)/fuzzer_engine

$(BUILD_DIR)/fuzzer_engine: $(BUILD_DIR)/main.o $(BUILD_DIR)/fuzzer_engine.o $(BUILD_DIR)/coverage_collector.o $(BUILD_DIR)/input_scheduler.o $(BUILD_DIR)/crash_detector.o $(BUILD_DIR)/mutator_api.o $(BUILD_DIR)/text_ui.o $(BUILD_DIR)/mutator_api_c.o $(BUILD_DIR)/bitflip_mutator.o $(BUILD_DIR)/byteflip_mutator.o $(BUILD_DIR)/arithmetic_mutator.o $(BUILD_DIR)/dictionary_mutator.o $(BUILD_DIR)/splicing_mutator.o $(BUILD_DIR)/instrumentation.o $(BUILD_DIR)/coverage_instrumentation.o
	$(CXX) $(CXXFLAGS) -o $@ $^ -pthread

$(BUILD_DIR)/%.o: $(SRC_DIR)/engine/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/ui/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/mutators/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/instrumentation/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/coverage_instrumentation.o: $(SRC_DIR)/instrumentation/coverage_instrumentation.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/fuzzer_engine

.PHONY: all clean
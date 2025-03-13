# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -pthread -Wall -Wextra -O2

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Executable name (no .exe for Linux)
TARGET = $(BIN_DIR)/SizeExplorer

# Find all .cpp source files in src/
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

# Default target
all: $(TARGET)

# Create the binary
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create necessary directories if missing
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean the build
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

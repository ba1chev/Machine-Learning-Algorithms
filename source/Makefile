CXX = g++
REPO_ROOT = ..
CXXFLAGS = -std=c++17 -Wall -Wextra -I$(REPO_ROOT) -I/opt/homebrew/include
LDLIBS = -lcurl

BUILD_DIR = bin
TARGET = $(BUILD_DIR)/main

SOURCES = main.cpp $(shell find $(REPO_ROOT)/source -type f -name '*.cpp' ! -name '*_test.cpp' ! -name 'main.cpp')

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(SOURCES) $(LDLIBS) -o $@

clean:
	rm -rf $(BUILD_DIR)

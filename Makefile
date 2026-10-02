CXX = clang++

TARGET = chip8_emu
BUILD_DIR = build
SRC_DIR = src
OBJ_DIR = $(BUILD_DIR)/obj
IMGUI_DIR = ./imgui

SOURCES = $(SRC_DIR)/main.cpp $(SRC_DIR)/chip8.cpp
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_sdl3.cpp $(IMGUI_DIR)/backends/imgui_impl_vulkan.cpp
OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(SOURCES:.cpp=.o)))

CXXFLAGS = --std=c++23 -stdlib=libc++ -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -g -Wall -Wformat
CXXFLAGS += $(shell pkg-config --cflags sdl3 vulkan)

LIBS = -ldl -lc++abi
LIBS += $(shell pkg-config --libs sdl3 vulkan)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(IMGUI_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(IMGUI_DIR)/backends/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $@

$(OBJ_DIR): $(BUILD_DIR)
	@mkdir -p $@

run: $(BUILD_DIR)/$(TARGET)
	./$<

all: $(BUILD_DIR)/$(TARGET)
	@echo Build complete

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

.PHONY: clean
clean: 
	rm -rf $(BUILD_DIR)/
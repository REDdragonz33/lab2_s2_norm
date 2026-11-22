CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2
TARGET := lab2_app

STRUCTURE_SRCS := $(filter-out ../structures/main.cpp,$(wildcard ../structures/*.cpp))
TASK_SRCS := main.cpp $(filter-out main.cpp,$(wildcard z*.cpp))
SRCS := $(TASK_SRCS) $(STRUCTURE_SRCS)

.PHONY: all clean
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -I.. $(SRCS) -o $@

clean:
	rm -f $(TARGET)

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++11

# Source files
SRC1 = main-test.cpp
SRC2 = unit-tests.cpp

HEADERS = ../external/ppm_reader.h ../qoif_cpp.h
IMAGES = images/sample.ppm

# Output
TARGET1 = test
TARGET2 = unit-test

# Default rule to build both executables
all: $(TARGET1) $(TARGET2)

# Rule to build the test executable for SRC1
$(TARGET1): $(SRC1) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET1) $(SRC1)

# Rule to build the unit test executable for SRC2
$(TARGET2): $(SRC2) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET2) $(SRC2)

# Clean rule
clean:
	rm -f $(TARGET1) $(TARGET2)

# Phony targets
.PHONY: all clean

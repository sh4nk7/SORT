CXX = g++
CXXFLAGS = -O3 -Wall -Wextra -pedantic -pthread -std=c++11
TARGET = matrix_threads
SRC = src/main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean

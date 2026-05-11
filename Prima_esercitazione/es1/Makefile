CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -pthread -std=c++11
TARGET = hello_thread
SRC = src/main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean

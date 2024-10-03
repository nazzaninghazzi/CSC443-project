CXX = g++
CXXFLAGS = -Wall -g
SOURCES = main.cpp AVLTree.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = main

all: $(TARGET) run

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)
    
clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
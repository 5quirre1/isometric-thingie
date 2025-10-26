CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

LDFLAGS = -lraylib -lgdi32 -lwinmm -lshell32 -lopengl32

SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, build/%.o, $(SRC))
EXEC = greg.exe

all: $(EXEC)

build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

run: $(EXEC)
	./$(EXEC)

build:
	mkdir -p build

clean:
	rm -rf build $(EXEC)

.PHONY: all clean run build

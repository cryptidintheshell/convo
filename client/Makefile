
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17

MAIN_OBJECTS := build/main.o build/client.o build/window.o build/messenger.o

main: $(MAIN_OBJECTS)
	$(CXX) $^ -o $@ -lncurses

build/main.o: src/main.cpp headers/client.h headers/window.h
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/client.o: src/client.cpp headers/client.h
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/window.o: src/window.cpp headers/window.h
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/messenger.o: src/messenger.cpp headers/messenger.h
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm build/*.o
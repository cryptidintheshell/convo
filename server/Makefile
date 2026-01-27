CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17

MAIN_OBJECTS := build/socket.o build/main.o

server: $(MAIN_OBJECTS)
	$(CXX) $^ -o $@

build/main.o: src/main.cpp headers/socket.h
	$(CXX) $(CXXFLAGS) -c $< -o build/main.o


build/socket.o: src/socket.cpp headers/socket.h headers/includes.h
	$(CXX) $(CXXFLAGS) -c $< -o build/socket.o


# build/window.o: headers/window.h src/window.cpp src/setup_controls.cpp src/system_handler.cpp
# 	g++ -c src/window.cpp -o build/window.o `wx-config --cxxflags --libs`

clean:
	rm build/*.o
cmake_minimum_required(VERSION 3.23)
project(emulator)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(SFML)

add_executable(emulator src/main.cpp src/gameBoy.cpp src/cpu.cpp src/cartridge.cpp src/registers.cpp src/PPU.cpp src/PPU.h src/memory.cpp src/memory.h)

target_link_libraries(emulator sfml-window sfml-graphics sfml-main)


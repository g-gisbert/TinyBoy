cmake_minimum_required(VERSION 3.22)
project(emulator)

set(CMAKE_CXX_STANDARD 17)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
set(SFML_IS_FRAMEWORK_INSTALL "@SFML_BUILD_FRAMEWORKS@")
set(config_name "Static")

add_subdirectory(SFML)

add_executable(emulator src/main.cpp src/gameBoy.cpp src/cpu.cpp src/cartridge.cpp src/registers.cpp src/PPU.cpp
        src/memory.cpp src/interrupts.cpp src/display.cpp src/timer.cpp src/timer.h src/joypad.cpp src/joypad.h)

target_link_libraries(emulator sfml-window sfml-graphics sfml-main)


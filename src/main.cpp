#include "gameBoy.h"


int main(int argc, char** argv)
{
    std::string filepath1 = std::string{argv[1]};
    GameBoy emulation(filepath1.c_str());
    emulation.run();

    return 0;
}


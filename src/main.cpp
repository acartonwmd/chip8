#include "chip8.h"

using namespace chip8;

int main(int argc, char *argv[]) 
{
    if (argc < 2) 
    {
        std::cerr << "usage: ./chip_emu <rom-file>"  << std::endl;
        return 1;
    }

    ch8_emu emulator(argv[1]);
    emulator.start();
}
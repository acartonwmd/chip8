#include "chip8.h"

using namespace chip8;
using chip8::ch8_emu;

/* CHIP-8 Emulator: Core functionality */

void ch8_emu::start()
{
    while(true) 
    {
        // Emulates a CPU cycle
        cpu_cycle(this->memory[this->pc], this->memory[this->pc + 1]);
    }
}

void ch8_emu::print_instrs(int size)
{
    std::cout << "[DEBUG]: instruction memory dump" << std::endl;

    for(int i = instrbuf_addr; i < instrbuf_addr + size; i++)
    {
        std::cout << std::format("{:#04x}", this->memory[i]) << std::endl;
    }
}

/**
 * @brief Constructor for the main emulator components
 * @param rompath filepath for the desired .ch8 program
 * 
 * If while reading the program an error occurs, this will complete exit the program
 */
ch8_emu::ch8_emu(fs::path rompath) 
{
    try 
    {
        std::ifstream data{rompath, std::ios_base::binary | std::ios_base::ate};
        std::streamsize size = data.tellg();
        data.seekg(0, std::ios_base::beg);
        data.read(this->memory.data() + instrbuf_addr, size);
        this->print_instrs(size);
    } catch(const fs::filesystem_error& e) 
    {
        std::cerr << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // P
    u8 font[80] = 
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    // I am going to place the font at 0x00 so I can then place the fmap below the instrs
    memmove(this->memory.data(), font, 80);

    // Register the function map
    this->register_fmap();
}


void ch8_emu::register_fmap() 
{
    this->funcs[0x00] = &ch8_emu::x00;
    this->funcs[0x01] = &ch8_emu::x01;
    this->funcs[0x02] = &ch8_emu::x02;
    this->funcs[0x03] = &ch8_emu::x03;
    this->funcs[0x04] = &ch8_emu::x04;
    this->funcs[0x05] = &ch8_emu::x05;
    this->funcs[0x06] = &ch8_emu::x06;
    this->funcs[0x07] = &ch8_emu::x07;
    this->funcs[0x08] = &ch8_emu::x08;
    this->funcs[0x09] = &ch8_emu::x09;
    this->funcs[0x0A] = &ch8_emu::x0A;
    this->funcs[0x0B] = &ch8_emu::x0B;
    this->funcs[0x0C] = &ch8_emu::x0C;
    this->funcs[0x0D] = &ch8_emu::x0D;
    this->funcs[0x0E] = &ch8_emu::x0E;
    this->funcs[0x0F] = &ch8_emu::x0F;
}

void ch8_emu::cpu_cycle(u8 first, u8 second) 
{
    //Fetch
    this->pc += 2;
    u16 fetched = ((((u16)first) << 8) & 0xff00) | (u16)second;

    // Decode
    ch8_emu::ch8_instr decoded;
    decoded.opcode = (u8)((fetched >> 12) & opcode_mask);
    decoded.vx = (u8)((fetched >> 8) & opcode_mask);
    decoded.vy = (u8)((fetched >> 4) & opcode_mask);
    decoded.n = (u8)(fetched & opcode_mask);
    decoded.nn = (u8)(fetched & nn_mask);
    decoded.nnn = (fetched & nnn_mask);

    // Command dispatch
    auto itr = this->funcs.find(decoded.opcode);
    if (itr != this->funcs.end()) 
    {
        std::invoke(itr->second, this, &decoded);
    }

    std::cerr << "Command unknown: " << decoded.opcode << std::endl;
    std::exit(EXIT_FAILURE);
}

/* Chipset functions */

/**
 * @brief execute the 0x00E0 and 0x00EE instructions
 * @param decoded deconstructed CHIP-8 instruction
 * 
 *
 */
void ch8_emu::x00(const ch8_instr* decoded) 
{
    if (decoded->nn == 0xE0) // Clear the display
    {
        this->framebuffer.fill(0);
    } else if (decoded->nn == 0xEE) // Return from subroutine
    {
        this->pc = this->stack->pop_addr();
    }
}

void ch8_emu::x01(const ch8_instr* decoded)
{

}

void ch8_emu::x02(const ch8_instr* decoded)
{

}

void ch8_emu::x03(const ch8_instr* decoded)
{

}

void ch8_emu::x04(const ch8_instr* decoded)
{

}

void ch8_emu::x05(const ch8_instr* decoded)
{

}

void ch8_emu::x06(const ch8_instr* decoded)
{

}

void ch8_emu::x07(const ch8_instr* decoded)
{

}

void ch8_emu::x08(const ch8_instr* decoded)
{

}

void ch8_emu::x09(const ch8_instr* decoded)
{

}

void ch8_emu::x0A(const ch8_instr* decoded)
{

}

void ch8_emu::x0B(const ch8_instr* decoded)
{

}

void ch8_emu::x0C(const ch8_instr* decoded)
{

}

void ch8_emu::x0D(const ch8_instr* decoded)
{

}

void ch8_emu::x0E(const ch8_instr* decoded)
{

}

void ch8_emu::x0F(const ch8_instr* decoded)
{

}

/* CHIP-8 Stack and Stack Frame functionality */

ch8_stack::ch8_stack()
{
    this->head = nullptr;
}

void ch8_stack::push_addr(u16 addr)
{
    ch8_stack::ch8_frame temp;
    temp.ret_addr = addr;
    temp.next = this->head;
    this->head = &temp;
}

u16 ch8_stack::pop_addr()
{
    ch8_stack::ch8_frame temp = *(this->head);
    this->head = this->head->next;
    return temp.ret_addr;
}
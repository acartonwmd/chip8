#include "chip8.h"
#include <cstdlib>

using namespace chip8;
using chip8::ch8_emu;

/**
 * @brief execute the 0x00E0 and 0x00EE instructions
 * @param decoded deconstructed CHIP-8 instruction
 * 
 * 
 .
 */
void ch8_emu::x00(const ch8_instr* decoded) 
{
    
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

ch8_emu::ch8_emu(fs::path rompath) 
{
    try 
    {
        std::ifstream data{rompath, std::ios_base::binary | std::ios_base::ate};
        std::streamsize size = data.tellg();
        data.seekg(0, std::ios_base::beg);
        data.read(this->memory + instrbuf_addr, size);
    } catch(const fs::filesystem_error& e) 
    {
        std::cerr << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }

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

    memmove(this->memory +  fontset_addr, font, 80);

    // Register the function map

}

void ch8_emu::start()
{
    while(true) 
    {
        // Emulates a CPU cycle
        cpu_cycle(this->memory[this->pc], this->memory[this->pc + 1]);
    }
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

    // Execute
    auto itr = this->funcs.find(decoded.opcode);
    if (itr == this->funcs.end()) 
    {
        std::cerr << "Command unknown: " << decoded.opcode << std::endl;
        std::exit(EXIT_FAILURE);
    }

    itr->second(&decoded);
}


/* Old opcode functions */
// switch (decoded_instr.opcode) {
//         case 0x00: {
//             if (decoded_instr.nn == 0xE0) {
//                 // 00E0: Clear the screen 
//                 for (int i = 0; i < 64; i++) {
//                     for (int j = 0; 32; j++) {
//                         this->framebuffer[i + j] = 0;
//                     }
//                 }

//             } else if (decoded_instr.nn == 0xEE) {
//                 // 00EE: Subroutine return, simply pop the top register from the stack
//                 this->pc = this->stack->pop_addr();

//             } else {
//                 fprintf(stderr, "00NN instr error: unknown instruction found - %x\n", decoded_instr.nn);
//                 exit(EXIT_FAILURE);
//             }

//             break;
//         }

//         case 0x01: {
//             if (!(decoded_instr.nnn < instrbuf_addr)) {
//                 this->pc = decoded_instr.nnn;
//             } else {
//                 fprintf(stderr, "1NNN memory error: address %x less than 0x200, the lowest allowed address\n", decoded_instr.nnn);
//                 exit(EXIT_FAILURE);
//             }

//             break;
//         }

//         case 0x02: {
//             if (!(decoded_instr.nnn < instrbuf_addr)) {
//                 this->stack->push_addr(this->pc);
//                 this->pc = decoded_instr.nnn;
//                 break;
//             } else {
//                 fprintf(stderr, "2NNN memory error: address %x less than 0x200, the lowest allowed address\n", decoded_instr.nnn);
//                 exit(EXIT_FAILURE);
//             }
//         }

//         case 0x03: {
//             // 3XNN: if VX == NN, skip 1 instr (2 bytes)
//             if (this->registers[decoded_instr.vx] == decoded_instr.nn) {
//                 this->pc += 2;
//             }

//             break;
//         }

//         case 0x04: {
//             // 4XNN: skip 1 instr if VX != NN
//             if (this->registers[decoded_instr.vx] != decoded_instr.nn) {
//                 this->pc += 2;
//             }

//             break;
//         }

//         case 0x05: {
//             // 5XY0: skip 1 instr if REGS[VX] == REGS[VY]
//             if (this->registers[decoded_instr.vx] == this->registers[decoded_instr.vy]) {
//                 this->pc += 2;
//             }

//             break;
//         }

//         case 0x06: {
//             // 6XNN: set VX = NN
//             this->registers[decoded_instr.vx] = decoded_instr.nn;
//             break;
//         }

//         case 0x07: {
//             // 7XNN: add NN to the value of VX
//             // REGS[VX] += NN
//             this->registers[decoded_instr.vx] += decoded_instr.nn;
//             break;
//         }

//         case 0x08: {
//             switch (decoded_instr.n) {
//                 case 0: {
//                     // 8XY0: VX = VY
//                     this->registers[decoded_instr.vx] = this->registers[decoded_instr.vy];
//                     break;
//                 }

//                 case 1: {
//                     // 8XY1: VX = VX | VY (bitwise OR)
//                     this->registers[decoded_instr.vx] |= this->registers[decoded_instr.vy];
//                     break;
//                 }

//                 case 2: {
//                     // 8XY2: VX = VX & VY (bitwise AND)
//                     this->registers[decoded_instr.vx] &= this->registers[decoded_instr.vy];
//                     break; 
//                 }

//                 case 3: {
//                     // 8XY3: VX = VX XOR VY (bitwise XOR)
//                     this->registers[decoded_instr.vx] ^= this->registers[decoded_instr.vy];
//                     break;
//                 }

//                 case 4: {
//                     // 8XY4: VX = VX + VY
//                     uint8_t vx_temp = this->registers[decoded_instr.vx];
//                     uint8_t vy_temp = this->registers[decoded_instr.vy];

//                     if (vx_temp > UINT8_MAX - vy_temp) {
//                         this->registers[0x0F] = 0x01;
//                     } else {
//                         this->registers[0x0F] = 0x00;
//                     }

//                     this->registers[decoded_instr.vx] += vy_temp;
//                     break;
//                 }

//                 case 5: {
//                     // 8XY5: VX = VX - VY

//                 }

//                 default: {
//                     fprintf(stderr, "8XYN error: not a recognized function opcode for N\n");
//                     exit(EXIT_FAILURE);
//                 }
//             }
//             break;
//         }

//         case 0x09: {
//             // 9XY0: skip 1 instr if REGS[VX] != REGS[VY]
//             if (this->registers[decoded_instr.vx] != this->registers[decoded_instr.vy]) {
//                 this->pc += 2;
//             }


//             break;
//         }

//         case 0x0A: {
//             // ANNN: Set index reg to NNN
//             if (!(decoded_instr.nnn < instrbuf_addr)) {
//                 this->index = decoded_instr.nnn;
//                 break;
//             } else {
//                 fprintf(stderr, "ANNN memory error: address %x less than 0x200, the lowest allowed address\n", decoded_instr.nnn);
//                 exit(EXIT_FAILURE);
//             }
//         }

//         case 0x0B: {
//             // BXNN: jump to XNN + REGS[VX]
//             this->pc = decoded_instr.nnn + this->registers[decoded_instr.vx];
//             break;
//         }

//         case 0x0C: {
//             srand(time(NULL));
//             this->registers[decoded_instr.vx] = rand() & decoded_instr.nn;
//             break;
//         }

//         case 0x0D: {
//             break;
//         }

//         case 0x0E: {
//             break;
//         }
        
//         case 0x0F: {
//             switch (decoded_instr.nn) {
//                 case 0x55: {
//                     // FX55: set each byte of index + i to REGS[i] until i == X
//                     u16 temp_index = this->index;
//                     for (u8 i = 0; i <= decoded_instr.vx; i++) {
//                         this->memory[temp_index + i] = this->registers[i];
//                     }

//                     break;
//                 }
//             }
//         }

//         default: {
//             fprintf(stderr, "Unknown instruction identified\n");
//             exit(EXIT_FAILURE);
//         }
//     }
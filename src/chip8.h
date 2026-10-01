#pragma once

#include "../imgui/imgui.h"
#include <string>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ios>
#include <filesystem>
#include <algorithm>
#include <cstring>
#include <functional>
#include <unordered_map>

namespace chip8 
{
    typedef std::uint8_t u8;
    typedef std::uint16_t u16;
    typedef std::uint32_t u32;

    const u16 instrbuf_addr = 0x0200;
    const u8 register_count = 16;
    const u16 memory_capacity = 4096;
    const u16 opcode_mask = 0x000F;
    const u16 nn_mask = 0x00FF;
    const u16 nnn_mask = 0x0FFF;
    const u8 fontset_addr = 0x00;
    const u16 display_resolution = 64 * 32;

    namespace fs = std::filesystem;

    class ch8_stack 
    {
        private:
            typedef struct stack_frame 
            {
                struct stack_frame* next;
                u16 ret_addr;
            } ch8_frame;
        
            ch8_frame* head;

        public:
            ch8_stack();
            void push_addr(u16 addr);
            u16 pop_addr();
    };

    class ch8_emu 
    {
        private:
            typedef struct instr 
            {
                u16 nnn;
                u8 nn;
                u8 n;
                u8 vx;
                u8 vy;
                u8 opcode;
            } ch8_instr;

            using execute_opcode = std::function<void(const ch8_instr*)>;
            using fmap = std::unordered_map<u8, execute_opcode>;

            char memory[memory_capacity] = {};
            u8 framebuffer[display_resolution] = {};
            fmap funcs;
            u8 registers[register_count] {};
            ch8_stack* stack;
            u16 pc = instrbuf_addr;
            u16 index = 0;
            u8 delay = 0;
            u8 sound = 0;

            // Chipset instructions
            void x00(const ch8_instr* decoded);
            void x01(const ch8_instr* decoded);
            void x02(const ch8_instr* decoded);
            void x03(const ch8_instr* decoded);
            void x04(const ch8_instr* decoded);
            void x05(const ch8_instr* decoded);
            void x06(const ch8_instr* decoded);
            void x07(const ch8_instr* decoded);
            void x08(const ch8_instr* decoded);
            void x09(const ch8_instr* decoded);
            void x0A(const ch8_instr* decoded);
            void x0B(const ch8_instr* decoded);
            void x0C(const ch8_instr* decoded);
            void x0D(const ch8_instr* decoded);
            void x0E(const ch8_instr* decoded);
            void x0F(const ch8_instr* decoded);
            
            void cpu_cycle(u8 first, u8 second);
            void register_fmap();

        public:
            ch8_emu(fs::path rompath);
            void start();
    };
};
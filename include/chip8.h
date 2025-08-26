#include <cstdint>  // unint8_t, uint16_t, etc..
#include <fstream>  // opening files
#include <iostream> // output to cli
#include <chrono>   // timing and random seed
#include <random>   // random functions

class chip8_tobias
{
private:
public:
    static constexpr uint16_t START = 0x200;
    static constexpr uint16_t END = 0x1FF;
    static constexpr uint16_t FONT_START = 0x050;
    static constexpr uint16_t FONT_START = 0x09F;
    static constexpr uint16_t START_SPACE = 0x000;
    static constexpr uint16_t END_SPACE = 0xFFF;
    // CHIP-8 has the following components:
    // Memory: CHIP-8 has direct access to up to 4 kilobytes of RAM
    static uint8_t memory[4096];
    // Display: 64 x 32 pixels (or 128 x 64 for c8-CHIP) monochrome, ie. black or white
    static uint32_t display[128 * 64];
    // A program counter, often called just “PC”, which points at the current instruction in memory
    static uint16_t pc;
    // One 16-bit index register called “I” which is used to point at locations in memory
    static uint16_t I;
    // A stack for 16-bit addresses, which is used to call subroutines/functions and return from them
    static u_int16_t stack[16]; // 16, 2 byte entries.
                                // An 8-bit delay timer which is decremented at a rate of 60 Hz (60 times per second) until it reaches 0
    static u_int8_t delay_timer;
    // An 8-bit sound timer which functions like the delay timer, but which also gives off a beeping sound as long as it’s not 0
    static u_int8_t sound_timer;
    // 16 8-bit (one byte) general-purpose variable registers numbered 0 through F hexadecimal, ie. 0 through 15 in decimal, called V0 through VF
    static u_int8_t registers[16];
    // VF is also used as a flag register; many instructions will set it to either 1 or 0 based on some rule, for example using it as a carry flag
    static char vf;

    static constexpr uint8_t FONT[80] = {
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

    static uint8_t keypad[16];

    chip8_tobias();
    ~chip8_tobias();

    void fetch();
    void decode();
};
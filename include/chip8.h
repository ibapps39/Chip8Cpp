#include <cstdint> // unint8_t, uint16_t, etc..
#include <fstream>
#include <iostream>
#include <chrono>
#include <random>

#define DEFAULT_MEM_SIZE 4096 // bytes
#define DEFAULT_WIDTH 64
#define DEFAULT_HEIGHT 32
#define DEFAULT_REGISTER_STACK_SIZE 16

class chip8
{
private:
    /* data */
    std::default_random_engine rng;
    std::uniform_int_distribution<uint8_t> randByte;

public:
    // static constexpr to avoid wasting memory, allocate to the class not the instance

    static constexpr int MEM_SIZE = DEFAULT_MEM_SIZE;
    static constexpr int DISPLAY_WIDTH = DEFAULT_WIDTH;
    static constexpr int DISPLAY_HEIGHT = DEFAULT_HEIGHT;
    static constexpr int REGISTER_STACK_SIZE = DEFAULT_REGISTER_STACK_SIZE;

    static constexpr uint16_t RESERVED_START = 0x000; // Memory starting address, reserved for interpreter
    static constexpr uint16_t RESERVED_END = 0x1FF;   // Memory ending address, reserved for interpreter

    static constexpr uint16_t MEM_START = 0x000;
    static constexpr uint16_t MEM_END = 0xFFF;

    static constexpr uint16_t DATA_START = 0x200;     // Data space min
    static constexpr uint16_t DATA_END = 0xFFF;       // Data space max
    static constexpr uint16_t DATA_ETI_START = 0x600; // (alt.) Data space

    static constexpr uint16_t FONT_START = 0x050;
    static constexpr uint16_t FONT_END = 0x09F;

    static constexpr uint16_t STORAGE_START = 0x050;
    static constexpr uint16_t STORAGE_END = 0x0A0;

    // struct Registers // 8bit General purpose registers array, Vx
    // {
    //     uint16_t I; // "I", index, register stores memory address
    //     union
    //     {
    //         uint8_t V[16];
    //         struct
    //         {
    //             uint8_t V0, V1, V2, V3, V4, V5, V6, V7, V8, V9, VA, VB, VC, VD, VE, VF;
    //         };
    //     };
    // } registers;

    uint8_t sp = 0;                // 8bit Stack pointer
    uint8_t memory[MEM_SIZE] = {}; // Memory, RAM, array
    uint8_t v_registers[16] = {};  // 8bit General purpose registers array, Vx
    uint8_t delay_timer = 0;
    uint8_t sound_timer = 0;
    uint8_t keypad[16] = {};

    uint16_t stack[REGISTER_STACK_SIZE] = {};
    uint16_t pc = DATA_START; // Program counter
    uint16_t index = {};      // "I", index, register stores memory address
                              // 0000 0000 0000 0000 -> opcode, x, y, value - nibble
    uint16_t opcode;

    uint32_t video[DISPLAY_HEIGHT * DISPLAY_WIDTH] = {};

    static constexpr uint8_t FONT_SET[80] = {
        // Fonts, 15 5bit characters
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

    chip8() = default;
    ~chip8() = default;

    // Fetch, Decode, Execute
    void Cycle() {};
    // Read ROMs
    void LoadROM(char const *filename) {};

    // OPCODES
    void OP_00E0() {};
    void OP_00EE() {};
    void OP_1nnn() {};
    void OP_2nnn() {};
    void OP_3xkk() {};
    void OP_4xkk() {};
    void OP_5xy0() {};
    void OP_6xkk() {};
    void OP_7xkk() {};
    void OP_8xy0() {};
    void OP_8xy1() {};
    void OP_8xy2() {};
    void OP_8xy3() {};
    void OP_8xy4() {};
    void OP_8xy5() {};
    void OP_8xy6() {};
    void OP_8xy7() {};
    void OP_8xyE() {};
    void OP_9xy0() {};
    void OP_Annn() {};
    void OP_Bnnn() {};
    void OP_Cxkk() {};
    void OP_Dxyn() {};
    void OP_Ex9E() {};
    void OP_ExA1() {};
    void OP_Fx07() {};
    void OP_Fx0A() {};
    void OP_Fx15() {};
    void OP_Fx18() {};
    void OP_Fx1E() {};
    void OP_Fx29() {};
    void OP_Fx33() {};
    void OP_Fx55() {};
    void OP_Fx65() {};
    void OP_NULL() {};

    // TABLES
    typedef void (chip8::*chip8Func)();
    chip8Func table[0x10]; // Master table
    chip8Func table0[0xF];
    chip8Func table8[0xF];
    chip8Func tableE[0xF];
    chip8Func tableF[0x65 + 1];

    void Table0()
    {
        ((*this).*(table0[opcode & 0x000Fu]))();
    }

    void TableE()
    {
        ((*this).*(tableE[opcode & 0x000Fu]))();
    }

    void Table8()
    {
        ((*this).*(table8[opcode & 0x000Fu]))();
    }

    void TableF()
    {
        ((*this).*(tableF[opcode & 0x00FFu]))();
    }

    // Does nothing, dummy function for bad calls
    void TableNULL() {}
};
chip8::chip8() : rng(std::chrono::system_clock::now().time_since_epoch().count())
{
    // Start Program
    pc = DATA_START;
    // Load fonts
    for (unsigned int i = 0; i < sizeof(FONT_SET); ++i)
    {
        memory[FONT_START + i] = FONT_SET[i];
    }

    // random byte via rng -> system_clock
    randByte = std::uniform_int_distribution<uint8_t>(0, 255U);

    // populate the empty spots for bad calls and padding
    for (size_t i = 0; i <= 0xE; i++)
    {
        table0[i] = &chip8::OP_NULL;
        table8[i] = &chip8::OP_NULL;
        tableE[i] = &chip8::OP_NULL;
    }

    // Table0
    table0[0x0] = &chip8::OP_00E0;
    table0[0xE] = &chip8::OP_00EE;

    // Table8
    table8[0x0] = &chip8::OP_8xy0;
    table8[0x1] = &chip8::OP_8xy1;
    table8[0x2] = &chip8::OP_8xy2;
    table8[0x3] = &chip8::OP_8xy3;
    table8[0x4] = &chip8::OP_8xy4;
    table8[0x5] = &chip8::OP_8xy5;
    table8[0x6] = &chip8::OP_8xy6;
    table8[0x7] = &chip8::OP_8xy7;
    table8[0xE] = &chip8::OP_8xyE;

    // TableE
    tableE[0x1] = &chip8::OP_ExA1;
    tableE[0xE] = &chip8::OP_Ex9E;

    // TableF
    tableF[0x07] = &chip8::OP_Fx07;
    tableF[0x0A] = &chip8::OP_Fx0A;
    tableF[0x15] = &chip8::OP_Fx15;
    tableF[0x18] = &chip8::OP_Fx18;
    tableF[0x1E] = &chip8::OP_Fx1E;
    tableF[0x29] = &chip8::OP_Fx29;
    tableF[0x33] = &chip8::OP_Fx33;
    tableF[0x55] = &chip8::OP_Fx55;
    tableF[0x65] = &chip8::OP_Fx65;

    // Populate Master Table
    using Instruction = void (chip8::*)();
    Instruction table[16] =
        {
            &chip8::Table0,
            &chip8::OP_1nnn,
            &chip8::OP_2nnn,
            &chip8::OP_3xkk,
            &chip8::OP_4xkk,
            &chip8::OP_5xy0,
            &chip8::OP_6xkk,
            &chip8::OP_7xkk,
            &chip8::Table8,
            &chip8::OP_9xy0,
            &chip8::OP_Annn,
            &chip8::OP_Bnnn,
            &chip8::OP_Cxkk,
            &chip8::OP_Dxyn,
            &chip8::TableE,
            &chip8::TableF
        };
}

chip8::~chip8()
{
}

void chip8::Cycle()
{
    // Fetch, whichever is true. Combines bytes to make a 16 No *(uint16_t*)&memory[pc], ignores endianess
    opcode = (memory[pc] << 8u) | memory[pc+1];
    
    // Increment before exec.
    pc += 2;

    // Decode and Execute
    ((*this).*(table[(opcode & 0xF000u) >> 12u]))();

    // Decrement the delay timer if it's been set
    if (delay_timer > 0)
    {
        --delay_timer;
    }
    // Decrement the sound timer if it's been set
    if (sound_timer > 0)
    {
        --sound_timer;
    }
}

// Loads ROM as stream of binary to buffer and copies to the start of chip8 memory/ram.
void chip8::LoadROM(char const *filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate); // std::ios::ate sets to end of stream

    if (!file.is_open())
    {
        std::clog << "Could not open file or no file is loaded!\n";
    }
    std::streampos size = file.tellg();
    char *buffer = new char[size];

    file.seekg(0, std::ios::beg); // move file (ptr) to beginning of file.
    file.read(buffer, size);
    file.close();

    for (long i = 0; i < size; ++i)
    {
        chip8::memory[DATA_START + i] = buffer[i];
    }

    delete[] buffer;
}

// OP CLS, clear screen.
void chip8::OP_00E0()
{
    memset(video, 0, sizeof(video));
}

// OP RET, return from sub-routine.
void chip8::OP_00EE()
{
    --sp;
    pc = stack[sp];
}

// OP JP addr, jump to address nnn via bitmask.
void chip8::OP_1nnn()
{
    // assign the program counter to the lower 12 bits
    pc = opcode & 0x0FFFu;
    // NNN = 0000 1111 1111 11111 -> ()0000 (N)ibble (N)ibble (N)ibble
}

// OP CALL addr, call subroutine and return.
void chip8::OP_2nnn()
{
    stack[sp] = pc; // Push current pc before JP
    ++sp;           // Move sp up, for 00EE
    pc = opcode & 0x0FFFu;
}

// OP SE Vx, kk. Skip(3) if Vx[x] == kk. Where kk -> 0000 0000 1111 1111, an 8bit/byte immediate value
void chip8::OP_3xkk()
{
    // uint8_t x = (opcode & 0x0F00u) >> 8u;
    // uint8_t kk = opcode & 0x00FFu;
    bool skip = v_registers[(opcode & 0x0F00u) >> 8u] == (opcode & 0x00FFu);
    if (!skip)
        return;
    pc += 2;
}

// OP SNE Vx, kk. Skip(4) if Vx[x] != kk. Opposite, similar, to OP_3xkk.
void chip8::OP_4xkk()
{
    // uint8_t x = (opcode & 0x0F00u) >> 8u;
    // uint8_t kk = opcode & 0x00FFu;
    bool skip = v_registers[(opcode & 0x0F00u) >> 8u] != (opcode & 0x00FFu);
    if (!skip)
        return;
    pc += 2;
}

// 0000 0000 0000 0000
//      x    y
// OP SE Vx, Vy. Skip if Vx = Vy
void chip8::OP_5xy0()
{
    // uint8_t x = (opcode & 0x0F00u) >> 8u; // Vx
    // uint8_t y = (opcode & 0x00F0u) >> 4u; // Vy
    bool skip = v_registers[(opcode & 0x0F00u) >> 8u] == v_registers[(opcode & 0x00F0u) >> 4u];
    if (!skip)
        return;
    pc += 2;
}

// OP LD Vx, kk
void chip8::OP_6xkk()
{
    v_registers[(opcode & 0x0F00u) >> 8u] = (opcode & 0x00FFu);
}

// OP ADD Vx, kk
void chip8::OP_7xkk()
{
    v_registers[(opcode & 0x0F00u) >> 8u] += (opcode & 0x00FFu);
}

// OP LD Vx, Vy
void chip8::OP_8xy0()
{
    v_registers[(opcode & 0x0F00u) >> 8u] = v_registers[(opcode & 0x00F0u) >> 4u];
}

// OP OR Vx, Vy
void chip8::OP_8xy1()
{
    v_registers[(opcode & 0x0F00u) >> 8u] |= v_registers[(opcode & 0x00F0u) >> 4u];
}

// OP AND Vx, Vy
void chip8::OP_8xy2()
{
    v_registers[(opcode & 0x0F00u) >> 8u] &= v_registers[(opcode & 0x00F0u) >> 4u];
}

// OP XOR Vx, Vy
void chip8::OP_8xy3()
{
    v_registers[(opcode & 0x0F00u) >> 8u] ^= v_registers[(opcode & 0x00F0u) >> 4u];
}

// OP ADD Vx, Vy. VF = carry.
void chip8::OP_8xy4()
{
    // Vx, mask out x and shift to make it 8 bit 0000 0000 0000 0000
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    // uint8_t Vf = 0xF;

    uint16_t sum = v_registers[Vx] + v_registers[Vy];

    // If sum is too big / >byte, set Vf to 1, else 0. Remember VF is a special flag!
    v_registers[0xF] = (sum > 255U) ? 1 : 0;

    // Keep only lowest 8 bits, big-endian. 0000 0000 xxxx xxxx & 1111 1111 -> xxxx xxxx
    v_registers[Vx] = sum & 0xFFu;
}

// // OP SUB Vx, Vy
void chip8::OP_8xy5()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    // uint8_t Vf = 0xF;

    v_registers[0xF] = (Vx > Vy) ? 1 : 0;

    v_registers[Vx] -= v_registers[Vy];
}

// OP SHR Vx, Vy. If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
void chip8::OP_8xy6()
{
    // uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    v_registers[0xF] = v_registers[(opcode & 0x0F00u) >> 8u] & 0x1u;
    v_registers[0xF] >>= 1;
}

// OP SUBN Vx, Vy. Set Vx = Vy - Vx, set VF = NOT borrow.
void chip8::OP_8xy7()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    v_registers[0xF] = (Vy > Vx) ? 1 : 0;
    v_registers[Vx] = v_registers[Vy] - v_registers[Vx];
}

// OP SHL Vx, {, Vy}. Vx = Vx SHL 1, Shift left. Similar to OP_SHR
void chip8::OP_8xyE()
{
    // uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    //  Mask first bit, then isolate.
    v_registers[0xF] = (v_registers[(opcode & 0x0F00u) >> 8u] & 0x80u) >> 7u;
    v_registers[0x0F00u] <<= 1;
}
// OP SNE Vx, Vy. Skip next instruction if Vx != Vy
void chip8::OP_9xy0()
{
    bool skip = v_registers[(opcode & 0x0F00u) >> 8u] != v_registers[(opcode & 0x00F0u) >> 4u];
    if (!skip)
        return;
    pc += 2;
}

// OP LD I, addr. Set I = nnn
void chip8::OP_Annn()
{
    chip8::index = opcode & 0x0FFFu;
}

// OP JP V0, addr. Jump to nnn + V0
void chip8::OP_Bnnn()
{
    pc = v_registers[0] + (opcode & 0x0FFFu);
}

// OP RND Vx, byte. Set Vx to a random byte AND kk
void chip8::OP_Cxkk()
{
    v_registers[(opcode & 0x0F00u) >> 8u] = randByte(chip8::rng) & (opcode & 0x00FFu);
}

// OP DRW Vx, Vy, nibble. Draw Sprite (starting from I) at (Vx, Vy), n = height, VF = collision
void chip8::OP_Dxyn()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    uint8_t height = opcode & 0x000Fu; // n

    uint8_t x_pos = v_registers[Vx] % DISPLAY_WIDTH;
    uint8_t y_pos = v_registers[Vy] % DISPLAY_HEIGHT;

    v_registers[0xF] = 0; // Initialize VF, collision

    for (size_t row = 0; row < height; ++row)
    {
        uint8_t sprite_b = chip8::memory[chip8::index + row];
        for (size_t col = 0; col < 8; ++col)
        {
            uint8_t spritePixel = sprite_b & (0x80 >> col);
            uint32_t *screenPx = &video[(y_pos + row) * DISPLAY_WIDTH + (x_pos + col)];

            if (spritePixel)
            {
                if (*screenPx == 0xFFFFFFFF)
                    v_registers[0xF] = 1;

                *screenPx ^= 0xFFFFFFFF;
            }
        }
    }
}

// OP Ex9E - SKP Vx, skip next instruction if keypad presses Vx
void chip8::OP_Ex9E()
{
    bool skip = keypad[v_registers[(opcode & 0x0F00u) >> 8u]];
    if (!skip)
        return;
    pc += 2;
}

// ExA1 - SKNP Vx, skip next instruction if Vx not pressed
void chip8::OP_ExA1()
{
    if (!keypad[v_registers[(opcode & 0x0F00u) >> 8u]])
        return;
    pc += 2;
}

// LD Vx, DT, Vx = delay timer
void chip8::OP_Fx07()
{
    v_registers[(opcode & 0x0F00u) >> 8u] = delay_timer;
}

// LD Vx, K. Wait for keypress and store in Vx
void chip8::OP_Fx0A()
{
    for (size_t key = 0; key < 16; ++key)
    {
        if (keypad[key])
        {
            v_registers[(opcode & 0x0F00u) >> 8u] = key;
            return;
        }
    }
    pc -= 2;
}

// LD DT, Vx
void chip8::OP_Fx15()
{
    delay_timer = v_registers[(opcode & 0x0F00u) >> 8u];
}

// LD ST = Vx
void chip8::OP_Fx18()
{
    sound_timer = v_registers[(opcode & 0x0F00u) >> 8u];
}

// ADD I, VX
void chip8::OP_Fx1E()
{
    index = index + v_registers[(opcode & 0x0F00u) >> 8u];
}

// LF F, Vx
void chip8::OP_Fx29()
{
    uint8_t digit = v_registers[(opcode & 0x0F00u) >> 8u];
    index = FONT_START + (5 * digit);
}

// LD B, Vx B => Binary-Codede Decimal.  (floats truncated)
void chip8::OP_Fx33()
{
    uint8_t val = v_registers[(opcode & 0x0F00u) >> 8u];
    // 100s
    memory[index] = val % 10;
    // 10s
    memory[index + 1] = (val / 10) % 10;
    // 1s
    memory[index + 2] = (val / 100) % 10;
}

// LD [I], Vx, store/write V0 through Vx in memory starting from I
void chip8::OP_Fx55()
{
    for (size_t i = 0; i <= ((opcode & 0x0F00u) >> 8u); ++i)
    {
        memory[index + i] = v_registers[i];
    }
}

// LD Vx, [I], read V0 through Vx
void chip8::OP_Fx65()
{
    for (size_t i = 0; i <= ((opcode & 0x0F00u) >> 8u); ++i)
    {
        v_registers[i] = memory[index + i];
    }
}
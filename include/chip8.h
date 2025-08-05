#include <cstdint> // unint8_t, uint16_t, etc..
#include <fstream>
#include <iostream>

#define DEFAULT_MEM_SIZE 4096 // bytes
#define DEFAULT_WIDTH 64
#define DEFAULT_HEIGHT 32
#define DEFAULT_REGISTER_STACK_SIZE 16

class chip8
{
private:
    /* data */

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

    uint8_t stack_pointer = 0;     // 8bit Stack pointer
    uint8_t memory[MEM_SIZE] = {}; // Memory, RAM, array
    uint8_t v_registers[16] = {};  // 8bit General purpose registers array, Vx
    uint8_t delay_timer = 0;
    uint8_t sound_timer = 0;
    uint8_t keypad[16] = {};

    uint16_t stack[REGISTER_STACK_SIZE] = {};
    uint16_t prog_counter = DATA_START; // Program counter
    uint16_t index = {};                // "I", index, register stores memory address
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

    void LoadROM(char const *filename) {};
};

chip8::chip8(/* args */)
{
    prog_counter = DATA_START;
}

chip8::~chip8()
{
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

    file.seekg(0, std::ios::beg);
    file.read(buffer, size);
    file.close();

    for (long i = 0; i < size; ++i)
    {
        chip8::memory[DATA_START + i] = buffer[i];
    }
    delete[] buffer;
}

// #include <cstdint> // unint8_t, uint16_t, etc..
// #include <fstream>
// #include <iostream>

// #define DEFAULT_MEM_SIZE 4096 // bytes
// #define DEFAULT_WIDTH 64
// #define DEFAULT_HEIGHT 32
// #define DEFAULT_REGISTER_STACK_SIZE 16

// class chip8
// {
// private:
//     /* data */

// public:
//     // static constexpr to avoid wasting memory, allocate to the class not the instance

//     static constexpr int MEM_SIZE = DEFAULT_MEM_SIZE;
//     static constexpr int DISPLAY_WIDTH = DEFAULT_WIDTH;
//     static constexpr int DISPLAY_HEIGHT = DEFAULT_HEIGHT;
//     static constexpr int REGISTER_STACK_SIZE = DEFAULT_REGISTER_STACK_SIZE;

//     static constexpr uint16_t RESERVED_START = 0x000; // Memory starting address, reserved for interpreter
//     static constexpr uint16_t RESERVED_END = 0x1FF; // Memory ending address, reserved for interpreter

//     static constexpr uint16_t MEM_START = 0x000;
//     static constexpr uint16_t MEM_END = 0xFFF;

//     static constexpr uint16_t DATA_START = 0x200;     // Data space min
//     static constexpr uint16_t DATA_END = 0xFFF;       // Data space max
//     static constexpr uint16_t DATA_ETI_START = 0x600; // (alt.) Data space

//     static constexpr uint16_t FONT_START = 0x050;
//     static constexpr uint16_t FONT_END = 0x09F;

//     static constexpr uint16_t STORAGE_START = 0x050;
//     static constexpr uint16_t STORAGE_END = 0x0A0;

//     uint8_t stack_pointer = 0;                        // 8bit Stack pointer
//     std::array<uint8_t, MEM_SIZE> memory = {}; // Memory, RAM, array
//     std::array<uint8_t, 16> v_registers = {};                    // 8bit General purpose registers array, Vx
//     uint8_t delay_timer = 0;
//     uint8_t sound_timer = 0;
//     std::array<uint8_t, 16>keypad = {};

//     std::array<uint16_t, REGISTER_STACK_SIZE> stack = {};
//     uint16_t prog_counter = DATA_START; // Program counter
//     uint16_t index = {};           // "I", index, register stores memory address
//     uint16_t opcode;

//     uint32_t video[DISPLAY_HEIGHT*DISPLAY_WIDTH] = {};

//     static constexpr uint8_t FONT_SET[80] = { // Fonts, 15 5bit characters
//         0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
//         0x20, 0x60, 0x20, 0x20, 0x70, // 1
//         0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
//         0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
//         0x90, 0x90, 0xF0, 0x10, 0x10, // 4
//         0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
//         0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
//         0xF0, 0x10, 0x20, 0x40, 0x40, // 7
//         0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
//         0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
//         0xF0, 0x90, 0xF0, 0x90, 0x90, // A
//         0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
//         0xF0, 0x80, 0x80, 0x80, 0xF0, // C
//         0xE0, 0x90, 0x90, 0x90, 0xE0, // D
//         0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
//         0xF0, 0x80, 0xF0, 0x80, 0x80  // F
//     };

//     chip8() = default;
//     ~chip8() = default;

//     void LoadROM(char const* filename){};
// };

// chip8::chip8(/* args */)
// {
// }

// chip8::~chip8()
// {
// }

// // Loads ROM as stream of binary to buffer and copies to the start of chip8 memory/ram.
// void chip8::LoadROM(char const* filename)
// {
//     std::ifstream file(filename, std::ios::binary | std::ios::ate); // std::ios::ate sets to end of stream

//     if (!file.is_open()){
//         std::clog << "Could not open file or no file is loaded!\n";
//         return;
//     }
//     std::streampos size = file.tellg();

//     // if size is zero...

//     file.seekg(0, std::ios::beg);
//     std::vector<char> buffer(size);

//     std::copy(buffer.begin(), buffer.end(), &chip8::memory[DATA_START]);

// }
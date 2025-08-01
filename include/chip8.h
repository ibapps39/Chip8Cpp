#include <cstdint> // unint8_t, uint16_t, etc..

#define DEFAULT_MEM_SIZE 4096 // bytes
#define DEFAULT_WIDTH 64
#define DEFAULT_HEIGHT 32
#define DEFAULT_REGISTER_STACK_SIZE 16

class chip8
{
private:
    /* data */
    ;
public:
    const int MEM_SIZE = DEFAULT_MEM_SIZE;
    const int DISPLAY_WIDTH = DEFAULT_WIDTH;
    const int DISPLAY_HEIGHT = DEFAULT_HEIGHT;
    const uint16_t PROG_START = 0x200;

    uint16_t stack[DEFAULT_REGISTER_STACK_SIZE];
    uint16_t PC = PROG_START;
    uint8_t SP = ;
    uint8_t MEM_SIZE[DEFAULT_MEM_SIZE];
    chip8() : PC(0x200), SP(0);
    ~chip8();
};

chip8::chip8(/* args */)
{
}

chip8::~chip8()
{
}

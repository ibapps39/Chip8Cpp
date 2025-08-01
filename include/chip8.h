#include <cstdint>

#define MEM_SIZE

class chip8
{
private:
    /* data */
    ;
public:
    uint16_t stack[16];
    uint16_t PC;
    uint8_t SP;
    uint8_t memory[4096];
    chip8() : PC(0x200), SP(0);
    ~chip8();
};

chip8::chip8(/* args */)
{
}

chip8::~chip8()
{
}

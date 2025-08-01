# Project Structure
```zsh
chip8/
├── src/
│   ├── main.cpp
│   ├── chip8.cpp
│   
├── include/
│   └── chip8.hpp
├── build/
├── roms/             # Optional ROMs for testing
├── assets/           # Fonts or UI assets (if needed)
├── Makefile
└── .gitignore
```
# Components - CPU
- [4K memory](#) 
    - 4096 bytes of memory for instructions and data  
- [16 8-bit registers](#) 
    - General purpose registers V0 to VF (VF is also a flag)  
- [16-bit Index register](#) 
    - Used for memory addressing  
- [Program Counter](#) 
    - Points to the current instruction in memory  
- [Stack + Stack Pointer](#) 
    - Supports up to 16 levels of subroutine calls  
- [Delay timer](#) 
    - Counts down at 60Hz, used for timing events  
- [Sound timer](#) 
    - Counts down at 60Hz, triggers beep when non-zero  
- [Instruction decoding & execution (about 35 opcodes)](#) 
    - Implements CHIP-8 instruction set  

# Components - Graphics
- [64x32 monochrome display](#) - Low resolution display for sprites and visuals  
- [Framebuffer (bool array or uint8_t[64][32])](#) - Stores pixel states for rendering  
- [Opcode DXYN to draw sprites using XOR](#) - Draws sprites by XORing pixels, supports collision detection  

# Components - Input
- [16-key hex keypad (0x0–0xF)](#) - CHIP-8 keypad input mapping  
- [Map to keyboard input (SDL, or ncurses, or raw terminal input)](#) - Interface for physical keyboard handling  
- [Handle Fx0A (wait for key press)](#) - Pauses execution until a key is pressed  

# Components - Timers
- [60Hz decrementing timers](#) - Delay and sound timers count down at 60Hz  
- [Possibly use std::chrono, std::thread, or SDL timers](#) - Methods for implementing timer updates  

# Extras / Optional Features
- [Super CHIP-8 support (higher resolution)](#) - Enhanced graphics mode (128x64)  
- [Save/load emulator state](#) - Ability to serialize and restore emulator state  
- [Speed control (e.g., cycle delay)](#) - Control execution speed of emulator cycles  
- [Sound (beep on timer)](#) - Generate audible beep when sound timer active  

# ROMs and References
- [Test ROM - dmatlack](https://github.com/dmatlack/chip8/tree/master/roms)
    - https://github.com/dmatlack/chip8/tree/master/roms
- [Tobias V. Langhoff - Opcode reference](https://tobiasvl.github.io/blog/write-a-chip-8-emulator)
    - https://tobiasvl.github.io/blog/write-a-chip-8-emulator
- [Tim Stamper’s spec](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
    - http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
- [BUILDING A CHIP-8 EMULATOR [C++]](https://austinmorlan.com/posts/chip8_emulator/?ref=codebldr#16-8-bit-registers) 
    - https://austinmorlan.com/posts/chip8_emulator/?ref=codebldr#16-8-bit-registers

# ✅ CHIP-8 Emulator TODO List (Minimal C++ Version, No Dependencies)

## ⚙️ Project Setup
1. **Create Project File**
   - Make a file: `main.cpp`.
   - Use standard headers only:  
     ```cpp
     #include <iostream>
     #include <fstream>
     #include <cstdint>
     #include <cstdlib>  // for rand()
     ```

2. **Define CHIP-8 Constants**
   ```cpp
   const int MEMORY_SIZE = 4096;
   const int DISPLAY_WIDTH = 64;
   const int DISPLAY_HEIGHT = 32;
   const uint16_t PROGRAM_START = 0x200;
   ```

## 🧠 Core CPU State
3. **Declare System Components**
   ```cpp
   uint8_t memory[MEMORY_SIZE] = {};
   uint8_t V[16] = {};          // General purpose registers
   uint16_t I = 0;              // Index register
   uint16_t pc = PROGRAM_START; // Program counter
   uint8_t delayTimer = 0;
   uint8_t soundTimer = 0;
   uint16_t stack[16] = {};
   uint8_t sp = 0;              // Stack pointer
   ```

4. **Initialize Fontset**
   - Create font data array (size: 80 bytes).
   - Copy to `memory[0x050]`.

5. **Load ROM into Memory**
   ```cpp
   std::ifstream rom("game.ch8", std::ios::binary);
   rom.read(reinterpret_cast<char*>(&memory[PROGRAM_START]), rom_size);
   ```

## 🖥️ Display & Input
6. **Create Display Buffer**
   ```cpp
   bool screen[DISPLAY_WIDTH][DISPLAY_HEIGHT] = {};
   ```

7. **Initialize Key State**
   ```cpp
   bool keys[16] = {};
   ```

## 🔁 Main Emulation Loop
8. **Start Main Loop**
   ```cpp
   while (true) {
       // Emulation cycle
   }
   ```

9. **Fetch Opcode**
   ```cpp
   uint16_t opcode = memory[pc] << 8 | memory[pc + 1];
   pc += 2;
   ```

10. **Decode & Execute Opcode**
   - Use masks:
     ```cpp
     uint8_t X = (opcode & 0x0F00) >> 8;
     uint8_t Y = (opcode & 0x00F0) >> 4;
     uint8_t NN = opcode & 0x00FF;
     uint16_t NNN = opcode & 0x0FFF;
     uint8_t N = opcode & 0x000F;
     ```

## 🧾 Instruction Set (Bare Minimum)

### 🔹 Control
11. `00E0` - Clear screen  
12. `00EE` - Return from subroutine  
13. `1NNN` - Jump to address  
14. `2NNN` - Call subroutine  

### 🔹 Conditionals
15. `3XNN` - Skip if Vx == NN  
16. `4XNN` - Skip if Vx != NN  
17. `5XY0` - Skip if Vx == Vy  
18. `9XY0` - Skip if Vx != Vy  

### 🔹 Data Manipulation
19. `6XNN` - Set Vx = NN  
20. `7XNN` - Add NN to Vx  

### 🔹 Math and Logic (8XY_)
21. `8XY0` - Vx = Vy  
22. `8XY1` - OR  
23. `8XY2` - AND  
24. `8XY3` - XOR  
25. `8XY4` - Vx += Vy (set VF on carry)  
26. `8XY5` - Vx -= Vy (set VF on borrow)  
27. `8XY6` - Shift right, store LSB in VF  
28. `8XY7` - Vx = Vy - Vx (set VF)  
29. `8XYE` - Shift left, store MSB in VF  

### 🔹 Memory / Jump
30. `ANNN` - Set I = NNN  
31. `BNNN` - Jump to V0 + NNN  

### 🔹 Random
32. `CXNN` - Vx = rand() & NN  

### 🔹 Drawing
33. `DXYN` - Draw sprite at (Vx, Vy), height N  
    - XOR draw pixels into `screen[x][y]`  
    - Set VF to 1 on pixel unset (collision)

## 🎹 Input
34. `EX9E` - Skip if key in Vx is pressed  
35. `EXA1` - Skip if key in Vx is NOT pressed  

## ⏲️ Timers and Wait
36. `FX07` - Vx = delay timer  
37. `FX0A` - Wait for key press, store in Vx  
38. `FX15` - Set delayTimer = Vx  
39. `FX18` - Set soundTimer = Vx (even if unused)  
40. `FX1E` - I += Vx  

## 🔢 Special Memory Ops
41. `FX29` - I = location of digit sprite for Vx  
42. `FX33` - BCD of Vx at I, I+1, I+2  
43. `FX55` - Store V0 to Vx at I  
44. `FX65` - Load V0 to Vx from I  

## ⏳ Timer Logic
45. **Decrement timers**
   ```cpp
   if (delayTimer > 0) delayTimer--;
   if (soundTimer > 0) soundTimer--;
   ```

## 🖨️ Rendering (Console Only)
46. **Draw screen using console**
   ```cpp
   system("clear");
   for (int y = 0; y < DISPLAY_HEIGHT; y++) {
       for (int x = 0; x < DISPLAY_WIDTH; x++) {
           std::cout << (screen[x][y] ? '#' : ' ');
       }
       std::cout << '\n';
   }
   ```

## ⌨️ Input Polling (Basic)
47. **Detect keypresses**
   - Map physical keys to CHIP-8 layout
   - Set `keys[hex_key] = true/false`

## 🧹 Finishing Up
48. **Loop**
   - Fetch → Decode → Execute → Update timers → Draw → Poll input

49. **Exit Conditions**
   - (Optional) Add way to quit (e.g., keypress or EOF)

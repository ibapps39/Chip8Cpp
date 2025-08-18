#include <chrono>
#include <iostream>

#include "chip8.h"
#include "platform.h"

#include <cassert>


int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM>\n";
        std::exit(EXIT_FAILURE);
    }

    int video_scale = std::stoi(argv[1]);
    int cycle_delay = std::stoi(argv[2]);
    char const* rom_filename = argv[3];

    
    Platform platform("CHIP-8 Emulator", DEFAULT_WIDTH * video_scale, DEFAULT_HEIGHT * video_scale, DEFAULT_WIDTH, DEFAULT_HEIGHT);


    chip8 active_chip;
    active_chip.LoadROM(rom_filename);
    

    int videoPitch = sizeof(active_chip.video[0]) * DEFAULT_WIDTH;

	auto lastCycleTime = std::chrono::high_resolution_clock::now();
	bool quit = false;

	while (!quit)
	{
		quit = platform.ProcessInput(active_chip.keypad);

		auto currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

		if (dt > cycle_delay)
		{
			lastCycleTime = currentTime;

			active_chip.Cycle();

			platform.Update(active_chip.video, videoPitch);
		}
	}
    return 0;
}
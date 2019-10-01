#pragma once

#include "../MMU/MMU.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <thread>
#include <chrono>
#include <mutex>

namespace Base {

    class GPU {
        private:
            SDL_Window *window = nullptr;
            SDL_Renderer *renderer = nullptr;

            Base::MMU *mmu;

            int cycles = 0;
            std::mutex cycle_lock;

            std::thread gpu_thread;
        
        public:
            GPU(Base::MMU *mmu);

            void update();

            inline void update_cycles(int num) { cycle_lock.lock(); cycles += num; cycle_lock.unlock(); }
    };

}

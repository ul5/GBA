#include "GPU.h"

#define MULTI_THREAD

Base::GPU::GPU(Base::MMU *m) : mmu(m) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    window = SDL_CreateWindow("GBA", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 240, 160, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_ShowWindow(window);
}

void Base::GPU::update() {
    int dec_by = 0;
    
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        if(e.type == SDL_WINDOWEVENT) {
            if(e.window.event == SDL_WINDOWEVENT_CLOSE) return;
        }
    }
    
    if(cycles > 1232) {
        dec_by += 1232;
        mmu->w8(0x04000006, mmu->r8(0x04000006) + 1);
    }

    cycle_lock.lock();
    cycles -= dec_by;
    cycle_lock.unlock();
    dec_by = 0;
    
    SDL_RenderPresent(renderer);
}

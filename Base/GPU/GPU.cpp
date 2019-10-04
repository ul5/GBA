#include "GPU.h"

#define MULTI_THREAD

Base::GPU::GPU(Base::MMU *m) : mmu(m) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    window = SDL_CreateWindow("GBA", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 240, 160, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_ShowWindow(window);
}

int frame = 0;

void Base::GPU::update() {
    int dec_by = 0;
    
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        if(e.type == SDL_WINDOWEVENT) {
            if(e.window.event == SDL_WINDOWEVENT_CLOSE) exit(0);
        }
    }
    

    hword flags = 0;


    while(cycles >= 4) {
        cycles -= 4;
        ++x_dot;
        if(x_dot >= 240) flags |= 0x2;
        if(x_dot >= 308) {
            x_dot = 0;
            ++y_dot;

            if(y_dot == 0xA0) flags |= 0x1;

            if(y_dot >= 228) {
                y_dot = 0;
            }
        }

        if(y_dot == mmu->r8(0x04000004)) flags |= 0x4;

        if(y_dot == 0x9F && x_dot == 0) printf("Frame %d\n", frame++); 

        byte *io = mmu->memory[4];
        io[0x202] |= flags;
        io[4] |= flags;
        mmu->w8(0x04000006, y_dot);
    }

    SDL_RenderPresent(renderer);
}

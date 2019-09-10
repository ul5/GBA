#include "GUI.h"

Debugger::GUI::GUI() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    TTF_Init();
    
    TTF_Font *font = TTF_OpenFont("Raleway-Regular.ttf", 35);
    
    debugger_window = SDL_CreateWindow("Debugger", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, 0);
    renderer = SDL_CreateRenderer(debugger_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    
    SDL_ShowWindow(debugger_window);
    
    SDL_Event e;
    while(true) {
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_WINDOWEVENT) {
                if(e.window.event == SDL_WINDOWEVENT_CLOSE) return;
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        
        SDL_Color col = {0xFF, 0xFF, 0xFF, 0xFF};
        SDL_Rect dst = {};
        
        SDL_Surface *txt = TTF_RenderText_Solid(font, "This is some text", col);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, txt);
        
        dst.w = txt->w;
        dst.h = txt->h;
        SDL_RenderCopy(renderer, texture, nullptr, &dst);
        
        SDL_RenderPresent(renderer);
    }
}

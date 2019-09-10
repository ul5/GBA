#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace Debugger {
    
    class GUI {
    private:
        SDL_Window *debugger_window = nullptr;
        SDL_Renderer *renderer = nullptr;
        
    public:
        GUI();
    };
    
}

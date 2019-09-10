#pragma once

#include "Debugger.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace Debugger {
    
    class GUI {
    private:
        SDL_Window *debugger_window = nullptr;
        SDL_Renderer *renderer = nullptr;
	TTF_Font *font = nullptr;
        
	void renderText(const char *text, int x, int y, int w = -1, int h = -1);

    public:
        GUI(Debugger *debugger);
    };
    
}

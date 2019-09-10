#include "GUI.h"

void Debugger::GUI::renderText(const char *text, int x, int y, int w, int h) {
	SDL_Surface *text_surface = TTF_RenderText_Solid(font, text, {0xFF, 0xFF, 0xFF, 0xFF});
	SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

	SDL_Rect dst = {x, y, w, h};
	if(w == -1) dst.w = text_surface->w;
	if(h == -1) dst.h = text_surface->h;

	SDL_RenderCopy(renderer, text_texture, nullptr, &dst);
    SDL_DestroyTexture(text_texture);
}

Debugger::GUI::GUI(Debugger *debugger) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    TTF_Init();
    
    font = TTF_OpenFont("Raleway-Regular.ttf", 25);
    
    debugger_window = SDL_CreateWindow("Debugger", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1500, 500, 0);
    renderer = SDL_CreateRenderer(debugger_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    
    SDL_ShowWindow(debugger_window);
    
    SDL_Event e;
    while(true) {
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_WINDOWEVENT) {
                if(e.window.event == SDL_WINDOWEVENT_CLOSE) return;
            } else if(e.type == SDL_KEYDOWN) {
                if(e.key.keysym.sym == SDLK_SPACE) debugger->executeNextInstruction();
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
        SDL_RenderClear(renderer);
        
        for(int i = 0; i < 18; i++) {
            std::string to_render;
            to_render += Decompiler::reg_names[i];
            while(to_render.length() < 6) to_render += " ";
            to_render += "     " + Decompiler::int_to_hex(debugger->cpu->reg(i).data.reg32);
            renderText(to_render.c_str(), 500 * (i % 3), 25 * (int)(i / 3));
        }
        
        SDL_RenderPresent(renderer);
    }
}

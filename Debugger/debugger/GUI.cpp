#include "GUI.h"

void Debugger::GUI::renderText(const char *text, int x, int y, int w, int h) {
	SDL_Surface *text_surface = TTF_RenderText_Shaded(font, text, {0xFF, 0xFF, 0xFF, 0xFF}, {0, 0, 0, 0});
	SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

	SDL_Rect dst = {x, y, w, h};
	if(w == -1) dst.w = text_surface->w;
	if(h == -1) dst.h = text_surface->h;

	SDL_RenderCopy(renderer, text_texture, nullptr, &dst);
    SDL_DestroyTexture(text_texture);
}

void Debugger::GUI::printPrompt(Debugger *debugger) {
    renderText("> ", 0, 300);
    for(int i = -2; i <= 10; i++) {
        word addr = i * ((debugger->cpu->reg(CPSR).data.reg32 & FLAG_T) ? 2 : 4);
        std::string dis = Decompiler::decompileInstruction(debugger->cpu, addr, false);

        std::string to_render = Decompiler::int_to_hex(debugger->cpu->pc().data.reg32 + addr);
        to_render += std::string(" = (");
        if(debugger->cpu->reg(CPSR).data.reg32 & FLAG_T) to_render += Decompiler::int_to_hex(debugger->cpu->r16(debugger->cpu->pc().data.reg32 + addr));
        else to_render += Decompiler::int_to_hex(debugger->cpu->r32(debugger->cpu->pc().data.reg32 + addr));
        to_render += std::string(") ==>");
        to_render += dis;

        renderText(to_render.c_str(), 25, i * 25 + 300);
    } 
}

Debugger::GUI::GUI(Debugger *debugger) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    TTF_Init();
    
    font = TTF_OpenFont("Raleway-Regular.ttf", 18);
    
    debugger_window = SDL_CreateWindow("Debugger", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 700, 750, 0);
    renderer = SDL_CreateRenderer(debugger_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    
    SDL_ShowWindow(debugger_window);
    
    SDL_Event e;
    bool running = true;
    while(running) {
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_WINDOWEVENT) {
                if(e.window.event == SDL_WINDOWEVENT_CLOSE) running = false;
            } else if(e.type == SDL_KEYDOWN) {
                if(e.key.keysym.sym == SDLK_SPACE) debugger->executeNextInstruction(false);
                else if(e.key.keysym.sym == SDLK_ESCAPE) running = false;
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
        SDL_RenderClear(renderer);

        SDL_Rect rect = {0, 0, 200, 25};
        for(int i = 0; i < 18; i++) {
            std::string to_render;
            to_render += Decompiler::reg_names_with_constant_length[i];
            to_render += "    " + Decompiler::int_to_hex(debugger->cpu->reg(i).data.reg32);
            renderText(to_render.c_str(), 200 * (i % 2), 25 * (int)(i / 2));
            rect.x = 200 * (i % 2);
            rect.y = 25 * (int)(i / 2);

            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderDrawRect(renderer, &rect);
        }

        printPrompt(debugger);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(debugger_window);
    SDL_Quit();
}

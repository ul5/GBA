#include "GUI.h"

void Debugger::GUI::renderText(const char *text, int x, int y, int col) {
	SDL_Surface *text_surface = TTF_RenderText_Shaded(font, text, {(uint8_t) ((col >> 16) & 0xFF), (uint8_t) ((col >> 8) & 0xFF), (uint8_t) ((col >> 0) & 0xFF), (uint8_t) ((col >> 24) & 0xFF) }, {0, 0, 0, 0});
	SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

	SDL_Rect dst = {x, y, text_surface->w, text_surface->h};

	SDL_RenderCopy(renderer, text_texture, nullptr, &dst);
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
}

void Debugger::GUI::renderDecompiler() {
    renderText("> ", 0, 300);
    for(int i = -2; i <= 10; i++) {
        word addr = i * ((mDebugger->cpu->reg(CPSR).data.reg32 & FLAG_T) ? 2 : 4);
        std::string dis = Decompiler::decompileInstruction(mDebugger->cpu, addr, false);

        std::string to_render = Decompiler::int_to_hex(mDebugger->cpu->pc().data.reg32 + addr);
        to_render += std::string(" = (");
        if(mDebugger->cpu->reg(CPSR).data.reg32 & FLAG_T) to_render += Decompiler::int_to_hex(mDebugger->cpu->r16(mDebugger->cpu->pc().data.reg32 + addr));
        else to_render += Decompiler::int_to_hex(mDebugger->cpu->r32(mDebugger->cpu->pc().data.reg32 + addr));
        to_render += std::string(") ==>  ");
        to_render += dis;

        renderText(to_render.c_str(), 25, i * 25 + 300);
    } 
}

void Debugger::GUI::renderFlags() {
    renderText("ZERO    ", 450, 0, mDebugger->cpu->reg(CPSR) & FLAG_Z ? 0xFF00FF00 : 0xFFFF0000);
    renderText("CARRY   ", 450, 25, mDebugger->cpu->reg(CPSR) & FLAG_C ? 0xFF00FF00 : 0xFFFF0000);
    renderText("OVERFLOW", 450, 50, mDebugger->cpu->reg(CPSR) & FLAG_V ? 0xFF00FF00 : 0xFFFF0000);
    renderText("NEGATIVE", 450, 75, mDebugger->cpu->reg(CPSR) & FLAG_N ? 0xFF00FF00 : 0xFFFF0000);
    
    renderText("THUMB", 450, 150, mDebugger->cpu->reg(CPSR) & FLAG_T ? 0xFF00FF00 : 0xFFFF0000);
    renderText("MODE: ", 450, 175);
    
    byte mode = mDebugger->cpu->reg(CPSR).data.reg32 & 0x1F;
    std::string mode_str = "";
    
    switch(mode) {
        case MODE_USER:
            mode_str = "USER";
            break;
        case MODE_FIQ:
            mode_str = "FIQ";
            break;
        case MODE_IRQ:
            mode_str = "IRQ";
            break;
        case MODE_SUPERVISOR:
            mode_str = "SUPERVISOR";
            break;
        case MODE_ABORT:
            mode_str = "ABORT";
            break;
        case MODE_UNDEFINED:
            mode_str = "UNDEFINED";
            break;
        case MODE_SYSTEM:
            mode_str = "SYSTEM";
            break;
    }
    
    renderText(mode_str.c_str(), 525, 175);
}

Debugger::GUI::GUI(Debugger *debugger) : mDebugger(debugger) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    TTF_Init();
    
    font = TTF_OpenFont("Raleway-Regular.ttf", 18);
    
    debugger_window = SDL_CreateWindow("Debugger", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 700, 750, 0);
    renderer = SDL_CreateRenderer(debugger_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
}

void Debugger::GUI::start() {
    SDL_ShowWindow(debugger_window);
    
    SDL_Event e;
    bool running = true;
    while(running) {
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_WINDOWEVENT) {
                if(e.window.event == SDL_WINDOWEVENT_CLOSE) running = false;
            } else if(e.type == SDL_KEYDOWN) {
                if(e.key.keysym.sym == SDLK_SPACE) mDebugger->executeNextInstruction(false);
                else if(e.key.keysym.sym == SDLK_ESCAPE) running = false;
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
        SDL_RenderClear(renderer);

        SDL_Rect rect = {0, 0, 200, 25};
        for(int i = 0; i < 18; i++) {
            std::string to_render;
            to_render += Decompiler::reg_names_with_constant_length[i];
            to_render += "    " + Decompiler::int_to_hex(mDebugger->cpu->reg(i).data.reg32);
            renderText(to_render.c_str(), 200 * (i % 2), 25 * (int)(i / 2));
            rect.x = 200 * (i % 2);
            rect.y = 25 * (int)(i / 2);

            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderDrawRect(renderer, &rect);
        }

        renderDecompiler();
        renderFlags();

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(debugger_window);
    SDL_Quit();
}

#include "VRAM_GUI.h"

Debugger::VRAM_GUI::VRAM_GUI(Base::CPU *_cpu) : cpu(_cpu) {
    window = SDL_CreateWindow("VRAM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void Debugger::VRAM_GUI::render() {
    // byte *ioctrl = cpu->mmu->memory[4];

    hword dispcnt = cpu->r16(0x04000000);
    hword bg0_cnt = cpu->r16(0x04000008);
    hword bg1_cnt = cpu->r16(0x0400000A);
    hword bg2_cnt = cpu->r16(0x0400000C);
    hword bg3_cnt = cpu->r16(0x0400000E);

    hword *palette_obj = (hword*) (cpu->mmu->memory[5] + 0x200);
    bool one_d_mapping = dispcnt & (1 << 6);

    hword color = *(hword*)(cpu->mmu->memory[5]);
    SDL_SetRenderDrawColor(renderer, (color & 0x1F) << 3, ((color >> 5) & 0x1F) << 3, ((color >> 10) & 0x1F) << 3, 0xFF);
    SDL_RenderClear(renderer);

    for(int index = 0; index < 128 && !(dispcnt & 0x80); index++) {
        hword attrib0 = cpu->r16(0x07000000 + index * 8);
        hword attrib1 = cpu->r16(0x07000002 + index * 8);
        hword attrib2 = cpu->r16(0x07000004 + index * 8);

        bool rotscal = attrib0 & 0x0100;
        bool bit9 = attrib0 & 0x0200;

        if(!rotscal && bit9) continue;
        else if(rotscal) {
            
        }

        byte shape = attrib0 >> 14;
        byte size = attrib1 >> 14;

        word x = (attrib1 & 0x1FF), y = (attrib0 & 0xFF), w, h;

        if(y & 0x80) y |= 0xFFFFFF00;
        if(x & 0x100) x |= 0xFFFFFE00;

        switch(shape) {
            case 0:
                w = 8 * (1 << size);
                h = 8 * (1 << size);
                break;
            case 1:
                w = 16 * (1 << size);
                h = 8 * (1 << (size > 0 ? size - 1 : 0));
                break;
            case 2:
                w = 8 * (1 << (size > 0 ? size - 1 : 0));
                h = 16 * (1 << size);
                break;
        }
        
        /*if(index == 0){
            printf("Attrib 0: %.04X, Attrib1: %.04X, attrib2: %.04X\n", attrib0, attrib1, attrib2);
            printf("\tRendering Obj with (%.02X, %.02X, %.02X, %.02X)\n\n", x, y, w, h);
        }*/
        byte depth = (attrib0 & 0x2000) ? 8 : 4;
        byte factor = depth / 4;
        hword name = attrib2 & 0x3FF;

        for(int yc = 0; yc < h / 8; yc++) {
            for(int xc = 0; xc < w / 8 / factor; xc++) {
                for(int j = 0; j < 8; j++) {
                    for(int i = 0; i < depth; i++) {
                        word offset = 0;
                        if(one_d_mapping) offset = name * 32 + i + factor * (xc * 32 + j * 4 + yc * w * 32);
                        else offset = name * 32 + i + factor * (xc * 32 + j * 4 + yc * 128 * 4);

                        // if(index == 7 && dispcnt != 0x1002) printf("Dispcntrl: %.08X\n", dispcnt);
                  
                        if(depth == 4) {
                            int a = ((attrib2 >> 8) & 0xF0) | (cpu->r8(0x06010000 + offset) & 0xF);
                            int b = ((attrib2 >> 8) & 0xF0) | (cpu->r8(0x06010000 + offset) >> 4);
                            hword color = palette_obj[a];
                            SDL_SetRenderDrawColor(renderer, (color & 0x1F) << 3, ((color >> 5) & 0x1F) << 3, ((color >> 10) & 0x1F) << 3, 0xFF);
                            if(a) SDL_RenderDrawPoint(renderer, (int) x + xc * 8 + i * 2 + 0, (int) y + yc * 8 + j);
                            color = palette_obj[b];
                            SDL_SetRenderDrawColor(renderer, (color & 0x1F) << 3, ((color >> 5) & 0x1F) << 3, ((color >> 10) & 0x1F) << 3, 0xFF);
                            if(b) SDL_RenderDrawPoint(renderer, (int) x + xc * 8 + i * 2 + 1, (int) y + yc * 8 + j);
                        } else {
                            hword color = palette_obj[cpu->r8(0x06010000 + offset)];
                            SDL_SetRenderDrawColor(renderer, (color & 0x1F) << 3, ((color >> 5) & 0x1F) << 3, ((color >> 10) & 0x1F) << 3, 0xFF);
                            if(cpu->r8(0x06010000 + offset)) SDL_RenderDrawPoint(renderer, (int) x + xc * 8 + i, (int) y + yc * 8 + j);
                        }
                    }
                }
            }
        }
    }


#define _COLOR_PALETTE
#ifdef _COLOR_PALETTE
    for(int i = 0; i < 32; i++) {
        for(int j = 0; j < 32; j++) {
            hword color = palette_obj[j * 32 + i];
                SDL_SetRenderDrawColor(renderer, (color & 0x1F) << 3, ((color >> 5) & 0x1F) << 3, ((color >> 10) & 0x1F) << 3, 0xFF);
                SDL_RenderDrawPoint(renderer, i, j + 250);
        }
    }
#endif
 

//#define _8_BIT_OBJS

#ifdef _8_BIT_OBJS
    int cx = 0, cy = 0;
    for(int i = 0x06000000; i < 0x06008000; i += 64) {
        for(int j = 0; j < 64; j++) {
            hword color = palette_obj[cpu->r8(i + j)];
            SDL_SetRenderDrawColor(renderer, (color & 0x1F) << 3, ((color >> 5) & 0x1F) << 3, ((color >> 10) & 0x1F) << 3, 0xFF);
            SDL_RenderDrawPoint(renderer, cx * 8 + (j % 8) + 128, cy * 8 + (int)(j / 8) + 250);
        }
        if(++cx == 32) {
            cx = 0;
            ++cy;
        }
    }
#endif

    // printf("Display Control register: %.04X\n", dispcnt);
    if((dispcnt & 0x7) == 0) {
    } else if((dispcnt & 0x7) == 2) {
        //printf("Display Control register: %.04X\n", dispcnt);
    }

    SDL_RenderPresent(renderer);
}

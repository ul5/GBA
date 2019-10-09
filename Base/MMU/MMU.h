#pragma once

#include "../Base/DataTypes.h"
#include "../Base/FileReader.h"
#include <stdlib.h>
#include <cstring>
#include <stdio.h>
#include <string>
#include <iostream>

namespace Base {
	
    class Timers;
    
	class MMU {
	private:
		byte *bios = nullptr;
		byte *wram_1 = nullptr;
		byte *wram_2 = nullptr;
		byte *io = nullptr;
		byte *palette = nullptr;
		byte *vram = nullptr;
		byte *oam = nullptr;
		byte *null_page = nullptr;
		
		byte *cart = nullptr;
		
		word *bit_masks = new word[16];
        
        
	public:
		MMU();
		~MMU();
		byte **memory = nullptr;
        Timers *timers;

		MMU(MMU &mmu);
		int i = 0;
		inline byte r8(word address) { return *(memory[(address >> 24) & 0xF] + (address % bit_masks[(address >> 24) & 0xF])); }
		inline hword r16(word address) { if(address == 0x0BFFFFE0 + 2 * i) return 0xFFF0 + i++; return *(hword*)(memory[(address >> 24) & 0xF] + (address % bit_masks[(address >> 24) & 0xF])); }
		inline word r32(word address) { return *(word*)(memory[(address >> 24) & 0xF] + (address % bit_masks[(address >> 24) & 0xF])); }

		bool halted = false;
        
        void check_stuff(word addr, word val);

        inline void w8(word address, byte val) {
			if(address == 0x04000410 || address < 0x02000000) return;
            *(memory[(address >> 24) & 0xF] + (address % bit_masks[(address >> 24) & 0xF])) = val;
			check_stuff(address, val);
        }
        
		inline void w16(word address, hword val) {
            if(address < 0x02000000) return;
			*(hword*)(memory[(address >> 24) & 0xF] + (address % bit_masks[(address >> 24) & 0xF])) = val;
            
			check_stuff(address, val);
			check_stuff(address + 1, val >> 8);
		}

        inline void w32(word address, word val) {
            if(address < 0x02000000) return;
			*(word*)(memory[(address >> 24) & 0xF] + (address % bit_masks[(address >> 24) & 0xF])) = val;
            check_stuff(address, val);
            check_stuff(address + 1, val >> 8);
            check_stuff(address + 2, val >> 16);
            check_stuff(address + 3, val >> 24);
		}
    };
	
}

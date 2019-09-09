#pragma once

#include "../Base/DataTypes.h"
#include "../Base/FileReader.h"
#include <stdlib.h>
#include <cstring>
#include <stdio.h>
#include <string>

namespace Base {
	
	class MMU {
	private:
		byte *bios = nullptr;
		byte *wram_1 = nullptr;
		byte *wram_2 = nullptr;
		byte *io = nullptr;
		byte *palette = nullptr;
		byte *vram = nullptr;
		byte *oam = nullptr;
		
		byte *cart = nullptr;
		
		word *bit_masks = new word[16];
		byte **memory = nullptr;
		
	public:
		MMU();
		~MMU();
		
		MMU(MMU &mmu);
		
		inline byte r8(word address) { return *(memory[(address >> 24) & 0xF] + (address & bit_masks[(address >> 24) & 0xF])); }
		inline hword r16(word address) { return *(hword*)(memory[(address >> 24) & 0xF] + (address & bit_masks[(address >> 24) & 0xF])); }
		inline word r32(word address) { return *(word*)(memory[(address >> 24) & 0xF] + (address & bit_masks[(address >> 24) & 0xF])); }

        inline void w8(word address, byte val) {
            *(memory[(address >> 24) & 0xF] + (address & bit_masks[(address >> 24) & 0xF])) = val;
        }
        inline void w16(word address, hword val) { *(hword*)(memory[(address >> 24) & 0xF] + (address & bit_masks[(address >> 24) & 0xF])) = val; }
        inline void w32(word address, word val) {
			*(word*)(memory[(address >> 24) & 0xF] + (address & bit_masks[(address >> 24) & 0xF])) = val;
			printf("[MMU][DEBUG] Written value of %.08X to address %.02X%.06X\n", val, (address >> 24) & 0xF, address & bit_masks[(address >> 24) & 0xF]);
		}
    };
	
}

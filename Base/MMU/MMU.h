#pragma once

#include "../Base/DataTypes.h"
#include "../Base/FileReader.h"
#include <stdlib.h>
#include <cstring>
#include <stdio.h>
#include <string>
#include <iostream>

//#define BREAK_ON_WRITE 0x880e000

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
		byte *null_page = nullptr;
		
		byte *cart = nullptr;
		
		word *bit_masks = new word[16];
		
	public:
		MMU();
		~MMU();
		byte **memory = nullptr;
		
		MMU(MMU &mmu);
		int i = 0;
		inline byte r8(word address) { return *(memory[(address >> 24) & 0xF] + (address % bit_masks[(address >> 24) & 0xF])); }
		inline hword r16(word address) { if(address == 0x0BFFFFE0 + 2 * i) return 0xFFF0 + i++; return *(hword*)(memory[(address >> 24) & 0xF] + (address % bit_masks[(address >> 24) & 0xF])); }
		
		inline word r32(word address) { 

			return *(word*)(memory[(address >> 24) & 0xF] + (address % bit_masks[(address >> 24) & 0xF])); 
		}

		bool halted = false;

		inline void check_stuff(word address, byte value) {
			/*if(address == 0x040000BB && (value & 0x80)) {
				printf("DMA control 0 set!\n");
			}
			if(address == 0x040000C7 && (value & 0x80)) {
				printf("DMA control 1 set! : %.04X\n", r16(0x040000C6));
				printf("\tCopying from: %.08X\n\tCopying to    %.08X\n\tCopy amount:  %.04X\n", r32(0x040000BC), r32(0x040000C0), r16(0x040000C4));
			}
			if(address == 0x040000D3 && (value & 0x80)) {
				printf("DMA control 2 set!\n");
			}
			if(address == 0x040000DF && (value & 0x80)) {
				printf("DMA control 3 set!\n");
			}*/

			if(address == 0x04000200 || address == 0x04000201) printf("Wrote to IE register: %.04X\n", r16(0x04000200));

			if(address == 0x04000202) {
				memory[4][0x202] &= ~value;
			}

			if(address == 0x04000301) {
				halted = true;
			} 
		}

        inline void w8(word address, byte val) {
			if(address == 0x04000410) return;
            *(memory[(address >> 24) & 0xF] + (address % bit_masks[(address >> 24) & 0xF])) = val;
			check_stuff(address, val);
#ifdef BREAK_ON_WRITE
			if(address == BREAK_ON_WRITE) {
				printf("Written value of %.08X to %.08X.\n", val, address);
				std::cin.get();
			}
#endif
        }
        
		inline void w16(word address, hword val) { 
			*(hword*)(memory[(address >> 24) & 0xF] + (address % bit_masks[(address >> 24) & 0xF])) = val;
			check_stuff(address, val & 0xFF);
			check_stuff(address + 1, val >> 8);
#ifdef BREAK_ON_WRITE
			if(address == BREAK_ON_WRITE) {
				printf("Written value of %.08X to %.08X.\n", val, address);
				std::cin.get();
			}
#endif
		}

        inline void w32(word address, word val) {
			*(word*)(memory[(address >> 24) & 0xF] + (address % bit_masks[(address >> 24) & 0xF])) = val;
		check_stuff(address, val & 0xFF);
		check_stuff(address + 1, val >> 8);
		check_stuff(address + 2, val >> 16);
		check_stuff(address + 3, val >> 24);
#ifdef BREAK_ON_WRITE
			if(address == BREAK_ON_WRITE) {
				printf("Written value of %.08X to %.08X.\n", val, address);
				std::cin.get();
			}
#endif
		}
    };
	
}

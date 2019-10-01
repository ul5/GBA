#pragma once

#include "../Register/RegisterSet.h"
#include "../MMU/MMU.h"
#include "../GPU/GPU.h"

namespace Base {
	
	class CPU {
	public:
        RegisterSet *set = nullptr;
        
        MMU *mmu = nullptr;
		GPU *gpu = nullptr;
		uint32_t cycle_count = 0;

	public:
		CPU();
		
		void reset();

		inline void update_cycles(int num) { gpu->update_cycles(num); }
        inline void update_gpu() { gpu->update(); }
        
        inline Register &pc() { return (*set)[PC]; }
        inline Register &reg(byte reg) { return (*set)[reg]; }
		
		inline byte r8(word address) { return mmu->r8(address); }
		inline hword r16(word address) { return mmu->r16(address); }
		inline word r32(word address) { return mmu->r32(address); }
		
		inline byte r8(Register* address) { return mmu->r8(address->data.reg32); }
		inline hword r16(Register* address) { return mmu->r16(address->data.reg32); }
		inline word r32(Register* address) { return mmu->r32(address->data.reg32); }

		inline byte r8(Register address) { return mmu->r8(address.data.reg32); }
		inline hword r16(Register address) { return mmu->r16(address.data.reg32); }
		inline word r32(Register address) { return mmu->r32(address.data.reg32); }
        
        inline void w8(word address, byte val) { mmu->w8(address, val); }
        inline void w16(word address, hword val) { mmu->w16(address, val); }
        inline void w32(word address, word val) { mmu->w32(address, val); }
        
        inline void w8(Register* address, byte val) { mmu->w8(address->data.reg32, val); }
        inline void w16(Register* address, hword val) { mmu->w16(address->data.reg32, val); }
        inline void w32(Register* address, word val) { mmu->w32(address->data.reg32, val); }
        
        inline void w8(Register address, byte val) { mmu->w8(address.data.reg32, val); }
        inline void w16(Register address, hword val) { mmu->w16(address.data.reg32, val); }
        inline void w32(Register address, word val) { mmu->w32(address.data.reg32, val); }
		
	};
	
}

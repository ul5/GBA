#include "MMU.h"

Base::MMU::MMU() {
	bios = Base::readFile("bios.gba").data; //(byte*) malloc(0x4000);
	filedata cartridge_full = Base::readFile("emerald.gba");
    cart = cartridge_full.data;
    
	wram_1 = (byte*) malloc(0x40000);
	wram_2 = (byte*) malloc(0x8000);
	io = (byte*) malloc(0x3FF);
	palette = (byte*) malloc(0x400);
	vram = (byte*) malloc(0x18000);
	oam = (byte*) malloc(0x400);

	null_page = (byte*) malloc(1);
	null_page[0] = 0;
	
    memset(wram_1, 0, 0x40000);
    memset(wram_2, 0, 0x8000);
    memset(io, 0, 0x3FF);
    memset(palette, 0, 0x400);
    memset(vram, 0, 0x18000);
    memset(oam, 0, 0x400);
    
	memory = new byte*[16];
	memory[0x0] = bios;
	memory[0x1] = null_page;
	memory[0x2] = wram_1;
	memory[0x3] = wram_2;
	memory[0x4] = io;
	memory[0x5] = palette;
	memory[0x6] = vram;
	memory[0x7] = oam;
	

	for(int i = 0; i < 6; i++) {
		memory[0x8 + i] = cartridge_full.size >= 0x1000000 * i ? cart + 0x1000000 * i : null_page;
		word size = cartridge_full.size - 0x1000000 * i;
		bit_masks[0x8 + i] = cartridge_full.size >= 0x1000000 * i ? (size > 0x1000000 ? 0xFFFFFF : size) : 0; 
	}

	memory[0xE] = cart; // Actually cart_ram
	
	bit_masks[0] = 0x00FFFFFF + 1;
	bit_masks[1] = 0x00000000 + 1;
	bit_masks[2] = 0x0003FFFF + 1;
	bit_masks[3] = 0x00007FFF + 1;
	bit_masks[4] = 0x00FFFFFF + 1;
	bit_masks[5] = 0x000003FF + 1;
	bit_masks[6] = 0x00017FFF + 1;
	bit_masks[7] = 0x000003FF + 1;

	// Default values for IO:
	w32(0x04000088, 0x200);
}

Base::MMU::~MMU() {
	free(bios);
    free(cart);
	free(wram_1);
	free(wram_2);
	free(io);
	free(palette);
	free(vram);
	free(oam);
}

Base::MMU::MMU(MMU &mmu) {
	bios = (byte*) malloc(0x4000);
    cart = (byte*) malloc(0x6000000);
	wram_1 = (byte*) malloc(0x40000);
	wram_2 = (byte*) malloc(0x8000);
	io = (byte*) malloc(0x3FF);
	palette = (byte*) malloc(0x400);
	vram = (byte*) malloc(0x18000);
	oam = (byte*) malloc(0x400);
	
	memcpy(bios, mmu.bios, 0x4000);
    memcpy(cart, mmu.cart, 0x6000000);
	memcpy(wram_1, mmu.wram_1, 0x40000);
	memcpy(wram_2, mmu.wram_2, 0x8000);
	memcpy(io, mmu.io, 0x3FF);
	memcpy(palette, mmu.palette, 0x400);
	memcpy(vram, mmu.vram, 0x18000);
	memcpy(oam, mmu.oam, 0x400);
}

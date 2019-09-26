#include "ARMInstructions.h"

void Debugger::arm_load(word instruction, Base::CPU *cpu) {
    word base_val = cpu->reg((instruction >> 16) & 0xF).data.reg32;
    bool pre = (instruction & (1 << 24));
    if(((instruction >> 16) & 0xF) == 0xF) base_val += 4;
    word off;
    if(instruction & (1 << 25)) off = Base::shift((instruction >> 8) & 0xFF, cpu->reg(instruction & 0xF).data.reg32 + ((instruction & 0xF) == 0xF ? 4 : 0), cpu, false);
    else off = instruction & 0xFFF;
    
    word total_address = base_val;
    if(pre) total_address += ((instruction & (1 << 23)) ? off : - off);
    
    // if(total_address % 4 != 0) rotate right?
        
    if(instruction & (1 << 22)) { // Load single byte
        cpu->reg((instruction >> 12) & 0xF).data.reg32 = (word) cpu->r8(total_address);
    } else { // Load word
        cpu->reg((instruction >> 12) & 0xF).data.reg32 = cpu->r32(total_address);
    }
    
    if(!pre) total_address += ((instruction & (1 << 23)) ? off : - off);

    if(instruction & (1 << 21) || !pre) cpu->reg((instruction >> 16) & 0xF).data.reg32 = total_address; // Writeback
}

void Debugger::arm_store(word instruction, Base::CPU *cpu) {
    word base_val = cpu->reg((instruction >> 16) & 0xF).data.reg32;
    bool pre = (instruction & (1 << 24));
    word off;
    if(instruction & (1 << 25)) off = Base::shift((instruction >> 8) & 0xFF, cpu->reg(instruction & 0xF).data.reg32 + ((instruction & 0xF) == 0xF ? 4 : 0), cpu, false);
    else off = instruction & 0xFFF;

    word total_address = base_val;
    if(pre) total_address += ((instruction & (1 << 23)) ? off : - off);
    
    // if(total_address % 4 != 0) rotate right?
    
    if(instruction & (1 << 22)) { // Store single byte
        cpu->w8(total_address, cpu->reg((instruction >> 12) & 0xF).data.reg32 & 0xFF);
    } else { // Store word
        cpu->w32(total_address, cpu->reg((instruction >> 12) & 0xF).data.reg32);
    }
    
    if(!pre) total_address += ((instruction & (1 << 23)) ? off : - off);

    if(instruction & (1 << 21) || !pre) cpu->reg((instruction >> 16) & 0xF).data.reg32 = total_address; // Writeback
}


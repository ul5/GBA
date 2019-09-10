#include "THUMBInstructions.h"

void Debugger::thumb_data_processing(hword instruction, Base::CPU *cpu) {
    word result = cpu->reg((instruction >> 8) & 0x7).data.reg32;
    byte off = instruction & 0xFF;
    
    switch((instruction >> 11) & 3) {
        case 0:
            result = off;
            break;
        case 1:
            
            break;
        case 2:
            result += off;
            break;
        case 3:
            result -= off;
            break;
    }
    
    if(result == 0) cpu->reg(CPSR).data.reg32 |= FLAG_Z;
    else cpu->reg(CPSR).data.reg32 &= ~FLAG_Z;
    
    if(result & 0x80000000) cpu->reg(CPSR).data.reg32 |= FLAG_N;
    else cpu->reg(CPSR).data.reg32 &= ~FLAG_N;
    
    cpu->reg((instruction >> 8) & 0x7).data.reg32 = result;
}

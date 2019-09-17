#include "THUMBInstructions.h"

void Debugger::thumb_data_processing(hword instruction, Base::CPU *cpu) {
    word result = cpu->reg((instruction >> 8) & 0x7).data.reg32;
    byte off = instruction & 0xFF;

    bool write = true;
    
    switch((instruction >> 11) & 3) {
        case 0:
            result = off;
            break;
        case 1:
            {
                word r = result - off;

                if(off > result) cpu->reg(CPSR) |= FLAG_C;
                else cpu->reg(CPSR) &= ~FLAG_C;
                
                if((r & 0x80000000) == (off & 0x80000000) && (r & 0x80000000) != (result & 0x80000000)) cpu->reg(CPSR) |= FLAG_V;
                else cpu->reg(CPSR) &= ~FLAG_V;
                
                result = r;

                write = false;
            }
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
    
    if(write) cpu->reg((instruction >> 8) & 0x7).data.reg32 = result;
}

#include "THUMBInstructions.h"

void Debugger::thumb_alu(hword instruction, Base::CPU *cpu) {
    word res = 0;
    
    word op1 = cpu->reg((instruction >> 3) & 0x7).data.reg32;
    word op2 = cpu->reg(instruction & 0x7).data.reg32;
    
    bool write = true;
    
    switch ((instruction >> 6) & 0xF) {
        case 0x0:
            res = op1 & op2;
            break;
        case 0x1:
            res = op1 ^ op2;
            break;
        case 0x2:
            res = op1 << op2;
            if((op1 >> (32 - op2)) & 1) cpu->reg(CPSR).data.reg32 |= FLAG_C;
            else cpu->reg(CPSR).data.reg32 &= ~FLAG_C;
            break;
        case 0x3:
            res = op1 >> op2;
            if((op1 >> (op2 - 1)) & 1) cpu->reg(CPSR).data.reg32 |= FLAG_C;
            else cpu->reg(CPSR).data.reg32 &= ~FLAG_C;
            break;
        case 0x4:
            res = op1 >> op2;
            if((op1 >> (op2 - 1)) & 1) cpu->reg(CPSR).data.reg32 |= FLAG_C;
            else cpu->reg(CPSR).data.reg32 &= ~FLAG_C;
            break;
        case 0x5:
            printf("ADC ");
            break;
        case 0x6:
            printf("SBC ");
            break;
        case 0x7:
            printf("ROR ");
            break;
        case 0x8:
            {
                res = op1 & op2;
                write = false;
            }
            break;
        case 0x9:
            printf("NEG ");
            break;
        case 0xA:
            printf("CMP ");
            break;
        case 0xB:
            printf("CMN ");
            break;
        case 0xC:
            {
                res = op1 | op2;
            }
            break;
        case 0xD:
            res = op1 * op2;
            // TODO carry & oVerflow
            break;
        case 0xE:
            res = op1 & ~op2;
            break;
        case 0xF:
            res = ~op1;
            break;
        default:
            break;
    }
    
    if(res == 0) cpu->reg(CPSR).data.reg32 |= FLAG_Z;
    else cpu->reg(CPSR).data.reg32 &= ~FLAG_Z;
    
    if(res & 0x80000000) cpu->reg(CPSR).data.reg32 |= FLAG_N;
    else cpu->reg(CPSR).data.reg32 &= ~FLAG_N;
    
    if(write) cpu->reg(instruction & 0x7).data.reg32 = res;
}
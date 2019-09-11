#include "ARMInstructions.h"

enum CHANGE_FLAG { NO_CHANGE, SET, RESET };

void Debugger::arm_data_processing(word instruction, Base::CPU *cpu) {
    bool imm = instruction & 0x02000000;
    bool s = instruction & 0x00100000;
    byte opcode = (instruction >> 21) & 0xF;
    
    Base::Register &dest = cpu->reg((instruction >> 12) & 0xF);
    word arg1 = cpu->reg((instruction >> 16) & 0xF).data.reg32;
    if(((instruction >> 16) & 0xF) == 0xF) arg1 += 4;
    word arg2 = instruction & 0xFFF;
    
    if(imm) arg2 = Base::rotate(instruction & 0xFF, (instruction >> 8) & 0xF);
    else {
        word reg = cpu->reg(instruction & 0xF).data.reg32 + ((instruction & 0xF) == 0xF ? 4 : 0);
        arg2 = Base::shift((instruction >> 4) & 0xFF, reg, *cpu->set);
    }
    
    CHANGE_FLAG set_n = NO_CHANGE;
    CHANGE_FLAG set_z = NO_CHANGE;
    CHANGE_FLAG set_c = NO_CHANGE;
    CHANGE_FLAG set_v = NO_CHANGE;
    
    switch (opcode) {
        case 0x0:
            printf("AND");
            break;
        case 0x1:
            printf("EOR");
            break;
        case 0x2:
            printf("SUB");
            break;
        case 0x3:
            printf("RSB");
            break;
        case 0x4: // ADD
        {
            word out = arg1 + arg2;
            
            set_n = out & 0x80000000 ? SET : RESET;
            set_z = out == 0 ? SET : RESET;
            set_c = ((uint64_t) arg1 + (uint64_t) arg2) > 0xFFFFFFFF ? SET : RESET;
            set_v = (arg1 & 0x80000000) == (arg2 & 0x80000000) && (arg1 & 0x80000000) != (out & 0x80000000) ? SET : RESET;
            
            dest.data.reg32 = out;
        }
            break;
        case 0x5:
            printf("ADC");
            break;
        case 0x6:
            printf("SBC");
            break;
        case 0x7:
            printf("RSC");
            break;
        case 0x8: // TST
        {
            word out = arg1 & arg2;
            set_n = (out & 0x80000000) ? SET : RESET;
            set_z = (out == 0) ? SET : RESET;
        }
            break;
        case 0x9: // EOR --> test
        {
            word out = arg1 ^ arg2;
            set_n = (out & 0x80000000) ? SET : RESET;
            set_z = (out == 0) ? SET : RESET;
        }
            break;
        case 0xA: // CMP
            {
                if(!s) {
                    printf("S bit should be set\n");
                    s = true;
                }
                word out = arg1 - arg2;
                set_n = (out & 0x80000000) ? SET : RESET;
                set_z = (out == 0) ? SET : RESET;
                set_c = (arg2 > arg1) ? SET : RESET;
                set_v = ((out & 0x80000000) == (arg2 & 0x80000000) && (out & 0x80000000) != (arg1 & 0x80000000)) ? SET : RESET;
            }
            break;
        case 0xB:
            printf("CMN");
            break;
        case 0xC:
            printf("ORR");
            break;
        case 0xD: // MOV
            dest.data.reg32 = arg2;
            break;
        case 0xE:
            printf("BIC");
            break;
        case 0xF:
            printf("MVN");
            break;
    }
    
    if(s) {        
        if(set_n == SET) cpu->reg(CPSR) |= FLAG_N;
        else if(set_n == RESET) cpu->reg(CPSR) &= ~FLAG_N;
        
        if(set_z == SET) cpu->reg(CPSR) |= FLAG_Z;
        else if(set_z == RESET) cpu->reg(CPSR) &= ~FLAG_Z;
        
        if(set_c == SET) cpu->reg(CPSR) |= FLAG_C;
        else if(set_c == RESET) cpu->reg(CPSR) &= ~FLAG_C;
        
        if(set_v == SET) cpu->reg(CPSR) |= FLAG_V;
        else if(set_v == RESET) cpu->reg(CPSR) &= ~FLAG_V;
    }
}

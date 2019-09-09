#include "ARMInstructions.h"

void Debugger::execute_arm(word instruction, Base::CPU *cpu) {
    if(!Base::isConditionMet((instruction >> 28) & 0xF, cpu->reg(CPSR).data.reg32)) {
        //printf("Condition not met...\n");
        return;
    }
    
    if (instruction & 0x08000000) {
        if (instruction & 0x04000000) {
            if ((instruction & 0x0F000000) == 0x0F000000) {
                printf("SWI");
            }
            else {
                if (instruction & 0x02000000) {
                    if (instruction & 0x00000010) {
                        printf("CoProcessor Register transfer");
                    }
                    else {
                        printf("CoProcessor Data operation");
                    }
                }
                else {
                    printf("CoProcessor Data transfer");
                }
            }
        }
        else {
            if (instruction & 0x02000000) arm_branch(instruction, cpu);
            else {
                printf("Block Data Transfer");
            }
        }
    }
    else {
        if (instruction & 0x04000000) {
            if ((instruction & 0x02000010) == 0x02000010) {
                printf("Undefined");
            }
            else {
                if(instruction & (1 << 20)) arm_load(instruction, cpu);
                else arm_store(instruction, cpu);
            }
        }
        else {
            if ((instruction & 0x020000F0) == 0x00000090) {
                if (instruction & 0x01000000) {
                    printf("Single Data Swap");
                }
                else if (instruction & 0x00800000) {
                    printf("Multiply (accumulate) long");
                }
                else {
                    printf("Multiply (accumulate)");
                }
            }
            else if ((instruction & 0x020000F0) == 0x000000B0) {
                if (instruction & 0x00400000) {
                    printf("hword data tranfer, immidiate offset");
                }
                else {
                    printf("hword data tranfer, register offset");
                }
            }
            else if ((instruction & 0x020000D0) == 0x000000D0) {
                printf("Signed data transfer (byte/hword)");
            }
            else if ((instruction & 0x0FFFFFF0) == 0x012FFF10) {
                word target_addr = cpu->reg(instruction & 0xF).data.reg32;
                cpu->pc().data.reg32 = target_addr & 0xFFFFFFFE;
                if(target_addr & 1) {
                    cpu->reg(CPSR) |= FLAG_T;
                }
            } else if((instruction & 0x0F800000) == 0x01000000 && (instruction & 0x003F0000) == 0x003F0000 && (instruction & 0x00000FFF) == 0x00000FFF) {
                printf("MRS\n");
            } else if((instruction & 0x0F800000) == 0x01000000 && (instruction & 0x003FFFF0) == 0x0029F000) {
                if(instruction & (1 << 22)) cpu->reg(SPSR).data.reg32 = cpu->reg(instruction & 0xF).data.reg32;
                else cpu->reg(CPSR).data.reg32 = cpu->reg(instruction & 0xF).data.reg32;
                cpu->set->setRegisterBank(cpu->reg(CPSR).data.reg32 & 0x1F);
            } else if((instruction & 0x0D800000) == 0x01000000 && (instruction & 0x003FF000) == 0x0028F000) {
                printf("MSR PSR only\n");
            }
            else arm_data_processing(instruction, cpu);
        }
    }
}

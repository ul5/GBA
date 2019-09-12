#include "ARMInstructions.h"

void Debugger::execute_arm(word instruction, Base::CPU *cpu) {
    if(!Base::isConditionMet((instruction >> 28) & 0xF, cpu->reg(CPSR).data.reg32)) {
        //printf("Condition not met...\n");
        return;
    }
    
    if (instruction & 0x08000000) {
        if (instruction & 0x04000000) {
            if ((instruction & 0x0F000000) == 0x0F000000) {
                printf("SWI\n");
            }
            else {
                if (instruction & 0x02000000) {
                    if (instruction & 0x00000010) {
                        printf("CoProcessor Register transfer\n");
                    }
                    else {
                        printf("CoProcessor Data operation\n");
                    }
                }
                else {
                    printf("CoProcessor Data transfer\n");
                }
            }
        }
        else {
            if (instruction & 0x02000000) arm_branch(instruction, cpu);
            else {
                word a = (instruction & 0x01800000);
                word start_address = cpu->reg((instruction >> 16) & 0xF).data.reg32;
                byte amount_regs = 0;
                for(int i = 0; i < 16; i++) if(instruction & (1 << i)) ++amount_regs;
                
                if(a == 0x01800000) start_address += 4;
                else if(a == 0x00800000) start_address += 0;
                else if(a == 0x01000000) start_address -= amount_regs * 4;
                else if(a == 0x00000000) start_address += amount_regs * 4 - 4;

                if(instruction & 0x00200000) {
                    if(instruction & 0x00800000) cpu->reg((instruction >> 16) & 0xF).data.reg32 += 4 * amount_regs;
                    else cpu->reg((instruction >> 16) & 0xF).data.reg32 -= 4 * amount_regs;
                }
                
                if(instruction & 0x00100000) {
                    for(int i = 0; i < 16; i++) if(instruction & (1 << i)) {
                        cpu->reg(i).data.reg32 = cpu->r32(start_address);
                        start_address += 4;
                    }
                } else {
                    for(int i = 0; i < 16; i++) if(instruction & (1 << i)) {
                        cpu->w32(start_address, cpu->reg(i).data.reg32);
                        start_address += 4;
                    }
                }
            }
        }
    }
    else {
        if (instruction & 0x04000000) {
            if ((instruction & 0x02000010) == 0x02000010) {
                printf("Undefined\n");
            }
            else {
                if(instruction & (1 << 20)) arm_load(instruction, cpu);
                else arm_store(instruction, cpu);
            }
        }
        else {
            if ((instruction & 0x020000F0) == 0x00000090) {
                if (instruction & 0x01000000) {
                    printf("Single Data Swap\n");
                }
                else if (instruction & 0x00800000) {
                    printf("Multiply (accumulate) long\n");
                }
                else {
                    printf("Multiply (accumulate)\n");
                }
            }
            else if ((instruction & 0x020000F0) == 0x00000090) {
                bool pre = instruction & 0x01000000;
                bool up = instruction & 0x00800000;
                bool writeback = (instruction & 0x00200000) || !pre;
                bool load = instruction & 0x0010000;
                bool sig = instruction & 0x40;
                bool half = instruction & 0x20;

                word offset = 0;

                if (instruction & 0x00400000) offset = (instruction & 0xF) | (((instruction >> 8) & 0xF) << 4);
                else offset = cpu->reg(instruction & 0xF).data.reg32;

                if(half) {
                    
                } else {

                }
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

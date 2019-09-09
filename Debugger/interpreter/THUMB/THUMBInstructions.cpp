#include "THUMBInstructions.h"

void Debugger::execute_thumb(hword instruction, Base::CPU *cpu) {
    if(instruction & 0x8000) {
        if(instruction & 0x4000) {
            if(instruction & 0x2000) {
                if(instruction & 0x1000) {
                    word off = ((instruction & 0x7FF) << 12);
                    if(instruction & 0x0400) off |= 0xFF800000;
                    cpu->reg(LR).data.reg32 = cpu->reg(PC).data.reg32 + 2 + off;
                    
                    hword p2 = cpu->r16(cpu->pc().data.reg32);
                    cpu->pc().data.reg32 += 2;
                    cpu->reg(LR).data.reg32 += (p2 & 0x7FF) << 1;

                    word addr = cpu->reg(LR).data.reg32;
                    cpu->reg(LR).data.reg32 = cpu->pc().data.reg32 & 0xFFFFFFFE;
                    cpu->pc().data.reg32 = addr;
                } else {
                    word offset = (instruction & 0x3FF) << 1;
                    if(offset & 0x0800) offset |= 0xFFFFF000;
                    cpu->pc().data.reg32 += 2 + offset;
                }
            } else {
                if(instruction & 0x1000) {
                    if((instruction & 0x0F00) == 0x0F00) {
                        printf("Software interrupt");
                    } else {
						if(Base::isConditionMet((instruction >> 8) & 0xF, cpu->reg(CPSR).data.reg32)) {
							word offset = (instruction & 0xFF) << 1;
							if(offset & 0x100) offset |= 0xFFFFFE00;
							cpu->pc().data.reg32 += offset + 2;
						}
                    }
                } else {
                    printf("Multiple load store (PUSH/POP)");
                }
            }
        } else {
            if(instruction & 0x2000) {
                if(instruction & 0x1000) {
                    if(instruction & 0x0400) {
                        int num = 0;
                        for(int i = 0; i < 9; i++) if(instruction & (1 << i)) ++num;
                        if(instruction & 0x0800) {
                            word start_address = cpu->reg(SP).data.reg32;
                            for(int i = 0; i < 8; i++) {
                                if(instruction & (1 << i)) {
                                    cpu->reg(i).data.reg32 = cpu->r32(start_address);
                                    start_address += 4;
                                }
                            }
                            if(instruction & 0x0100) cpu->reg(PC).data.reg32 = cpu->r32(start_address);
                            cpu->reg(SP).data.reg32 += 4 * num;
                        } else {
                            word start_address = cpu->reg(SP).data.reg32 - 4 * num;
                            for(int i = 0; i < 8; i++) {
                                if(instruction & (1 << i)) {
                                    cpu->w32(start_address, cpu->reg(i).data.reg32);
                                    start_address += 4;
                                }
                            }
                            if(instruction & 0x0100) cpu->w32(start_address, cpu->reg(LR).data.reg32);
                            cpu->reg(SP).data.reg32 -= 4 * num;
                        }
                    } else {
                        word offset = (instruction & 0x7F) << 2;
                        if(instruction & 0x0080) cpu->reg(SP).data.reg32 -= offset;
                        else cpu->reg(SP).data.reg32 += offset;
                    }
                } else {
                    printf("Load Address");
                }
            } else {
                if(instruction & 0x1000) {
                    word addr = cpu->reg(SP).data.reg32 + ((instruction & 0xFF) << 2);
                    if(instruction & 0x0800) cpu->reg((instruction >> 8) & 0x7).data.reg32 = cpu->r32(addr);
                    else cpu->w32(addr, cpu->reg((instruction >> 8) & 0x7).data.reg32);
                } else {
                    word addr = cpu->reg((instruction >> 3) & 0x7).data.reg32;
                    addr += ((instruction >> 6) & 0x1F) << 1; // Offset
                    if(instruction & 0x0100) cpu->reg(instruction & 0x7).data.reg32 = cpu->r16(addr);
                    else cpu->w16(addr, cpu->reg(instruction & 0x7).data.reg32 & 0xFFFF);
                }
            }
        }
    } else {
        if(instruction & 0x4000) {
            if((instruction & 0x1C00) == 0x0000) {
                thumb_alu(instruction, cpu);
            } else if((instruction & 0x1C00) == 0x0400) {
				switch((instruction >> 8) & 3) {
					case 0:
                        cpu->reg((instruction & 0x7) | ((instruction >> 4) & 0x8)).data.reg32 += cpu->reg((instruction >> 3) & 0xF).data.reg32;
						break;
					case 1:
                    {
                        word a1 = cpu->reg((instruction & 0x7) | ((instruction >> 4) & 0x8)).data.reg32;
                        word a2 = cpu->reg((instruction >> 3) & 0xF).data.reg32;
                        word out = a1 - a2;
                        
                        if(out == 0) cpu->reg(CPSR).data.reg32 |= FLAG_Z;
                        else cpu->reg(CPSR).data.reg32 &= ~FLAG_Z;

                        if(out & 0x80000000) cpu->reg(CPSR).data.reg32 |= FLAG_N;
                        else cpu->reg(CPSR).data.reg32 &= ~FLAG_N;

                        if(a2 < a1) cpu->reg(CPSR).data.reg32 |= FLAG_C;
                        else cpu->reg(CPSR).data.reg32 &= ~FLAG_C;
                        
                        if((a1 & 0x80000000) != (a2 & 0x80000000) && (a1 & 0x80000000) == (a2 & 0x80000000)) cpu->reg(CPSR).data.reg32 |= FLAG_V;
                        else cpu->reg(CPSR).data.reg32 &= ~FLAG_V;

                    }
						break;
					case 2:
                        cpu->reg((instruction & 0x7) | ((instruction >> 4) & 0x8)).data.reg32 = cpu->reg((instruction >> 3) & 0xF).data.reg32;
						break;
					case 3: // BX
					{
						word addr = cpu->reg((instruction >> 3) & 0xF).data.reg32;
						cpu->pc().data.reg32 = addr & 0xFFFFFFFE;
						if(!(addr & 1)) cpu->reg(CPSR) &= ~FLAG_T;
					}
						break;
				}
            } else if((instruction & 0x1800) == 0x0800) {
                cpu->reg((instruction >> 8) & 0x7) = cpu->r32(((cpu->pc().data.reg32 + 2) & 0xFFFFFFFC) + ((instruction & 0xFF) << 2));
            } else if(instruction & 0x2000) {
                printf("Load/Store with immidiate offset");
            } else if(instruction & 0x0200) {
                printf("Load/Store with sign extended halfword");
            } else {
                word addr = (cpu->reg((instruction >> 3) & 0x7).data.reg32 + cpu->reg((instruction >> 6) & 0x7).data.reg32);
                if(instruction & 0x0800) {
                    if(instruction & 0x0400) cpu->reg(instruction & 0x7).data.reg32 = (word) cpu->r8(addr);
                    else cpu->reg(instruction & 0x7).data.reg32 = cpu->r32(addr);
                } else {
					if(instruction & 0x0400) cpu->w8(addr, cpu->reg(instruction & 0x7).data.reg32 & 0xFF);
					else cpu->w32(addr, cpu->reg(instruction & 0x7).data.reg32);
                }
            }
        } else {
            if(instruction & 0x2000) thumb_data_processing(instruction, cpu);
			else {
                word res = cpu->reg((instruction >> 3) & 0x7).data.reg32;
                word offset = (instruction >> 6) & 0x1F;
				switch((instruction >> 11) & 3) {
					case 0:
                    {
                        word out = res << offset;
                        if((res >> (32 - offset)) & 1) cpu->reg(CPSR).data.reg32 |= FLAG_C;
                        else cpu->reg(CPSR).data.reg32 &= ~FLAG_C;
                        if(out == 0) cpu->reg(CPSR).data.reg32 |= FLAG_Z;
                        else cpu->reg(CPSR).data.reg32 &= ~FLAG_Z;
                        if(out & 0x80000000) cpu->reg(CPSR).data.reg32 |= FLAG_N;
                        else cpu->reg(CPSR).data.reg32 &= ~FLAG_N;
                        cpu->reg(instruction & 0x7).data.reg32 = out;
                    }
						break;
					case 1:
                    {
                        word out = res >> offset;
                        if((res >> (offset - 1)) & 1) cpu->reg(CPSR).data.reg32 |= FLAG_C;
                        else cpu->reg(CPSR).data.reg32 &= ~FLAG_C;
                        if(out == 0) cpu->reg(CPSR).data.reg32 |= FLAG_Z;
                        else cpu->reg(CPSR).data.reg32 &= ~FLAG_Z;
                        if(out & 0x80000000) cpu->reg(CPSR).data.reg32 |= FLAG_N;
                        else cpu->reg(CPSR).data.reg32 &= ~FLAG_N;
                        cpu->reg(instruction & 0x7).data.reg32 = out;
                    }
						break;
					case 2:
                    {
                        word out = (res >> offset);
                        if(res & 0x80000000) out |= (0xFFFFFFFF << (32 - offset));
                        if((res >> (offset - 1)) & 1) cpu->reg(CPSR).data.reg32 |= FLAG_C;
                        else cpu->reg(CPSR).data.reg32 &= ~FLAG_C;
                        if(out == 0) cpu->reg(CPSR).data.reg32 |= FLAG_Z;
                        else cpu->reg(CPSR).data.reg32 &= ~FLAG_Z;
                        if(out & 0x80000000) cpu->reg(CPSR).data.reg32 |= FLAG_N;
                        else cpu->reg(CPSR).data.reg32 &= ~FLAG_N;
                        cpu->reg(instruction & 0x7).data.reg32 = out;
                    }
						break;
					case 3:
					{
						word a1 = cpu->reg((instruction >> 3) & 0x7).data.reg32;
						word a2 = instruction & 0x0400 ? ((instruction >> 6) & 0x7) : cpu->reg(((instruction >> 6) & 0x7)).data.reg32;
						if(instruction & 0x0200) {
							cpu->reg(instruction & 0x7).data.reg32 = a1 - a2;
							
							if(a2 > a1) cpu->reg(CPSR).data.reg32 |= FLAG_C;
							else cpu->reg(CPSR).data.reg32 &= ~FLAG_C;
							
							if((a2 & 0x80000000) != (a1 & 0x80000000) && (a1 & 0x80000000) == ((a1-a2) & 0x80000000)) cpu->reg(CPSR).data.reg32 |= FLAG_V;
							else cpu->reg(CPSR).data.reg32 &= ~FLAG_V;
						} else {
							cpu->reg(instruction & 0x7).data.reg32 = a1 + a2;
							
							if((uint64_t) a2 + (uint64_t) a1 >= 0x100000000) cpu->reg(CPSR).data.reg32 |= FLAG_C;
							else cpu->reg(CPSR).data.reg32 &= ~FLAG_C;
							
							if((a2 & 0x80000000) == (a1 & 0x80000000) && (a1 & 0x80000000) != ((a1+a2) & 0x80000000)) cpu->reg(CPSR).data.reg32 |= FLAG_V;
							else cpu->reg(CPSR).data.reg32 &= ~FLAG_V;
						}
						
						if(!cpu->reg(instruction & 0x7).data.reg32) cpu->reg(CPSR).data.reg32 |= FLAG_Z;
						else cpu->reg(CPSR).data.reg32 &= ~FLAG_Z;
						
						if(cpu->reg(instruction & 0x7).data.reg32 & 0x80000000) cpu->reg(CPSR).data.reg32 |= FLAG_N;
						else cpu->reg(CPSR).data.reg32 &= ~FLAG_N;
					}
						break;
				}
            }
        }
    }
}

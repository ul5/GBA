#include "THUMBDecompiler.h"


void Decompiler::decompileTHUMB(hword instruction, Base::CPU *cpu) {
    printf("[DECOMPILER][%.08X] > %.04X ==> ", cpu->pc().data.reg32 - 2, instruction);
    
    if(instruction & 0x8000) {
        if(instruction & 0x4000) {
            if(instruction & 0x2000) {
                if(instruction & 0x1000) {
                    word off = ((instruction & 0x7FF) << 12);
                    if(instruction & 0x400) off |= 0xFF800000;
                    word lr = cpu->reg(PC).data.reg32 + 2 + off;
                    
                    hword p2 = cpu->r16(cpu->pc().data.reg32);
                    lr += (p2 & 0x7FF) << 1;

                    printf("BL 0x%.08X", lr);
                } else {
                    printf("Unconditional branch");
                }
            } else {
                if(instruction & 0x1000) {
                    if((instruction & 0x0F00) == 0x0F00) {
                        printf("Software interrupt");
                    } else {
						word offset = (instruction & 0xFF) << 1;
						if(offset & 0x100) offset |= 0xFFFFFE00;
						printf("B%s %.02X", conditions[(instruction >> 8) & 0xF], cpu->pc() + 2 + offset);
                    }
                } else {
                    if(instruction & 0x0800) printf("LDMIA");
                    else printf("STMIA");
                
                    printf("%s!, {", reg_names[(instruction >> 8) & 0x7]);
                    for(int i = 0; i < 8; i++) if(instruction & (1 << i)) printf("%s%s", reg_names[i], (instruction & ((1 << i) - 1)) ? ", " : "");
                    printf("}");
                }
            }
        } else {
            if(instruction & 0x2000) {
                if(instruction & 0x1000) {
                    if(instruction & 0x0400) {
                        if(instruction & 0x0800) printf("POP ");
                        else printf("PUSH ");
                        
                        for(int i = 0; i < 8; i++) if(instruction & (1 << i)) printf("%s%s", reg_names[i], (instruction & 0x0100) || (((1<<i)-1) & instruction) ? ", " : "");
                        if(instruction & 0x0100) printf(instruction & 0x0800 ? "PC" : "LR");
                    } else {
                        word offset = (instruction & 0x7F) << 2;
                        if(instruction & 0x0080) printf("SUB SP, %.03X", offset);
                        else printf("ADD SP, %.03X", offset);
                    }
                } else {
                    printf("Load Address");
                }
            } else {
                if(instruction & 0x1000) {
                    if(instruction & 0x0800) printf("LDR %s, [SP, ", reg_names[(instruction >> 8) & 0x7]);
                    else printf("STR %s, [SP, ", reg_names[(instruction >> 8) & 0x7]);
                    
                    printf("#%.03X]", (instruction & 0xFF) << 2);
                    
                } else {
                    if(instruction & 0x0100) printf("LDRH ");
                    else printf("STRH ");
                    printf("%s, [%s, #%.02X]", reg_names[instruction & 0x7], reg_names[(instruction >> 3) & 0x7], ((instruction >> 6) & 0x1F) << 1);
                }
            }
        }
    } else {
        if(instruction & 0x4000) {
            if((instruction & 0x1C00) == 0x0000) {
                switch ((instruction >> 6) & 0xF) {
                    case 0x0:
                        printf("AND ");
                        break;
                    case 0x1:
                        printf("EOR ");
                        break;
                    case 0x2:
                        printf("LSL ");
                        break;
                    case 0x3:
                        printf("LSR ");
                        break;
                    case 0x4:
                        printf("ASR ");
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
                        printf("TST ");
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
                        printf("ORR ");
                        break;
                    case 0xD:
                        printf("MUL ");
                        break;
                    case 0xE:
                        printf("BIC ");
                        break;
                    case 0xF:
                        printf("MVN ");
                        break;
                    default:
                        break;
                }
                
                printf("%s, %s", reg_names[instruction & 0x7], reg_names[(instruction >> 3) & 0x7]);
            } else if((instruction & 0x1C00) == 0x0400) {
				switch((instruction >> 8) & 3) {
					case 0:
                        printf("ADD %s, %s", reg_names[(instruction & 0x7) | ((instruction >> 4) & 0x8)], reg_names[(instruction >> 3) & 0xF]);
						break;
					case 1:
                        printf("CMP %s, %s", reg_names[(instruction & 0x7) | ((instruction >> 4) & 0x8)], reg_names[(instruction >> 3) & 0xF]);
						break;
					case 2:
                        printf("MOV %s, %s", reg_names[(instruction & 0x7) | ((instruction >> 4) & 0x8)], reg_names[(instruction >> 3) & 0xF]);
						break;
					case 3:
						printf("BX %s", reg_names[(instruction >> 3) & 0xF]);
						break;
				}
            } else if((instruction & 0x1800) == 0x0800) {
                word res = cpu->r32(((cpu->pc().data.reg32 + 2) & 0xFFFFFFFC) + ((instruction & 0xFF) << 2));
                printf("LDR %s, = %.08X", reg_names[(instruction >> 8) & 0x7], res);
            } else if(instruction & 0x2000) {
                printf("Load/Store with immidiate offset");
            } else if(instruction & 0x0200) {
                printf("Load/Store with sign extended halfword");
            } else {
                printf(instruction & 0x0800 ? "LDR" : "STR");
                printf(" %s, [%s, %s]", reg_names[(instruction & 0x7)], reg_names[((instruction >> 3) & 0x7)], reg_names[((instruction >> 6) & 0x7)]);
            }
        } else {
            if(instruction & 0x2000) {
                switch((instruction >> 11) & 3) {
                    case 0:
                        printf("MOV");
                        break;
                    case 1:
                        printf("CMP");
                        break;
                    case 2:
                        printf("ADD");
                        break;
                    case 3:
                        printf("SUB");
                        break;
                }
                
                printf(" %s, %.02X", reg_names[(instruction >> 8) & 0x7], instruction & 0xFF);
            } else {
				switch((instruction >> 11) & 3) {
					case 0:
                        printf("LSLS ");
                        printf("%s, %s, #%.02X", reg_names[instruction & 0x7], reg_names[(instruction >> 3) & 0x7], (instruction >> 6) & 0x1F);
						break;
					case 1:
                        printf("LSRS ");
                        printf("%s, %s, #%.02X", reg_names[instruction & 0x7], reg_names[(instruction >> 3) & 0x7], (instruction >> 6) & 0x1F);
						break;
					case 2:
                        printf("ASRS ");
                        printf("%s, %s, #%.02X", reg_names[instruction & 0x7], reg_names[(instruction >> 3) & 0x7], (instruction >> 6) & 0x1F);
						break;
					case 3:
						printf(instruction & 0x0200 ? "SUBS" : "ADDS");
						printf(" %s, %s, ", reg_names[instruction & 0x7], reg_names[(instruction >> 3) & 0x7]);
						if(instruction & 0x0400) printf("#%.01X", (instruction >> 6) & 0x7);
						else printf("%s", reg_names[(instruction >> 6) & 0x7]);
						break;
				}
            }
        }
    }
    
    printf("\n");
}

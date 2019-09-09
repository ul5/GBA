#include "ARMDecompiler.h"

const char** Decompiler::conditions = new const char*[16] {
	"EQ",
	"NE",
	"CS",
	"CC",
	"MI",
	"PL",
	"VS",
	"VC",
	"HI",
	"LS",
	"GE",
	"LT",
	"GT",
	"LE",
	"",
	"NE"
};

const char** Decompiler::reg_names = new const char*[18] {
    "R0",
    "R1",
    "R2",
    "R3",
    "R4",
    "R5",
    "R6",
    "R7",
    "R8",
    "R9",
    "R10",
    "R11",
    "R12",
    "SP",
    "LR",
    "PC",
    "CPSR",
    "SPSR"
};

const char** Decompiler::reg_names_with_constant_length = new const char*[18] {
    "  R0",
    "  R1",
    "  R2",
    "  R3",
    "  R4",
    "  R5",
    "  R6",
    "  R7",
    "  R8",
    "  R9",
    " R10",
    " R11",
    " R12",
    "  SP",
    "  LR",
    "  PC",
    "CPSR",
    "SPSR"
};

void Decompiler::decompileARM(word instruction, Base::CPU *cpu) {
	printf("[DECOMPILER][%.08X] > %.08X ==> ", cpu->pc().data.reg32 - 4, instruction);

    // Condition = conditions[(instruction >> 28) & 0xF]
    
    
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
			if (instruction & 0x02000000) {
				//printf("Branch");
                printf("B");
                if(instruction & 0x01000000) printf("L");
                printf("%s", conditions[(instruction >> 28) & 0xF]);
                word addr = (instruction & 0x00FFFFFF) << 2;
                if(addr & 0x02000000) addr |= 0xFC000000;
                addr += cpu->pc().data.reg32 + 4;
                printf(" %.08X", addr);
			}
			else {
                if(instruction & 0x00100000) printf("LDM");
                else printf("STM");
                printf("%s", conditions[(instruction >> 28) & 0xF]);
                
                if((instruction & 0x01800000) == 0x00800000) printf("IA");
                else if((instruction & 0x01800000) == 0x01800000) printf("IB");
                else if((instruction & 0x01800000) == 0x00000000) printf("DA");
                else if((instruction & 0x01800000) == 0x01000000) printf("DB");
                
                printf("%s, {", reg_names[(instruction >> 16) & 0xF]);
                for(int i = 0; i < 15; i++) if(instruction & (1 << i)) printf("%s%s", reg_names[i], (instruction & ((1 << i) - 1)) ? ", " : "");
                printf("}");
			}
		}
	}
	else {
		if (instruction & 0x04000000) {
            if ((instruction & 0x02000010) == 0x02000010) {
				printf("Undefined");
			}
			else {
                if(instruction & 0x00100000) printf("LDR");
                else printf("STR");
                printf("%s", conditions[(instruction >> 28) & 0xF]);
                if(instruction & 0x00400000) printf("B");
                if(instruction & 0x00200000) printf("W");
                
                printf(" %s, ", reg_names[(instruction >> 12) & 0xF]);
                printf("[%s, ", reg_names[(instruction >> 16) & 0xF]);
                if(!(instruction & 0x02000000)) printf("#%.03X]", instruction & 0xFFF);
                else {
                    const char *shift = (instruction & 0x40) ? (instruction & 0x20 ? "ROR" : "ASR") : (instruction & 0x20 ? "LSR" : "LSL");
                    if(instruction & 0x10) { // Shift by reg
                        printf(" %s %s]", shift, reg_names[(instruction >> 8) & 0xF]);
                    } else { // Shift by immidiate
                        word amt = (instruction >> 7) & 0x1F;
                        if(amt) {
                            printf("%s #%.08X]", shift, amt);
                        } else printf("]");
                    }
                }
			}
		}
		else {
			if ((instruction & 0x020000F0) == 0x00000090) {
				if (instruction & 0x01000000) {
                    printf("SWP");
                    printf("%s", conditions[(instruction >> 28) & 0xF]);
                    if(instruction & 0x00400000) printf("B");
                    printf(" %s, %s, [%s]", reg_names[(instruction >> 12) & 0xF], reg_names[(instruction >> 0) & 0xF], reg_names[(instruction >> 16) & 0xF]);
				}
				else if (instruction & 0x00800000) {
					printf("Multiply (accumulate) long");
				}
				else {
                    if(instruction & 0x00200000) printf("MLA%s ", (instruction & 0x00100000) ? "S" : "");
                    else printf("MUL%s ", (instruction & 0x00100000) ? "S" : "");

                    if(instruction & 0x00200000) printf("%s, %s, %s", reg_names[(instruction >> 16) & 0xF], reg_names[(instruction & 0xF)],reg_names[(instruction >> 8) & 0xF]);
                    else printf("%s, %s, %s, %s", reg_names[(instruction >> 16) & 0xF], reg_names[(instruction & 0xF)],reg_names[(instruction >> 8) & 0xF], reg_names[(instruction >> 12) & 0xF]);
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
                printf("%s%s", (instruction & 0x00100000) ? "LDRS" : "STRS", (instruction & 0x00000020) ? "H" : "B");
                printf("(NYI)");
			}
			else if ((instruction & 0x0FFFFFF0) == 0x012FFF10) {
				printf("BX %s", reg_names[instruction & 0xF]);
                printf("%s", conditions[(instruction >> 28) & 0xF]);
            } else if((instruction & 0x0F800000) == 0x01000000 && (instruction & 0x003F0000) == 0x003F0000 && (instruction & 0x00000FFF) == 0x00000FFF) {
                printf("MRS");
                printf("%s", conditions[(instruction >> 28) & 0xF]);
            } else if((instruction & 0x0F800000) == 0x01000000 && (instruction & 0x003FFFF0) == 0x0029F000) {
                if(instruction & (1 << 22)) printf("MSR SPSR, %s", reg_names[instruction & 0xF]);
                else printf("MSR CPSR, %s", reg_names[instruction & 0xF]);
            } else if((instruction & 0x0D800000) == 0x01000000 && (instruction & 0x003FF000) == 0x0028F000) {
                printf("MSR PSR only");
            }
			else {
                bool imm = instruction & 0x02000000;
                bool s = instruction & 0x00100000;
                byte opcode = (instruction >> 21) & 0xF;
                
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
                    case 0x4:
                        printf("ADD");
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
                    case 0x8:
                        printf("TST");
                        break;
                    case 0x9:
                        printf("TEQ");
                        break;
                    case 0xA:
                        printf("CMP");
                        break;
                    case 0xB:
                        printf("CMN");
                        break;
                    case 0xC:
                        printf("ORR");
                        break;
                    case 0xD:
                        printf("MOV");
                        break;
                    case 0xE:
                        printf("BIC");
                        break;
                    case 0xF:
                        printf("MVN");
                        break;
                }
                
                printf("%s", conditions[(instruction >> 28) & 0xF]);
                if(s && (opcode >= 0x8 && opcode <= 0xA)) printf("S");
                
                if(opcode <= 0x8 || opcode >= 0xA) printf(" %s, ", reg_names[(instruction >> 12) & 0xF]);
                if(opcode != 0xD) printf(" %s, ", reg_names[(instruction >> 16) & 0xF]);
                
                if(imm) printf("#%.08X", Base::rotate(instruction & 0xFF, (instruction >> 8) & 0xF));
                else {
                    printf("[%s", reg_names[instruction & 0xF]);
                    const char *shift = (instruction & 0x40) ? (instruction & 0x20 ? "ROR" : "ASR") : (instruction & 0x20 ? "LSR" : "LSL");
                    if(instruction & 0x10) { // Shift by reg
                        printf(" %s %s]", shift, reg_names[(instruction >> 8) & 0xF]);
                    } else { // Shift by immidiate
                        word amt = (instruction >> 7) & 0x1F;
                        if(amt) {
                            printf("%s #%.08X]", shift, amt);
                        } else printf("]");
                    }
                }
			}
		}
    }
    
    printf("\n");
}

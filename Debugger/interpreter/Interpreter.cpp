#include "Interpreter.h"

Debugger::Interpreter::Interpreter() : Debugger::Debugger() {

}

Debugger::Interpreter::~Interpreter() {
    
}

void Debugger::Interpreter::executeNextInstruction(bool disass) {
    cpu->update_cycles(1); // All instructions increase the cycle by at least one

    // printf("Cpu cycle count: %.08X\n", cpu->cycle_count);

    if(!cpu->mmu->halted) {
        word ni = fetchNextInstruction();
        if(cpu->reg(CPSR).data.reg32 & FLAG_T) {
            if(disass) Decompiler::decompileTHUMB(ni & 0xFFFF, cpu);
            execute_thumb(ni & 0xFFFF, cpu);
        } else {
            if(disass) std::string out = Decompiler::decompileARM(ni, cpu);
            execute_arm(ni, cpu);
        }
    }
    
    cpu->update_gpu();
}

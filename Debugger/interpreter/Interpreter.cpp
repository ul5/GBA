#include "Interpreter.h"

Debugger::Interpreter::Interpreter() : Debugger::Debugger() {

}

Debugger::Interpreter::~Interpreter() {
    
}

void Debugger::Interpreter::executeNextInstruction() {
    word ni = fetchNextInstruction();
    
    if(cpu->reg(CPSR).data.reg32 & FLAG_T) {
        Decompiler::decompileTHUMB(ni & 0xFFFF, cpu);
        execute_thumb(ni & 0xFFFF, cpu);
    } else {
        Decompiler::decompileARM(ni, cpu);
        execute_arm(ni, cpu);
    }
}

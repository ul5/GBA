#include <stdio.h>
#include "GBABase.h"
#include "GBADebugger.h"
#include "GBADecompiler.h"

int main(int argc, const char **args) {
    
    printf("[INFO] Starting GBA Emulator in %s\n", args[0]);
    Debugger::Interpreter *interpreter = new Debugger::Interpreter();
    for(int i = 0; i < 36 || interpreter->pc().data.reg32 != 0xAB4; i++) {
        interpreter->executeNextInstruction();
    }
    
    // Execute two more instructions (and see the difference it made in the registers)
    
    interpreter->executeNextInstruction();
    interpreter->printRegisters();
    interpreter->executeNextInstruction();
    interpreter->printRegisters();
    
    delete interpreter;
	return 0;
}

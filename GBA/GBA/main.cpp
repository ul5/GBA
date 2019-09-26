#include <stdio.h>
#include "GBABase.h"
#include "GBADebugger.h"
#include "GBADecompiler.h"

int main(int argc, const char **args) {
    printf("[INFO] Starting GBA Emulator in %s\n", args[0]);
    Debugger::Interpreter *interpreter = new Debugger::Interpreter();
    for(int i = 0; interpreter->pc().data.reg32 != 0x1098; i++) {
        interpreter->executeNextInstruction(true);
        interpreter->printRegisters();
    }
    
    // 6CC
    
    Debugger::GUI a(interpreter);
    a.start();

    //start_command_line(interpreter);
    
    
    delete interpreter;
	return 0;
}

#include <stdio.h>
#include "GBABase.h"
#include "GBADebugger.h"
#include "GBADecompiler.h"

int main(int argc, const char **args) {
    printf("[INFO] Starting GBA Emulator in %s\n", args[0]);
    Debugger::Interpreter *interpreter = new Debugger::Interpreter();

    int counter = 0; // 0x082E062D
    while(interpreter->pc().data.reg32 != 8 && interpreter->pc().data.reg32 >> 24 != 3 && (interpreter->pc().data.reg32 & 1) == 0) {
        interpreter->executeNextInstruction(false);
        if(++counter >= 280896) {
            counter = 0;
            interpreter->cpu->render();
        }
    }
        
    Debugger::GUI a(interpreter);
    a.start();

    //start_command_line(interpreter);
    
    
    delete interpreter;
	return 0;
}

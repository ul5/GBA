#pragma once

#include "../debugger/Debugger.h"
#include "ARM/ARMInstructions.h"
#include "THUMB/THUMBInstructions.h"

namespace Debugger {
	
    class Interpreter : public Debugger::Debugger {
	private:
        typedef struct {
            word pc;
            word instruction;
            std::string disass;
            word lr;
        } instruction_ref;
        std::vector<instruction_ref> instruction_trace;
        
	public:
        Interpreter();
        ~Interpreter();
		
		Base::Register &pc() { return cpu->pc(); }
        
        inline void printTrace() override {
            for(int i = 0; i < instruction_trace.size(); i++) printf("[%.08X] %.08X (LR=%.08X) > %s\n", instruction_trace[i].pc, instruction_trace[i].instruction, instruction_trace[i].lr, instruction_trace[i].disass.c_str());
        }
        
		void executeNextInstruction(bool disass = true) override;
	};
	
}

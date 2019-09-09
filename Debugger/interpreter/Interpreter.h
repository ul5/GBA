#pragma once

#include "../debugger/Debugger.h"
#include "ARM/ARMInstructions.h"
#include "THUMB/THUMBInstructions.h"

namespace Debugger {
	
    class Interpreter : public Debugger::Debugger {
	private:
		
	public:
        Interpreter();
        ~Interpreter();
		
		Base::Register &pc() { return cpu->pc(); }
        
		void executeNextInstruction(bool disass = true) override;
	};
	
}

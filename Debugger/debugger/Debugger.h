#pragma once

#include "GBABase.h"
#include "GBADecompiler.h"

namespace Debugger {
	
	class Debugger {
	protected:
        Base::CPU *cpu = nullptr;
        
	public:
        Debugger();
        ~Debugger();
        
        word fetchNextInstruction();
		virtual void executeNextInstruction() = 0;
		
        void printRegisters();
	};
	
}

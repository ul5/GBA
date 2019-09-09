#pragma once

#include "GBABase.h"

namespace Decompiler {
	
    extern const char** reg_names;
    extern const char** conditions;
	void decompileARM(word instruction, Base::CPU *cpu);
	
}

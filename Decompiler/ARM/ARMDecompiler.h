#pragma once

#include "GBABase.h"

namespace Decompiler {
	
    extern const char** reg_names, **reg_names_with_constant_length;
    extern const char** conditions;
	void decompileARM(word instruction, Base::CPU *cpu);
	
}

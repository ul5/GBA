#pragma once

#include "../ARM/ARMDecompiler.h" // Some definitions for the register names

namespace Decompiler {
    void decompileTHUMB(hword instruction, Base::CPU *cpu);
}

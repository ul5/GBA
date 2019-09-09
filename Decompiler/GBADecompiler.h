#pragma once

#include "ARM/ARMDecompiler.h"
#include "THUMB/THUMBDecompiler.h"

namespace Decompiler {

    inline void decompileInstruction(Base::CPU *cpu, word offset) {
        cpu->pc().data.reg32 += offset;
        if(cpu->reg(CPSR).data.reg32 & FLAG_T) {
            hword i = cpu->r16(cpu->pc().data.reg32);
            cpu->pc().data.reg32 += 2;
            decompileTHUMB(i, cpu);
            cpu->pc().data.reg32 -= 2;
        }
        else {
            word i = cpu->r32(cpu->pc().data.reg32);
            cpu->pc().data.reg32 += 4;
            decompileARM(i, cpu);
            cpu->pc().data.reg32 -= 4;
        }
        
        cpu->pc().data.reg32 -= offset;
    }

}
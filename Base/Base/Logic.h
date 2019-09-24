#pragma once

#include "DataTypes.h"
#include "../Register/RegisterSet.h"

namespace Base {
	
	bool isConditionMet(byte condition, word cpsrContents);
    word shift(byte data, word reg, RegisterSet set, bool set_cond = true);
	word rotate(byte value, byte rotate, Base::RegisterSet set, bool set_cond = true);
	
}

#pragma once

#include "DataTypes.h"
#include "../Register/RegisterSet.h"

namespace Base {
	
	bool isConditionMet(byte condition, word cpsrContents);
    word shift(byte data, word reg, RegisterSet set);
	word rotate(byte value, byte rotate);
	
}

#include "pch.h"
#include "vec.h"
#include "direction.h"

namespace hats
{
	template<space SPACE>
	direction<SPACE> vec<SPACE>::normalize_copy() const
	{
		return direction<SPACE>(x, y, z);
	}
}
#include "pch.h"
#include "vec.h"
#include "direction.h"

namespace hats
{
	template<space SPACE>
	constexpr direction<SPACE> vec<SPACE>::normalize_copy() const
	{
		return direction<SPACE>(x, y, z);
	}
} // namespace hats
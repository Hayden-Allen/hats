#include "pch.h"
#include "direction.h"
#include "vec.h"

namespace hats
{
	template<space SPACE>
	constexpr vec<SPACE> direction<SPACE>::operator*(const f32 s) const
	{
		return vec<SPACE>(x * s, y * s, z * s);
	}
}
#include "pch.h"
#include "direction.h"
#include "vec.h"

namespace hats
{
	template<space SPACE>
	template<typename T>
	constexpr vec<SPACE> direction<SPACE>::operator*(const T s) const
	{
		const f32 fs = HATS_CAST(f32, s);
		return vec<SPACE>(x * fs, y * fs, z * fs);
	}
}
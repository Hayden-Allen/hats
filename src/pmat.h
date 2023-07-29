#pragma once
#include "pch.h"
#include "mat.h"
#include "tmat.h"

namespace hats
{
	// mat4.asm
	extern "C" void mat_multiply(f32* const dst, const f32* const src1, const f32* const src2);
	template<space FROM, space TO>
	struct alignas(64) pmat : public mat<FROM, TO>
	{
		using mat<FROM, TO>::i;
		using mat<FROM, TO>::j;
		using mat<FROM, TO>::k;
		using mat<FROM, TO>::t;
		using mat<FROM, TO>::e;
		using mat<FROM, TO>::m;
	public:
		constexpr pmat(const f32 x, const f32 y, const f32 a, const f32 b, const f32 w) :
			mat<FROM, TO>
			(
				x, 0, 0, 0,
				0, y, 0, 0,
				0, 0, a, b,
				0, 0, w, 0
			)
		{}
	public:
		template<space FROM2>
		mat<FROM2, TO> operator*(const tmat<FROM2, FROM>& o) const
		{
			mat<FROM2, TO> ret;
			mat_multiply(ret.i, i, o.i);
			// mat_multiply(ret.i, o.i, i);
			return ret;
		}
	};
}
#pragma once
#include "pch.h"
#include "mat.h"
#include "tmat.h"

namespace hats
{
	template<space FROM, space TO>
	struct pmat : public mat<FROM, TO>
	{
		using mat<FROM, TO>::i;
		using mat<FROM, TO>::j;
		using mat<FROM, TO>::k;
		using mat<FROM, TO>::t;
		using mat<FROM, TO>::e;
		using mat<FROM, TO>::m;
	public:
		template<typename X, typename Y, typename A, typename B, typename W>
		constexpr pmat(const X x, const Y y, const A a, const B b, const W w) :
			mat<FROM, TO>(
				x,
				0,
				0,
				0,
				0,
				y,
				0,
				0,
				0,
				0,
				a,
				b,
				0,
				0,
				w,
				0)
		{}
	public:
		template<space FROM2>
		mat<FROM2, TO> operator*(const tmat<FROM2, FROM>& o) const
		{
			mat<FROM2, TO> ret;
			mat_multiply(ret.i, i, o.i);
			return ret;
		}
	};
} // namespace hats
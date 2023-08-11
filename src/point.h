#pragma once
#include "pch.h"
#include "vec_base.h"
#include "tmat.h"

namespace hats
{
	template<space SPACE>
	struct vec;
	template<space SPACE>
	struct direction;

	template<space SPACE>
	struct alignas(16) point : public vec_base<SPACE>
	{
		using vec_base<SPACE>::x;
		using vec_base<SPACE>::y;
		using vec_base<SPACE>::z;
		using vec_base<SPACE>::w;
		using vec_base<SPACE>::e;
	public:
		constexpr point() :
			vec_base<SPACE>(0.f, 0.f, 0.f, 1.f) {}
		template<typename X, typename Y, typename Z>
		constexpr point(const X x, const Y y, const Z z) :
			vec_base<SPACE>(x, y, z, 1.f)
		{}
	public:
		constexpr point<SPACE> operator+(const vec<SPACE>& v) const;
		constexpr point<SPACE> operator+(const direction<SPACE>& d) const;
		constexpr point<SPACE>& operator+=(const vec<SPACE>& v);
		constexpr point<SPACE>& operator+=(const direction<SPACE>& d);
		constexpr point<SPACE> operator-() const
		{
			return point<SPACE>(-x, -y, -z);
		}
		constexpr vec<SPACE> operator-(const point<SPACE>& o) const;
		constexpr point<SPACE> operator-(const vec<SPACE>& v) const;
		constexpr point<SPACE> operator-(const direction<SPACE>& d) const;
		constexpr point<SPACE>& operator-=(const vec<SPACE>& v);
		constexpr point<SPACE>& operator-=(const direction<SPACE>& d);
		constexpr bool operator==(const point<SPACE>& o) const
		{
			return vec_base<SPACE>::is_equal(o);
		}
		constexpr bool operator!=(const point<SPACE>& o) const
		{
			return vec_base<SPACE>::is_not_equal(o);
		}
	public:
		template<space TO>
		constexpr point<TO> transform_copy(const tmat<SPACE, TO>& m) const
		{
			const f32 nx = m.i[0] * x + m.j[0] * y + m.k[0] * z + m.t[0] * w;
			const f32 ny = m.i[1] * x + m.j[1] * y + m.k[1] * z + m.t[1] * w;
			const f32 nz = m.i[2] * x + m.j[2] * y + m.k[2] * z + m.t[2] * w;
			return point<TO>(nx, ny, nz);
		}
		constexpr virtual void print() const override
		{
			printf("point<%d>\t{ %06f\t%06f\t%06f }\n", SPACE, x, y, z);
		}
	};
} // namespace hats
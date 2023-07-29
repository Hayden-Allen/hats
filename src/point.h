#pragma once
#include "pch.h"
#include "vec4.h"
#include "tmat.h"

namespace hats
{
	template<space SPACE>
	struct vec;
	template<space SPACE>
	struct direction;
	template<space SPACE>
	struct alignas(16) point : public vec4<SPACE>
	{
		using vec4<SPACE>::x;
		using vec4<SPACE>::y;
		using vec4<SPACE>::z;
		using vec4<SPACE>::w;
		using vec4<SPACE>::e;
	public:
		point() : vec4<SPACE>(0.f, 0.f, 0.f, 1.f) {}
		point(const vec4<SPACE>& v) : vec4<SPACE>(v) {}
		constexpr point(const f32 x, const f32 y, const f32 z) : vec4<SPACE>(x, y, z, 1.f) {}
	public:
		point<SPACE> operator+(const vec<SPACE>& v) const;
		point<SPACE> operator+(const direction<SPACE>& d) const;
		point<SPACE>& operator+=(const vec<SPACE>& v);
		point<SPACE>& operator+=(const direction<SPACE>& d);
		point<SPACE> operator-() const
		{
			return point<SPACE>(-x, -y, -z);
		}
		vec<SPACE> operator-(const point<SPACE>& o) const;
		point<SPACE> operator-(const vec<SPACE>& v) const;
		point<SPACE> operator-(const direction<SPACE>& d) const;
		point<SPACE>& operator-=(const vec<SPACE>& v);
		point<SPACE>& operator-=(const direction<SPACE>& d);
		constexpr bool operator==(const point<SPACE>& o) const
		{
			const f32 dx = x - o.x, dy = y - o.y, dz = z - o.z;
			return	(dx < 0 ? -dx < c::EPSILON : dx < c::EPSILON) &&
					(dy < 0 ? -dy < c::EPSILON : dy < c::EPSILON) &&
					(dz < 0 ? -dz < c::EPSILON : dz < c::EPSILON);
		}
		constexpr bool operator!=(const point<SPACE>& o) const
		{
			const f32 dx = x - o.x, dy = y - o.y, dz = z - o.z;
			return	(dx < 0 ? -dx >= c::EPSILON : dx >= c::EPSILON) ||
					(dy < 0 ? -dy >= c::EPSILON : dy >= c::EPSILON) ||
					(dz < 0 ? -dz >= c::EPSILON : dz >= c::EPSILON);
		}
		template<space TO>
		point<TO> transform_copy(const tmat<SPACE, TO>& m) const
		{
			const f32 nx = m.i[0] * x + m.j[0] * y + m.k[0] * z + m.t[0] * w;
			const f32 ny = m.i[1] * x + m.j[1] * y + m.k[1] * z + m.t[1] * w;
			const f32 nz = m.i[2] * x + m.j[2] * y + m.k[2] * z + m.t[2] * w;
			return point<TO>(nx, ny, nz);
		}
		virtual void print() const override
		{
			printf("point<%d>\t{ %06f\t%06f\t%06f }\n", SPACE, x, y, z);
		}
	};
}
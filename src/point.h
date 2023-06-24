#pragma once
#include "pch.h"
#include "vec4.h"
#include "hmat.h"

namespace hats
{
	template<space SPACE>
	struct vec;
	template<space SPACE>
	struct direction;
	template<space SPACE>
	struct alignas(16) point : public vec4<SPACE>
	{
		using vec4<SPACE>::x, vec4<SPACE>::y, vec4<SPACE>::z, vec4<SPACE>::w;

		point() : vec4<SPACE>(0.f, 0.f, 0.f, 1.f) {}
		point(const vec4<SPACE>& v) : vec4<SPACE>(v) {}
		constexpr point(const f32 x, const f32 y, const f32 z) : vec4<SPACE>(x, y, z, 1.f) {}

		point<SPACE> operator+(const vec<SPACE>& v) const;
		point<SPACE> operator+(const direction<SPACE>& d) const;
		point<SPACE>& operator+=(const vec<SPACE>& v);
		point<SPACE>& operator+=(const direction<SPACE>& d);
		vec<SPACE> operator-(const point<SPACE>& o) const;
		point<SPACE> operator-(const vec<SPACE>& v) const;
		point<SPACE> operator-(const direction<SPACE>& d) const;
		point<SPACE>& operator-=(const vec<SPACE>& v);
		point<SPACE>& operator-=(const direction<SPACE>& d);
		constexpr bool operator==(const point<SPACE>& o) const
		{
			const f32 dx = x - o.x, dy = y - o.y, dz = z - o.z;
			return	(dx < 0 ? -dx < EPSILON : dx < EPSILON) &&
					(dy < 0 ? -dy < EPSILON : dy < EPSILON) &&
					(dz < 0 ? -dz < EPSILON : dz < EPSILON);
		}
		constexpr bool operator!=(const point<SPACE>& o) const
		{
			const f32 dx = x - o.x, dy = y - o.y, dz = z - o.z;
			return	(dx < 0 ? -dx >= EPSILON : dx >= EPSILON) ||
					(dy < 0 ? -dy >= EPSILON : dy >= EPSILON) ||
					(dz < 0 ? -dz >= EPSILON : dz >= EPSILON);
		}
		template<space TO>
		point<TO> transform_copy(const hmat<SPACE, TO>& m) const
		{
			const f32 nx = m.i[0] * x + m.j[0] * y + m.k[0] * z + m.t[0] * w;
			const f32 ny = m.i[1] * x + m.j[1] * y + m.k[1] * z + m.t[1] * w;
			const f32 nz = m.i[2] * x + m.j[2] * y + m.k[2] * z + m.t[2] * w;
			return point<TO>(nx, ny, nz);
		}
		/*point& transform(const hmat& m)
		{
			vec4::transform(m);
			return *this;
		}*/
		virtual void print() const override
		{
			printf("point<%d>\t{ %06f\t%06f\t%06f }\n", SPACE, x, y, z);
		}
	};
}
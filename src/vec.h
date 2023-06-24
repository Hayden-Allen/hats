#pragma once
#include "pch.h"
#include "vec4.h"
#include "hmat.h"

namespace hats
{
	template<space SPACE>
	struct direction;
	template<space SPACE>
	struct alignas(16) vec : public vec4<SPACE>
	{
		using vec4<SPACE>::x, vec4<SPACE>::y, vec4<SPACE>::z;

		vec() : vec4<SPACE>(0.f, 0.f, 0.f, 0.f) {}
		vec(const vec4<SPACE>& v) : vec4<SPACE>(v) {}
		constexpr vec(const f32 x, const f32 y, const f32 z) : vec4<SPACE>(x, y, z, 0.f) {}

		vec<SPACE>& set_length(const f32 l)
		{
			return (*this * (l / length()));
		}
		template<space TO>
		vec<TO> transform_copy(const hmat<SPACE, TO>& m) const
		{
			const f32 nx = m.i[0] * x + m.j[0] * y + m.k[0] * z;
			const f32 ny = m.i[1] * x + m.j[1] * y + m.k[1] * z;
			const f32 nz = m.i[2] * x + m.j[2] * y + m.k[2] * z;
			return vec<TO>(nx, ny, nz);
		}
		/*vec& transform(const hmat& m)
		{
			vec4::transform(m);
			return *this;
		}*/
		f32 length2() const
		{
			return x * x + y * y + z * z;
		}
		f32 length() const
		{
			return sqrt(length2());
		}
		direction<SPACE> normalize_copy() const;
		vec<SPACE>& cross(const vec<SPACE>& o)
		{
			const f32 cx = y * o.z - z * o.y;
			const f32 cy = z * o.x - x * o.z;
			const f32 cz = x * o.y - y * o.x;
			x = cx; y = cy; z = cz;
			return *this;
		}
		vec<SPACE> cross_copy(const vec<SPACE>& o) const
		{
			const f32 cx = y * o.z - z * o.y;
			const f32 cy = z * o.x - x * o.z;
			const f32 cz = x * o.y - y * o.x;
			return vec(cx, cy, cz);
		}
		vec<SPACE> operator-() const
		{
			return vec<SPACE>(-x, -y, -z);
		}
		vec<SPACE> operator*(const f32 s) const
		{
			return vec<SPACE>(x * s, y * s, z * z);
		}
		vec<SPACE>& operator*=(const f32 s)
		{
			x *= s; y *= s; z *= s;
			return *this;
		}
		constexpr bool operator==(const vec<SPACE>& o) const
		{
			const f32 dx = x - o.x, dy = y - o.y, dz = z - o.z;
			return	(dx < 0 ? -dx < EPSILON : dx < EPSILON) &&
				(dy < 0 ? -dy < EPSILON : dy < EPSILON) &&
				(dz < 0 ? -dz < EPSILON : dz < EPSILON);
		}
		constexpr bool operator!=(const vec<SPACE>& o) const
		{
			const f32 dx = x - o.x, dy = y - o.y, dz = z - o.z;
			return	(dx < 0 ? -dx >= EPSILON : dx >= EPSILON) ||
				(dy < 0 ? -dy >= EPSILON : dy >= EPSILON) ||
				(dz < 0 ? -dz >= EPSILON : dz >= EPSILON);
		}
		virtual void print() const override
		{
			printf("vec<%d>\t{ %06f\t%06f\t%06f }\n", SPACE, x, y, z);
		}
	};
}
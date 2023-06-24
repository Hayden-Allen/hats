#pragma once
#include "pch.h"
#include "vec4.h"
#include "hmat.h"

namespace hats
{
	template<space SPACE>
	struct alignas(16) direction : public vec4<SPACE>
	{
		using vec4<SPACE>::x, vec4<SPACE>::y, vec4<SPACE>::z;
	public:
		direction(const f32 x, const f32 y, const f32 z) : vec4<SPACE>(x, y, z, 0.f)
		{
			normalize();
		}
		direction(const vec4<SPACE>& v) : vec4<SPACE>(v)
		{
			normalize();
		}
	public:
		static direction<SPACE> i_hat()
		{
			return direction<SPACE>(1, 0, 0);
		}
		static direction<SPACE> j_hat()
		{
			return direction<SPACE>(0, 1, 0);
		}
		static direction<SPACE> k_hat()
		{
			return direction<SPACE>(0, 0, 1);
		}
	public:
		template<space TO>
		direction<TO> transform_copy(const hmat<SPACE, TO>& m) const
		{
			const f32 nx = m.i[0] * x + m.j[0] * y + m.k[0] * z;
			const f32 ny = m.i[1] * x + m.j[1] * y + m.k[1] * z;
			const f32 nz = m.i[2] * x + m.j[2] * y + m.k[2] * z;
			return direction<TO>(nx, ny, nz);
		}
		direction<SPACE>& cross(const direction<SPACE>& o)
		{
			const f32 cx = y * o.z - z * o.y;
			const f32 cy = z * o.x - x * o.z;
			const f32 cz = x * o.y - y * o.x;
			x = cx; y = cy; z = cz;
			normalize();
			return *this;
		}
		direction<SPACE> cross_copy(const direction<SPACE>& o) const
		{
			const f32 cx = y * o.z - z * o.y;
			const f32 cy = z * o.x - x * o.z;
			const f32 cz = x * o.y - y * o.x;
			return direction<SPACE>(cx, cy, cz);
		}
		direction<SPACE> operator-() const
		{
			return direction<SPACE>(-x, -y, -z);
		}
		direction<SPACE> operator*(const f32 s) const
		{
			return direction<SPACE>(x * s, y * s, z * z);
		}
		direction<SPACE>& operator*=(const f32 s)
		{
			x *= s; y *= s; z *= s;
			normalize();
			return *this;
		}
		constexpr bool operator==(const direction<SPACE>& o) const
		{
			const f32 dx = x - o.x, dy = y - o.y, dz = z - o.z;
			return	(dx < 0 ? -dx < EPSILON : dx < EPSILON) &&
				(dy < 0 ? -dy < EPSILON : dy < EPSILON) &&
				(dz < 0 ? -dz < EPSILON : dz < EPSILON);
		}
		constexpr bool operator!=(const direction<SPACE>& o) const
		{
			const f32 dx = x - o.x, dy = y - o.y, dz = z - o.z;
			return	(dx < 0 ? -dx >= EPSILON : dx >= EPSILON) ||
				(dy < 0 ? -dy >= EPSILON : dy >= EPSILON) ||
				(dz < 0 ? -dz >= EPSILON : dz >= EPSILON);
		}
		virtual void print() const override
		{
			printf("dir<%d>\t{ %06f\t%06f\t%06f }\n", SPACE, x, y, z);
		}
	private:
		void normalize()
		{
			const f32 l = sqrt(x * x + y * y + z * z);
			x /= l;
			y /= l;
			z /= l;
		}
	};
}
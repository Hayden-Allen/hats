#pragma once
#include "pch.h"
#include "vec4.h"
#include "hmat.h"
#include "vec_util.h"

namespace hats
{
	template<space SPACE>
	struct alignas(16) direction : public vec4<SPACE>
	{
		using vec4<SPACE>::x;
		using vec4<SPACE>::y;
		using vec4<SPACE>::z;
		using vec4<SPACE>::w;
		using vec4<SPACE>::e;
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
		template<space TO>
		direction<TO> transform_copy(const hmat<SPACE, TO>& m) const
		{
			f32 t[4] = { 0.f };
			vec_util::transform(t, m.e, e);
			return direction<TO>(t[0], t[1], t[2]);
		}
		direction<SPACE>& cross(const direction<SPACE>& o)
		{
			vec_util::cross(e, e, o.e);
			normalize();
			return *this;
		}
		direction<SPACE> cross_copy(const direction<SPACE>& o) const
		{
			f32 t[3] = { 0.f };
			vec_util::cross(t, e, o.e);
			return direction<SPACE>(t[0], t[1], t[2]);
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
			vec_util::scale(e, e, s);
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
		__forceinline void normalize()
		{
			vec_util::normalize(e, e);
		}
	};
}
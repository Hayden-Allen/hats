#pragma once
#include "pch.h"
#include "vec4.h"
#include "tmat.h"
#include "vec_util.h"

namespace hats
{
	template<space SPACE>
	struct direction;
	template<space SPACE>
	struct alignas(16) vec : public vec4<SPACE>
	{
		using vec4<SPACE>::x;
		using vec4<SPACE>::y;
		using vec4<SPACE>::z;
		using vec4<SPACE>::w;
		using vec4<SPACE>::e;
	public:
		vec() : vec4<SPACE>(0.f, 0.f, 0.f, 0.f) {}
		vec(const vec4<SPACE>& v) : vec4<SPACE>(v) {}
		constexpr vec(const f32 x, const f32 y, const f32 z) : vec4<SPACE>(x, y, z, 0.f) {}
	public:
		f32 dot(const vec<SPACE>& o) const
		{
			return vec_util::dot(e, o.e);
		}
		vec<SPACE>& set_length(const f32 l)
		{
			vec_util::scale(e, e, l / vec_util::length(e));
			return *this;
		}
		template<space TO>
		vec<TO> transform_copy(const tmat<SPACE, TO>& m) const
		{
			f32 t[4] = { 0.f };
			vec_util::transform(t, m.e, e);
			return vec<TO>(t[0], t[1], t[2]);
		}
		f32 length2() const
		{
			return vec_util::length2(e);
		}
		f32 length() const
		{
			return vec_util::length(e);
		}
		direction<SPACE> normalize_copy() const;
		vec<SPACE>& cross(const vec<SPACE>& o)
		{
			vec_util::cross(e, e, o.e);
			return *this;
		}
		vec<SPACE> cross_copy(const vec<SPACE>& o) const
		{
			vec v(0, 0, 0);
			vec_util::cross(v.e, e, o.e);
			return v;
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
			vec_util::scale(e, e, s);
			return *this;
		}
		constexpr bool operator==(const vec<SPACE>& o) const
		{
			const f32 dx = x - o.x, dy = y - o.y, dz = z - o.z;
			return	(dx < 0 ? -dx < c::EPSILON : dx < c::EPSILON) &&
				(dy < 0 ? -dy < c::EPSILON : dy < c::EPSILON) &&
				(dz < 0 ? -dz < c::EPSILON : dz < c::EPSILON);
		}
		constexpr bool operator!=(const vec<SPACE>& o) const
		{
			const f32 dx = x - o.x, dy = y - o.y, dz = z - o.z;
			return	(dx < 0 ? -dx >= c::EPSILON : dx >= c::EPSILON) ||
				(dy < 0 ? -dy >= c::EPSILON : dy >= c::EPSILON) ||
				(dz < 0 ? -dz >= c::EPSILON : dz >= c::EPSILON);
		}
		virtual void print() const override
		{
			printf("vec<%d>\t{ %06f\t%06f\t%06f }\n", SPACE, x, y, z);
		}
	};
}
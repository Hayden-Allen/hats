#pragma once
#include "pch.h"
#include "vec_base.h"
#include "tmat.h"
#include "vec_util.h"

namespace hats
{
	template<space SPACE>
	struct vec;

	template<space SPACE>
	struct alignas(16) direction : public vec_base<SPACE>
	{
		using vec_base<SPACE>::x;
		using vec_base<SPACE>::y;
		using vec_base<SPACE>::z;
		using vec_base<SPACE>::w;
		using vec_base<SPACE>::e;
	public:
		template<typename X, typename Y, typename Z>
		constexpr direction(const X x, const Y y, const Z z) : vec_base<SPACE>(x, y, z, 0.f)
		{
			normalize();
		}
		explicit constexpr direction(const vec_base<SPACE>& v) : vec_base<SPACE>(v.x, v.y, v.z, 0.f)
		{
			normalize();
		}
	public:
		template<typename T>
		constexpr vec<SPACE> operator*(const T s) const;
		constexpr direction<SPACE> operator-() const
		{
			return direction<SPACE>(-x, -y, -z);
		}
		constexpr direction<SPACE>& operator=(const direction<SPACE>& o)
		{
			vec_util::copy(e, o.e);
			return *this;
		}
		constexpr bool operator==(const direction<SPACE>& o) const
		{
			return vec_base<SPACE>::is_equal(o);
		}
		constexpr bool operator!=(const direction<SPACE>& o) const
		{
			return vec_base<SPACE>::is_not_equal(o);
		}
	public:
		template<space TO>
		constexpr direction<TO> transform_copy(const tmat<SPACE, TO>& m) const
		{
			f32 t[4] = { 0.f };
			vec_util::transform(t, m.e, e);
			return direction<TO>(t[0], t[1], t[2]);
		}
		constexpr direction<SPACE>& transform(const tmat<SPACE, SPACE>& m)
		{
			vec_util::transform(e, m.e, e);
			return *this;
		}
		constexpr direction<SPACE>& cross(const direction<SPACE>& o)
		{
			vec_util::cross(e, e, o.e);
			normalize();
			return *this;
		}
		constexpr direction<SPACE> cross_copy(const direction<SPACE>& o) const
		{
			f32 t[3] = { 0.f };
			vec_util::cross(t, e, o.e);
			return direction<SPACE>(t[0], t[1], t[2]);
		}
		constexpr virtual void print() const override
		{
			printf("dir<%d>\t{ %06f\t%06f\t%06f }\n", SPACE, x, y, z);
		}
	private:
		constexpr void normalize()
		{
			const f32 length2 = vec_util::length2(e);
			// special "null direction" case
			if (length2 == 0.f)
				return;
			// if length is sufficiently different from 1, renormalize
			if (abs(length2 - 1.f) > c::EPSILON)
				vec_util::normalize(e, e);
		}
	};
}
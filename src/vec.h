#pragma once
#include "pch.h"
#include "vec_base.h"
#include "tmat.h"
#include "vec_util.h"

namespace hats
{
	template<space SPACE>
	struct direction;

	template<space SPACE>
	struct alignas(16) vec : public vec_base<SPACE>
	{
		using vec_base<SPACE>::x;
		using vec_base<SPACE>::y;
		using vec_base<SPACE>::z;
		using vec_base<SPACE>::w;
		using vec_base<SPACE>::e;
	public:
		constexpr vec() : vec_base<SPACE>(0.f, 0.f, 0.f, 0.f) {}
		template<typename X, typename Y, typename Z>
		constexpr vec(const X x, const Y y, const Z z) : vec_base<SPACE>(x, y, z, 0.f) {}
		constexpr vec(const vec<SPACE>& v) : vec_base<SPACE>(v.x, v.y, v.z, 0.f) {}
	public:
		constexpr vec<SPACE> operator+(const vec<SPACE>& o) const
		{
			return vec<SPACE>(x + o.x, y + o.y, z + o.z);
		}
		constexpr vec<SPACE>& operator+=(const vec<SPACE>& o)
		{
			vec_util::add(e, e, o.e);
			return *this;
		}
		constexpr vec<SPACE> operator-() const
		{
			return vec<SPACE>(-x, -y, -z);
		}
		constexpr vec<SPACE> operator-(const vec<SPACE>& o) const
		{
			return vec<SPACE>(x - o.x, y - o.y, z - o.z);
		}
		constexpr vec<SPACE>& operator-=(const vec<SPACE>& o)
		{
			vec_util::sub(e, e, o.e);
			return *this;
		}
		constexpr vec<SPACE> operator*(const f32 s) const
		{
			return vec<SPACE>(x * s, y * s, z * s);
		}
		constexpr vec<SPACE>& operator*=(const f32 s)
		{
			vec_util::scale(e, e, s);
			return *this;
		}
		constexpr vec<SPACE> operator/(const f32 s) const
		{
			return operator*(1.f / s);
		}
		constexpr vec<SPACE>& operator/=(const f32 s)
		{
			return operator*=(1.f / s);
		}
		constexpr vec<SPACE>& operator=(const vec<SPACE>& o)
		{
			vec_util::copy(e, o.e);
			return *this;
		}
		constexpr bool operator==(const vec<SPACE>& o) const
		{
			return vec_base<SPACE>::is_equal(o);
		}
		constexpr bool operator!=(const vec<SPACE>& o) const
		{
			return vec_base<SPACE>::is_not_equal(o);
		}
	public:
		constexpr direction<SPACE> normalize_copy() const;
		constexpr f32 dot(const vec<SPACE>& o) const
		{
			return vec_util::dot(e, o.e);
		}
		constexpr f32 length2() const
		{
			return vec_util::length2(e);
		}
		constexpr f32 length() const
		{
			return vec_util::length(e);
		}
		constexpr vec<SPACE>& set_length(const f32 l)
		{
			vec_util::scale(e, e, l / vec_util::length(e));
			return *this;
		}
		template<space TO>
		constexpr vec<TO> transform_copy(const tmat<SPACE, TO>& m) const
		{
			f32 t[4] = { 0.f };
			vec_util::transform(t, m.e, e);
			return vec<TO>(t[0], t[1], t[2]);
		}
		constexpr vec<SPACE>& cross(const vec<SPACE>& o)
		{
			vec_util::cross(e, e, o.e);
			return *this;
		}
		constexpr vec<SPACE> cross_copy(const vec<SPACE>& o) const
		{
			vec v(0, 0, 0);
			vec_util::cross(v.e, e, o.e);
			return v;
		}
		constexpr virtual void print() const override
		{
			printf("vec<%d>\t{ %06f\t%06f\t%06f }\n", SPACE, x, y, z);
		}
	};
	
	template<space SPACE>
	constexpr vec<SPACE> operator*(const f32 s, const vec<SPACE>& v)
	{
		return v * s;
	}
	template<space SPACE>
	constexpr vec<SPACE>& operator*=(const f32 s, vec<SPACE>& v)
	{
		return v *= s;
	}
	template<space SPACE>
	constexpr vec<SPACE> operator/(const f32 s, const vec<SPACE>& v)
	{
		return v / s;
	}
	template<space SPACE>
	constexpr vec<SPACE>& operator/=(const f32 s, vec<SPACE>& v)
	{
		return v /= s;
	}
}
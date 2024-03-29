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
		constexpr vec() :
			vec_base<SPACE>(0.f, 0.f, 0.f, 0.f)
		{}
		template<typename T>
		constexpr vec(const T t) :
			vec_base<SPACE>(t, t, t, 0.f)
		{}
		template<typename X, typename Y, typename Z>
		constexpr vec(const X x, const Y y, const Z z) :
			vec_base<SPACE>(x, y, z, 0.f)
		{}
		constexpr vec(vec<SPACE> const& v) :
			vec_base<SPACE>(v.x, v.y, v.z, 0.f)
		{}
		constexpr vec(direction<SPACE> const& d) :
			vec_base<SPACE>(d.x, d.y, d.z, 0.f)
		{}
	public:
		vec<SPACE> constexpr operator+(vec<SPACE> const& o) const
		{
			return vec<SPACE>(x + o.x, y + o.y, z + o.z);
		}
		vec<SPACE> constexpr& operator+=(vec<SPACE> const& o)
		{
			vec_util::add(e, e, o.e);
			return *this;
		}
		vec<SPACE> constexpr operator-() const
		{
			return vec<SPACE>(-x, -y, -z);
		}
		vec<SPACE> constexpr operator-(vec<SPACE> const& o) const
		{
			return vec<SPACE>(x - o.x, y - o.y, z - o.z);
		}
		vec<SPACE> constexpr& operator-=(vec<SPACE> const& o)
		{
			vec_util::sub(e, e, o.e);
			return *this;
		}
		template<typename X>
		vec<SPACE> constexpr operator*(const X s) const
		{
			return vec<SPACE>(x * s, y * s, z * s);
		}
		template<typename X>
		vec<SPACE> constexpr& operator*=(const X s)
		{
			vec_util::scale(e, e, s);
			return *this;
		}
		template<typename X>
		vec<SPACE> constexpr operator/(const X s) const
		{
			return operator*(1.f / s);
		}
		template<typename X>
		vec<SPACE> constexpr& operator/=(const X s)
		{
			return operator*=(1.f / s);
		}
		vec<SPACE> constexpr& operator=(vec<SPACE> const& o)
		{
			vec_util::copy(e, o.e);
			return *this;
		}
		bool constexpr operator==(vec<SPACE> const& o) const
		{
			return vec_base<SPACE>::is_equal(o);
		}
		bool constexpr operator!=(vec<SPACE> const& o) const
		{
			return vec_base<SPACE>::is_not_equal(o);
		}
	public:
		vec<SPACE> constexpr& normalize()
		{
			vec_util::normalize(e, e);
			return *this;
		}
		direction<SPACE> constexpr normalize_copy() const;
		constexpr f32 dot(vec<SPACE> const& o) const
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
		template<typename X>
		vec<SPACE> constexpr& set_length(const X l)
		{
			const f32 l2 = length2();
			if (l2 == 0.f)
				return *this;
			vec_util::scale(e, e, HATS_CAST(f32, l) / vec_util::length(e));
			return *this;
		}
		template<space TO>
		vec<TO> constexpr transform_copy(tmat<SPACE, TO> const& m) const
		{
			f32 t[4] = { 0.f };
			vec_util::transform(t, m.e, e);
			return vec<TO>(t[0], t[1], t[2]);
		}
		vec<SPACE> constexpr& cross(vec<SPACE> const& o)
		{
			vec_util::cross(e, e, o.e);
			return *this;
		}
		vec<SPACE> constexpr cross_copy(vec<SPACE> const& o) const
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

	template<space SPACE, typename X>
	vec<SPACE> constexpr operator*(const X s, vec<SPACE> const& v)
	{
		return v * s;
	}
	template<space SPACE, typename X>
	vec<SPACE> constexpr& operator*=(const X s, vec<SPACE>& v)
	{
		return v *= s;
	}
	template<space SPACE, typename X>
	vec<SPACE> constexpr operator/(const X s, vec<SPACE> const& v)
	{
		return v / s;
	}
	template<space SPACE, typename X>
	vec<SPACE> constexpr& operator/=(const X s, vec<SPACE>& v)
	{
		return v /= s;
	}
} // namespace hats
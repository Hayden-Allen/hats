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
		constexpr direction() {}
		template<typename X, typename Y, typename Z>
		constexpr direction(X const x, Y const y, Z const z) :
			vec_base<SPACE>(x, y, z, 0.f)
		{
			normalize();
		}
		explicit constexpr direction(vec_base<SPACE> const& v) :
			vec_base<SPACE>(v.x, v.y, v.z, 0.f)
		{
			normalize();
		}
	public:
		template<typename T>
		vec<SPACE> constexpr operator*(T const s) const;
		direction<SPACE> constexpr operator-() const
		{
			return direction<SPACE>(-x, -y, -z);
		}
		direction<SPACE> constexpr& operator=(direction<SPACE> const& o)
		{
			vec_util::copy(e, o.e);
			return *this;
		}
		bool constexpr operator==(direction<SPACE> const& o) const
		{
			return vec_base<SPACE>::is_equal(o);
		}
		bool constexpr operator!=(direction<SPACE> const& o) const
		{
			return vec_base<SPACE>::is_not_equal(o);
		}
	public:
		constexpr f32 dot(direction<SPACE> const& o) const
		{
			return vec_util::dot(e, o.e);
		}
		constexpr f32 angle_to(direction<SPACE> const& o) const
		{
			return std::acos(dot(o));
		}
		template<space TO>
		direction<TO> constexpr transform_copy(tmat<SPACE, TO> const& m) const
		{
			f32 t[4] = { 0.f };
			vec_util::transform(t, m.e, e);
			return direction<TO>(t[0], t[1], t[2]);
		}
		direction<SPACE> constexpr& transform(tmat<SPACE, SPACE> const& m)
		{
			vec_util::transform(e, m.e, e);
			return *this;
		}
		direction<SPACE> constexpr& cross(direction<SPACE> const& o)
		{
			vec_util::cross(e, e, o.e);
			normalize();
			return *this;
		}
		direction<SPACE> constexpr cross_copy(direction<SPACE> const& o) const
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
		void constexpr normalize()
		{
			const f32 length2 = vec_util::length2(e);
			// special "null direction" case
			if (length2 == 0.f)
				return;
			// if length is sufficiently different from 1, renormalize
			// if (abs(length2 - 1.f) > c::EPSILON)
			vec_util::normalize(e, e);
		}
	};

	template<space SPACE, typename T>
	vec<SPACE> constexpr operator*(T const s, direction<SPACE> const& dir)
	{
		const f32 fs = HATS_CAST(f32, s);
		return vec<SPACE>(dir.x * fs, dir.y * fs, dir.z * fs);
	}
} // namespace hats
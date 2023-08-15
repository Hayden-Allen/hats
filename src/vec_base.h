#pragma once
#include "pch.h"

namespace hats
{
	template<space SPACE>
	struct alignas(16) vec_base
	{
	public:
		union
		{
			f32 e[4] = { 0.f };
			struct
			{
				f32 x, y, z, w;
			};
		};
	public:
		constexpr vec_base() :
			x(0.f), y(0.f), z(0.f), w(0.f)
		{}
		template<typename X, typename Y, typename Z, typename W>
		vec_base(const X _x, const Y _y, const Z _z, const W _w) :
			x(HATS_CAST(f32, _x)),
			y(HATS_CAST(f32, _y)),
			z(HATS_CAST(f32, _z)),
			w(HATS_CAST(f32, _w))
		{}
		constexpr vec_base(const vec_base<SPACE>& o) :
			x(o.x), y(o.y), z(o.z), w(o.w)
		{}
	public:
		template<typename I>
		f32 operator[](const I i) const
		{
			HATS_ASSERT(i >= 0 && i < 4);
			return e[i];
		}
		constexpr virtual void print() const = 0;
	protected:
		constexpr bool is_equal(const vec_base<SPACE>& o) const
		{
			const f32 dx = abs(x - o.x);
			const f32 dy = abs(y - o.y);
			const f32 dz = abs(z - o.z);
			return dx <= c::EPSILON && dy <= c::EPSILON && dz <= c::EPSILON;
		}
		constexpr bool is_not_equal(const vec_base<SPACE>& o) const
		{
			const f32 dx = abs(x - o.x);
			const f32 dy = abs(y - o.y);
			const f32 dz = abs(z - o.z);
			return dx > c::EPSILON || dy > c::EPSILON || dz > c::EPSILON;
		}
	};
} // namespace hats
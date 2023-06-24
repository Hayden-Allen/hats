#pragma once
#include "pch.h"

namespace hats
{
	template<space SPACE>
	struct alignas(16) vec4
	{
	public:
		union
		{
			struct
			{
				f32 x, y, z, w;
			};
			f32 e[4];
		};
	public:
		vec4() :
			x(0.f), y(0.f), z(0.f), w(0.f)
		{}
		constexpr vec4(const f32 _x, const f32 _y, const f32 _z, const f32 _w) :
			x(_x), y(_y), z(_z), w(_w)
		{}
		vec4(const vec4<SPACE>& o) :
			x(o.x), y(o.y), z(o.z), w(o.w)
		{}
	public:
		f32 operator[](const int i) const
		{
			HATS_ASSERT(i >= 0 && i < 4);
			return e[i];
		}
		virtual void print() const = 0;
	};
}
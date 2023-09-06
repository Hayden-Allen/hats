#include "pch.h"
#include "point.h"
#include "vec.h"
#include "direction.h"

namespace hats
{
	template<space SPACE>
	constexpr point<SPACE> point<SPACE>::operator+(const vec<SPACE>& v) const
	{
		return point<SPACE>(x + v.x, y + v.y, z + v.z);
	}
	template<space SPACE>
	constexpr point<SPACE> point<SPACE>::operator+(const direction<SPACE>& d) const
	{
		return point<SPACE>(x + d.x, y + d.y, z + d.z);
	}
	template<space SPACE>
	constexpr point<SPACE>& point<SPACE>::operator+=(const vec<SPACE>& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	template<space SPACE>
	constexpr point<SPACE>& point<SPACE>::operator+=(const direction<SPACE>& d)
	{
		x += d.x;
		y += d.y;
		z += d.z;
		return *this;
	}
	template<space SPACE>
	constexpr vec<SPACE> point<SPACE>::operator-(const point<SPACE>& o) const
	{
		return vec<SPACE>(x - o.x, y - o.y, z - o.z);
	}
	template<space SPACE>
	constexpr point<SPACE> point<SPACE>::operator-(const vec<SPACE>& v) const
	{
		return point<SPACE>(x - v.x, y - v.y, z - v.z);
	}
	template<space SPACE>
	constexpr point<SPACE> point<SPACE>::operator-(const direction<SPACE>& d) const
	{
		return point<SPACE>(x - d.x, y - d.y, z - d.z);
	}
	template<space SPACE>
	constexpr point<SPACE>& point<SPACE>::operator-=(const vec<SPACE>& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	template<space SPACE>
	constexpr point<SPACE>& point<SPACE>::operator-=(const direction<SPACE>& d)
	{
		x -= d.x;
		y -= d.y;
		z -= d.z;
		return *this;
	}
} // namespace hats
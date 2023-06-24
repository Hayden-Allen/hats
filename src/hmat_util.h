#pragma once
#include "pch.h"
#include "hmat.h"
#include "direction.h"
#include "point.h"

namespace hats::hmat_util
{
	template<space FROM, space TO = FROM>
	static hmat<FROM, TO> scale(const f32 x, const f32 y, const f32 z)
	{
		return hmat<FROM, TO>(
			1.f / x, 0, 0, 0,
			0, 1.f / y, 0, 0,
			0, 0, 1.f / z, 0
		);
	}
	template<space FROM, space TO = FROM>
	static hmat<FROM, TO> translation(const f32 x, const f32 y, const f32 z)
	{
		return hmat<FROM, TO>(
			1, 0, 0, -x,
			0, 1, 0, -y,
			0, 0, 1, -z
		);
	}
	template<space FROM, space TO = FROM>
	static hmat<FROM, TO> translation(const point<FROM>& t)
	{
		return hmat<FROM, TO>(
			1, 0, 0, -t[0],
			0, 1, 0, -t[1],
			0, 0, 1, -t[2]
		);
	}
	template<space FROM, space TO = FROM>
	static hmat<FROM, TO> rotation_x(const f32 theta)
	{
		const f32 c = cosf(theta), s = sinf(theta);
		return hmat<FROM, TO>(
			1, 0, 0, 0,
			0, c, -s, 0,
			0, s, c, 0
		);
	}
	template<space FROM, space TO = FROM>
	static hmat<FROM, TO> rotation_y(const f32 theta)
	{
		const f32 c = cosf(theta), s = sinf(theta);
		return hmat<FROM, TO>(
			c, 0, s, 0,
			0, 1, 0, 0,
			-s, 0, c, 0
		);
	}
	template<space FROM, space TO = FROM>
	static hmat<FROM, TO> rotation_z(const f32 theta)
	{
		const f32 c = cosf(theta), s = sinf(theta);
		return hmat<FROM, TO>(
			c, -s, 0, 0,
			s, c, 0, 0,
			0, 0, 1, 0
		);
	}
	template<space FROM, space TO = FROM>
	static hmat<FROM, TO> rotation_xyz(const f32 x, const f32 y, const f32 z)
	{
		const f32 cx = cosf(x), sx = sinf(x);
		const f32 cy = cosf(y), sy = sinf(y);
		const f32 cz = cosf(z), sz = sinf(z);
		return hmat<FROM, TO>(
			cy * cz, -cy * sz, sy, 0,
			sx * sy * cz + cx * sz, -sx * sy * sz + cx * cz, -sx * cy, 0,
			-cx * sy * cz + sx * sz, cx * sy * sz + sx * cz, cx * cy, 0
		);
	}
	template<space FROM, space TO = FROM>
	static hmat<FROM, TO> rotation_xzy(const f32 x, const f32 y, const f32 z)
	{
		const f32 cx = cosf(x), sx = sinf(x);
		const f32 cy = cosf(y), sy = sinf(y);
		const f32 cz = cosf(z), sz = sinf(z);
		return hmat<FROM, TO>(
			cy * cz, -sz, sy * cz, 0,
			cx * cy * sz - sx * sy, cx * cz, cx * sy * sz + sx * cy, 0,
			sx * cy * sz - cx * sy, sx * cz, sx * sy * sz + cx * cy, 0
		);
	}
	template<space FROM, space TO = FROM>
	static hmat<FROM, TO> rotation_yxz(const f32 x, const f32 y, const f32 z)
	{
		const f32 cx = cosf(x), sx = sinf(x);
		const f32 cy = cosf(y), sy = sinf(y);
		const f32 cz = cosf(z), sz = sinf(z);
		return hmat<FROM, TO>(
			cy * cz + sx * sy * sz, -cy * sz + sx * sy * cz, cx * sy, 0,
			cx * sz, cx * cz, -sx, 0
			-sy * cz + sx * cy * sz, sy * sz + sx * cy * cz, cx * cy, 0
		);
	}
	template<space FROM, space TO = FROM>
	static hmat<FROM, TO> rotation_yzx(const f32 x, const f32 y, const f32 z)
	{
		const f32 cx = cosf(x), sx = sinf(x);
		const f32 cy = cosf(y), sy = sinf(y);
		const f32 cz = cosf(z), sz = sinf(z);
		return hmat<FROM, TO>(
			cy * cz, -cx * cy * sz + sx * sy, sx * cy * sz + cx * sy, 0,
			sz, cx * cz, -sx * cz, 0,
			-sy * cz, cx * sy * sz + sx * cy, -sx * sy * sz + cx * cy, 0
		);
	}
	template<space FROM, space TO = FROM>
	static hmat<FROM, TO> rotation_zxy(const f32 x, const f32 y, const f32 z)
	{
		const f32 cx = cosf(x), sx = sinf(x);
		const f32 cy = cosf(y), sy = sinf(y);
		const f32 cz = cosf(z), sz = sinf(z);
		return hmat<FROM, TO>(
			cy * cz - sx * sy * sz, -cx * sz, sy * cz + sx * cy * sz, 0,
			cy * sz + sx * sy * cz, cx * cz, sy * sz - sx * cy * cz, 0,
			-cx * sy, sx, cx * cy, 0
		);
	}
	template<space FROM, space TO = FROM>
	static hmat<FROM, TO> rotation_zyx(const f32 x, const f32 y, const f32 z)
	{
		const f32 cx = cosf(x), sx = sinf(x);
		const f32 cy = cosf(y), sy = sinf(y);
		const f32 cz = cosf(z), sz = sinf(z);
		return hmat<FROM, TO>(
			cy * cz, -cx * sz + sx * sy * cz, sx * sz + cx * sy * cz, 0,
			cy * sz, cx * cz + sx * sy * sz, -sx * cz + cx * sy * sz, 0,
			-sy, sx * cy, cx * cy, 0
		);
	}
	template<space FROM, space TO = FROM>
	static hmat<FROM, TO> rotation(const direction<FROM>& a, const f32 theta)
	{
		const f32 c = cosf(theta), s = sinf(theta);
		return hmat<FROM, TO>(
			c + (1 - c) * a[0] * a[0],
			(1 - c) * a[0] * a[1] - s * a[2],
			(1 - c) * a[0] * a[2] + s * a[1],
			0,
			(1 - c) * a[0] * a[1] + s * a[2],
			c + (1 - c) * a[1] * a[1],
			(1 - c) * a[1] * a[2] - s * a[0],
			0,
			(1 - c) * a[0] * a[2] - s * a[1],
			(1 - c) * a[1] * a[2] + s * a[0],
			c + (1 - c) * a[2] * a[2],
			0
		);
	}
	template<space FROM, space TO = FROM>
	static hmat<FROM, TO> rotation_about_point(const direction<FROM>& a, const f32 theta, const point<FROM>& p)
	{
		const hmat<FROM, TO>& t = translation<FROM, TO>(p);
		const hmat<FROM, FROM> r = rotation<FROM, FROM>(a, theta);
		const hmat<FROM, FROM> mt = translation<FROM, FROM>(-p);
		return t * r * mt;
	}
}
#pragma once
#include "pch.h"
#include "tmat.h"
#include "direction.h"
#include "point.h"

namespace hats::tmat_util
{
	template<space FROM, space TO>
	static tmat<FROM, TO> look_at(const point<FROM>& eye, const direction<FROM>& look, const direction<FROM>& up)
	{
		const direction<FROM>& i = look.cross_copy(up);
		const direction<FROM>& j = i.cross_copy(look);
		const direction<FROM>& k = -look;
		return tmat<FROM, TO>(i, j, k, eye);
	}
	template<space FROM, space TO = FROM>
	static tmat<FROM, TO> scale(const f32 x, const f32 y, const f32 z)
	{
		return tmat<FROM, TO>(
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0
		);
	}
	template<space FROM, space TO = FROM>
	static tmat<FROM, TO> translation(const f32 x, const f32 y, const f32 z)
	{
		return tmat<FROM, TO>(
			1, 0, 0, x,
			0, 1, 0, y,
			0, 0, 1, z
		);
	}
	// implement separately for point<FROM> and point<TO>
	/*template<space FROM, space TO = FROM>
	static tmat<FROM, TO> translation(const point<FROM>& t)
	{
		return tmat<FROM, TO>(
			1, 0, 0, -t[0],
			0, 1, 0, -t[1],
			0, 0, 1, -t[2]
		);
	}*/
	template<space FROM, space TO = FROM>
	static tmat<FROM, TO> rotation_x(f32 theta)
	{
		theta = clean_angle(theta);
		const f32 c = cosf(theta), s = sinf(theta);
		return tmat<FROM, TO>(
			1, 0, 0, 0,
			0, c, -s, 0,
			0, s, c, 0
		);
	}
	template<space FROM, space TO = FROM>
	static tmat<FROM, TO> rotation_y(f32 theta)
	{
		theta = clean_angle(theta);
		const f32 c = cosf(theta), s = sinf(theta);
		return tmat<FROM, TO>(
			c, 0, s, 0,
			0, 1, 0, 0,
			-s, 0, c, 0
		);
	}
	template<space FROM, space TO = FROM>
	static tmat<FROM, TO> rotation_z(f32 theta)
	{
		theta = clean_angle(theta);
		const f32 c = cosf(theta), s = sinf(theta);
		return tmat<FROM, TO>(
			c, -s, 0, 0,
			s, c, 0, 0,
			0, 0, 1, 0
		);
	}
	template<space FROM, space TO = FROM>
	static tmat<FROM, TO> rotation_xyz(f32 x, f32 y, f32 z)
	{
		x = clean_angle(x);
		y = clean_angle(y);
		z = clean_angle(z);
		const f32 cx = cosf(x), sx = sinf(x);
		const f32 cy = cosf(y), sy = sinf(y);
		const f32 cz = cosf(z), sz = sinf(z);
		return tmat<FROM, TO>(
			cy * cz, -cy * sz, sy, 0,
			sx * sy * cz + cx * sz, -sx * sy * sz + cx * cz, -sx * cy, 0,
			-cx * sy * cz + sx * sz, cx * sy * sz + sx * cz, cx * cy, 0
		);
	}
	template<space FROM, space TO = FROM>
	static tmat<FROM, TO> rotation_xzy(f32 x, f32 y, f32 z)
	{
		x = clean_angle(x);
		y = clean_angle(y);
		z = clean_angle(z);
		const f32 cx = cosf(x), sx = sinf(x);
		const f32 cy = cosf(y), sy = sinf(y);
		const f32 cz = cosf(z), sz = sinf(z);
		return tmat<FROM, TO>(
			cy * cz, -sz, sy * cz, 0,
			cx * cy * sz - sx * sy, cx * cz, cx * sy * sz + sx * cy, 0,
			sx * cy * sz - cx * sy, sx * cz, sx * sy * sz + cx * cy, 0
		);
	}
	template<space FROM, space TO = FROM>
	static tmat<FROM, TO> rotation_yxz(f32 x, f32 y, f32 z)
	{
		x = clean_angle(x);
		y = clean_angle(y);
		z = clean_angle(z);
		const f32 cx = cosf(x), sx = sinf(x);
		const f32 cy = cosf(y), sy = sinf(y);
		const f32 cz = cosf(z), sz = sinf(z);
		return tmat<FROM, TO>(
			cy * cz + sx * sy * sz, -cy * sz + sx * sy * cz, cx * sy, 0,
			cx * sz, cx * cz, -sx, 0
			-sy * cz + sx * cy * sz, sy * sz + sx * cy * cz, cx * cy, 0
		);
	}
	template<space FROM, space TO = FROM>
	static tmat<FROM, TO> rotation_yzx(f32 x, f32 y, f32 z)
	{
		x = clean_angle(x);
		y = clean_angle(y);
		z = clean_angle(z);
		const f32 cx = cosf(x), sx = sinf(x);
		const f32 cy = cosf(y), sy = sinf(y);
		const f32 cz = cosf(z), sz = sinf(z);
		return tmat<FROM, TO>(
			cy * cz, -cx * cy * sz + sx * sy, sx * cy * sz + cx * sy, 0,
			sz, cx * cz, -sx * cz, 0,
			-sy * cz, cx * sy * sz + sx * cy, -sx * sy * sz + cx * cy, 0
		);
	}
	template<space FROM, space TO = FROM>
	static tmat<FROM, TO> rotation_zxy(f32 x, f32 y, f32 z)
	{
		x = clean_angle(x);
		y = clean_angle(y);
		z = clean_angle(z);
		const f32 cx = cosf(x), sx = sinf(x);
		const f32 cy = cosf(y), sy = sinf(y);
		const f32 cz = cosf(z), sz = sinf(z);
		return tmat<FROM, TO>(
			cy * cz - sx * sy * sz, -cx * sz, sy * cz + sx * cy * sz, 0,
			cy * sz + sx * sy * cz, cx * cz, sy * sz - sx * cy * cz, 0,
			-cx * sy, sx, cx * cy, 0
		);
	}
	template<space FROM, space TO = FROM>
	static tmat<FROM, TO> rotation_zyx(f32 x, f32 y, f32 z)
	{
		x = clean_angle(x);
		y = clean_angle(y);
		z = clean_angle(z);
		const f32 cx = cosf(x), sx = sinf(x);
		const f32 cy = cosf(y), sy = sinf(y);
		const f32 cz = cosf(z), sz = sinf(z);
		return tmat<FROM, TO>(
			cy * cz, -cx * sz + sx * sy * cz, sx * sz + cx * sy * cz, 0,
			cy * sz, cx * cz + sx * sy * sz, -sx * cz + cx * sy * sz, 0,
			-sy, sx * cy, cx * cy, 0
		);
	}
	template<space FROM, space TO = FROM>
	static tmat<FROM, TO> rotation(const direction<FROM>& a, f32 theta)
	{
		theta = clean_angle(theta);
		const f32 c = cosf(theta), s = sinf(theta);
		return tmat<FROM, TO>(
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
	static tmat<FROM, TO> rotation_x_about_point(f32 theta, const point<FROM>& p)
	{
		theta = clean_angle(theta);
		const tmat<FROM, TO>& t = translation<FROM, TO>(p);
		const tmat<FROM, FROM> r = rotation_x<FROM, FROM>(theta);
		const tmat<FROM, FROM> mt = translation<FROM, FROM>(-p);
		return t * r * mt;
	}
	template<space FROM, space TO = FROM>
	static tmat<FROM, TO> rotation_y_about_point(f32 theta, const point<FROM>& p)
	{
		theta = clean_angle(theta);
		const tmat<FROM, TO>& t = translation<FROM, TO>(p);
		const tmat<FROM, FROM> r = rotation_y<FROM, FROM>(theta);
		const tmat<FROM, FROM> mt = translation<FROM, FROM>(-p);
		return t * r * mt;
	}
	template<space FROM, space TO = FROM>
	static tmat<FROM, TO> rotation_z_about_point(f32 theta, const point<FROM>& p)
	{
		theta = clean_angle(theta);
		const tmat<FROM, TO>& t = translation<FROM, TO>(p);
		const tmat<FROM, FROM> r = rotation_z<FROM, FROM>(theta);
		const tmat<FROM, FROM> mt = translation<FROM, FROM>(-p);
		return t * r * mt;
	}
	template<space FROM, space TO = FROM>
	static tmat<FROM, TO> rotation_about_point(const direction<FROM>& a, f32 theta, const point<FROM>& p)
	{
		theta = clean_angle(theta);
		const tmat<FROM, TO>& t = translation<FROM, TO>(p);
		const tmat<FROM, FROM> r = rotation<FROM, FROM>(a, theta);
		const tmat<FROM, FROM> mt = translation<FROM, FROM>(-p);
		return t * r * mt;
	}
	template<space FROM, space TO = FROM>
	static tmat<FROM, TO> rotation_xyz_about_point(f32 x, f32 y, f32 z, const point<FROM>& p)
	{
		x = clean_angle(x);
		y = clean_angle(y);
		z = clean_angle(z);
		const tmat<FROM, TO>& t = translation<FROM, TO>(p);
		const tmat<FROM, FROM> r = rotation_xyz<FROM, FROM>(x, y, z);
		const tmat<FROM, FROM> mt = translation<FROM, FROM>(-p);
		return t * r * mt;
	}
	template<space FROM, space TO = FROM>
	static tmat<FROM, TO> rotation_xzy_about_point(f32 x, f32 y, f32 z, const point<FROM>& p)
	{
		x = clean_angle(x);
		y = clean_angle(y);
		z = clean_angle(z);
		const tmat<FROM, TO>& t = translation<FROM, TO>(p);
		const tmat<FROM, FROM> r = rotation_xzy<FROM, FROM>(x, y, z);
		const tmat<FROM, FROM> mt = translation<FROM, FROM>(-p);
		return t * r * mt;
	}
	template<space FROM, space TO = FROM>
	static tmat<FROM, TO> rotation_yxz_about_point(f32 x, f32 y, f32 z, const point<FROM>& p)
	{
		x = clean_angle(x);
		y = clean_angle(y);
		z = clean_angle(z);
		const tmat<FROM, TO>& t = translation<FROM, TO>(p);
		const tmat<FROM, FROM> r = rotation_yxz<FROM, FROM>(x, y, z);
		const tmat<FROM, FROM> mt = translation<FROM, FROM>(-p);
		return t * r * mt;
	}
	template<space FROM, space TO = FROM>
	static tmat<FROM, TO> rotation_yzx_about_point(f32 x, f32 y, f32 z, const point<FROM>& p)
	{
		x = clean_angle(x);
		y = clean_angle(y);
		z = clean_angle(z);
		const tmat<FROM, TO>& t = translation<FROM, TO>(p);
		const tmat<FROM, FROM> r = rotation_yzx<FROM, FROM>(x, y, z);
		const tmat<FROM, FROM> mt = translation<FROM, FROM>(-p);
		return t * r * mt;
	}
	template<space FROM, space TO = FROM>
	static tmat<FROM, TO> rotation_zxy_about_point(f32 x, f32 y, f32 z, const point<FROM>& p)
	{
		x = clean_angle(x);
		y = clean_angle(y);
		z = clean_angle(z);
		const tmat<FROM, TO>& t = translation<FROM, TO>(p);
		const tmat<FROM, FROM> r = rotation_zxy<FROM, FROM>(x, y, z);
		const tmat<FROM, FROM> mt = translation<FROM, FROM>(-p);
		return t * r * mt;
	}
	template<space FROM, space TO = FROM>
	static tmat<FROM, TO> rotation_zyx_about_point(f32 x, f32 y, f32 z, const point<FROM>& p)
	{
		x = clean_angle(x);
		y = clean_angle(y);
		z = clean_angle(z);
		const tmat<FROM, TO>& t = translation<FROM, TO>(p);
		const tmat<FROM, FROM> r = rotation_zyx<FROM, FROM>(x, y, z);
		const tmat<FROM, FROM> mt = translation<FROM, FROM>(-p);
		return t * r * mt;
	}
}
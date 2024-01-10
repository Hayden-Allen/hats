#pragma once
#include "pch.h"
#include "tmat.h"
#include "direction.h"
#include "point.h"

namespace hats::tmat_util
{
	template<space FROM, space TO>
	static tmat<FROM, TO> zeros()
	{
		f32 const zeros[16] = { 0 };
		return tmat<space::OBJECT, space::WORLD>(zeros);
	}
	template<space FROM, space TO>
	static tmat<FROM, TO> look_at(point<FROM> const& eye, direction<FROM> const& look, direction<FROM> const& up)
	{
		direction<FROM> const& i = look.cross_copy(up);
		direction<FROM> const& j = i.cross_copy(look);
		direction<FROM> const& k = -look;
		return tmat<FROM, TO>(i, j, k, eye);
	}
	template<space FROM, space TO = FROM, typename X, typename Y, typename Z>
	static tmat<FROM, TO> scale(const X x, const Y y, const Z z)
	{
		return tmat<FROM, TO>(
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0);
	}
	template<space FROM, space TO = FROM, typename X, typename Y, typename Z>
	static tmat<FROM, TO> translation(const X x, const Y y, const Z z)
	{
		return tmat<FROM, TO>(
			1, 0, 0, x,
			0, 1, 0, y,
			0, 0, 1, z);
	}
	template<space FROM, space TO = FROM>
	static tmat<FROM, TO> translation(point<FROM> const& t)
	{
		return tmat<FROM, TO>(
			1, 0, 0, -t[0],
			0, 1, 0, -t[1],
			0, 0, 1, -t[2]);
	}
	template<space FROM, space TO = FROM>
	static tmat<FROM, TO> translation(point<TO> const& t)
	{
		return tmat<FROM, TO>(
			1, 0, 0, t[0],
			0, 1, 0, t[1],
			0, 0, 1, t[2]);
	}
	template<space FROM, space TO = FROM, typename X>
	static tmat<FROM, TO> rotation_x(const X theta)
	{
		const f32 ft = clean_angle(HATS_CAST(f32, theta));
		const f32 c = cosf(ft), s = sinf(ft);
		return tmat<FROM, TO>(
			1, 0, 0, 0,
			0, c, -s, 0,
			0, s, c, 0);
	}
	template<space FROM, space TO = FROM, typename X>
	static tmat<FROM, TO> rotation_y(const X theta)
	{
		const f32 ft = clean_angle(HATS_CAST(f32, theta));
		const f32 c = cosf(ft), s = sinf(ft);
		return tmat<FROM, TO>(
			c, 0, s, 0,
			0, 1, 0, 0,
			-s, 0, c, 0);
	}
	template<space FROM, space TO = FROM, typename X>
	static tmat<FROM, TO> rotation_z(const X theta)
	{
		const f32 ft = clean_angle(HATS_CAST(f32, theta));
		const f32 c = cosf(ft), s = sinf(ft);
		return tmat<FROM, TO>(
			c, -s, 0, 0,
			s, c, 0, 0,
			0, 0, 1, 0);
	}
	template<space FROM, space TO = FROM, typename X, typename Y, typename Z>
	static tmat<FROM, TO> rotation_xyz(const X x, const Y y, const Z z)
	{
		const f32 fx = clean_angle(HATS_CAST(f32, x));
		const f32 fy = clean_angle(HATS_CAST(f32, y));
		const f32 fz = clean_angle(HATS_CAST(f32, z));
		const f32 cx = cosf(fx), sx = sinf(fx);
		const f32 cy = cosf(fy), sy = sinf(fy);
		const f32 cz = cosf(fz), sz = sinf(fz);
		return tmat<FROM, TO>(
			cy * cz, -cy * sz, sy, 0,
			sx * sy * cz + cx * sz, -sx * sy * sz + cx * cz, -sx * cy, 0,
			-cx * sy * cz + sx * sz, cx * sy * sz + sx * cz, cx * cy, 0);
	}
	template<space FROM, space TO = FROM, typename X, typename Y, typename Z>
	static tmat<FROM, TO> rotation_xzy(const X x, const Y y, const Z z)
	{
		const f32 fx = clean_angle(HATS_CAST(f32, x));
		const f32 fy = clean_angle(HATS_CAST(f32, y));
		const f32 fz = clean_angle(HATS_CAST(f32, z));
		const f32 cx = cosf(fx), sx = sinf(fx);
		const f32 cy = cosf(fy), sy = sinf(fy);
		const f32 cz = cosf(fz), sz = sinf(fz);
		return tmat<FROM, TO>(
			cy * cz, -sz, sy * cz, 0,
			cx * cy * sz - sx * sy, cx * cz, cx * sy * sz + sx * cy, 0,
			sx * cy * sz - cx * sy, sx * cz, sx * sy * sz + cx * cy, 0);
	}
	template<space FROM, space TO = FROM, typename X, typename Y, typename Z>
	static tmat<FROM, TO> rotation_yxz(const X x, const Y y, const Z z)
	{
		const f32 fx = clean_angle(HATS_CAST(f32, x));
		const f32 fy = clean_angle(HATS_CAST(f32, y));
		const f32 fz = clean_angle(HATS_CAST(f32, z));
		const f32 cx = cosf(fx), sx = sinf(fx);
		const f32 cy = cosf(fy), sy = sinf(fy);
		const f32 cz = cosf(fz), sz = sinf(fz);
		return tmat<FROM, TO>(
			cy * cz + sx * sy * sz, -cy * sz + sx * sy * cz, cx * sy, 0,
			cx * sz, cx * cz, -sx, 0,
			-sy * cz + sx * cy * sz, sy * sz + sx * cy * cz, cx * cy, 0);
	}
	template<space FROM, space TO = FROM, typename X, typename Y, typename Z>
	static tmat<FROM, TO> rotation_yzx(const X x, const Y y, const Z z)
	{
		const f32 fx = clean_angle(HATS_CAST(f32, x));
		const f32 fy = clean_angle(HATS_CAST(f32, y));
		const f32 fz = clean_angle(HATS_CAST(f32, z));
		const f32 cx = cosf(fx), sx = sinf(fx);
		const f32 cy = cosf(fy), sy = sinf(fy);
		const f32 cz = cosf(fz), sz = sinf(fz);
		return tmat<FROM, TO>(
			cy * cz, -cx * cy * sz + sx * sy, sx * cy * sz + cx * sy, 0,
			sz, cx * cz, -sx * cz, 0,
			-sy * cz, cx * sy * sz + sx * cy, -sx * sy * sz + cx * cy, 0);
	}
	template<space FROM, space TO = FROM, typename X, typename Y, typename Z>
	static tmat<FROM, TO> rotation_zxy(const X x, const Y y, const Z z)
	{
		const f32 fx = clean_angle(HATS_CAST(f32, x));
		const f32 fy = clean_angle(HATS_CAST(f32, y));
		const f32 fz = clean_angle(HATS_CAST(f32, z));
		const f32 cx = cosf(fx), sx = sinf(fx);
		const f32 cy = cosf(fy), sy = sinf(fy);
		const f32 cz = cosf(fz), sz = sinf(fz);
		return tmat<FROM, TO>(
			cy * cz - sx * sy * sz, -cx * sz, sy * cz + sx * cy * sz, 0,
			cy * sz + sx * sy * cz, cx * cz, sy * sz - sx * cy * cz, 0,
			-cx * sy, sx, cx * cy, 0);
	}
	template<space FROM, space TO = FROM, typename X, typename Y, typename Z>
	static tmat<FROM, TO> rotation_zyx(const X x, const Y y, const Z z)
	{
		const f32 fx = clean_angle(HATS_CAST(f32, x));
		const f32 fy = clean_angle(HATS_CAST(f32, y));
		const f32 fz = clean_angle(HATS_CAST(f32, z));
		const f32 cx = cosf(fx), sx = sinf(fx);
		const f32 cy = cosf(fy), sy = sinf(fy);
		const f32 cz = cosf(fz), sz = sinf(fz);
		return tmat<FROM, TO>(
			cy * cz, -cx * sz + sx * sy * cz, sx * sz + cx * sy * cz, 0,
			cy * sz, cx * cz + sx * sy * sz, -sx * cz + cx * sy * sz, 0,
			-sy, sx * cy, cx * cy, 0);
	}
	template<space FROM, space TO = FROM, typename X>
	static tmat<FROM, TO> rotation(direction<FROM> const& a, const X theta)
	{
		const f32 ft = clean_angle(HATS_CAST(f32, theta));
		const f32 c = cosf(ft), s = sinf(ft);
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
			0);
	}
	template<space FROM, space TO = FROM, typename X>
	static tmat<FROM, TO> rotation_x_about_point(const X theta, point<FROM> const& p)
	{
		const f32 ft = clean_angle(HATS_CAST(f32, theta));
		tmat<FROM, TO> const& t = translation<FROM, TO>(p);
		tmat<FROM, FROM> const r = rotation_x<FROM, FROM>(ft);
		tmat<FROM, FROM> const mt = translation<FROM, FROM>(-p);
		return t * r * mt;
	}
	template<space FROM, space TO = FROM, typename X>
	static tmat<FROM, TO> rotation_y_about_point(const X theta, point<FROM> const& p)
	{
		const f32 ft = clean_angle(HATS_CAST(f32, theta));
		tmat<FROM, TO> const& t = translation<FROM, TO>(p);
		tmat<FROM, FROM> const r = rotation_y<FROM, FROM>(ft);
		tmat<FROM, FROM> const mt = translation<FROM, FROM>(-p);
		return t * r * mt;
	}
	template<space FROM, space TO = FROM, typename X>
	static tmat<FROM, TO> rotation_z_about_point(const X theta, point<FROM> const& p)
	{
		const f32 ft = clean_angle(HATS_CAST(f32, theta));
		tmat<FROM, TO> const& t = translation<FROM, TO>(p);
		tmat<FROM, FROM> const r = rotation_z<FROM, FROM>(ft);
		tmat<FROM, FROM> const mt = translation<FROM, FROM>(-p);
		return t * r * mt;
	}
	template<space FROM, space TO = FROM, typename X>
	static tmat<FROM, TO> rotation_about_point(direction<FROM> const& a, const X theta, point<FROM> const& p)
	{
		const f32 ft = clean_angle(HATS_CAST(f32, theta));
		tmat<FROM, TO> const& t = translation<FROM, TO>(p);
		tmat<FROM, FROM> const r = rotation<FROM, FROM>(a, ft);
		tmat<FROM, FROM> const mt = translation<FROM, FROM>(-p);
		return t * r * mt;
	}
	template<space FROM, space TO = FROM, typename X, typename Y, typename Z>
	static tmat<FROM, TO> rotation_xyz_about_point(const X x, const Y y, const Z z, point<FROM> const& p)
	{
		const f32 fx = clean_angle(HATS_CAST(f32, x));
		const f32 fy = clean_angle(HATS_CAST(f32, y));
		const f32 fz = clean_angle(HATS_CAST(f32, z));
		tmat<FROM, TO> const& t = translation<FROM, TO>(p);
		tmat<FROM, FROM> const r = rotation_xyz<FROM, FROM>(fx, fy, fz);
		tmat<FROM, FROM> const mt = translation<FROM, FROM>(-p);
		return t * r * mt;
	}
	template<space FROM, space TO = FROM, typename X, typename Y, typename Z>
	static tmat<FROM, TO> rotation_xzy_about_point(const X x, const Y y, const Z z, point<FROM> const& p)
	{
		const f32 fx = clean_angle(HATS_CAST(f32, x));
		const f32 fy = clean_angle(HATS_CAST(f32, y));
		const f32 fz = clean_angle(HATS_CAST(f32, z));
		tmat<FROM, TO> const& t = translation<FROM, TO>(p);
		tmat<FROM, FROM> const r = rotation_xzy<FROM, FROM>(fx, fy, fz);
		tmat<FROM, FROM> const mt = translation<FROM, FROM>(-p);
		return t * r * mt;
	}
	template<space FROM, space TO = FROM, typename X, typename Y, typename Z>
	static tmat<FROM, TO> rotation_yxz_about_point(const X x, const Y y, const Z z, point<FROM> const& p)
	{
		const f32 fx = clean_angle(HATS_CAST(f32, x));
		const f32 fy = clean_angle(HATS_CAST(f32, y));
		const f32 fz = clean_angle(HATS_CAST(f32, z));
		tmat<FROM, TO> const& t = translation<FROM, TO>(p);
		tmat<FROM, FROM> const r = rotation_yxz<FROM, FROM>(fx, fy, fz);
		tmat<FROM, FROM> const mt = translation<FROM, FROM>(-p);
		return t * r * mt;
	}
	template<space FROM, space TO = FROM, typename X, typename Y, typename Z>
	static tmat<FROM, TO> rotation_yzx_about_point(const X x, const Y y, const Z z, point<FROM> const& p)
	{
		const f32 fx = clean_angle(HATS_CAST(f32, x));
		const f32 fy = clean_angle(HATS_CAST(f32, y));
		const f32 fz = clean_angle(HATS_CAST(f32, z));
		tmat<FROM, TO> const& t = translation<FROM, TO>(p);
		tmat<FROM, FROM> const r = rotation_yzx<FROM, FROM>(fx, fy, fz);
		tmat<FROM, FROM> const mt = translation<FROM, FROM>(-p);
		return t * r * mt;
	}
	template<space FROM, space TO = FROM, typename X, typename Y, typename Z>
	static tmat<FROM, TO> rotation_zxy_about_point(const X x, const Y y, const Z z, point<FROM> const& p)
	{
		const f32 fx = clean_angle(HATS_CAST(f32, x));
		const f32 fy = clean_angle(HATS_CAST(f32, y));
		const f32 fz = clean_angle(HATS_CAST(f32, z));
		tmat<FROM, TO> const& t = translation<FROM, TO>(p);
		tmat<FROM, FROM> const r = rotation_zxy<FROM, FROM>(fx, fy, fz);
		tmat<FROM, FROM> const mt = translation<FROM, FROM>(-p);
		return t * r * mt;
	}
	template<space FROM, space TO = FROM, typename X, typename Y, typename Z>
	static tmat<FROM, TO> rotation_zyx_about_point(const X x, const Y y, const Z z, point<FROM> const& p)
	{
		const f32 fx = clean_angle(HATS_CAST(f32, x));
		const f32 fy = clean_angle(HATS_CAST(f32, y));
		const f32 fz = clean_angle(HATS_CAST(f32, z));
		tmat<FROM, TO> const& t = translation<FROM, TO>(p);
		tmat<FROM, FROM> const r = rotation_zyx<FROM, FROM>(fx, fy, fz);
		tmat<FROM, FROM> const mt = translation<FROM, FROM>(-p);
		return t * r * mt;
	}
} // namespace hats::tmat_util
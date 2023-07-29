#pragma once
#include "pch.h"
#include "../vec.h"
#include "../direction.h"

namespace hats::test_vec
{
	static void default_constructor()
	{
		constexpr vec<space::NONE> v;
		HATS_ASSERT(v.x == 0.f);
		HATS_ASSERT(v.y == 0.f);
		HATS_ASSERT(v.z == 0.f);
		HATS_ASSERT(v.w == 0.f);
		HATS_ASSERT(v.e[0] == 0.f);
		HATS_ASSERT(v.e[1] == 0.f);
		HATS_ASSERT(v.e[2] == 0.f);
		HATS_ASSERT(v.e[3] == 0.f);
	}
	static void element_constructor(const f32 x, const f32 y, const f32 z)
	{
		const vec<space::NONE> v(x, y, z);
		HATS_ASSERT(v.x == x);
		HATS_ASSERT(v.y == y);
		HATS_ASSERT(v.z == z);
		HATS_ASSERT(v.w == 0.f);
		HATS_ASSERT(v.e[0] == x);
		HATS_ASSERT(v.e[1] == y);
		HATS_ASSERT(v.e[2] == z);
		HATS_ASSERT(v.e[3] == 0.f);
	}
	static void copy_constructor()
	{
		constexpr f32 x = 1.f, y = 2.f, z = 3.f;
		constexpr vec<space::NONE> v0(x, y, z);
		constexpr vec<space::NONE> v(v0);
		HATS_ASSERT(v.x == x);
		HATS_ASSERT(v.y == y);
		HATS_ASSERT(v.z == z);
		HATS_ASSERT(v.w == 0.f);
		HATS_ASSERT(v.e[0] == x);
		HATS_ASSERT(v.e[1] == y);
		HATS_ASSERT(v.e[2] == z);
		HATS_ASSERT(v.e[3] == 0.f);
	}
	static void operator_add(const f32 x0, const f32 y0, const f32 z0, const f32 x1, const f32 y1, const f32 z1)
	{
		const vec<space::NONE> v0(x0, y0, z0);
		const vec<space::NONE> v1(x1, y1, z1);
		const vec<space::NONE> v2 = v0 + v1;
		HATS_ASSERT(v2.x == x0 + x1);
		HATS_ASSERT(v2.y == y0 + y1);
		HATS_ASSERT(v2.z == z0 + z1);
	}
	static void operator_add_equals(const f32 x0, const f32 y0, const f32 z0, const f32 x1, const f32 y1, const f32 z1)
	{
		vec<space::NONE> v0(x0, y0, z0);
		const vec<space::NONE> v1(x1, y1, z1);
		v0 += v1;
		HATS_ASSERT(v0.x == x0 + x1);
		HATS_ASSERT(v0.y == y0 + y1);
		HATS_ASSERT(v0.z == z0 + z1);
	}
	static void operator_negate(const f32 x, const f32 y, const f32 z)
	{
		const vec<space::NONE> v1(x, y, z);
		const vec<space::NONE> v = -v1;
		HATS_ASSERT(v.x == -x);
		HATS_ASSERT(v.y == -y);
		HATS_ASSERT(v.z == -z);
		HATS_ASSERT(v.w == 0.f);
		HATS_ASSERT(v.e[0] == -x);
		HATS_ASSERT(v.e[1] == -y);
		HATS_ASSERT(v.e[2] == -z);
		HATS_ASSERT(v.e[3] == 0.f);
	}
	static void operator_sub(const f32 x0, const f32 y0, const f32 z0, const f32 x1, const f32 y1, const f32 z1)
	{
		const vec<space::NONE> v0(x0, y0, z0);
		const vec<space::NONE> v1(x1, y1, z1);
		const vec<space::NONE> v2 = v0 - v1;
		HATS_ASSERT(v2.x == x0 - x1);
		HATS_ASSERT(v2.y == y0 - y1);
		HATS_ASSERT(v2.z == z0 - z1);
	}
	static void operator_sub_equals(const f32 x0, const f32 y0, const f32 z0, const f32 x1, const f32 y1, const f32 z1)
	{
		vec<space::NONE> v0(x0, y0, z0);
		const vec<space::NONE> v1(x1, y1, z1);
		v0 -= v1;
		HATS_ASSERT(v0.x == x0 - x1);
		HATS_ASSERT(v0.y == y0 - y1);
		HATS_ASSERT(v0.z == z0 - z1);
	}
	static void operator_mul(const f32 x0, const f32 y0, const f32 z0, const f32 s)
	{
		const vec<space::NONE> v0(x0, y0, z0);
		const vec<space::NONE> v2 = v0 * s;
		HATS_ASSERT(v2.x == x0 * s);
		HATS_ASSERT(v2.y == y0 * s);
		HATS_ASSERT(v2.z == z0 * s);
	}
	static void operator_mul_equals(const f32 x0, const f32 y0, const f32 z0, const f32 s)
	{
		vec<space::NONE> v0(x0, y0, z0);
		v0 *= s;
		HATS_ASSERT(v0.x == x0 * s);
		HATS_ASSERT(v0.y == y0 * s);
		HATS_ASSERT(v0.z == z0 * s);
	}
	static void operator_div(const f32 x0, const f32 y0, const f32 z0, const f32 s)
	{
		const vec<space::NONE> v0(x0, y0, z0);
		const vec<space::NONE> v2 = v0 / s;
		HATS_ASSERT(v2.x == x0 / s);
		HATS_ASSERT(v2.y == y0 / s);
		HATS_ASSERT(v2.z == z0 / s);
	}
	static void operator_div_equals(const f32 x0, const f32 y0, const f32 z0, const f32 s)
	{
		vec<space::NONE> v0(x0, y0, z0);
		v0 /= s;
		HATS_ASSERT(v0.x == x0 / s);
		HATS_ASSERT(v0.y == y0 / s);
		HATS_ASSERT(v0.z == z0 / s);
	}
	static void operator_assign(const f32 x0, const f32 y0, const f32 z0)
	{
		const vec<space::NONE> v(x0, y0, z0);
		const vec<space::NONE> v0 = v;
		HATS_ASSERT(v0.x == v.x);
		HATS_ASSERT(v0.y == v.y);
		HATS_ASSERT(v0.z == v.z);
	}
	static void operator_equals(const f32 x0, const f32 y0, const f32 z0, const f32 x1, const f32 y1, const f32 z1)
	{
		const vec<space::NONE> v0(x0, y0, z0);
		const vec<space::NONE> v1(x1, y1, z1);
		const bool r = v0 == v1;
		HATS_ASSERT(r == (v0.x == v1.x && v0.y == v1.y && v0.z == v1.z));
	}
	static void operator_not_equals(const f32 x0, const f32 y0, const f32 z0, const f32 x1, const f32 y1, const f32 z1)
	{
		const vec<space::NONE> v0(x0, y0, z0);
		const vec<space::NONE> v1(x1, y1, z1);
		const bool r = v0 != v1;
		HATS_ASSERT(r == (v0.x != v1.x || v0.y != v1.y || v0.z != v1.z));
	}
	static void dot(const f32 x0, const f32 y0, const f32 z0, const f32 x1, const f32 y1, const f32 z1, const f32 expected)
	{
		const vec<space::NONE> v0(x0, y0, z0);
		const vec<space::NONE> v1(x1, y1, z1);
		HATS_ASSERT(v0.dot(v1) == expected);
	}
	static void run()
	{
		constexpr int NLIMITS = 5;
		constexpr f32 limits[NLIMITS] =
		{
			0.f,
			std::numeric_limits<f32>::lowest(),
			std::numeric_limits<f32>::max(),
			std::numeric_limits<f32>::min(),
			std::numeric_limits<f32>::denorm_min()
		};

		default_constructor();
		element_constructor(1.f, 2.f, 3.f);
		for (int i = 0; i < NLIMITS; i++)
			element_constructor(limits[i], limits[i], limits[i]);
		copy_constructor();

		operator_add(1.f, 2.f, 3.f, 1.f, 2.f, 3.f);
		for (int i = 0; i < NLIMITS; i++)
			operator_add(limits[i], limits[i], limits[i], limits[i], limits[i], limits[i]);
		operator_add(limits[1], limits[1], limits[1], limits[2], limits[2], limits[2]);
		operator_add_equals(1.f, 2.f, 3.f, 1.f, 2.f, 3.f);
		for (int i = 0; i < NLIMITS; i++)
			operator_add_equals(limits[i], limits[i], limits[i], limits[i], limits[i], limits[i]);
		operator_add_equals(limits[1], limits[1], limits[1], limits[2], limits[2], limits[2]);

		operator_negate(1.f, 2.f, 3.f);
		for (int i = 0; i < NLIMITS; i++)
			operator_negate(limits[i], limits[i], limits[i]);

		operator_sub(1.f, 2.f, 3.f, 1.f, 2.f, 3.f);
		for (int i = 0; i < NLIMITS; i++)
			operator_sub(limits[i], limits[i], limits[i], limits[i], limits[i], limits[i]);
		operator_sub(limits[1], limits[1], limits[1], limits[2], limits[2], limits[2]);
		operator_sub_equals(1.f, 2.f, 3.f, 1.f, 2.f, 3.f);
		for (int i = 0; i < NLIMITS; i++)
			operator_sub_equals(limits[i], limits[i], limits[i], limits[i], limits[i], limits[i]);
		operator_sub_equals(limits[1], limits[1], limits[1], limits[2], limits[2], limits[2]);

		operator_mul(1.f, 2.f, 3.f, 2.f);
		for (int i = 0; i < NLIMITS; i++)
			operator_mul(1.f, 2.f, 3.f, limits[i]);
		for (int i = 0; i < NLIMITS; i++)
			operator_mul(limits[i], limits[i], limits[i], 2.f);
		operator_mul_equals(1.f, 2.f, 3.f, 2.f);
		for (int i = 0; i < NLIMITS; i++)
			operator_mul_equals(1.f, 2.f, 3.f, limits[i]);
		for (int i = 0; i < NLIMITS; i++)
			operator_mul_equals(limits[i], limits[i], limits[i], 2.f);

		operator_div(1.f, 2.f, 3.f, 2.f);
		for (int i = 0; i < NLIMITS; i++)
			operator_div(1.f, 2.f, 3.f, limits[i]);
		for (int i = 0; i < NLIMITS; i++)
			operator_div(limits[i], limits[i], limits[i], 2.f);
		operator_div_equals(1.f, 2.f, 3.f, 2.f);
		for (int i = 0; i < NLIMITS; i++)
			operator_div_equals(1.f, 2.f, 3.f, limits[i]);
		for (int i = 0; i < NLIMITS; i++)
			operator_div_equals(limits[i], limits[i], limits[i], 2.f);

		operator_assign(1.f, 2.f, 3.f);
		for (int i = 0; i < NLIMITS; i++)
			operator_assign(limits[i], limits[i], limits[i]);

		operator_equals(1.f, 2.f, 3.f, 1.f, 2.f, 3.f);
		operator_equals(1.f, 2.f, 3.f, 0.f, 0.f, 0.f);

		operator_not_equals(1.f, 2.f, 3.f, 1.f, 2.f, 3.f);
		operator_not_equals(1.f, 2.f, 3.f, 0.f, 0.f, 0.f);

		dot(1, 0, 0, 1, 0, 0, 1);
		dot(1, 0, 0, 0, 1, 0, 0);
		dot(0, 0, 0, 0, 0, 0, 0);
		dot(1 / sqrt(2), 1 / sqrt(2), 0, 1, 0, 0, 1 / sqrt(2));
	}
}
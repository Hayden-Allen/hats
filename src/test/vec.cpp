#include "pch.h"
#include "vec.h"

namespace hats::test_vec
{
	static void default_constructor()
	{
		vec<space::NONE> v;
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
		vec<space::NONE> v(x, y, z);
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
		const f32 x = 1.f, y = 2.f, z = 3.f;
		vec<space::NONE> v0(x, y, z);
		vec<space::NONE> v(v0);
		HATS_ASSERT(v.x == x);
		HATS_ASSERT(v.y == y);
		HATS_ASSERT(v.z == z);
		HATS_ASSERT(v.w == 0.f);
		HATS_ASSERT(v.e[0] == x);
		HATS_ASSERT(v.e[1] == y);
		HATS_ASSERT(v.e[2] == z);
		HATS_ASSERT(v.e[3] == 0.f);
	}
	static void operator_plus(const f32 x0, const f32 y0, const f32 z0, const f32 x1, const f32 y1, const f32 z1)
	{

	}
	static void run()
	{
		default_constructor();
		element_constructor(1.f, 2.f, 3.f);
		element_constructor(std::numeric_limits<f32>::lowest(), std::numeric_limits<f32>::lowest(), std::numeric_limits<f32>::lowest());
		element_constructor(std::numeric_limits<f32>::max(), std::numeric_limits<f32>::max(), std::numeric_limits<f32>::max());
		element_constructor(std::numeric_limits<f32>::min(), std::numeric_limits<f32>::min(), std::numeric_limits<f32>::min());
		element_constructor(std::numeric_limits<f32>::denorm_min(), std::numeric_limits<f32>::denorm_min(), std::numeric_limits<f32>::denorm_min());
		copy_constructor();

		operator_plus();
	}
}
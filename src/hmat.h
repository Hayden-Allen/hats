#pragma once
#include "pch.h"
#include "mat.h"

namespace hats
{
	template<space SPACE>
	struct direction;
	template<space SPACE>
	struct point;
	// mat4.asm
	extern "C" void mat_multiply(f32* const dst, const f32* const src1, const f32* const src2);

	template<space FROM, space TO>
	struct alignas(64) hmat : public mat<FROM, TO>
	{
		using mat<FROM, TO>::i;
		using mat<FROM, TO>::j;
		using mat<FROM, TO>::k;
		using mat<FROM, TO>::t;
		using mat<FROM, TO>::e;
		using mat<FROM, TO>::m;
	public:
		constexpr hmat() : mat<FROM, TO>() {}
		constexpr hmat
		(
			const f32 i0, const f32 j0, const f32 k0, const f32 t0,
			const f32 i1, const f32 j1, const f32 k1, const f32 t1,
			const f32 i2, const f32 j2, const f32 k2, const f32 t2
		) : mat<FROM, TO>(
			i0, j0, k0, t0,
			i1, j1, k1, t1,
			i2, j2, k2, t2,
			0, 0, 0, 1
		)
		{
			// verify orthogonal basis
			HATS_ASSERT(abs(i0 * j0 + i1 * j1 + i2 * j2) < EPSILON);
			HATS_ASSERT(abs(i0 * k0 + i1 * k1 + i2 * k2) < EPSILON);
			HATS_ASSERT(abs(k0 * j0 + k1 * j1 + k2 * j2) < EPSILON);
		}
		constexpr hmat(const direction<FROM>& i, const direction<FROM>& j, const direction<FROM>& k, const point<FROM>& t) :
			 hmat<FROM, TO>
			 (
				 i[0], i[1], i[2], -(t[0] * i[0] + t[1] * i[1] + t[2] * i[2]),
				 j[0], j[1], j[2], -(t[0] * j[0] + t[1] * j[1] + t[2] * j[2]),
				 k[0], k[1], k[2], -(t[0] * k[0] + t[1] * k[1] + t[2] * k[2])
			 )
		 {}
	public:
		static hmat<FROM, TO> make_scale(const f32 x, const f32 y, const f32 z)
		{
			return hmat<FROM, TO>(
				x, 0, 0, 0,
				0, y, 0, 0,
				0, 0, z, 0
			);
		}
	public:
		hmat<FROM, TO>& operator*=(const hmat<FROM, FROM>& o)
		{
			mat_multiply(i, o.i, i);
			return *this;
		}
		template<space FROM2>
		hmat<FROM2, TO> operator*(const hmat<FROM2, FROM>& o) const
		{
			hmat<FROM2, TO> ret;
			mat_multiply(ret.i, o.i, i);
			return ret;
		}
		hmat<TO, FROM> invert_copy() const
		{
			const f32 x = i[0] * -t[0] + i[1] * -t[1] + i[2] * -t[2];
			const f32 y = j[0] * -t[0] + j[1] * -t[1] + j[2] * -t[2];
			const f32 z = k[0] * -t[0] + k[1] * -t[1] + k[2] * -t[2];
			return hmat<TO, FROM>
			(
				i[0], i[1], i[2], x,
				j[0], j[1], j[2], y,
				k[0], k[1], k[2], z
			);
		}
		hmat<FROM, TO> normalize_copy() const
		{
			const f32 mi = sqrt(i[0] * i[0] + i[1] * i[1] + i[2] * i[2]);
			const f32 mj = sqrt(j[0] * j[0] + j[1] * j[1] + j[2] * j[2]);
			const f32 mk = sqrt(k[0] * k[0] + k[1] * k[1] + k[2] * k[2]);
			return hmat<FROM, TO>(
				i[0] / mi, j[0] / mj, k[0] / mk, t[0],
				i[1] / mi, j[1] / mj, k[1] / mk, t[1],
				i[2] / mi, j[2] / mj, k[2] / mk, t[2]
			);
		}
	};
}
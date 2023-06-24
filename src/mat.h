#pragma once
#include "pch.h"

namespace hats
{
	// mat4.asm
	extern "C" void mat_multiply(f32* const dst, const f32* const src1, const f32* const src2);

	template<space FROM, space TO>
	struct alignas(64) mat
	{
	public:
		union
		{
			struct
			{
				f32 i[4], j[4], k[4], t[4];
			};
			f32 e[16];
			f32 m[4][4];
		};
	public:
		constexpr mat()
		{
			for (s32 i = 0; i < 4; i++)
				for (s32 j = 0; j < 4; j++)
					m[i][j] = (f32)(i == j);
		}
		constexpr mat
		(
			const f32 i0, const f32 j0, const f32 k0, const f32 t0,
			const f32 i1, const f32 j1, const f32 k1, const f32 t1,
			const f32 i2, const f32 j2, const f32 k2, const f32 t2,
			const f32 i3, const f32 j3, const f32 k3, const f32 t3
		)
		{
			i[0] = i0; i[1] = i1; i[2] = i2; i[3] = i3;
			j[0] = j0; j[1] = j1; j[2] = j2; j[3] = j3;
			k[0] = k0; k[1] = k1; k[2] = k2; k[3] = k3;
			t[0] = t0; t[1] = t1; t[2] = t2; t[3] = t3;
		}
		constexpr mat(const mat<FROM, TO>& o)
		{
			for (s32 i = 0; i < 16; i++)
				e[i] = o.e[i];
		}
	public:
		template<space FROM2>
		mat<FROM2, TO> operator*(const mat<FROM2, FROM>& o) const
		{
			mat<FROM2, TO> ret;
			mat_multiply(ret.i, o.i, i);
			return ret;
		}
		void print() const
		{
			printf("mat<%d,%d>{\n", FROM, TO);
			for (s32 row = 0; row < 4; row++)
			{
				printf("\t\t%06f\t%06f\t%06f\t%06f\n", i[row], j[row], k[row], t[row]);
			}
			printf("\t}\n");
		}
	};
}
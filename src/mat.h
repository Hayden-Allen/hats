#pragma once
#include "pch.h"

namespace hats
{
	template<space FROM, space TO>
	struct alignas(64) mat
	{
	public:
		union
		{
			f32 e[16] = { 0.f };
			f32 m[4][4];
			struct
			{
				f32 i[4], j[4], k[4], t[4];
			};
		};
	public:
		constexpr mat()
		{
			for (s32 i = 0; i < 4; i++)
				for (s32 j = 0; j < 4; j++)
					m[i][j] = HATS_CAST(f32, i == j);
		}
		constexpr mat(const f32 (&elements)[16])
		{
			for (s32 i = 0; i < 16; i++)
				e[i] = elements[i];
		}
		template<
			typename I0,
			typename J0,
			typename K0,
			typename T0,
			typename I1,
			typename J1,
			typename K1,
			typename T1,
			typename I2,
			typename J2,
			typename K2,
			typename T2,
			typename I3,
			typename J3,
			typename K3,
			typename T3>
		constexpr mat(
			const I0 i0,
			const J0 j0,
			const K0 k0,
			const T0 t0,
			const I1 i1,
			const J1 j1,
			const K1 k1,
			const T1 t1,
			const I2 i2,
			const J2 j2,
			const K2 k2,
			const T2 t2,
			const I3 i3,
			const J3 j3,
			const K3 k3,
			const T3 t3)
		{
			i[0] = HATS_CAST(f32, i0);
			j[0] = HATS_CAST(f32, j0);
			k[0] = HATS_CAST(f32, k0);
			t[0] = HATS_CAST(f32, t0);
			i[1] = HATS_CAST(f32, i1);
			j[1] = HATS_CAST(f32, j1);
			k[1] = HATS_CAST(f32, k1);
			t[1] = HATS_CAST(f32, t1);
			i[2] = HATS_CAST(f32, i2);
			j[2] = HATS_CAST(f32, j2);
			k[2] = HATS_CAST(f32, k2);
			t[2] = HATS_CAST(f32, t2);
			i[3] = HATS_CAST(f32, i3);
			j[3] = HATS_CAST(f32, j3);
			k[3] = HATS_CAST(f32, k3);
			t[3] = HATS_CAST(f32, t3);
		}
		constexpr mat(const mat<FROM, TO>& o)
		{
			for (s32 i = 0; i < 16; i++)
				e[i] = o.e[i];
		}
	public:
		// this returns the determinant of the 3x3 rotation submatrix
		float determinant() const
		{
			return i[0] * (j[1] * k[2] - k[1] * j[2]) -
				   j[0] * (i[1] * k[2] - k[1] * i[2]) +
				   k[0] * (i[1] * j[2] - j[1] * i[2]);
		}
		float determinant_sign() const
		{
			return signbit(determinant());
		}
		template<space FROM2>
		mat<FROM2, TO> operator*(const mat<FROM2, FROM>& o) const
		{
			mat<FROM2, TO> ret;
			// mat_multiply(ret.i, o.i, i);
			mat_multiply(ret.i, i, o.i);
			return ret;
		}
		bool operator==(const mat<FROM, TO>& o) const
		{
			for (s32 i = 0; i < 16; i++)
				if (e[i] != o.e[i])
					return false;
			return true;
		}
		bool operator!=(const mat<FROM, TO>& o) const
		{
			return !operator==(o);
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
} // namespace hats
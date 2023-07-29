#pragma once
#include "pch.h"
#include "mat.h"
#include "vec_util.h"

namespace hats
{
	template<space SPACE>
	struct vec;
	template<space SPACE>
	struct direction;
	template<space SPACE>
	struct point;
	// mat4.asm
	extern "C" void mat_multiply(f32* const dst, const f32* const src1, const f32* const src2);

	template<space FROM, space TO>
	struct alignas(64) tmat : public mat<FROM, TO>
	{
		using mat<FROM, TO>::i;
		using mat<FROM, TO>::j;
		using mat<FROM, TO>::k;
		using mat<FROM, TO>::t;
		using mat<FROM, TO>::e;
		using mat<FROM, TO>::m;
	public:
		constexpr tmat() : mat<FROM, TO>() {}
		constexpr tmat
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
			const float idj = i0 * j0 + i1 * j1 + i2 * j2;
			const float idk = i0 * k0 + i1 * k1 + i2 * k2;
			const float jdk = k0 * j0 + k1 * j1 + k2 * j2;
			// given basis not sufficiently orthogonal, renormalize (Gram-Schmidt process)
			if (abs(idj) > c::EPSILON || abs(idk) > c::EPSILON || abs(jdk) > c::EPSILON)
			{
				float ni[3] = { 0.f }, nj[3] = { 0.f }, nk[3] = { 0.f };
				vec_util::normalize(ni, ni);
			}
		}
		constexpr tmat(const direction<FROM>& i, const direction<FROM>& j, const direction<FROM>& k, const point<FROM>& t) :
			 tmat<FROM, TO>
			 (
				 i[0], i[1], i[2], -(t[0] * i[0] + t[1] * i[1] + t[2] * i[2]),
				 j[0], j[1], j[2], -(t[0] * j[0] + t[1] * j[1] + t[2] * j[2]),
				 k[0], k[1], k[2], -(t[0] * k[0] + t[1] * k[1] + t[2] * k[2])
			 )
		 {}
	public:
		vec<TO> get_i() const
		{
			return vec<TO>(i[0], i[1], i[2]);
		}
		point<TO> get_t() const
		{
			return point<TO>(t[0], t[1], t[2]);
		}
		tmat<FROM, TO>& operator*=(const tmat<FROM, FROM>& o)
		{
			mat_multiply(i, o.i, i);
			return *this;
		}
		template<space FROM2>
		tmat<FROM2, TO> operator*(const tmat<FROM2, FROM>& o) const
		{
			tmat<FROM2, TO> ret;
			mat_multiply(ret.i, o.i, i);
			return ret;
		}
		tmat<TO, FROM> invert_copy() const
		{
			const f32* const a = &e[0];
			const f32* const b = &e[4];
			const f32* const c = &e[8];
			const f32* const d = &e[12];
			f32 s[3] = { 0.f }, t[3] = { 0.f };
			vec_util::cross(s, a, b);
			vec_util::cross(t, c, d);
			const f32 r_det = 1.f / vec_util::dot(s, c);
			vec_util::scale(s, s, r_det);
			vec_util::scale(t, t, r_det);
			f32 v[3] = { 0.f };
			vec_util::scale(v, c, r_det);
			f32 r0[3] = { 0.f }, r1[3] = { 0.f };
			vec_util::cross(r0, b, v);
			vec_util::cross(r1, v, a);
			return tmat<TO, FROM>(
				r0[0], r0[1], r0[2], -vec_util::dot(b, t),
				r1[0], r1[1], r1[2], vec_util::dot(a, t),
				s[0], s[1], s[2], -vec_util::dot(d, s)
			);
		}
		tmat<FROM, TO> normalize_copy() const
		{
			const f32 mi = sqrt(i[0] * i[0] + i[1] * i[1] + i[2] * i[2]);
			const f32 mj = sqrt(j[0] * j[0] + j[1] * j[1] + j[2] * j[2]);
			const f32 mk = sqrt(k[0] * k[0] + k[1] * k[1] + k[2] * k[2]);
			return tmat<FROM, TO>(
				i[0] / mi, j[0] / mj, k[0] / mk, t[0],
				i[1] / mi, j[1] / mj, k[1] / mk, t[1],
				i[2] / mi, j[2] / mj, k[2] / mk, t[2]
			);
		}
	};
}
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
		template<
			typename I0, typename J0, typename K0, typename T0,
			typename I1, typename J1, typename K1, typename T1,
			typename I2, typename J2, typename K2, typename T2
		>
		constexpr tmat
		(
			const I0 i0, const J0 j0, const K0 k0, const T0 t0,
			const I1 i1, const J1 j1, const K1 k1, const T1 t1,
			const I2 i2, const J2 j2, const K2 k2, const T2 t2
		) : mat<FROM, TO>(
			i0, j0, k0, t0,
			i1, j1, k1, t1,
			i2, j2, k2, t2,
			0, 0, 0, 1
		)
		{
			// verify orthogonal basis
			const f32 idj = vec_util::dot(i, j);
			const f32 idk = vec_util::dot(i, k);
			const f32 jdk = vec_util::dot(j, k);
			// basis not sufficiently orthogonal, renormalize (Gram-Schmidt process)
			if (abs(idj) > c::EPSILON || abs(idk) > c::EPSILON || abs(jdk) > c::EPSILON)
			{
				// v1 = i, v2 = j, v3 = k
				f32 v1[3] = { i[0], i[1], i[2] };
				f32 v2[3] = { j[0], j[1], j[2] };
				f32 v3[3] = { k[0], k[1], k[2] };
				vec_util::gram_schmidt(i, j, k, v1, v2, v3);
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
		direction<FROM> get_i() const
		{
			return direction<FROM>(i[0], i[1], i[2]);
		}
		direction<FROM> get_j() const
		{
			return direction<FROM>(j[0], j[1], j[2]);
		}
		direction<FROM> get_k() const
		{
			return direction<FROM>(k[0], k[1], k[2]);
		}
		point<TO> get_t() const
		{
			return point<TO>(t[0], t[1], t[2]);
		}
		tmat<FROM, TO>& operator*=(const tmat<FROM, FROM>& o)
		{
			mat_multiply(e, e, o.e);
			return *this;
		}
		template<space FROM2>
		tmat<FROM2, TO> operator*(const tmat<FROM2, FROM>& o) const
		{
			tmat<FROM2, TO> ret;
			mat_multiply(ret.e, e, o.e);
			return ret;
		}
		tmat<TO, FROM> invert_copy() const
		{
			const f32* const a = i;
			const f32* const b = j;
			const f32* const c = k;
			const f32* const d = t;
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
				r1[0], r1[1], r1[2],  vec_util::dot(a, t),
				 s[0],  s[1],  s[2], -vec_util::dot(d, s)
			);
		}
		// removes scale effect on any axis
		tmat<FROM, TO>& normalize()
		{
			vec_util::normalize(i, i);
			vec_util::normalize(j, j);
			vec_util::normalize(k, k);
			return *this;
		}
		tmat<FROM, TO> normalize_copy() const
		{
			const f32 mi = vec_util::length(i);
			const f32 mj = vec_util::length(j);
			const f32 mk = vec_util::length(k);
			return tmat<FROM, TO>(
				i[0] / mi, j[0] / mj, k[0] / mk, t[0],
				i[1] / mi, j[1] / mj, k[1] / mk, t[1],
				i[2] / mi, j[2] / mj, k[2] / mk, t[2]
			);
		}
		// removes translation
		tmat<FROM, TO> basis_copy() const
		{
			tmat<FROM, TO> copy = *this;
			copy.t[0] = copy.t[1] = copy.t[2] = 0;
			return copy;
		}
	};
}
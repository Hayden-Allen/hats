#pragma once
#include "pch.h"

namespace hats::vec_util
{
	// m is a tmat, dst and v are vecs
	static inline void transform(f32* const dst, const f32* const m, const f32* const v)
	{
		dst[0] = m[0] * v[0] + m[4] * v[1] + m[8] * v[2] + m[12] * v[3];
		dst[1] = m[1] * v[0] + m[5] * v[1] + m[9] * v[2] + m[13] * v[3];
		dst[2] = m[2] * v[0] + m[6] * v[1] + m[10] * v[2] + m[14] * v[3];
		dst[3] = v[3];
	}
	static inline void copy(f32* const dst, const f32* const src)
	{
		dst[0] = src[0];
		dst[1] = src[1];
		dst[2] = src[2];
	}
	static inline void scale(f32* const dst, const f32* const src, const f32 s)
	{
		dst[0] = src[0] * s;
		dst[1] = src[1] * s;
		dst[2] = src[2] * s;
	}
	static inline f32 dot(const f32* const a, const f32* const b)
	{
		return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	}
	static inline void cross(f32* const dst, const f32* const src1, const f32* const src2)
	{
		dst[0] = src1[1] * src2[2] - src1[2] * src2[1];
		dst[1] = src1[2] * src2[0] - src1[0] * src2[2];
		dst[2] = src1[0] * src2[1] - src1[1] * src2[0];
	}
	static inline f32 length2(const f32* const v)
	{
		return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	}
	static inline f32 length(const f32* const v)
	{
		return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	}
	static inline void normalize(f32* const dst, const f32* const src)
	{
		const f32 m = length(src);
		dst[0] = src[0] / m;
		dst[1] = src[1] / m;
		dst[2] = src[2] / m;
	}
	static inline void proj(f32* const dst, const f32* const u, const f32* const v)
	{
		const f32 scale = dot(u, v) / dot(u, u);
		dst[0] = u[0] * scale;
		dst[1] = u[1] * scale;
		dst[2] = u[2] * scale; 
	}
	static inline void add(f32* const dst, const f32* const src1, const f32* const src2)
	{
		dst[0] = src1[0] + src2[0];
		dst[1] = src1[1] + src2[1];
		dst[2] = src1[2] + src2[2];
	}
	static inline void sub(f32* const dst, const f32* const src1, const f32* const src2)
	{
		dst[0] = src1[0] - src2[0];
		dst[1] = src1[1] - src2[1];
		dst[2] = src1[2] - src2[2];
	}
	static inline void gram_schmidt(f32* const e1, f32* const e2, f32* const e3, const f32* const v1, const f32* const v2, const f32* const v3)
	{
		// to store intermediate results
		f32 u2[3] = { 0.f }, u3[3] = { 0.f };
		f32 tmp0[3] = { 0.f };
		f32 tmp1[3] = { 0.f };

		// u1 = v1
		// copy(u1, v1);
		normalize(e1, v1);

		// u2 = v2 - proj_v1(v2)
		proj(tmp0, v1, v2);
		sub(u2, v2, tmp0);
		normalize(e2, u2);

		// u3 = v3 - proj_v1(v3) - proj_v2(v3)
		proj(tmp0, v1, v3);
		proj(tmp1, u2, v3);
		sub(u3, v3, tmp0);
		sub(u3, u3, tmp1);
		normalize(e3, u3);
	}
}
#pragma once
#include "pch.h"

namespace hats::vec_util
{
	// m is a column-order 4x4 Hamiltonian matrix, dst and v are 4-vectors
	static void transform(f32* const dst, const f32* const m, const f32* const v)
	{
		dst[0] = m[0] * v[0] + m[4] * v[1] + m[8] * v[2] + m[12] * v[3];
		dst[1] = m[1] * v[0] + m[5] * v[1] + m[9] * v[2] + m[13] * v[3];
		dst[2] = m[2] * v[0] + m[6] * v[1] + m[10] * v[2] + m[14] * v[3];
		dst[3] = v[3];
	}
	static __forceinline void copy(f32* const dst, const f32* const src)
	{
		dst[0] = src[0];
		dst[1] = src[1];
		dst[2] = src[2];
	}
	static __forceinline void scale(f32* const dst, const f32* const src, const f32 s)
	{
		dst[0] = src[0] * s;
		dst[1] = src[1] * s;
		dst[2] = src[2] * s;
	}
	static __forceinline f32 dot(const f32* const a, const f32* const b)
	{
		return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	}
	static __forceinline void cross(f32* const dst, const f32* const src1, const f32* const src2)
	{
		dst[0] = src1[1] * src2[2] - src1[2] * src2[1];
		dst[1] = src1[2] * src2[0] - src1[0] * src2[2];
		dst[2] = src1[0] * src2[1] - src1[1] * src2[0];
	}
	static __forceinline f32 length2(const f32* const v)
	{
		return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	}
	static __forceinline f32 length(const f32* const v)
	{
		return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	}
	static __forceinline void normalize(f32* const dst, const f32* const src)
	{
		const f32 m = length(src);
		dst[0] = src[0] / m;
		dst[1] = src[1] / m;
		dst[2] = src[2] / m;
	}
}
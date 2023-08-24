#pragma once
#include "pch.h"
#include "pmat.h"

namespace hats::pmat_util
{
	template<typename FOV, typename AR, typename N, typename F>
	static pmat<space::CAMERA, space::CLIP> projection(const FOV fovy, const AR aspect_ratio, const N near, const F far)
	{
		const f32 g = 1.f / tanf(.5f * (HATS_CAST(f32, fovy) * c::PI / 180.f));
		const f32 x = g / HATS_CAST(f32, aspect_ratio);
		const f32 y = g;
		// y negated to move from OpenGL RH camera space to Direct3D LH clip space
		// const f32 y = -g;

		// reversed projection (1 @ near, 0 @ far)
		const f32 fn = HATS_CAST(f32, near), ff = HATS_CAST(f32, far);
		// const f32 a = fn / (fn - ff), b = -(fn * ff) / (fn - ff);
		const f32 a = ff / (fn - ff), b = -(ff * fn) / (ff - fn);
		// this is negative because converting from RH space to LH space
		const f32 w = -1.f;
		return pmat<space::CAMERA, space::CLIP>(x, y, a, b, w);
	}
} // namespace hats::pmat_util
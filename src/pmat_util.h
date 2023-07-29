#pragma once
#include "pch.h"
#include "pmat.h"

namespace hats::pmat_util
{
	static pmat<space::CAMERA, space::CLIP> projection(const f32 fovy, const f32 aspect_ratio, const f32 near, const f32 far)
	{
		const f32 g = 1.f / tanf(.5f * (fovy * c::PI / 180.f));
		// y negated to move from OpenGL RH camera space to Direct3D LH clip space
		const f32 x = g / aspect_ratio, y = -g;
		// reversed projection (1 @ near, 0 @ far)
		const f32 a = near / (near - far), b = -(near * far) / (near - far);
		// not sure why this has to be negative, something with OpenGL
		const f32 w = -1.f;
		return pmat<space::CAMERA, space::CLIP>(x, y, a, b, w);
	}
}
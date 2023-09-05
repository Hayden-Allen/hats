#pragma once
#include <stdio.h>
#include <chrono>
#include <iostream>
#include <numeric>
#include <numbers>

#ifdef _DEBUG
#define HATS_ASSERT(x) if(!(x)) { __debugbreak(); }
#else
#define HATS_ASSERT(x) (void)(x)
#endif
#define HATS_CAST(T, x) static_cast<T>(x)

namespace hats
{
	typedef int8_t	s8;
	typedef int16_t s16;
	typedef int32_t s32;
	typedef int64_t s64;
	typedef uint8_t	 u8;
	typedef uint16_t u16;
	typedef uint32_t u32;
	typedef uint64_t u64;
	typedef float f32;
	typedef double f64;

	namespace c
	{
		static constexpr f32 EPSILON = 1e-5f;
		static constexpr f32 PI = std::numbers::pi_v<f32>;
		static constexpr f32 TWO_PI = 2.f * PI;
	}

	// TODO replace with glm one?
	static void mat_multiply(f32* const dst, const f32* const src1, const f32* const src2)
	{
		dst[0] = src1[0] * src2[0] + src1[4] * src2[1] + src1[8] * src2[2] + src1[12] * src2[3];
		dst[1] = src1[1] * src2[0] + src1[5] * src2[1] + src1[9] * src2[2] + src1[13] * src2[3];
		dst[2] = src1[2] * src2[0] + src1[6] * src2[1] + src1[10] * src2[2] + src1[14] * src2[3];
		dst[3] = src1[3] * src2[0] + src1[7] * src2[1] + src1[11] * src2[2] + src1[15] * src2[3];

		dst[4] = src1[0] * src2[4] + src1[4] * src2[5] + src1[8] * src2[6] + src1[12] * src2[7];
		dst[5] = src1[1] * src2[4] + src1[5] * src2[5] + src1[9] * src2[6] + src1[13] * src2[7];
		dst[6] = src1[2] * src2[4] + src1[6] * src2[5] + src1[10] * src2[6] + src1[14] * src2[7];
		dst[7] = src1[3] * src2[4] + src1[7] * src2[5] + src1[11] * src2[6] + src1[15] * src2[7];

		dst[8] = src1[0] * src2[8] + src1[4] * src2[9] + src1[8] * src2[10] + src1[12] * src2[11];
		dst[9] = src1[1] * src2[8] + src1[5] * src2[9] + src1[9] * src2[10] + src1[13] * src2[11];
		dst[10] = src1[2] * src2[8] + src1[6] * src2[9] + src1[10] * src2[10] + src1[14] * src2[11];
		dst[11] = src1[3] * src2[8] + src1[7] * src2[9] + src1[11] * src2[10] + src1[15] * src2[11];

		dst[12] = src1[0] * src2[12] + src1[4] * src2[13] + src1[8] * src2[14] + src1[12] * src2[15];
		dst[13] = src1[1] * src2[12] + src1[5] * src2[13] + src1[9] * src2[14] + src1[13] * src2[15];
		dst[14] = src1[2] * src2[12] + src1[6] * src2[13] + src1[10] * src2[14] + src1[14] * src2[15];
		dst[15] = src1[3] * src2[12] + src1[7] * src2[13] + src1[11] * src2[14] + src1[15] * src2[15];
	}
	template<typename T>
	static __forceinline T clean_angle(const T theta)
	{
		return theta - c::TWO_PI * std::floor(theta / c::TWO_PI);
	}

	enum class space
	{
		NONE = -1,
		// an object's local right-handed reference frame (UNIT: world)
		OBJECT = 0,
		// the right-handed reference frame of the scene (UNIT: world)
		WORLD = 1,
		// the camera's local right-handed reference frame (UNIT: world)
		CAMERA = 2,
		// the right-handed reference frame representing the projection applied to camera space (UNIT: 4D)
		CLIP = 3,
		// the right-handed reference frame representing the normalized view volume resulting from the perspective divide (UNIT: NDC)
		DEVICE = 4,
		// the left-handed reference frame of the render target (UNIT: pixel)
		VIEWPORT = 5,

		// spaces relative to a specific object. provided so that typed transformations can be applied at any level in the scene graph
		PARENT = 6,
		CHILD = 7
	};
}
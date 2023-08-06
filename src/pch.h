#pragma once
#include <stdio.h>
#include <chrono>
#include <iostream>
#include <numeric>
#include <numbers>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#ifdef _DEBUG
#define HATS_ASSERT(x) if(!(x)) { __debugbreak(); }
#else
#define HATS_ASSERT(x) x
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
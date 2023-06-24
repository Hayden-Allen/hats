#pragma once
#include <stdio.h>
#include <chrono>
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#ifdef _DEBUG
#define HATS_ASSERT(x) if(!(x)) { __debugbreak(); }
#else
#define HATS_ASSERT(x)
#endif
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

	static constexpr f32 EPSILON = 1e-6;

	static std::chrono::nanoseconds get_time()
	{
		return std::chrono::high_resolution_clock::now().time_since_epoch();
	}

	template<typename T>
	static __forceinline void swap(T* const a, T* const b)
	{
		const T tmp = *a;
		*a = *b;
		*b = tmp;
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
		VIEWPORT = 5
	};
}
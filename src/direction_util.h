#pragma once
#include "pch.h"
#include "direction.h"

namespace hats::direction_util
{
	template<space SPACE>
	static direction<SPACE> i_hat()
	{
		return direction<SPACE>(1, 0, 0);
	}
	template<space SPACE>
	static direction<SPACE> j_hat()
	{
		return direction<SPACE>(0, 1, 0);
	}
	template<space SPACE>
	static direction<SPACE> k_hat()
	{
		return direction<SPACE>(0, 0, 1);
	}
}
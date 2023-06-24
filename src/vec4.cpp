#include "pch.h"
#include "vec4.h"
#include "hmat.h"

namespace hats
{
	//template<space SPACE>
	//template<space TO>
	//vec4<TO> vec4<SPACE>::transform_copy(const hmat<SPACE, TO>& m) const
	//{
	//	f32 newv[4] = { 0.f };
	//	newv[0] = m.i[0] * x + m.j[0] * y + m.k[0] * z + m.t[0] * w;
	//	newv[1] = m.i[1] * x + m.j[1] * y + m.k[1] * z + m.t[1] * w;
	//	newv[2] = m.i[2] * x + m.j[2] * y + m.k[2] * z + m.t[2] * w;
	//	// i[3] = j[3] = k[3] = 0 for hamiltonian matrices
	//	newv[3] = m.t[3] * w;
	//	return vec4<TO>(newv[0], newv[1], newv[2], newv[3]);
	//}
	//template<space SPACE>
	//template<space TO>
	//vec4<TO>& vec4<SPACE>::transform(const hmat<SPACE, TO>& m)
	//{
	//	f32 newv[4] = { 0.f };
	//	newv[0] = m.i[0] * x + m.j[0] * y + m.k[0] * z + m.t[0] * w;
	//	newv[1] = m.i[1] * x + m.j[1] * y + m.k[1] * z + m.t[1] * w;
	//	newv[2] = m.i[2] * x + m.j[2] * y + m.k[2] * z + m.t[2] * w;
	//	// i[3] = j[3] = k[3] = 0 for hamiltonian matrices
	//	newv[3] = m.t[3] * w;
	//	x = newv[0];
	//	y = newv[1];
	//	z = newv[2];
	//	w = newv[3];
	//	return *this;
	//}
}
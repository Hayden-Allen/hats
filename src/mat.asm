	include <lib/MacrosX86-64-AVX.asmh>

mat_multiply_row macro disp, dst
	; xmm4 = [r8[disp / 16][0]] * 4
	; xmm5 = [r8[disp / 16][1]] * 4
	; xmm6 = [r8[disp / 16][2]] * 4
	; xmm7 = [r8[disp / 16][3]] * 4
	vbroadcastss xmm4, real4 ptr [r8 + disp + 00]
	vbroadcastss xmm5, real4 ptr [r8 + disp + 04]
	vbroadcastss xmm6, real4 ptr [r8 + disp + 08]
	vbroadcastss xmm7, real4 ptr [r8 + disp + 12]

	; xmm4 = r8[disp / 16][0] * rdx[0]
	; xmm5 = r8[disp / 16][1] * rdx[1]
	; xmm6 = r8[disp / 16][2] * rdx[2]
	; xmm7 = r8[disp / 16][3] * rdx[3]
	vmulps xmm4, xmm4, xmm0
	vmulps xmm5, xmm5, xmm1
	vmulps xmm6, xmm6, xmm2
	vmulps xmm7, xmm7, xmm3
	
	; xmm4 = vector containing dot products of each row of rdx with column `disp / 16` of r8
	; this becomes the `disp / 16`'th column of the result
	vaddps xmm4, xmm4, xmm5
	vaddps xmm6, xmm6, xmm7
	vaddps xmm4, xmm4, xmm6

	vmovaps dst, xmm4
	endm

	.code
; computes rcx = rdx * r8, where all 3 are column-major 4x4 single-precision floating-point matrices
mat_multiply proc frame
	_CreateFrame MT_, 0, 32
	_SaveXmmRegs xmm6, xmm7
	_EndProlog

	; extract each column of the left matrix
	vmovaps xmm0, [rdx + 00]
	vmovaps xmm1, [rdx + 16]
	vmovaps xmm2, [rdx + 32]
	vmovaps xmm3, [rdx + 48]

	; compute columns of result and store in temp registers
	; this copying ensures that the algorithm works when the
	; destination is either of the sources
	mat_multiply_row 00, xmm8
	mat_multiply_row 16, xmm9
	mat_multiply_row 32, xmm10
	mat_multiply_row 48, xmm11

	; now that computation is done, overwrite columns of destination
	vmovaps [rcx + 00], xmm8
	vmovaps [rcx + 16], xmm9
	vmovaps [rcx + 32], xmm10
	vmovaps [rcx + 48], xmm11

	_RestoreXmmRegs xmm6, xmm7
	_DeleteFrame
	ret
mat_multiply endp
	end
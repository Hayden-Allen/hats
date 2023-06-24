	include <lib/MacrosX86-64-AVX.asmh>

mat_multiply_row macro disp
	
	vbroadcastss xmm4,real4 ptr [rdx+disp]
	vbroadcastss xmm5,real4 ptr [rdx+disp+4]
	vbroadcastss xmm6,real4 ptr [rdx+disp+8]
	vbroadcastss xmm7,real4 ptr [rdx+disp+12]

	vmulps xmm4,xmm4,xmm0
	vmulps xmm5,xmm5,xmm1
	vmulps xmm6,xmm6,xmm2
	vmulps xmm7,xmm7,xmm3

	vaddps xmm4,xmm4,xmm5
	vaddps xmm6,xmm6,xmm7
	vaddps xmm4,xmm4,xmm6

	vmovaps[rcx+disp],xmm4
	endm

	.code
mat_multiply proc frame
	_CreateFrame MT_,0,32
	_SaveXmmRegs xmm6,xmm7
	_EndProlog

	vmovaps xmm0,[r8]
	vmovaps xmm1,[r8+16]
	vmovaps xmm2,[r8+32]
	vmovaps xmm3,[r8+48]

	mat_multiply_row 0
	mat_multiply_row 16
	mat_multiply_row 32
	mat_multiply_row 48

	_RestoreXmmRegs xmm6,xmm7
	_DeleteFrame
	ret
mat_multiply endp
	end
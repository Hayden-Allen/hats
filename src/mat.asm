	        .nolist
        extern __chkstk:proc

;-------------------------------------------------------------------------
; X86-64 Macro Notes
;
; The macros defined in this file assume that the stack frame is
; organized as described in the text.
;
; A VS project rebuild may be necessary if any changes are made
; in this file.
;
; A function must use the macros in the following order:
;   _CreateFrame        required
;   _SaveXmmRegs        optional
;   _EndProlog          required
;   _RestoreXmmRegs     optional
;   _DeleteFrame        required
;-------------------------------------------------------------------------

;-------------------------------------------------------------------------
; _CreateFrame
;
; Description:  The following macro generates code that creates a stack
;               frame for x86-64 functions.
;
; Macro Parameters: Prefix          Macro symbolic name prefix
;                   StkSizeLocal1   Size in bytes for local1 vars
;                   StkSizeLocal2   Size in bytes for XMM reg save area
;                   Rnv1 - Rnv7     Non-volatile registers to save
;-------------------------------------------------------------------------

_CreateFrame macro Prefix,StkSizeLocal1,StkSizeLocal2,Rnv1,Rnv2,Rnv3,Rnv4,Rnv5,Rnv6,Rnv7

; Make sure stack sizes are valid
.errnz StkSizeLocal1 MOD 16, <Macro parameter 'StkSizeLocal1' must be evenly divisible by 16>
.errnz StkSizeLocal2 MOD 16, <Macro parameter 'StkSizeLocal2' must be evenly divisible by 16>
.errnz StkSizeLocal2 GT 240, <Macro parameter 'StkSizeLocal2' must be less than or equal to 240>

        push rbp
       .pushreg rbp
        NumPushReg=1
IFNB <Rnv1>
        push Rnv1
        .pushreg Rnv1
        NumPushReg=NumPushReg+1
ENDIF
IFNB <Rnv2>
        push Rnv2
        .pushreg Rnv2
        NumPushReg=NumPushReg+1
ENDIF
IFNB <Rnv3>
        push Rnv3
        .pushreg Rnv3
        NumPushReg=NumPushReg+1
ENDIF
IFNB <Rnv4>
        push Rnv4
        .pushreg Rnv4
        NumPushReg=NumPushReg+1
ENDIF
IFNB <Rnv5>
        push Rnv5
        .pushreg Rnv5
        NumPushReg=NumPushReg+1
ENDIF
IFNB <Rnv6>
        push Rnv6
        .pushreg Rnv6
        NumPushReg=NumPushReg+1
ENDIF
IFNB <Rnv7>
        push Rnv7
        .pushreg Rnv7
        NumPushReg=NumPushReg+1
ENDIF

        StackPad=((NumPushReg AND 1) XOR 1) * 8
        StackSizeTotal=StkSizeLocal1+StkSizeLocal2+StackPad

; Call helper function __chkstk if StackSizeTotal >= 4096 bytes (1 page)
; See Visual Studio documentation for more information
IF (StackSizeTotal GE 4096)
        mov rax,StackSizeTotal
        call __chkstk
        sub rsp,rax
        .allocstack StackSizeTotal
ELSEIF (StackSizeTotal GT 0)
        sub rsp,StackSizeTotal
        .allocstack StackSizeTotal
ENDIF

IF (StkSizeLocal2 GT 0)
        lea rbp,[rsp+StkSizeLocal2]
ELSE
        mov rbp,rsp
ENDIF
        .setframe rbp,StkSizeLocal2

; Create the symbols for current function
        ValStackSizeTotal CATSTR <Prefix>,<StackSizeTotal>
        ValStackSizeTotal=StackSizeTotal

        ValStackSizeLocal1 CATSTR <Prefix>,<StackSizeLocal1>
        ValStackSizeLocal1=StkSizeLocal1

        ValStackSizeLocal2 CATSTR <Prefix>,<StackSizeLocal2>
        ValStackSizeLocal2=StkSizeLocal2

        ValNameOffsetHomeRCX CATSTR <Prefix>,<OffsetHomeRCX>
        ValNameOffsetHomeRCX=StkSizeLocal1+NumPushReg*8+StackPad+8

        ValNameOffsetHomeRDX CATSTR <Prefix>,<OffsetHomeRDX>
        ValNameOffsetHomeRDX=StkSizeLocal1+NumPushReg*8+StackPad+16

        ValNameOffsetHomeR8 CATSTR <Prefix>,<OffsetHomeR8>
        ValNameOffsetHomeR8=StkSizeLocal1+NumPushReg*8+StackPad+24

        ValNameOffsetHomeR9 CATSTR <Prefix>,<OffsetHomeR9>
        ValNameOffsetHomeR9=StkSizeLocal1+NumPushReg*8+StackPad+32

        ValNameOffsetStackArgs CATSTR <Prefix>,<OffsetStackArgs>
        ValNameOffsetStackArgs=StkSizeLocal1+NumPushReg*8+StackPad+40

        ValNameOffsetSaveXmmRegs CATSTR <Prefix>,<OffsetSaveXmmRegs>
        ValNameOffsetSaveXmmRegs=StkSizeLocal2

        ValNameOffsetDeleteFrame CATSTR <Prefix>,<OffsetDeleteFrame>
        ValNameOffsetDeleteFrame=StkSizeLocal1+StackPad
        endm

;-------------------------------------------------------------------------
; _EndProlog
;
; Description:  The following macro generates the .endprolog directive.
;-------------------------------------------------------------------------

_EndProlog  macro
        .endprolog
        endm

;-------------------------------------------------------------------------
; _DeleteFrame
;
; Description:  The following macro generates code that de-allocate a
;               stack frame previously created using _CreateFrame.
;
; Macro Parameters: Rnv1 - Rnv7     Non-volatile registers to restore
;-------------------------------------------------------------------------

_DeleteFrame macro Rnv1,Rnv2,Rnv3,Rnv4,Rnv5,Rnv6,Rnv7

IF (ValNameOffsetDeleteFrame GT 0)
        lea rsp,[rbp+ValNameOffsetDeleteFrame]
ELSE
        mov rsp,rbp
ENDIF

IFNB <Rnv7>
        pop Rnv7
ENDIF
IFNB <Rnv6>
        pop Rnv6
ENDIF
IFNB <Rnv5>
        pop Rnv5
ENDIF
IFNB <Rnv4>
        pop Rnv4
ENDIF
IFNB <Rnv3>
        pop Rnv3
ENDIF
IFNB <Rnv2>
        pop Rnv2
ENDIF
IFNB <Rnv1>
        pop Rnv1
ENDIF
        pop rbp
        endm

;-------------------------------------------------------------------------
; _SaveXmmRegs
;
; Description:  The following macro generates code that saves the
;               specified non-volatile registers to the local save area.
;
; Macro Parameters: Rnv1 - Rnv7     Non-volatile XMM registers to save.
;-------------------------------------------------------------------------

_SaveXmmRegs    macro Rnv0,Rnv1,Rnv2,Rnv3,Rnv4,Rnv5,Rnv6,Rnv7,Rnv8,Rnv9

NUMSAVEXMM = 0

IFNB <Rnv0>
        vmovdqa xmmword ptr [rbp-ValNameOffsetSaveXmmRegs],Rnv0
        .savexmm128 Rnv0,0
        NUMSAVEXMM = NUMSAVEXMM + 1
ENDIF
IFNB <Rnv1>
        vmovdqa xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+16],Rnv1
        .savexmm128 Rnv1,16
        NUMSAVEXMM = NUMSAVEXMM + 1
ENDIF
IFNB <Rnv2>
        vmovdqa xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+32],Rnv2
        .savexmm128 Rnv2,32
        NUMSAVEXMM = NUMSAVEXMM + 1
ENDIF
IFNB <Rnv3>
        vmovdqa xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+48],Rnv3
        .savexmm128 Rnv3,48
        NUMSAVEXMM = NUMSAVEXMM + 1
ENDIF
IFNB <Rnv4>
        vmovdqa xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+64],Rnv4
        .savexmm128 Rnv4,64
        NUMSAVEXMM = NUMSAVEXMM + 1
ENDIF
IFNB <Rnv5>
        vmovdqa xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+80],Rnv5
        .savexmm128 Rnv5,80
        NUMSAVEXMM = NUMSAVEXMM + 1
ENDIF
IFNB <Rnv6>
        vmovdqa xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+96],Rnv6
        .savexmm128 Rnv6,96
        NUMSAVEXMM = NUMSAVEXMM + 1
ENDIF
IFNB <Rnv7>
        vmovdqa xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+112],Rnv7
        .savexmm128 Rnv7,112
        NUMSAVEXMM = NUMSAVEXMM + 1
ENDIF
IFNB <Rnv8>
        vmovdqa xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+128],Rnv8
        .savexmm128 Rnv8,128
        NUMSAVEXMM = NUMSAVEXMM + 1
ENDIF
IFNB <Rnv9>
        vmovdqa xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+144],Rnv9
        .savexmm128 Rnv9,144
        NUMSAVEXMM = NUMSAVEXMM + 1
ENDIF

; Make sure the stack frame was created with enough space to save
; all of the specified XMM registers.
.errnz (NUMSAVEXMM * 16) GT ValStackSizeLocal2, <XMM register save area is too small>
        endm

;-------------------------------------------------------------------------
; _RestoreXmmRegs
;
; Description:  The following macro generates code that restores the
;               specified non-volatile registers from the local save area.
;
; Macro Parameters: Rnv1 - Rnv7     Non-volatile XMM registers to restore.
;-------------------------------------------------------------------------

_RestoreXmmRegs macro Rnv0,Rnv1,Rnv2,Rnv3,Rnv4,Rnv5,Rnv6,Rnv7,Rnv8,Rnv9

IFNB <Rnv0>
        vmovdqa rnv0,xmmword ptr [rbp-ValNameOffsetSaveXmmRegs]
ENDIF
IFNB <Rnv1>
        vmovdqa rnv1,xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+16]
ENDIF
IFNB <Rnv2>
        vmovdqa rnv2,xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+32]
ENDIF
IFNB <Rnv3>
        vmovdqa rnv3,xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+48]
ENDIF
IFNB <Rnv4>
        vmovdqa rnv4,xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+64]
ENDIF
IFNB <Rnv5>
        vmovdqa rnv5,xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+80]
ENDIF
IFNB <Rnv6>
        vmovdqa rnv6,xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+96]
ENDIF
IFNB <Rnv7>
        vmovdqa rnv7,xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+112]
ENDIF
IFNB <Rnv8>
        vmovdqa rnv8,xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+128]
ENDIF
IFNB <Rnv9>
        vmovdqa rnv9,xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+144]
ENDIF
        endm

        .list
        .listmacro



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
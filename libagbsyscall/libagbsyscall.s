	.include "../asminclude/gba_constants.inc"
	.include "../asminclude/macros.inc"

	.syntax unified

	.text

	.ifdef NO_GRANULAR_AGBSYSCALL
    .set L_ArcTan2, 1
    .set L_BgAffineSet, 1
    .set L_CpuFastSet, 1
    .set L_CpuSet, 1
    .set L_Div, 1
    .set L_LZ77UnCompVram, 1
    .set L_LZ77UnCompWram, 1
    .set L_MultiBoot, 1
    .set L_ObjAffineSet, 1
    .set L_RLUnCompVram, 1
    .set L_RLUnCompWram, 1
    .set L_RegisterRamReset, 1
    .set L_SoftReset, 1
    .set L_Sqrt, 1
    .set L_VBlankIntrWait, 1
    .set L_SoundBiasReset, 1
    .set L_SoundBiasSet, 1
    .endif

    .ifdef L_ArcTan2
	thumb_func_start ArcTan2
ArcTan2:
	swi 0xA
	bx lr
	thumb_func_end ArcTan2
	.endif

    .ifdef L_BgAffineSet
	thumb_func_start BgAffineSet
@ void BgAffineSet(BgAffineSet_src_data *src, BgAffineSet_dest_data *dest, int entry_count)
BgAffineSet:
	swi 0xE
	bx lr
	thumb_func_end BgAffineSet
	.endif

    .ifdef L_CpuFastSet
	thumb_func_start CpuFastSet
@ void CpuFastSet(void *src, void *dest, unsigned int mode)
CpuFastSet:
	swi 0xC
	bx lr
	thumb_func_end CpuFastSet
	.endif

    .ifdef L_CpuSet
	thumb_func_start CpuSet
@ void CpuSet(void *src, void *dest, unsigned int mode)
CpuSet:
	swi 0xB
	bx lr
	thumb_func_end CpuSet
	.endif

    .ifdef L_Div
	thumb_func_start Div
Div:
	swi 0x6
	bx lr
	thumb_func_end Div
	.endif

    .ifdef L_Mod
    thumb_func_start Mod
Mod:
    swi 0x6
    adds r0, r1, 0
    bx lr
    thumb_func_end Mod
    .endif

    .ifdef L_LZ77UnCompVram
	thumb_func_start LZ77UnCompVram
@ void LZ77UnCompVram(void *src, void *dest)
LZ77UnCompVram:
	swi 0x12
	bx lr
	thumb_func_end LZ77UnCompVram
	.endif

    .ifdef L_LZ77UnCompWram
	thumb_func_start LZ77UnCompWram
@ void LZ77UnCompWram(void *src, void *dest)
LZ77UnCompWram:
	swi 0x11
	bx lr
	thumb_func_end LZ77UnCompWram
	.endif

    .ifdef L_MultiBoot
	thumb_func_start MultiBoot
@ s32 MultiBoot(struct MultiBootParam *mp)
MultiBoot:
	movs r1, 0x1
	swi 0x25
	bx lr
	thumb_func_end MultiBoot
	.endif

    .ifdef L_ObjAffineSet
	thumb_func_start ObjAffineSet
ObjAffineSet:
	swi 0xF
	bx lr
	thumb_func_end ObjAffineSet
	.endif

    .ifdef L_RLUnCompVram
	thumb_func_start RLUnCompVram
RLUnCompVram:
	swi 0x15
	bx lr
	thumb_func_end RLUnCompVram
	.endif

    .ifdef L_RLUnCompWram
	thumb_func_start RLUnCompWram
RLUnCompWram:
	swi 0x14
	bx lr
	thumb_func_end RLUnCompWram
	.endif

    .ifdef L_RegisterRamReset
	thumb_func_start RegisterRamReset
@ void RegisterRamReset(int ResetFlags)
RegisterRamReset:
	swi 0x1
	bx lr
	thumb_func_end RegisterRamReset
	.endif

    .ifdef L_SoftReset
	thumb_func_start SoftReset
@ void SoftReset()
SoftReset:
	ldr r3, =REG_IME
	movs r2, 0
	strb r2, [r3]
	ldr r1, =0x3007F00
	mov sp, r1
	swi 0x1
	swi 0
	.pool
	thumb_func_end SoftReset
	.endif

    .ifdef L_Sqrt
	thumb_func_start Sqrt
@ s16 Sqrt(int)
Sqrt:
	swi 0x8
	bx lr
	thumb_func_end Sqrt
	.endif

	.ifdef L_SoundBiasReset
	thumb_func_start SoundBiasReset
@ void SoundBiasReset()
SoundBiasReset:
	movs r0, 0
	swi 0x19
	bx lr
	thumb_func_end SoundBiasReset
	.endif

	.ifdef L_SoundBiasSet
	thumb_func_start SoundBiasSet
@ void SoundBiasSet()
SoundBiasSet:
	movs r0, 1
	swi 0x19
	bx lr
	thumb_func_end SoundBiasSet
	.endif

    .ifdef L_VBlankIntrWait
	thumb_func_start VBlankIntrWait
@ void VBlankIntrWait()
VBlankIntrWait:
	movs r2, 0
	swi 0x5
	bx lr
	thumb_func_end VBlankIntrWait
	.endif

	.align 2, 0 @ Don't pad with nop.

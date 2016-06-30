;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.5.0 #9253 (Mar 24 2016) (Linux)
; This file was generated Wed Jun 22 19:05:32 2016
;--------------------------------------------------------
	.module rune
	.optsdcc -mmcs51 --model-small
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _fullrune_PARM_2
	.globl _runenlen_PARM_2
	.globl _runetochar_PARM_2
	.globl _chartorune_PARM_2
	.globl _chartorune
	.globl _runetochar
	.globl _runelen
	.globl _runenlen
	.globl _fullrune
;--------------------------------------------------------
; special function registers
;--------------------------------------------------------
	.area RSEG    (ABS,DATA)
	.org 0x0000
;--------------------------------------------------------
; special function bits
;--------------------------------------------------------
	.area RSEG    (ABS,DATA)
	.org 0x0000
;--------------------------------------------------------
; overlayable register banks
;--------------------------------------------------------
	.area REG_BANK_0	(REL,OVR,DATA)
	.ds 8
;--------------------------------------------------------
; internal ram data
;--------------------------------------------------------
	.area DSEG    (DATA)
_runelen_rune_1_65:
	.ds 2
_runelen_str_1_65:
	.ds 10
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
	.area	OSEG    (OVR,DATA)
_chartorune_PARM_2:
	.ds 3
_chartorune_rune_1_52:
	.ds 3
_chartorune_c_1_53:
	.ds 2
_chartorune_c1_1_53:
	.ds 2
_chartorune_c2_1_53:
	.ds 2
	.area	OSEG    (OVR,DATA)
_runetochar_PARM_2:
	.ds 3
_runetochar_str_1_59:
	.ds 3
_runetochar_c_1_60:
	.ds 4
	.area	OSEG    (OVR,DATA)
_runenlen_PARM_2:
	.ds 2
_runenlen_nb_1_67:
	.ds 2
	.area	OSEG    (OVR,DATA)
_fullrune_PARM_2:
	.ds 2
;--------------------------------------------------------
; indirectly addressable internal ram data
;--------------------------------------------------------
	.area ISEG    (DATA)
;--------------------------------------------------------
; absolute internal ram data
;--------------------------------------------------------
	.area IABS    (ABS,DATA)
	.area IABS    (ABS,DATA)
;--------------------------------------------------------
; bit data
;--------------------------------------------------------
	.area BSEG    (BIT)
;--------------------------------------------------------
; paged external ram data
;--------------------------------------------------------
	.area PSEG    (PAG,XDATA)
;--------------------------------------------------------
; external ram data
;--------------------------------------------------------
	.area XSEG    (XDATA)
;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area XABS    (ABS,XDATA)
;--------------------------------------------------------
; external initialized ram data
;--------------------------------------------------------
	.area XISEG   (XDATA)
	.area HOME    (CODE)
	.area GSINIT0 (CODE)
	.area GSINIT1 (CODE)
	.area GSINIT2 (CODE)
	.area GSINIT3 (CODE)
	.area GSINIT4 (CODE)
	.area GSINIT5 (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area CSEG    (CODE)
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area HOME    (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area GSINIT  (CODE)
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME    (CODE)
	.area HOME    (CODE)
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG    (CODE)
;------------------------------------------------------------
;Allocation info for local variables in function 'chartorune'
;------------------------------------------------------------
;str                       Allocated with name '_chartorune_PARM_2'
;rune                      Allocated with name '_chartorune_rune_1_52'
;c                         Allocated with name '_chartorune_c_1_53'
;c1                        Allocated with name '_chartorune_c1_1_53'
;c2                        Allocated with name '_chartorune_c2_1_53'
;c3                        Allocated to registers r2 r6 
;l                         Allocated to registers r1 r0 r3 r7 
;------------------------------------------------------------
;	libutf/rune.c:47: chartorune(Rune *rune, char *str)
;	-----------------------------------------
;	 function chartorune
;	-----------------------------------------
_chartorune:
	ar7 = 0x07
	ar6 = 0x06
	ar5 = 0x05
	ar4 = 0x04
	ar3 = 0x03
	ar2 = 0x02
	ar1 = 0x01
	ar0 = 0x00
	mov	_chartorune_rune_1_52,dpl
	mov	(_chartorune_rune_1_52 + 1),dph
	mov	(_chartorune_rune_1_52 + 2),b
;	libutf/rune.c:56: c = *(uchar*)str;
	mov	r2,_chartorune_PARM_2
	mov	r3,(_chartorune_PARM_2 + 1)
	mov	r4,(_chartorune_PARM_2 + 2)
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
	mov	r2,a
	mov	_chartorune_c_1_53,r2
	mov	(_chartorune_c_1_53 + 1),#0x00
;	libutf/rune.c:57: if(c < Tx) {
	clr	c
	mov	a,_chartorune_c_1_53
	subb	a,#0x80
	mov	a,(_chartorune_c_1_53 + 1)
	xrl	a,#0x80
	subb	a,#0x80
	jnc	00102$
;	libutf/rune.c:58: *rune = c;
	mov	dpl,_chartorune_rune_1_52
	mov	dph,(_chartorune_rune_1_52 + 1)
	mov	b,(_chartorune_rune_1_52 + 2)
	mov	a,_chartorune_c_1_53
	lcall	__gptrput
	inc	dptr
	mov	a,(_chartorune_c_1_53 + 1)
	lcall	__gptrput
;	libutf/rune.c:59: return 1;
	mov	dptr,#0x0001
	ret
00102$:
;	libutf/rune.c:66: c1 = *(uchar*)(str+1) ^ Tx;
	mov	a,#0x01
	add	a,_chartorune_PARM_2
	mov	r0,a
	clr	a
	addc	a,(_chartorune_PARM_2 + 1)
	mov	r1,a
	mov	r3,(_chartorune_PARM_2 + 2)
	mov	dpl,r0
	mov	dph,r1
	mov	b,r3
	lcall	__gptrget
	mov	r0,a
	xrl	ar0,#0x80
	mov	_chartorune_c1_1_53,r0
	mov	(_chartorune_c1_1_53 + 1),#0x00
;	libutf/rune.c:67: if(c1 & Testx)
	mov	a,_chartorune_c1_1_53
	anl	a,#0xC0
	jz	00168$
	ljmp	00127$
00168$:
;	libutf/rune.c:69: if(c < T3) {
	clr	c
	mov	a,_chartorune_c_1_53
	subb	a,#0xE0
	mov	a,(_chartorune_c_1_53 + 1)
	xrl	a,#0x80
	subb	a,#0x80
	jnc	00110$
;	libutf/rune.c:70: if(c < T2)
	clr	c
	mov	a,_chartorune_c_1_53
	subb	a,#0xC0
	mov	a,(_chartorune_c_1_53 + 1)
	xrl	a,#0x80
	subb	a,#0x80
	jnc	00170$
	ljmp	00127$
00170$:
;	libutf/rune.c:72: l = ((c << Bitx) | c1) & Rune2;
	mov	r1,_chartorune_c_1_53
	mov	a,(_chartorune_c_1_53 + 1)
	anl	a,#0x03
	mov	c,acc.0
	xch	a,r1
	rrc	a
	xch	a,r1
	rrc	a
	mov	c,acc.0
	xch	a,r1
	rrc	a
	xch	a,r1
	rrc	a
	xch	a,r1
	mov	r3,a
	mov	a,_chartorune_c1_1_53
	orl	ar1,a
	mov	a,(_chartorune_c1_1_53 + 1)
	orl	ar3,a
	anl	ar3,#0x07
	mov	a,r3
	mov	r0,a
	rlc	a
	subb	a,acc
	mov	r3,a
	mov	r7,a
;	libutf/rune.c:73: if(l <= Rune1)
	clr	c
	mov	a,#0x7F
	subb	a,r1
	clr	a
	subb	a,r0
	clr	a
	subb	a,r3
	mov	a,#(0x00 ^ 0x80)
	mov	b,r7
	xrl	b,#0x80
	subb	a,b
	jc	00171$
	ljmp	00127$
00171$:
;	libutf/rune.c:75: *rune = l;
	mov	ar5,r1
	mov	ar6,r0
	mov	dpl,_chartorune_rune_1_52
	mov	dph,(_chartorune_rune_1_52 + 1)
	mov	b,(_chartorune_rune_1_52 + 2)
	mov	a,r5
	lcall	__gptrput
	inc	dptr
	mov	a,r6
	lcall	__gptrput
;	libutf/rune.c:76: return 2;
	mov	dptr,#0x0002
	ret
00110$:
;	libutf/rune.c:83: c2 = *(uchar*)(str+2) ^ Tx;
	mov	a,#0x02
	add	a,_chartorune_PARM_2
	mov	r4,a
	clr	a
	addc	a,(_chartorune_PARM_2 + 1)
	mov	r5,a
	mov	r6,(_chartorune_PARM_2 + 2)
	mov	dpl,r4
	mov	dph,r5
	mov	b,r6
	lcall	__gptrget
	mov	r4,a
	xrl	ar4,#0x80
	mov	_chartorune_c2_1_53,r4
	mov	(_chartorune_c2_1_53 + 1),#0x00
;	libutf/rune.c:84: if(c2 & Testx)
	mov	a,_chartorune_c2_1_53
	anl	a,#0xC0
	jz	00173$
	ljmp	00127$
00173$:
;	libutf/rune.c:86: if(c < T4) {
	clr	c
	mov	a,_chartorune_c_1_53
	subb	a,#0xF0
	mov	a,(_chartorune_c_1_53 + 1)
	xrl	a,#0x80
	subb	a,#0x80
	jnc	00125$
;	libutf/rune.c:87: l = ((((c << Bitx) | c1) << Bitx) | c2) & Rune3;
	mov	r2,_chartorune_c_1_53
	mov	a,(_chartorune_c_1_53 + 1)
	anl	a,#0x03
	mov	c,acc.0
	xch	a,r2
	rrc	a
	xch	a,r2
	rrc	a
	mov	c,acc.0
	xch	a,r2
	rrc	a
	xch	a,r2
	rrc	a
	xch	a,r2
	mov	r5,a
	mov	a,_chartorune_c1_1_53
	orl	ar2,a
	mov	a,(_chartorune_c1_1_53 + 1)
	orl	ar5,a
	mov	a,r5
	anl	a,#0x03
	mov	c,acc.0
	xch	a,r2
	rrc	a
	xch	a,r2
	rrc	a
	mov	c,acc.0
	xch	a,r2
	rrc	a
	xch	a,r2
	rrc	a
	xch	a,r2
	mov	r5,a
	mov	a,_chartorune_c2_1_53
	orl	ar2,a
	mov	a,(_chartorune_c2_1_53 + 1)
	orl	ar5,a
	mov	ar1,r2
	mov	a,r5
	mov	r0,a
	rlc	a
	subb	a,acc
	mov	r3,a
	mov	r7,a
;	libutf/rune.c:88: if(l <= Rune2)
	clr	c
	mov	a,#0xFF
	subb	a,r1
	mov	a,#0x07
	subb	a,r0
	clr	a
	subb	a,r3
	mov	a,#(0x00 ^ 0x80)
	mov	b,r7
	xrl	b,#0x80
	subb	a,b
	jc	00175$
	ljmp	00127$
00175$:
;	libutf/rune.c:90: *rune = l;
	mov	ar2,r1
	mov	ar5,r0
	mov	dpl,_chartorune_rune_1_52
	mov	dph,(_chartorune_rune_1_52 + 1)
	mov	b,(_chartorune_rune_1_52 + 2)
	mov	a,r2
	lcall	__gptrput
	inc	dptr
	mov	a,r5
	lcall	__gptrput
;	libutf/rune.c:91: return 3;
	mov	dptr,#0x0003
	ret
;	libutf/rune.c:98: if(UTFmax >= 4) {
00125$:
;	libutf/rune.c:99: c3 = *(uchar*)(str+3) ^ Tx;
	mov	a,#0x03
	add	a,_chartorune_PARM_2
	mov	r2,a
	clr	a
	addc	a,(_chartorune_PARM_2 + 1)
	mov	r5,a
	mov	r6,(_chartorune_PARM_2 + 2)
	mov	dpl,r2
	mov	dph,r5
	mov	b,r6
	lcall	__gptrget
	mov	r2,a
	xrl	ar2,#0x80
	mov	r6,#0x00
;	libutf/rune.c:100: if(c3 & Testx)
	mov	a,r2
	anl	a,#0xC0
	jz	00177$
	ljmp	00127$
00177$:
;	libutf/rune.c:102: if(c < T5) {
	clr	c
	mov	a,_chartorune_c_1_53
	subb	a,#0xF8
	mov	a,(_chartorune_c_1_53 + 1)
	xrl	a,#0x80
	subb	a,#0x80
	jc	00178$
	ljmp	00127$
00178$:
;	libutf/rune.c:103: l = ((((((c << Bitx) | c1) << Bitx) | c2) << Bitx) | c3) & Rune4;
	mov	r4,_chartorune_c_1_53
	mov	a,(_chartorune_c_1_53 + 1)
	anl	a,#0x03
	mov	c,acc.0
	xch	a,r4
	rrc	a
	xch	a,r4
	rrc	a
	mov	c,acc.0
	xch	a,r4
	rrc	a
	xch	a,r4
	rrc	a
	xch	a,r4
	mov	r5,a
	mov	a,_chartorune_c1_1_53
	orl	ar4,a
	mov	a,(_chartorune_c1_1_53 + 1)
	orl	ar5,a
	mov	a,r5
	anl	a,#0x03
	mov	c,acc.0
	xch	a,r4
	rrc	a
	xch	a,r4
	rrc	a
	mov	c,acc.0
	xch	a,r4
	rrc	a
	xch	a,r4
	rrc	a
	xch	a,r4
	mov	r5,a
	mov	a,_chartorune_c2_1_53
	orl	ar4,a
	mov	a,(_chartorune_c2_1_53 + 1)
	orl	ar5,a
	mov	a,r5
	anl	a,#0x03
	mov	c,acc.0
	xch	a,r4
	rrc	a
	xch	a,r4
	rrc	a
	mov	c,acc.0
	xch	a,r4
	rrc	a
	xch	a,r4
	rrc	a
	xch	a,r4
	mov	r5,a
	mov	a,r4
	orl	ar2,a
	mov	a,r5
	orl	ar6,a
	mov	ar1,r2
	mov	a,r6
	mov	r0,a
	rlc	a
	subb	a,acc
	mov	r3,a
	mov	r7,a
;	libutf/rune.c:104: if(l <= Rune3)
	clr	c
	mov	a,#0xFF
	subb	a,r1
	mov	a,#0xFF
	subb	a,r0
	mov	a,#0xFF
	subb	a,r3
	mov	a,#(0xFF ^ 0x80)
	mov	b,r7
	xrl	b,#0x80
	subb	a,b
	jnc	00127$
;	libutf/rune.c:106: if(l > Runemax)
	clr	c
	mov	a,#0xFF
	subb	a,r1
	mov	a,#0xFF
	subb	a,r0
	mov	a,#0x10
	subb	a,r3
	mov	a,#(0x00 ^ 0x80)
	mov	b,r7
	xrl	b,#0x80
	subb	a,b
	jc	00127$
;	libutf/rune.c:108: *rune = l;
	mov	dpl,_chartorune_rune_1_52
	mov	dph,(_chartorune_rune_1_52 + 1)
	mov	b,(_chartorune_rune_1_52 + 2)
	mov	a,r1
	lcall	__gptrput
	inc	dptr
	mov	a,r0
	lcall	__gptrput
;	libutf/rune.c:109: return 4;
	mov	dptr,#0x0004
;	libutf/rune.c:116: bad:
	ret
00127$:
;	libutf/rune.c:117: *rune = Bad;
	mov	dpl,_chartorune_rune_1_52
	mov	dph,(_chartorune_rune_1_52 + 1)
	mov	b,(_chartorune_rune_1_52 + 2)
	mov	a,#0xFD
	lcall	__gptrput
	inc	dptr
	mov	a,#0xFF
	lcall	__gptrput
;	libutf/rune.c:118: return 1;
	mov	dptr,#0x0001
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'runetochar'
;------------------------------------------------------------
;rune                      Allocated with name '_runetochar_PARM_2'
;str                       Allocated with name '_runetochar_str_1_59'
;c                         Allocated with name '_runetochar_c_1_60'
;------------------------------------------------------------
;	libutf/rune.c:122: runetochar(char *str, Rune *rune)
;	-----------------------------------------
;	 function runetochar
;	-----------------------------------------
_runetochar:
	mov	_runetochar_str_1_59,dpl
	mov	(_runetochar_str_1_59 + 1),dph
	mov	(_runetochar_str_1_59 + 2),b
;	libutf/rune.c:130: c = *rune;
	mov	r2,_runetochar_PARM_2
	mov	r3,(_runetochar_PARM_2 + 1)
	mov	r4,(_runetochar_PARM_2 + 2)
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
	mov	r2,a
	inc	dptr
	lcall	__gptrget
	mov	r3,a
	mov	_runetochar_c_1_60,r2
	mov	(_runetochar_c_1_60 + 1),r3
	mov	(_runetochar_c_1_60 + 2),#0x00
	mov	(_runetochar_c_1_60 + 3),#0x00
;	libutf/rune.c:131: if(c <= Rune1) {
	clr	c
	mov	a,#0x7F
	subb	a,_runetochar_c_1_60
	clr	a
	subb	a,(_runetochar_c_1_60 + 1)
	clr	a
	subb	a,(_runetochar_c_1_60 + 2)
	mov	a,#(0x00 ^ 0x80)
	mov	b,(_runetochar_c_1_60 + 3)
	xrl	b,#0x80
	subb	a,b
	jc	00102$
;	libutf/rune.c:132: str[0] = c;
	mov	r0,_runetochar_c_1_60
	mov	dpl,_runetochar_str_1_59
	mov	dph,(_runetochar_str_1_59 + 1)
	mov	b,(_runetochar_str_1_59 + 2)
	mov	a,r0
	lcall	__gptrput
;	libutf/rune.c:133: return 1;
	mov	dptr,#0x0001
	ret
00102$:
;	libutf/rune.c:140: if(c <= Rune2) {
	clr	c
	mov	a,#0xFF
	subb	a,_runetochar_c_1_60
	mov	a,#0x07
	subb	a,(_runetochar_c_1_60 + 1)
	clr	a
	subb	a,(_runetochar_c_1_60 + 2)
	mov	a,#(0x00 ^ 0x80)
	mov	b,(_runetochar_c_1_60 + 3)
	xrl	b,#0x80
	subb	a,b
	jc	00104$
;	libutf/rune.c:141: str[0] = T2 | (c >> 1*Bitx);
	mov	r0,_runetochar_c_1_60
	mov	a,(_runetochar_c_1_60 + 1)
	mov	c,acc.7
	xch	a,r0
	rlc	a
	xch	a,r0
	rlc	a
	mov	c,acc.7
	xch	a,r0
	rlc	a
	xch	a,r0
	rlc	a
	xch	a,r0
	anl	a,#0x03
	mov	r5,a
	mov	a,(_runetochar_c_1_60 + 2)
	add	a,(_runetochar_c_1_60 + 2)
	add	a,acc
	orl	a,r5
	mov	r5,a
	mov	r6,(_runetochar_c_1_60 + 2)
	mov	a,(_runetochar_c_1_60 + 3)
	mov	c,acc.7
	xch	a,r6
	rlc	a
	xch	a,r6
	rlc	a
	mov	c,acc.7
	xch	a,r6
	rlc	a
	xch	a,r6
	rlc	a
	xch	a,r6
	anl	a,#0x03
	jnb	acc.1,00125$
	orl	a,#0xFC
00125$:
	orl	ar0,#0xC0
	mov	dpl,_runetochar_str_1_59
	mov	dph,(_runetochar_str_1_59 + 1)
	mov	b,(_runetochar_str_1_59 + 2)
	mov	a,r0
	lcall	__gptrput
;	libutf/rune.c:142: str[1] = Tx | (c & Maskx);
	mov	a,#0x01
	add	a,_runetochar_str_1_59
	mov	r5,a
	clr	a
	addc	a,(_runetochar_str_1_59 + 1)
	mov	r6,a
	mov	r7,(_runetochar_str_1_59 + 2)
	mov	a,#0x3F
	anl	a,_runetochar_c_1_60
	mov	r0,a
	orl	ar0,#0x80
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	mov	a,r0
	lcall	__gptrput
;	libutf/rune.c:143: return 2;
	mov	dptr,#0x0002
	ret
00104$:
;	libutf/rune.c:150: if(c > Runemax)
	clr	c
	mov	a,#0xFF
	subb	a,_runetochar_c_1_60
	mov	a,#0xFF
	subb	a,(_runetochar_c_1_60 + 1)
	mov	a,#0x10
	subb	a,(_runetochar_c_1_60 + 2)
	mov	a,#(0x00 ^ 0x80)
	mov	b,(_runetochar_c_1_60 + 3)
	xrl	b,#0x80
	subb	a,b
	jnc	00106$
;	libutf/rune.c:151: c = Runeerror;
	mov	_runetochar_c_1_60,#0xFD
	mov	(_runetochar_c_1_60 + 1),#0xFF
	clr	a
	mov	(_runetochar_c_1_60 + 2),a
	mov	(_runetochar_c_1_60 + 3),a
00106$:
;	libutf/rune.c:152: if(c <= Rune3) {
	clr	c
	mov	a,#0xFF
	subb	a,_runetochar_c_1_60
	mov	a,#0xFF
	subb	a,(_runetochar_c_1_60 + 1)
	mov	a,#0xFF
	subb	a,(_runetochar_c_1_60 + 2)
	mov	a,#(0xFF ^ 0x80)
	mov	b,(_runetochar_c_1_60 + 3)
	xrl	b,#0x80
	subb	a,b
	jnc	00127$
	ljmp	00108$
00127$:
;	libutf/rune.c:153: str[0] = T3 |  (c >> 2*Bitx);
	mov	r4,(_runetochar_c_1_60 + 1)
	mov	a,(_runetochar_c_1_60 + 2)
	swap	a
	xch	a,r4
	swap	a
	anl	a,#0x0F
	xrl	a,r4
	xch	a,r4
	anl	a,#0x0F
	xch	a,r4
	xrl	a,r4
	xch	a,r4
	mov	r5,a
	mov	a,(_runetochar_c_1_60 + 3)
	swap	a
	anl	a,#0xF0
	orl	a,r5
	mov	r5,a
	mov	a,(_runetochar_c_1_60 + 3)
	swap	a
	anl	a,#0x0F
	jnb	acc.3,00128$
	orl	a,#0xF0
00128$:
	rlc	a
	subb	a,acc
	orl	ar4,#0xE0
	mov	dpl,_runetochar_str_1_59
	mov	dph,(_runetochar_str_1_59 + 1)
	mov	b,(_runetochar_str_1_59 + 2)
	mov	a,r4
	lcall	__gptrput
;	libutf/rune.c:154: str[1] = Tx | ((c >> 1*Bitx) & Maskx);
	mov	a,#0x01
	add	a,_runetochar_str_1_59
	mov	r5,a
	clr	a
	addc	a,(_runetochar_str_1_59 + 1)
	mov	r6,a
	mov	r7,(_runetochar_str_1_59 + 2)
	mov	r1,_runetochar_c_1_60
	mov	a,(_runetochar_c_1_60 + 1)
	mov	c,acc.7
	xch	a,r1
	rlc	a
	xch	a,r1
	rlc	a
	mov	c,acc.7
	xch	a,r1
	rlc	a
	xch	a,r1
	rlc	a
	xch	a,r1
	anl	a,#0x03
	mov	r2,a
	mov	a,(_runetochar_c_1_60 + 2)
	add	a,(_runetochar_c_1_60 + 2)
	add	a,acc
	orl	a,r2
	mov	r2,a
	mov	r3,(_runetochar_c_1_60 + 2)
	mov	a,(_runetochar_c_1_60 + 3)
	mov	c,acc.7
	xch	a,r3
	rlc	a
	xch	a,r3
	rlc	a
	mov	c,acc.7
	xch	a,r3
	rlc	a
	xch	a,r3
	rlc	a
	xch	a,r3
	anl	a,#0x03
	jnb	acc.1,00129$
	orl	a,#0xFC
00129$:
	anl	ar1,#0x3F
	orl	ar1,#0x80
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	mov	a,r1
	lcall	__gptrput
;	libutf/rune.c:155: str[2] = Tx |  (c & Maskx);
	mov	a,#0x02
	add	a,_runetochar_str_1_59
	mov	r5,a
	clr	a
	addc	a,(_runetochar_str_1_59 + 1)
	mov	r6,a
	mov	r7,(_runetochar_str_1_59 + 2)
	mov	a,#0x3F
	anl	a,_runetochar_c_1_60
	mov	r1,a
	orl	ar1,#0x80
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	mov	a,r1
	lcall	__gptrput
;	libutf/rune.c:156: return 3;
	mov	dptr,#0x0003
	ret
00108$:
;	libutf/rune.c:163: str[0] = T4 |  (c >> 3*Bitx);
	mov	r4,(_runetochar_c_1_60 + 2)
	mov	a,(_runetochar_c_1_60 + 3)
	mov	c,acc.7
	rrc	a
	xch	a,r4
	rrc	a
	xch	a,r4
	mov	c,acc.7
	rrc	a
	xch	a,r4
	rrc	a
	xch	a,r4
	rlc	a
	subb	a,acc
	orl	ar4,#0xF0
	mov	dpl,_runetochar_str_1_59
	mov	dph,(_runetochar_str_1_59 + 1)
	mov	b,(_runetochar_str_1_59 + 2)
	mov	a,r4
	lcall	__gptrput
;	libutf/rune.c:164: str[1] = Tx | ((c >> 2*Bitx) & Maskx);
	mov	a,#0x01
	add	a,_runetochar_str_1_59
	mov	r5,a
	clr	a
	addc	a,(_runetochar_str_1_59 + 1)
	mov	r6,a
	mov	r7,(_runetochar_str_1_59 + 2)
	mov	r1,(_runetochar_c_1_60 + 1)
	mov	a,(_runetochar_c_1_60 + 2)
	swap	a
	xch	a,r1
	swap	a
	anl	a,#0x0F
	xrl	a,r1
	xch	a,r1
	anl	a,#0x0F
	xch	a,r1
	xrl	a,r1
	xch	a,r1
	mov	r2,a
	mov	a,(_runetochar_c_1_60 + 3)
	swap	a
	anl	a,#0xF0
	orl	a,r2
	mov	r2,a
	mov	a,(_runetochar_c_1_60 + 3)
	swap	a
	anl	a,#0x0F
	jnb	acc.3,00130$
	orl	a,#0xF0
00130$:
	rlc	a
	subb	a,acc
	anl	ar1,#0x3F
	mov	r4,#0x00
	orl	ar1,#0x80
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	mov	a,r1
	lcall	__gptrput
;	libutf/rune.c:165: str[2] = Tx | ((c >> 1*Bitx) & Maskx);
	mov	a,#0x02
	add	a,_runetochar_str_1_59
	mov	r5,a
	clr	a
	addc	a,(_runetochar_str_1_59 + 1)
	mov	r6,a
	mov	r7,(_runetochar_str_1_59 + 2)
	mov	r1,_runetochar_c_1_60
	mov	a,(_runetochar_c_1_60 + 1)
	mov	c,acc.7
	xch	a,r1
	rlc	a
	xch	a,r1
	rlc	a
	mov	c,acc.7
	xch	a,r1
	rlc	a
	xch	a,r1
	rlc	a
	xch	a,r1
	anl	a,#0x03
	mov	r2,a
	mov	a,(_runetochar_c_1_60 + 2)
	add	a,(_runetochar_c_1_60 + 2)
	add	a,acc
	orl	a,r2
	mov	r2,a
	mov	r3,(_runetochar_c_1_60 + 2)
	mov	a,(_runetochar_c_1_60 + 3)
	mov	c,acc.7
	xch	a,r3
	rlc	a
	xch	a,r3
	rlc	a
	mov	c,acc.7
	xch	a,r3
	rlc	a
	xch	a,r3
	rlc	a
	xch	a,r3
	anl	a,#0x03
	jnb	acc.1,00131$
	orl	a,#0xFC
00131$:
	anl	ar1,#0x3F
	orl	ar1,#0x80
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	mov	a,r1
	lcall	__gptrput
;	libutf/rune.c:166: str[3] = Tx |  (c & Maskx);
	mov	a,#0x03
	add	a,_runetochar_str_1_59
	mov	r5,a
	clr	a
	addc	a,(_runetochar_str_1_59 + 1)
	mov	r6,a
	mov	r7,(_runetochar_str_1_59 + 2)
	mov	a,#0x3F
	anl	a,_runetochar_c_1_60
	mov	r1,a
	orl	ar1,#0x80
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	mov	a,r1
	lcall	__gptrput
;	libutf/rune.c:167: return 4;
	mov	dptr,#0x0004
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'runelen'
;------------------------------------------------------------
;c                         Allocated to registers r4 r5 r6 r7 
;rune                      Allocated with name '_runelen_rune_1_65'
;str                       Allocated with name '_runelen_str_1_65'
;------------------------------------------------------------
;	libutf/rune.c:171: runelen(long c)
;	-----------------------------------------
;	 function runelen
;	-----------------------------------------
_runelen:
	mov	r4,dpl
	mov	r5,dph
;	libutf/rune.c:176: rune = c;
	mov	_runelen_rune_1_65,r4
	mov	(_runelen_rune_1_65 + 1),r5
;	libutf/rune.c:177: return runetochar(str, &rune);
	mov	_runetochar_PARM_2,#_runelen_rune_1_65
	mov	(_runetochar_PARM_2 + 1),#0x00
	mov	(_runetochar_PARM_2 + 2),#0x40
	mov	dptr,#_runelen_str_1_65
	mov	b,#0x40
	ljmp	_runetochar
;------------------------------------------------------------
;Allocation info for local variables in function 'runenlen'
;------------------------------------------------------------
;nrune                     Allocated with name '_runenlen_PARM_2'
;r                         Allocated to registers 
;nb                        Allocated with name '_runenlen_nb_1_67'
;c                         Allocated to registers r3 r4 
;------------------------------------------------------------
;	libutf/rune.c:181: runenlen(Rune *r, int nrune)
;	-----------------------------------------
;	 function runenlen
;	-----------------------------------------
_runenlen:
	mov	r5,dpl
	mov	r6,dph
	mov	r7,b
;	libutf/rune.c:185: nb = 0;
	clr	a
	mov	_runenlen_nb_1_67,a
	mov	(_runenlen_nb_1_67 + 1),a
;	libutf/rune.c:186: while(nrune--) {
	mov	r1,_runenlen_PARM_2
	mov	r2,(_runenlen_PARM_2 + 1)
00111$:
	mov	ar0,r1
	mov	ar4,r2
	dec	r1
	cjne	r1,#0xFF,00132$
	dec	r2
00132$:
	mov	a,r0
	orl	a,r4
	jnz	00133$
	ljmp	00113$
00133$:
;	libutf/rune.c:187: c = *r++;
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	r3,a
	inc	dptr
	lcall	__gptrget
	mov	r4,a
	inc	dptr
	mov	r5,dpl
	mov	r6,dph
;	libutf/rune.c:188: if(c <= Rune1)
	clr	c
	mov	a,#0x7F
	subb	a,r3
	mov	a,#(0x00 ^ 0x80)
	mov	b,r4
	xrl	b,#0x80
	subb	a,b
	jc	00109$
;	libutf/rune.c:189: nb++;
	inc	_runenlen_nb_1_67
	clr	a
	cjne	a,_runenlen_nb_1_67,00111$
	inc	(_runenlen_nb_1_67 + 1)
	sjmp	00111$
00109$:
;	libutf/rune.c:191: if(c <= Rune2)
	clr	c
	mov	a,#0xFF
	subb	a,r3
	mov	a,#(0x07 ^ 0x80)
	mov	b,r4
	xrl	b,#0x80
	subb	a,b
	jc	00106$
;	libutf/rune.c:192: nb += 2;
	mov	a,#0x02
	add	a,_runenlen_nb_1_67
	mov	_runenlen_nb_1_67,a
	clr	a
	addc	a,(_runenlen_nb_1_67 + 1)
	mov	(_runenlen_nb_1_67 + 1),a
	sjmp	00111$
00106$:
;	libutf/rune.c:194: if(c <= Rune3 || c > Runemax)
	clr	c
	mov	a,#0xFF
	subb	a,r3
	mov	a,#(0xFF ^ 0x80)
	mov	b,r4
	xrl	b,#0x80
	subb	a,b
	jc	00102$
;	libutf/rune.c:195: nb += 3;
	mov	a,#0x03
	add	a,_runenlen_nb_1_67
	mov	_runenlen_nb_1_67,a
	clr	a
	addc	a,(_runenlen_nb_1_67 + 1)
	mov	(_runenlen_nb_1_67 + 1),a
	ljmp	00111$
00102$:
;	libutf/rune.c:197: nb += 4;
	mov	a,#0x04
	add	a,_runenlen_nb_1_67
	mov	_runenlen_nb_1_67,a
	clr	a
	addc	a,(_runenlen_nb_1_67 + 1)
	mov	(_runenlen_nb_1_67 + 1),a
	ljmp	00111$
00113$:
;	libutf/rune.c:199: return nb;
	mov	dpl,_runenlen_nb_1_67
	mov	dph,(_runenlen_nb_1_67 + 1)
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'fullrune'
;------------------------------------------------------------
;n                         Allocated with name '_fullrune_PARM_2'
;str                       Allocated to registers r5 r6 r7 
;c                         Allocated to registers r5 r7 
;------------------------------------------------------------
;	libutf/rune.c:203: fullrune(char *str, int n)
;	-----------------------------------------
;	 function fullrune
;	-----------------------------------------
_fullrune:
	mov	r5,dpl
	mov	r6,dph
	mov	r7,b
;	libutf/rune.c:207: if(n <= 0)
	clr	c
	clr	a
	subb	a,_fullrune_PARM_2
	mov	a,#(0x00 ^ 0x80)
	mov	b,(_fullrune_PARM_2 + 1)
	xrl	b,#0x80
	subb	a,b
	jc	00102$
;	libutf/rune.c:208: return 0;
	mov	dptr,#0x0000
	ret
00102$:
;	libutf/rune.c:209: c = *(uchar*)str;
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	r5,a
	mov	r7,#0x00
;	libutf/rune.c:210: if(c < Tx)
	clr	c
	mov	a,r5
	subb	a,#0x80
	mov	a,r7
	xrl	a,#0x80
	subb	a,#0x80
	jnc	00104$
;	libutf/rune.c:211: return 1;
	mov	dptr,#0x0001
	ret
00104$:
;	libutf/rune.c:212: if(c < T3)
	clr	c
	mov	a,r5
	subb	a,#0xE0
	mov	a,r7
	xrl	a,#0x80
	subb	a,#0x80
	jnc	00109$
;	libutf/rune.c:213: return n >= 2;
	clr	c
	mov	a,_fullrune_PARM_2
	subb	a,#0x02
	mov	a,(_fullrune_PARM_2 + 1)
	xrl	a,#0x80
	subb	a,#0x80
	cpl	c
	clr	a
	rlc	a
	mov	r6,a
	rlc	a
	subb	a,acc
	mov	r4,a
	mov	dpl,r6
	mov	dph,r4
;	libutf/rune.c:214: if(UTFmax == 3 || c < T4)
	ret
00109$:
	clr	c
	mov	a,r5
	subb	a,#0xF0
	mov	a,r7
	xrl	a,#0x80
	subb	a,#0x80
	jnc	00108$
;	libutf/rune.c:215: return n >= 3;
	clr	c
	mov	a,_fullrune_PARM_2
	subb	a,#0x03
	mov	a,(_fullrune_PARM_2 + 1)
	xrl	a,#0x80
	subb	a,#0x80
	cpl	c
	clr	a
	rlc	a
	mov	r7,a
	rlc	a
	subb	a,acc
	mov	r6,a
	mov	dpl,r7
	mov	dph,r6
	ret
00108$:
;	libutf/rune.c:216: return n >= 4;
	clr	c
	mov	a,_fullrune_PARM_2
	subb	a,#0x04
	mov	a,(_fullrune_PARM_2 + 1)
	xrl	a,#0x80
	subb	a,#0x80
	cpl	c
	clr	a
	rlc	a
	mov	r7,a
	rlc	a
	subb	a,acc
	mov	dpl,r7
	mov	dph,a
	ret
	.area CSEG    (CODE)
	.area CONST   (CODE)
	.area XINIT   (CODE)
	.area CABS    (ABS,CODE)

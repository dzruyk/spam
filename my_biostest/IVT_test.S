.code16
.globl _start

INIT_OFFSET = 0x7c00

_start:
	//first we need to set IVT interpret handler
	xorw	%ax,		%ax
	movw	%ax,		%es
	movw	$0,		0x2
	//count offset
	movw	$_dbz_ih,	%ax
	addw	$INIT_OFFSET,	%ax
	movw	%ax,		0x0

	movw	$0x7c0,		%ax
	movw	%ax,		%es
	movw	%ax,		%ds

	movw	$0,		%ax
	movw	%ax,		%ds
	movw	%ax,		%es
	movw	$0x1,		%ax
	movw	$0x0,		%cx
	divw	%cx
	nop
1:
	nop
	nop
	jmp 1b

_dbz_ih:
	movw	$0x0,		%dx
	movw	$0x0007,	%bx
	movw	$strsz,		%cx
	movw	$str,		%ax
	addw	$INIT_OFFSET,	%ax
	movw	%ax,		%bp
	movw	$0x1301,	%ax
	int	$0x10
	iret
	str:
	.ascii	"DZ_lnly was here\n"
	.set strsz, . - str - 1


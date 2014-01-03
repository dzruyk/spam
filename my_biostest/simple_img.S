.code16
.globl _start
_start:
	movw	$0x0007,	%bx
	movw	$strsz,		%cx
	#movw	$0x7c0,		%ax
	xorw	%ax,		%ax
	movw	%ax,		%es
	movw	$str,		%bp
	movw	$0x1301,	%ax
	int	$0x10
	incb	%dh
	cmpb	$20,		%dh
	jne	_start
	movb	$0,		%dh
	jmp	_start
	str:
	.ascii	"DZ_lnly was here\n"
	.set strsz, . - str - 1


.data
msg:
	.asciz	"hello\n"
.set msg_len, . - msg - 1

.globl _start	
.text
_start:
	xorl 	%eax,	%eax
	
	#print_msg
	movl	$4,	%eax
	movl	$1,	%ebx
	movl	$msg,	%ecx
	movl	$msg_len,%edx
	int	$0x80

	#EXIT(0)
	movl	$1,	%eax
	movl	$0,	%ebx
	
	int	$0x80

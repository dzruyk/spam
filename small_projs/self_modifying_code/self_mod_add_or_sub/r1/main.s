	.file	"main.c"
	.text
	.globl	func
	.type	func, @function
func:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
#APP
# 19 "main.c" 1
	movl 8(%ebp), %eax			
call get_ip				
get_ip: 					
addl 12(%esp), %eax			
popl %edx					
xorl $0x28, (%edx)			

# 0 "" 2
#NO_APP
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	func, .-func
	.globl	func_end
	.type	func_end, @function
func_end:
.LFB1:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	func_end, .-func_end
	.section	.rodata
.LC0:
	.string	"mprotect"
.LC1:
	.string	"%d) %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	subl	$32, %esp
	movl	$30, (%esp)
	call	sysconf
	movl	%eax, 24(%esp)
	movl	24(%esp), %edx
	movl	24(%esp), %eax
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	memalign
	movl	%eax, 20(%esp)
	movl	$func_end, %edx
	movl	$func, %eax
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	%ecx, %eax
	movl	%eax, 8(%esp)
	movl	$func, 4(%esp)
	movl	20(%esp), %eax
	movl	%eax, (%esp)
	call	memcpy
	movl	$func_end, %edx
	movl	$func, %eax
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	%ecx, %eax
	movl	$7, 8(%esp)
	movl	%eax, 4(%esp)
	movl	20(%esp), %eax
	movl	%eax, (%esp)
	call	mprotect
	testl	%eax, %eax
	je	.L4
	movl	$.LC0, (%esp)
	call	perror
.L4:
	movl	$0, 28(%esp)
	jmp	.L5
.L6:
	movl	$4, 4(%esp)
	movl	$2, (%esp)
	movl	20(%esp), %eax
	call	*%eax
	movl	%eax, 8(%esp)
	movl	28(%esp), %eax
	movl	%eax, 4(%esp)
	movl	$.LC1, (%esp)
	call	printf
	addl	$1, 28(%esp)
.L5:
	cmpl	$9, 28(%esp)
	jle	.L6
	movl	$0, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (Debian 4.7.2-4) 4.7.2"
	.section	.note.GNU-stack,"",@progbits

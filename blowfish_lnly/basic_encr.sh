#!/bin/sh

function pre()
{
	cat <<-EOF
	pushl	%ebp
	movl	%esp, %ebp
	
	EOF
}

function post()
{
	cat <<-EOF
	movl    %ebp, %esp
	popl	%ebp
	
	EOF
}

function push_sidibx()
{
	cat <<-EOF
	pushl	%esi
	pushl	%edi
	pushl	%ebx
	
	EOF
}

function pop_sidibx()
{
	cat <<-EOF
	popl	%ebx
	popl	%edi
	popl	%esi
	
	EOF
}

# EAX EDX
function swap()
{
	cat <<-EOF
	movl	$1, %eax
	movl	$2, %edx
	movl	%edx, $1
	movl	%eax, $2
	
	EOF
}

#used ESI=X EDI=P EBX=SBOX
# EDX
function bf_feistel_round()
{
	let "num= ($1 % 2) * 4"	# зависит от чётности/нечётности помогает избавится от свапа
	cat <<-EOF
	movl	\$$1, %eax
	leal	(%edi, %eax, 4), %ecx
	movl	(%ecx), %eax
	xorl	%eax, $num(%esi)
	
	movl	$num(%esi), %eax
	shrl	\$24, %eax
	leal	(%ebx, %eax, 4), %ecx
	movl	(%ecx), %edx
	
	movl	$num(%esi), %eax
	andl	\$0xff0000, %eax
	shrl	\$16, %eax
	addl	\$256, %eax
	leal	(%ebx, %eax, 4), %ecx
	
	addl	(%ecx), %edx
	
	movl	$num(%esi), %eax
	andl	\$0xff00, %eax
	shrl	\$8, %eax
	addl	\$512, %eax
	leal	(%ebx, %eax, 4), %ecx
	
	xorl	(%ecx), %edx
	
	movl	$num(%esi), %eax
	andl	\$0xff, %eax
	addl	\$768, %eax
	leal	(%ebx, %eax, 4), %ecx
	
	addl	(%ecx), %edx
	
	xorl	%edx, `expr 4 - $num`(%esi)
	
	EOF
	#   swap "(%esi)" "4(%esi)"

}

#used ESI=X EDI=P EBX=SBOX
function blowfish_basic_encrypt()
{
	pre
	push_sidibx
	
	cat <<-EOF
	movl	8(%ebp), %esi
	movl	12(%ebp), %edi
	movl	16(%ebp), %ebx
	
	EOF
	for((i = 0; i < 16; i++))
	do
		bf_feistel_round $i
	done
	swap "(%esi)" "4(%esi)"
	cat <<-EOF
	movl	64(%edi), %eax
	xorl	%eax, 4(%esi)
	
	movl	68(%edi), %eax
	xorl	%eax, (%esi)
	EOF
	pop_sidibx
	post
	cat <<-EOF
	ret
	EOF
}

function main()
{
	cat <<-EOF
	.text
	.globl blowfish_basic_encrypt
	blowfish_basic_encrypt:
	EOF
}

main
blowfish_basic_encrypt

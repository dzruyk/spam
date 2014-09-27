/* See COPYRIGHT for copyright information. */

#include <inc/mmu.h>
#include <inc/memlayout.h>
#include <inc/trap.h>

#include <kern/picirq.h>


###################################################################
# exceptions/interrupts
###################################################################

/* TRAPHANDLER defines a globally-visible function for handling a trap.
 * It pushes a trap number onto the stack, then jumps to _alltraps.
 * Use TRAPHANDLER for traps where the CPU automatically pushes an error code.
 *
 * You shouldn't call a TRAPHANDLER function from C, but you may
 * need to _declare_ one in C (for instance, to get a function pointer
 * during IDT setup).  You can declare the function with
 *   void NAME();
 * where NAME is the argument passed to TRAPHANDLER.
 */
#define TRAPHANDLER(name, num)						\
	.globl name;		/* define global symbol for 'name' */	\
	.type name, @function;	/* symbol type is function */		\
	.align 2;		/* align function definition */		\
	name:			/* function starts here */		\
	pushl $(num);							\
	jmp _alltraps

/* Use TRAPHANDLER_NOEC for traps where the CPU doesn't push an error code.
 * It pushes a 0 in place of the error code, so the trap frame has the same
 * format in either case.
 */
#define TRAPHANDLER_NOEC(name, num)					\
	.globl name;							\
	.type name, @function;						\
	.align 2;							\
	name:								\
	pushl $0;							\
	pushl $(num);							\
	jmp _alltraps

.text

/*
 * Lab 3: Your code here for generating entry points for the different traps.
 */

TRAPHANDLER_NOEC(traphndlr_0, T_DIVIDE);
TRAPHANDLER_NOEC(traphndlr_1, T_DEBUG);
TRAPHANDLER_NOEC(traphndlr_3, T_BRKPT);
TRAPHANDLER_NOEC(traphndlr_4, T_OFLOW);
TRAPHANDLER_NOEC(traphndlr_5, T_BOUND);
TRAPHANDLER_NOEC(traphndlr_6, T_ILLOP);
TRAPHANDLER_NOEC(traphndlr_7, T_DEVICE);
TRAPHANDLER(traphndlr_8, T_DBLFLT);
TRAPHANDLER(traphndlr_10, T_TSS);
TRAPHANDLER(traphndlr_11, T_SEGNP);
TRAPHANDLER(traphndlr_12, T_STACK);
TRAPHANDLER(traphndlr_13, T_GPFLT);
TRAPHANDLER(traphndlr_14, T_PGFLT);
TRAPHANDLER_NOEC(traphndlr_16, T_FPERR);
TRAPHANDLER_NOEC(traphndlr_30, T_SYSCALL);

TRAPHANDLER_NOEC(traphndlr_i0, IRQ_OFFSET + 0);
TRAPHANDLER_NOEC(traphndlr_i1, IRQ_OFFSET + 1);
TRAPHANDLER_NOEC(traphndlr_i2, IRQ_OFFSET + 2);
TRAPHANDLER_NOEC(traphndlr_i3, IRQ_OFFSET + 3);
TRAPHANDLER_NOEC(traphndlr_i4, IRQ_OFFSET + 4);
TRAPHANDLER_NOEC(traphndlr_i5, IRQ_OFFSET + 5);
TRAPHANDLER_NOEC(traphndlr_i6, IRQ_OFFSET + 6);
TRAPHANDLER_NOEC(traphndlr_i7, IRQ_OFFSET + 7);
TRAPHANDLER_NOEC(traphndlr_i8, IRQ_OFFSET + 8);
TRAPHANDLER_NOEC(traphndlr_i9, IRQ_OFFSET + 9);
TRAPHANDLER_NOEC(traphndlr_i10, IRQ_OFFSET + 10);
TRAPHANDLER_NOEC(traphndlr_i11, IRQ_OFFSET + 11);
TRAPHANDLER_NOEC(traphndlr_i12, IRQ_OFFSET + 12);
TRAPHANDLER_NOEC(traphndlr_i13, IRQ_OFFSET + 13);
TRAPHANDLER_NOEC(traphndlr_i14, IRQ_OFFSET + 14);
TRAPHANDLER_NOEC(traphndlr_i15, IRQ_OFFSET + 15);

/*
 * Lab 3: Your code here for _alltraps
 */
.globl _alltraps

_alltraps:

	pushl	%ds
	pushl	%es
	pushal
	//load GD_KD %ds %es
	movw	$GD_KD,	%ax
	movw	%ax,	%es
	movw	%ax,	%ds
	pushl	%esp
	call	trap

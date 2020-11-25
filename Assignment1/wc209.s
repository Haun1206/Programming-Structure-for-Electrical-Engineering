	.file	"wc209.c"
	.section	.rodata
.LC0:
	.string	"wc209.c"
.LC1:
	.string	"0"
	.align 8
.LC2:
	.string	"Error: line %d: unterminated comment\n"
.LC3:
	.string	"%d %d %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	$0, -4(%rbp)
	movl	$0, -28(%rbp)
	movl	$0, -24(%rbp)
	movl	$0, -20(%rbp)
	movl	$1, -12(%rbp)
	movl	$1, -8(%rbp)
	jmp	.L2
.L28:
	cmpl	$10, -4(%rbp)
	jne	.L3
	addl	$1, -28(%rbp)
.L3:
	cmpl	$4, -12(%rbp)
	ja	.L4
	movl	-12(%rbp), %eax
	movq	.L6(,%rax,8), %rax
	jmp	*%rax
	.section	.rodata
	.align 8
	.align 4
.L6:
	.quad	.L5
	.quad	.L7
	.quad	.L8
	.quad	.L9
	.quad	.L10
	.text
.L5:
	cmpl	$47, -4(%rbp)
	jne	.L11
	movl	$2, -12(%rbp)
	movl	$0, -8(%rbp)
	jmp	.L12
.L11:
	call	__ctype_b_loc
	movq	(%rax), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	je	.L12
	movl	$1, -12(%rbp)
.L12:
	addl	$1, -20(%rbp)
	jmp	.L2
.L7:
	call	__ctype_b_loc
	movq	(%rax), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	jne	.L13
	cmpl	$47, -4(%rbp)
	jne	.L14
	movl	$2, -12(%rbp)
	movl	$1, -8(%rbp)
	addl	$1, -20(%rbp)
	jmp	.L2
.L14:
	movl	$0, -12(%rbp)
	addl	$1, -24(%rbp)
.L13:
	addl	$1, -20(%rbp)
	jmp	.L2
.L8:
	cmpl	$42, -4(%rbp)
	jne	.L15
	movl	$3, -12(%rbp)
	movl	-28(%rbp), %eax
	movl	%eax, -16(%rbp)
	jmp	.L2
.L15:
	cmpl	$1, -8(%rbp)
	jne	.L17
	addl	$1, -24(%rbp)
.L17:
	cmpl	$47, -4(%rbp)
	jne	.L18
	movl	$2, -12(%rbp)
	movl	$2, -8(%rbp)
	jmp	.L19
.L18:
	movl	$0, -12(%rbp)
.L19:
	addl	$1, -20(%rbp)
	jmp	.L2
.L9:
	cmpl	$10, -4(%rbp)
	je	.L20
	cmpl	$12, -4(%rbp)
	je	.L20
	cmpl	$13, -4(%rbp)
	je	.L20
	cmpl	$9, -4(%rbp)
	je	.L20
	cmpl	$11, -4(%rbp)
	jne	.L21
.L20:
	addl	$1, -20(%rbp)
.L21:
	cmpl	$42, -4(%rbp)
	jne	.L2
	movl	$4, -12(%rbp)
	jmp	.L2
.L10:
	cmpl	$47, -4(%rbp)
	jne	.L23
	movl	$1, -12(%rbp)
	jmp	.L2
.L23:
	cmpl	$42, -4(%rbp)
	jne	.L25
	movl	$4, -12(%rbp)
	jmp	.L2
.L25:
	cmpl	$10, -4(%rbp)
	je	.L26
	cmpl	$12, -4(%rbp)
	je	.L26
	cmpl	$13, -4(%rbp)
	je	.L26
	cmpl	$9, -4(%rbp)
	je	.L26
	cmpl	$11, -4(%rbp)
	jne	.L27
.L26:
	addl	$1, -20(%rbp)
	jmp	.L24
.L27:
	movl	$3, -12(%rbp)
.L24:
	jmp	.L2
.L4:
	movl	$__PRETTY_FUNCTION__.2248, %ecx
	movl	$69, %edx
	movl	$.LC0, %esi
	movl	$.LC1, %edi
	call	__assert_fail
.L2:
	call	getchar
	movl	%eax, -4(%rbp)
	cmpl	$-1, -4(%rbp)
	jne	.L28
	cmpl	$3, -12(%rbp)
	je	.L29
	cmpl	$4, -12(%rbp)
	jne	.L30
.L29:
	movl	-16(%rbp), %eax
	leal	1(%rax), %edx
	movq	stderr(%rip), %rax
	movl	$.LC2, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf
	movl	$1, %eax
	jmp	.L31
.L30:
	cmpl	$0, -20(%rbp)
	je	.L32
	addl	$1, -28(%rbp)
.L32:
	movl	-20(%rbp), %ecx
	movl	-24(%rbp), %edx
	movl	-28(%rbp), %eax
	movl	%eax, %esi
	movl	$.LC3, %edi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
.L31:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.section	.rodata
	.type	__PRETTY_FUNCTION__.2248, @object
	.size	__PRETTY_FUNCTION__.2248, 5
__PRETTY_FUNCTION__.2248:
	.string	"main"
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.12) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits

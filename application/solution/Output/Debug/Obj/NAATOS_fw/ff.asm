	.cpu cortex-m4
	.eabi_attribute 27, 1
	.eabi_attribute 28, 1
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"ff.c"
	.text
.Ltext0:
	.section	.bss.FatFs,"aw",%nobits
	.align	2
	.type	FatFs, %object
	.size	FatFs, 4
FatFs:
	.space	4
	.section	.bss.Fsid,"aw",%nobits
	.align	1
	.type	Fsid, %object
	.size	Fsid, 2
Fsid:
	.space	2
	.section	.bss.LfnBuf,"aw",%nobits
	.align	2
	.type	LfnBuf, %object
	.size	LfnBuf, 512
LfnBuf:
	.space	512
	.section	.text.ld_word,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	ld_word, %function
ld_word:
.LFB0:
	.file 1 "C:\\Nordic\\NAATOS_fw_src\\application\\sdk\\external\\fatfs\\src\\ff.c"
	.loc 1 603 1
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #16
.LCFI0:
	str	r0, [sp, #4]
	.loc 1 606 10
	ldr	r3, [sp, #4]
	adds	r3, r3, #1
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 606 5
	strh	r3, [sp, #14]	@ movhi
	.loc 1 607 10
	ldrh	r3, [sp, #14]
	lsls	r3, r3, #8
	.loc 1 607 15
	sxth	r2, r3
	.loc 1 607 20
	ldr	r3, [sp, #4]
	ldrb	r3, [r3]	@ zero_extendqisi2
	sxth	r3, r3
	.loc 1 607 15
	orrs	r3, r3, r2
	sxth	r3, r3
	.loc 1 607 5
	strh	r3, [sp, #14]	@ movhi
	.loc 1 608 9
	ldrh	r3, [sp, #14]
	.loc 1 609 1
	mov	r0, r3
	add	sp, sp, #16
.LCFI1:
	@ sp needed
	bx	lr
.LFE0:
	.size	ld_word, .-ld_word
	.section	.text.ld_dword,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	ld_dword, %function
ld_dword:
.LFB1:
	.loc 1 613 1
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #16
.LCFI2:
	str	r0, [sp, #4]
	.loc 1 616 10
	ldr	r3, [sp, #4]
	adds	r3, r3, #3
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 616 5
	str	r3, [sp, #12]
	.loc 1 617 10
	ldr	r3, [sp, #12]
	lsls	r3, r3, #8
	.loc 1 617 20
	ldr	r2, [sp, #4]
	adds	r2, r2, #2
	ldrb	r2, [r2]	@ zero_extendqisi2
	.loc 1 617 5
	orrs	r3, r3, r2
	str	r3, [sp, #12]
	.loc 1 618 10
	ldr	r3, [sp, #12]
	lsls	r3, r3, #8
	.loc 1 618 20
	ldr	r2, [sp, #4]
	adds	r2, r2, #1
	ldrb	r2, [r2]	@ zero_extendqisi2
	.loc 1 618 5
	orrs	r3, r3, r2
	str	r3, [sp, #12]
	.loc 1 619 10
	ldr	r3, [sp, #12]
	lsls	r3, r3, #8
	.loc 1 619 20
	ldr	r2, [sp, #4]
	ldrb	r2, [r2]	@ zero_extendqisi2
	.loc 1 619 5
	orrs	r3, r3, r2
	str	r3, [sp, #12]
	.loc 1 620 9
	ldr	r3, [sp, #12]
	.loc 1 621 1
	mov	r0, r3
	add	sp, sp, #16
.LCFI3:
	@ sp needed
	bx	lr
.LFE1:
	.size	ld_dword, .-ld_dword
	.section	.text.st_word,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	st_word, %function
st_word:
.LFB2:
	.loc 1 644 1
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #8
.LCFI4:
	str	r0, [sp, #4]
	mov	r3, r1
	strh	r3, [sp, #2]	@ movhi
	.loc 1 645 6
	ldr	r3, [sp, #4]
	adds	r2, r3, #1
	str	r2, [sp, #4]
	.loc 1 645 11
	ldrh	r2, [sp, #2]	@ movhi
	uxtb	r2, r2
	.loc 1 645 9
	strb	r2, [r3]
	.loc 1 645 26
	ldrh	r3, [sp, #2]
	lsrs	r3, r3, #8
	strh	r3, [sp, #2]	@ movhi
	.loc 1 646 6
	ldr	r3, [sp, #4]
	adds	r2, r3, #1
	str	r2, [sp, #4]
	.loc 1 646 11
	ldrh	r2, [sp, #2]	@ movhi
	uxtb	r2, r2
	.loc 1 646 9
	strb	r2, [r3]
	.loc 1 647 1
	nop
	add	sp, sp, #8
.LCFI5:
	@ sp needed
	bx	lr
.LFE2:
	.size	st_word, .-st_word
	.section	.text.st_dword,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	st_dword, %function
st_dword:
.LFB3:
	.loc 1 651 1
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #8
.LCFI6:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 652 6
	ldr	r3, [sp, #4]
	adds	r2, r3, #1
	str	r2, [sp, #4]
	.loc 1 652 11
	ldr	r2, [sp]
	uxtb	r2, r2
	.loc 1 652 9
	strb	r2, [r3]
	.loc 1 652 26
	ldr	r3, [sp]
	lsrs	r3, r3, #8
	str	r3, [sp]
	.loc 1 653 6
	ldr	r3, [sp, #4]
	adds	r2, r3, #1
	str	r2, [sp, #4]
	.loc 1 653 11
	ldr	r2, [sp]
	uxtb	r2, r2
	.loc 1 653 9
	strb	r2, [r3]
	.loc 1 653 26
	ldr	r3, [sp]
	lsrs	r3, r3, #8
	str	r3, [sp]
	.loc 1 654 6
	ldr	r3, [sp, #4]
	adds	r2, r3, #1
	str	r2, [sp, #4]
	.loc 1 654 11
	ldr	r2, [sp]
	uxtb	r2, r2
	.loc 1 654 9
	strb	r2, [r3]
	.loc 1 654 26
	ldr	r3, [sp]
	lsrs	r3, r3, #8
	str	r3, [sp]
	.loc 1 655 6
	ldr	r3, [sp, #4]
	adds	r2, r3, #1
	str	r2, [sp, #4]
	.loc 1 655 11
	ldr	r2, [sp]
	uxtb	r2, r2
	.loc 1 655 9
	strb	r2, [r3]
	.loc 1 656 1
	nop
	add	sp, sp, #8
.LCFI7:
	@ sp needed
	bx	lr
.LFE3:
	.size	st_dword, .-st_dword
	.section	.text.mem_cpy,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	mem_cpy, %function
mem_cpy:
.LFB4:
	.loc 1 682 53
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #24
.LCFI8:
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	str	r2, [sp, #4]
	.loc 1 683 8
	ldr	r3, [sp, #12]
	str	r3, [sp, #20]
	.loc 1 684 14
	ldr	r3, [sp, #8]
	str	r3, [sp, #16]
	.loc 1 686 5
	ldr	r3, [sp, #4]
	cmp	r3, #0
	beq	.L10
.L9:
	.loc 1 687 15 discriminator 1
	ldr	r2, [sp, #16]
	adds	r3, r2, #1
	str	r3, [sp, #16]
	.loc 1 687 8 discriminator 1
	ldr	r3, [sp, #20]
	adds	r1, r3, #1
	str	r1, [sp, #20]
	.loc 1 687 13 discriminator 1
	ldrb	r2, [r2]	@ zero_extendqisi2
	.loc 1 687 11 discriminator 1
	strb	r2, [r3]
	.loc 1 687 3 discriminator 1
	ldr	r3, [sp, #4]
	subs	r3, r3, #1
	str	r3, [sp, #4]
	ldr	r3, [sp, #4]
	cmp	r3, #0
	bne	.L9
.L10:
	.loc 1 689 1
	nop
	add	sp, sp, #24
.LCFI9:
	@ sp needed
	bx	lr
.LFE4:
	.size	mem_cpy, .-mem_cpy
	.section	.text.mem_set,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	mem_set, %function
mem_set:
.LFB5:
	.loc 1 693 45
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #24
.LCFI10:
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	str	r2, [sp, #4]
	.loc 1 694 8
	ldr	r3, [sp, #12]
	str	r3, [sp, #20]
.L12:
	.loc 1 696 7 discriminator 1
	ldr	r3, [sp, #20]
	adds	r2, r3, #1
	str	r2, [sp, #20]
	.loc 1 696 12 discriminator 1
	ldr	r2, [sp, #8]
	uxtb	r2, r2
	.loc 1 696 10 discriminator 1
	strb	r2, [r3]
	.loc 1 696 2 discriminator 1
	ldr	r3, [sp, #4]
	subs	r3, r3, #1
	str	r3, [sp, #4]
	ldr	r3, [sp, #4]
	cmp	r3, #0
	bne	.L12
	.loc 1 697 1
	nop
	nop
	add	sp, sp, #24
.LCFI11:
	@ sp needed
	bx	lr
.LFE5:
	.size	mem_set, .-mem_set
	.section	.text.mem_cmp,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	mem_cmp, %function
mem_cmp:
.LFB6:
	.loc 1 701 58
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #32
.LCFI12:
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	str	r2, [sp, #4]
	.loc 1 702 14
	ldr	r3, [sp, #12]
	str	r3, [sp, #28]
	.loc 1 702 38
	ldr	r3, [sp, #8]
	str	r3, [sp, #24]
	.loc 1 703 6
	movs	r3, #0
	str	r3, [sp, #20]
.L15:
	.loc 1 706 9 discriminator 2
	ldr	r3, [sp, #28]
	adds	r2, r3, #1
	str	r2, [sp, #28]
	.loc 1 706 7 discriminator 2
	ldrb	r3, [r3]	@ zero_extendqisi2
	mov	r1, r3
	.loc 1 706 16 discriminator 2
	ldr	r3, [sp, #24]
	adds	r2, r3, #1
	str	r2, [sp, #24]
	.loc 1 706 14 discriminator 2
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 706 5 discriminator 2
	subs	r3, r1, r3
	str	r3, [sp, #20]
	.loc 1 707 2 discriminator 2
	ldr	r3, [sp, #4]
	subs	r3, r3, #1
	str	r3, [sp, #4]
	ldr	r3, [sp, #4]
	cmp	r3, #0
	beq	.L14
	.loc 1 707 17 discriminator 1
	ldr	r3, [sp, #20]
	cmp	r3, #0
	beq	.L15
.L14:
	.loc 1 709 9
	ldr	r3, [sp, #20]
	.loc 1 710 1
	mov	r0, r3
	add	sp, sp, #32
.LCFI13:
	@ sp needed
	bx	lr
.LFE6:
	.size	mem_cmp, .-mem_cmp
	.section	.text.chk_chr,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	chk_chr, %function
chk_chr:
.LFB7:
	.loc 1 714 40
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #8
.LCFI14:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 715 8
	b	.L18
.L20:
	.loc 1 715 33 discriminator 3
	ldr	r3, [sp, #4]
	adds	r3, r3, #1
	str	r3, [sp, #4]
.L18:
	.loc 1 715 9 discriminator 1
	ldr	r3, [sp, #4]
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 715 8 discriminator 1
	cmp	r3, #0
	beq	.L19
	.loc 1 715 17 discriminator 2
	ldr	r3, [sp, #4]
	ldrb	r3, [r3]	@ zero_extendqisi2
	mov	r2, r3
	.loc 1 715 14 discriminator 2
	ldr	r3, [sp]
	cmp	r3, r2
	bne	.L20
.L19:
	.loc 1 716 9
	ldr	r3, [sp, #4]
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 717 1
	mov	r0, r3
	add	sp, sp, #8
.LCFI15:
	@ sp needed
	bx	lr
.LFE7:
	.size	chk_chr, .-chk_chr
	.section	.text.sync_window,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	sync_window, %function
sync_window:
.LFB8:
	.loc 1 871 1
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI16:
	sub	sp, sp, #28
.LCFI17:
	str	r0, [sp, #4]
	.loc 1 874 10
	movs	r3, #0
	strb	r3, [sp, #15]
	.loc 1 877 8
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #3]	@ zero_extendqisi2
	.loc 1 877 5
	cmp	r3, #0
	beq	.L23
	.loc 1 878 9
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #52]
	str	r3, [sp, #20]
	.loc 1 879 7
	ldr	r3, [sp, #4]
	ldrb	r0, [r3, #1]	@ zero_extendqisi2
	.loc 1 879 29
	ldr	r3, [sp, #4]
	add	r1, r3, #56
	.loc 1 879 7
	movs	r3, #1
	ldr	r2, [sp, #20]
	bl	disk_write
	mov	r3, r0
	.loc 1 879 6
	cmp	r3, #0
	beq	.L24
	.loc 1 880 8
	movs	r3, #1
	strb	r3, [sp, #15]
	b	.L23
.L24:
	.loc 1 882 14
	ldr	r3, [sp, #4]
	movs	r2, #0
	strb	r2, [r3, #3]
	.loc 1 883 18
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #40]
	.loc 1 883 14
	ldr	r2, [sp, #20]
	subs	r2, r2, r3
	.loc 1 883 32
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #32]
	.loc 1 883 7
	cmp	r2, r3
	bcs	.L23
	.loc 1 884 17
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	.loc 1 884 13
	str	r3, [sp, #16]
	.loc 1 884 5
	b	.L25
.L26:
	.loc 1 885 17 discriminator 3
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #32]
	.loc 1 885 12 discriminator 3
	ldr	r2, [sp, #20]
	add	r3, r3, r2
	str	r3, [sp, #20]
	.loc 1 886 6 discriminator 3
	ldr	r3, [sp, #4]
	ldrb	r0, [r3, #1]	@ zero_extendqisi2
	.loc 1 886 28 discriminator 3
	ldr	r3, [sp, #4]
	add	r1, r3, #56
	.loc 1 886 6 discriminator 3
	movs	r3, #1
	ldr	r2, [sp, #20]
	bl	disk_write
	.loc 1 884 38 discriminator 3
	ldr	r3, [sp, #16]
	subs	r3, r3, #1
	str	r3, [sp, #16]
.L25:
	.loc 1 884 5 discriminator 1
	ldr	r3, [sp, #16]
	cmp	r3, #1
	bhi	.L26
.L23:
	.loc 1 891 9
	ldrb	r3, [sp, #15]	@ zero_extendqisi2
	.loc 1 892 1
	mov	r0, r3
	add	sp, sp, #28
.LCFI18:
	@ sp needed
	ldr	pc, [sp], #4
.LFE8:
	.size	sync_window, .-sync_window
	.section	.text.move_window,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	move_window, %function
move_window:
.LFB9:
	.loc 1 901 1
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI19:
	sub	sp, sp, #20
.LCFI20:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 902 10
	movs	r3, #0
	strb	r3, [sp, #15]
	.loc 1 905 18
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #52]
	.loc 1 905 5
	ldr	r2, [sp]
	cmp	r2, r3
	beq	.L29
	.loc 1 907 9
	ldr	r0, [sp, #4]
	bl	sync_window
	mov	r3, r0
	strb	r3, [sp, #15]
	.loc 1 909 6
	ldrb	r3, [sp, #15]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L29
	.loc 1 910 8
	ldr	r3, [sp, #4]
	ldrb	r0, [r3, #1]	@ zero_extendqisi2
	.loc 1 910 29
	ldr	r3, [sp, #4]
	add	r1, r3, #56
	.loc 1 910 8
	movs	r3, #1
	ldr	r2, [sp]
	bl	disk_read
	mov	r3, r0
	.loc 1 910 7
	cmp	r3, #0
	beq	.L30
	.loc 1 911 12
	mov	r3, #-1
	str	r3, [sp]
	.loc 1 912 9
	movs	r3, #1
	strb	r3, [sp, #15]
.L30:
	.loc 1 914 16
	ldr	r3, [sp, #4]
	ldr	r2, [sp]
	str	r2, [r3, #52]
.L29:
	.loc 1 917 9
	ldrb	r3, [sp, #15]	@ zero_extendqisi2
	.loc 1 918 1
	mov	r0, r3
	add	sp, sp, #20
.LCFI21:
	@ sp needed
	ldr	pc, [sp], #4
.LFE9:
	.size	move_window, .-move_window
	.section	.text.sync_fs,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	sync_fs, %function
sync_fs:
.LFB10:
	.loc 1 932 1
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI22:
	sub	sp, sp, #20
.LCFI23:
	str	r0, [sp, #4]
	.loc 1 936 8
	ldr	r0, [sp, #4]
	bl	sync_window
	mov	r3, r0
	strb	r3, [sp, #15]
	.loc 1 937 5
	ldrb	r3, [sp, #15]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L33
	.loc 1 939 9
	ldr	r3, [sp, #4]
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 939 6
	cmp	r3, #3
	bne	.L34
	.loc 1 939 36 discriminator 1
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #4]	@ zero_extendqisi2
	.loc 1 939 31 discriminator 1
	cmp	r3, #1
	bne	.L34
	.loc 1 941 14
	ldr	r3, [sp, #4]
	adds	r3, r3, #56
	.loc 1 941 4
	mov	r2, #512
	movs	r1, #0
	mov	r0, r3
	bl	mem_set
	.loc 1 942 12
	ldr	r3, [sp, #4]
	adds	r3, r3, #56
	.loc 1 942 4
	add	r3, r3, #510
	movw	r1, #43605
	mov	r0, r3
	bl	st_word
	.loc 1 943 13
	ldr	r3, [sp, #4]
	adds	r3, r3, #56
	.loc 1 943 4
	ldr	r1, .L36
	mov	r0, r3
	bl	st_dword
	.loc 1 944 13
	ldr	r3, [sp, #4]
	adds	r3, r3, #56
	.loc 1 944 4
	add	r3, r3, #484
	ldr	r1, .L36+4
	mov	r0, r3
	bl	st_dword
	.loc 1 945 13
	ldr	r3, [sp, #4]
	adds	r3, r3, #56
	.loc 1 945 4
	add	r2, r3, #488
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #20]
	mov	r1, r3
	mov	r0, r2
	bl	st_dword
	.loc 1 946 13
	ldr	r3, [sp, #4]
	adds	r3, r3, #56
	.loc 1 946 4
	add	r2, r3, #492
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #16]
	mov	r1, r3
	mov	r0, r2
	bl	st_dword
	.loc 1 948 20
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #36]
	.loc 1 948 30
	adds	r2, r3, #1
	.loc 1 948 16
	ldr	r3, [sp, #4]
	str	r2, [r3, #52]
	.loc 1 949 4
	ldr	r3, [sp, #4]
	ldrb	r0, [r3, #1]	@ zero_extendqisi2
	.loc 1 949 26
	ldr	r3, [sp, #4]
	add	r1, r3, #56
	.loc 1 949 4
	ldr	r3, [sp, #4]
	ldr	r2, [r3, #52]
	movs	r3, #1
	bl	disk_write
	.loc 1 950 17
	ldr	r3, [sp, #4]
	movs	r2, #0
	strb	r2, [r3, #4]
.L34:
	.loc 1 953 7
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #1]	@ zero_extendqisi2
	movs	r2, #0
	movs	r1, #0
	mov	r0, r3
	bl	disk_ioctl
	mov	r3, r0
	.loc 1 953 6
	cmp	r3, #0
	beq	.L33
	.loc 1 953 56 discriminator 1
	movs	r3, #1
	strb	r3, [sp, #15]
.L33:
	.loc 1 956 9
	ldrb	r3, [sp, #15]	@ zero_extendqisi2
	.loc 1 957 1
	mov	r0, r3
	add	sp, sp, #20
.LCFI24:
	@ sp needed
	ldr	pc, [sp], #4
.L37:
	.align	2
.L36:
	.word	1096897106
	.word	1631679090
.LFE10:
	.size	sync_fs, .-sync_fs
	.section	.text.clust2sect,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	clust2sect, %function
clust2sect:
.LFB11:
	.loc 1 972 1
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #8
.LCFI25:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 973 7
	ldr	r3, [sp]
	subs	r3, r3, #2
	str	r3, [sp]
	.loc 1 974 16
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #28]
	.loc 1 974 27
	subs	r3, r3, #2
	.loc 1 974 5
	ldr	r2, [sp]
	cmp	r2, r3
	bcc	.L39
	.loc 1 974 39 discriminator 1
	movs	r3, #0
	b	.L40
.L39:
	.loc 1 975 18
	ldr	r3, [sp, #4]
	ldrh	r3, [r3, #10]
	mov	r2, r3
	.loc 1 975 14
	ldr	r3, [sp]
	mul	r2, r3, r2
	.loc 1 975 30
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #48]
	.loc 1 975 26
	add	r3, r3, r2
.L40:
	.loc 1 976 1
	mov	r0, r3
	add	sp, sp, #8
.LCFI26:
	@ sp needed
	bx	lr
.LFE11:
	.size	clust2sect, .-clust2sect
	.section	.text.get_fat,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	get_fat, %function
get_fat:
.LFB12:
	.loc 1 990 1
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI27:
	sub	sp, sp, #28
.LCFI28:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 993 9
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	str	r3, [sp, #16]
	.loc 1 996 5
	ldr	r3, [sp]
	cmp	r3, #1
	bls	.L42
	.loc 1 996 28 discriminator 1
	ldr	r3, [sp, #16]
	ldr	r3, [r3, #28]
	.loc 1 996 15 discriminator 1
	ldr	r2, [sp]
	cmp	r2, r3
	bcc	.L43
.L42:
	.loc 1 997 7
	movs	r3, #1
	str	r3, [sp, #20]
	b	.L44
.L43:
	.loc 1 1000 7
	mov	r3, #-1
	str	r3, [sp, #20]
	.loc 1 1002 13
	ldr	r3, [sp, #16]
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 1002 3
	cmp	r3, #3
	beq	.L45
	cmp	r3, #3
	bgt	.L46
	cmp	r3, #1
	beq	.L47
	cmp	r3, #2
	beq	.L48
	b	.L46
.L47:
	.loc 1 1004 7
	ldr	r3, [sp]
	str	r3, [sp, #12]
	.loc 1 1004 30
	ldr	r3, [sp, #12]
	lsrs	r3, r3, #1
	.loc 1 1004 24
	ldr	r2, [sp, #12]
	add	r3, r3, r2
	str	r3, [sp, #12]
	.loc 1 1005 26
	ldr	r3, [sp, #16]
	ldr	r2, [r3, #40]
	.loc 1 1005 42
	ldr	r3, [sp, #12]
	lsrs	r3, r3, #9
	.loc 1 1005 8
	add	r3, r3, r2
	mov	r1, r3
	ldr	r0, [sp, #16]
	bl	move_window
	mov	r3, r0
	.loc 1 1005 7
	cmp	r3, #0
	bne	.L56
	.loc 1 1006 19
	ldr	r3, [sp, #12]
	adds	r2, r3, #1
	str	r2, [sp, #12]
	.loc 1 1006 22
	ubfx	r3, r3, #0, #9
	.loc 1 1006 16
	ldr	r2, [sp, #16]
	add	r3, r3, r2
	ldrb	r3, [r3, #56]	@ zero_extendqisi2
	.loc 1 1006 7
	str	r3, [sp, #8]
	.loc 1 1007 26
	ldr	r3, [sp, #16]
	ldr	r2, [r3, #40]
	.loc 1 1007 42
	ldr	r3, [sp, #12]
	lsrs	r3, r3, #9
	.loc 1 1007 8
	add	r3, r3, r2
	mov	r1, r3
	ldr	r0, [sp, #16]
	bl	move_window
	mov	r3, r0
	.loc 1 1007 7
	cmp	r3, #0
	bne	.L57
	.loc 1 1008 21
	ldr	r3, [sp, #12]
	ubfx	r3, r3, #0, #9
	.loc 1 1008 17
	ldr	r2, [sp, #16]
	add	r3, r3, r2
	ldrb	r3, [r3, #56]	@ zero_extendqisi2
	.loc 1 1008 31
	lsls	r3, r3, #8
	mov	r2, r3
	.loc 1 1008 7
	ldr	r3, [sp, #8]
	orrs	r3, r3, r2
	str	r3, [sp, #8]
	.loc 1 1009 16
	ldr	r3, [sp]
	and	r3, r3, #1
	.loc 1 1009 33
	cmp	r3, #0
	beq	.L51
	.loc 1 1009 33 is_stmt 0 discriminator 1
	ldr	r3, [sp, #8]
	lsrs	r3, r3, #4
	b	.L52
.L51:
	.loc 1 1009 33 discriminator 2
	ldr	r3, [sp, #8]
	ubfx	r3, r3, #0, #12
.L52:
	.loc 1 1009 8 is_stmt 1 discriminator 4
	str	r3, [sp, #20]
	.loc 1 1010 4 discriminator 4
	b	.L44
.L48:
	.loc 1 1013 26
	ldr	r3, [sp, #16]
	ldr	r2, [r3, #40]
	.loc 1 1013 44
	ldr	r3, [sp]
	lsrs	r3, r3, #8
	.loc 1 1013 8
	add	r3, r3, r2
	mov	r1, r3
	ldr	r0, [sp, #16]
	bl	move_window
	mov	r3, r0
	.loc 1 1013 7
	cmp	r3, #0
	bne	.L58
	.loc 1 1014 18
	ldr	r3, [sp, #16]
	add	r2, r3, #56
	.loc 1 1014 33
	ldr	r3, [sp]
	lsls	r3, r3, #1
	.loc 1 1014 37
	and	r3, r3, #510
	.loc 1 1014 10
	add	r3, r3, r2
	mov	r0, r3
	bl	ld_word
	mov	r3, r0
	.loc 1 1014 8
	str	r3, [sp, #20]
	.loc 1 1015 4
	b	.L44
.L45:
	.loc 1 1018 26
	ldr	r3, [sp, #16]
	ldr	r2, [r3, #40]
	.loc 1 1018 44
	ldr	r3, [sp]
	lsrs	r3, r3, #7
	.loc 1 1018 8
	add	r3, r3, r2
	mov	r1, r3
	ldr	r0, [sp, #16]
	bl	move_window
	mov	r3, r0
	.loc 1 1018 7
	cmp	r3, #0
	bne	.L59
	.loc 1 1019 19
	ldr	r3, [sp, #16]
	add	r2, r3, #56
	.loc 1 1019 34
	ldr	r3, [sp]
	lsls	r3, r3, #2
	.loc 1 1019 38
	and	r3, r3, #508
	.loc 1 1019 10
	add	r3, r3, r2
	mov	r0, r3
	bl	ld_dword
	mov	r3, r0
	.loc 1 1019 8
	bic	r3, r3, #-268435456
	str	r3, [sp, #20]
	.loc 1 1020 4
	b	.L44
.L46:
	.loc 1 1046 8
	movs	r3, #1
	str	r3, [sp, #20]
	b	.L44
.L56:
	.loc 1 1005 4
	nop
	b	.L44
.L57:
	.loc 1 1007 4
	nop
	b	.L44
.L58:
	.loc 1 1013 4
	nop
	b	.L44
.L59:
	.loc 1 1018 4
	nop
.L44:
	.loc 1 1050 9
	ldr	r3, [sp, #20]
	.loc 1 1051 1
	mov	r0, r3
	add	sp, sp, #28
.LCFI29:
	@ sp needed
	ldr	pc, [sp], #4
.LFE12:
	.size	get_fat, .-get_fat
	.section	.text.put_fat,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	put_fat, %function
put_fat:
.LFB13:
	.loc 1 1067 1
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
.LCFI30:
	sub	sp, sp, #32
.LCFI31:
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	str	r2, [sp, #4]
	.loc 1 1070 10
	movs	r3, #2
	strb	r3, [sp, #31]
	.loc 1 1073 5
	ldr	r3, [sp, #8]
	cmp	r3, #1
	bls	.L74
	.loc 1 1073 28 discriminator 1
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #28]
	.loc 1 1073 16 discriminator 1
	ldr	r2, [sp, #8]
	cmp	r2, r3
	bcs	.L74
	.loc 1 1074 13
	ldr	r3, [sp, #12]
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 1074 3
	cmp	r3, #3
	beq	.L62
	cmp	r3, #3
	bgt	.L61
	cmp	r3, #1
	beq	.L63
	cmp	r3, #2
	beq	.L64
	b	.L61
.L63:
	.loc 1 1076 7
	ldr	r3, [sp, #8]
	str	r3, [sp, #24]
	.loc 1 1076 30
	ldr	r3, [sp, #24]
	lsrs	r3, r3, #1
	.loc 1 1076 24
	ldr	r2, [sp, #24]
	add	r3, r3, r2
	str	r3, [sp, #24]
	.loc 1 1077 28
	ldr	r3, [sp, #12]
	ldr	r2, [r3, #40]
	.loc 1 1077 44
	ldr	r3, [sp, #24]
	lsrs	r3, r3, #9
	.loc 1 1077 10
	add	r3, r3, r2
	mov	r1, r3
	ldr	r0, [sp, #12]
	bl	move_window
	mov	r3, r0
	strb	r3, [sp, #31]
	.loc 1 1078 7
	ldrb	r3, [sp, #31]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L75
	.loc 1 1079 8
	ldr	r3, [sp, #12]
	add	r2, r3, #56
	.loc 1 1079 23
	ldr	r3, [sp, #24]
	adds	r1, r3, #1
	str	r1, [sp, #24]
	ubfx	r3, r3, #0, #9
	.loc 1 1079 6
	add	r3, r3, r2
	str	r3, [sp, #20]
	.loc 1 1080 15
	ldr	r3, [sp, #8]
	and	r3, r3, #1
	.loc 1 1080 7
	cmp	r3, #0
	beq	.L66
	.loc 1 1080 24 discriminator 1
	ldr	r3, [sp, #20]
	ldrb	r3, [r3]	@ zero_extendqisi2
	sxtb	r3, r3
	.loc 1 1080 27 discriminator 1
	and	r3, r3, #15
	sxtb	r2, r3
	.loc 1 1080 38 discriminator 1
	ldr	r3, [sp, #4]
	uxtb	r3, r3
	.loc 1 1080 48 discriminator 1
	lsls	r3, r3, #4
	.loc 1 1080 35 discriminator 1
	sxtb	r3, r3
	orrs	r3, r3, r2
	sxtb	r3, r3
	.loc 1 1080 7 discriminator 1
	uxtb	r3, r3
	b	.L67
.L66:
	.loc 1 1080 7 is_stmt 0 discriminator 2
	ldr	r3, [sp, #4]
	uxtb	r3, r3
.L67:
	.loc 1 1080 7 discriminator 4
	ldr	r2, [sp, #20]
	strb	r3, [r2]
	.loc 1 1081 14 is_stmt 1 discriminator 4
	ldr	r3, [sp, #12]
	movs	r2, #1
	strb	r2, [r3, #3]
	.loc 1 1082 28 discriminator 4
	ldr	r3, [sp, #12]
	ldr	r2, [r3, #40]
	.loc 1 1082 44 discriminator 4
	ldr	r3, [sp, #24]
	lsrs	r3, r3, #9
	.loc 1 1082 10 discriminator 4
	add	r3, r3, r2
	mov	r1, r3
	ldr	r0, [sp, #12]
	bl	move_window
	mov	r3, r0
	strb	r3, [sp, #31]
	.loc 1 1083 7 discriminator 4
	ldrb	r3, [sp, #31]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L76
	.loc 1 1084 8
	ldr	r3, [sp, #12]
	add	r2, r3, #56
	.loc 1 1084 21
	ldr	r3, [sp, #24]
	ubfx	r3, r3, #0, #9
	.loc 1 1084 6
	add	r3, r3, r2
	str	r3, [sp, #20]
	.loc 1 1085 15
	ldr	r3, [sp, #8]
	and	r3, r3, #1
	.loc 1 1085 7
	cmp	r3, #0
	beq	.L69
	.loc 1 1085 33 discriminator 1
	ldr	r3, [sp, #4]
	lsrs	r3, r3, #4
	.loc 1 1085 7 discriminator 1
	uxtb	r3, r3
	b	.L70
.L69:
	.loc 1 1085 43 discriminator 2
	ldr	r3, [sp, #20]
	ldrb	r3, [r3]	@ zero_extendqisi2
	sxtb	r3, r3
	.loc 1 1085 46 discriminator 2
	bic	r3, r3, #15
	sxtb	r2, r3
	.loc 1 1085 68 discriminator 2
	ldr	r3, [sp, #4]
	lsrs	r3, r3, #8
	.loc 1 1085 57 discriminator 2
	sxtb	r3, r3
	.loc 1 1085 74 discriminator 2
	and	r3, r3, #15
	sxtb	r3, r3
	.loc 1 1085 54 discriminator 2
	orrs	r3, r3, r2
	sxtb	r3, r3
	.loc 1 1085 7 discriminator 2
	uxtb	r3, r3
.L70:
	.loc 1 1085 7 is_stmt 0 discriminator 4
	ldr	r2, [sp, #20]
	strb	r3, [r2]
	.loc 1 1086 14 is_stmt 1 discriminator 4
	ldr	r3, [sp, #12]
	movs	r2, #1
	strb	r2, [r3, #3]
	.loc 1 1087 4 discriminator 4
	b	.L61
.L64:
	.loc 1 1090 28
	ldr	r3, [sp, #12]
	ldr	r2, [r3, #40]
	.loc 1 1090 46
	ldr	r3, [sp, #8]
	lsrs	r3, r3, #8
	.loc 1 1090 10
	add	r3, r3, r2
	mov	r1, r3
	ldr	r0, [sp, #12]
	bl	move_window
	mov	r3, r0
	strb	r3, [sp, #31]
	.loc 1 1091 7
	ldrb	r3, [sp, #31]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L77
	.loc 1 1092 12
	ldr	r3, [sp, #12]
	add	r2, r3, #56
	.loc 1 1092 27
	ldr	r3, [sp, #8]
	lsls	r3, r3, #1
	.loc 1 1092 31
	and	r3, r3, #510
	.loc 1 1092 4
	add	r3, r3, r2
	ldr	r2, [sp, #4]
	uxth	r2, r2
	mov	r1, r2
	mov	r0, r3
	bl	st_word
	.loc 1 1093 14
	ldr	r3, [sp, #12]
	movs	r2, #1
	strb	r2, [r3, #3]
	.loc 1 1094 4
	b	.L61
.L62:
	.loc 1 1100 28
	ldr	r3, [sp, #12]
	ldr	r2, [r3, #40]
	.loc 1 1100 46
	ldr	r3, [sp, #8]
	lsrs	r3, r3, #7
	.loc 1 1100 10
	add	r3, r3, r2
	mov	r1, r3
	ldr	r0, [sp, #12]
	bl	move_window
	mov	r3, r0
	strb	r3, [sp, #31]
	.loc 1 1101 7
	ldrb	r3, [sp, #31]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L78
	.loc 1 1103 16
	ldr	r3, [sp, #4]
	bic	r4, r3, #-268435456
	.loc 1 1103 42
	ldr	r3, [sp, #12]
	add	r2, r3, #56
	.loc 1 1103 57
	ldr	r3, [sp, #8]
	lsls	r3, r3, #2
	.loc 1 1103 61
	and	r3, r3, #508
	.loc 1 1103 33
	add	r3, r3, r2
	mov	r0, r3
	bl	ld_dword
	mov	r3, r0
	.loc 1 1103 71
	and	r3, r3, #-268435456
	.loc 1 1103 9
	orrs	r3, r3, r4
	str	r3, [sp, #4]
	.loc 1 1105 13
	ldr	r3, [sp, #12]
	add	r2, r3, #56
	.loc 1 1105 28
	ldr	r3, [sp, #8]
	lsls	r3, r3, #2
	.loc 1 1105 32
	and	r3, r3, #508
	.loc 1 1105 4
	add	r3, r3, r2
	ldr	r1, [sp, #4]
	mov	r0, r3
	bl	st_dword
	.loc 1 1106 14
	ldr	r3, [sp, #12]
	movs	r2, #1
	strb	r2, [r3, #3]
	.loc 1 1107 4
	b	.L61
.L74:
	.loc 1 1109 2
	nop
	b	.L61
.L75:
	.loc 1 1078 4
	nop
	b	.L61
.L76:
	.loc 1 1083 4
	nop
	b	.L61
.L77:
	.loc 1 1091 4
	nop
	b	.L61
.L78:
	.loc 1 1101 4
	nop
.L61:
	.loc 1 1110 9
	ldrb	r3, [sp, #31]	@ zero_extendqisi2
	.loc 1 1111 1
	mov	r0, r3
	add	sp, sp, #32
.LCFI32:
	@ sp needed
	pop	{r4, pc}
.LFE13:
	.size	put_fat, .-put_fat
	.section	.text.remove_chain,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	remove_chain, %function
remove_chain:
.LFB14:
	.loc 1 1237 1
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI33:
	sub	sp, sp, #36
.LCFI34:
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	str	r2, [sp, #4]
	.loc 1 1238 10
	movs	r3, #0
	strb	r3, [sp, #31]
	.loc 1 1240 9
	ldr	r3, [sp, #12]
	ldr	r3, [r3]
	str	r3, [sp, #24]
	.loc 1 1248 5
	ldr	r3, [sp, #8]
	cmp	r3, #1
	bls	.L80
	.loc 1 1248 28 discriminator 2
	ldr	r3, [sp, #24]
	ldr	r3, [r3, #28]
	.loc 1 1248 15 discriminator 2
	ldr	r2, [sp, #8]
	cmp	r2, r3
	bcc	.L81
.L80:
	.loc 1 1248 47 discriminator 3
	movs	r3, #2
	b	.L82
.L81:
	.loc 1 1251 5
	ldr	r3, [sp, #4]
	cmp	r3, #0
	beq	.L90
	.loc 1 1252 9
	mov	r2, #-1
	ldr	r1, [sp, #4]
	ldr	r0, [sp, #24]
	bl	put_fat
	mov	r3, r0
	strb	r3, [sp, #31]
	.loc 1 1253 6
	ldrb	r3, [sp, #31]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L90
	.loc 1 1253 28 discriminator 1
	ldrb	r3, [sp, #31]	@ zero_extendqisi2
	b	.L82
.L90:
	.loc 1 1258 9
	ldr	r1, [sp, #8]
	ldr	r0, [sp, #12]
	bl	get_fat
	str	r0, [sp, #20]
	.loc 1 1259 6
	ldr	r3, [sp, #20]
	cmp	r3, #0
	beq	.L91
	.loc 1 1260 6
	ldr	r3, [sp, #20]
	cmp	r3, #1
	bne	.L86
	.loc 1 1260 24 discriminator 1
	movs	r3, #2
	b	.L82
.L86:
	.loc 1 1261 6
	ldr	r3, [sp, #20]
	cmp	r3, #-1
	bne	.L87
	.loc 1 1261 33 discriminator 1
	movs	r3, #1
	b	.L82
.L87:
	.loc 1 1263 10
	movs	r2, #0
	ldr	r1, [sp, #8]
	ldr	r0, [sp, #24]
	bl	put_fat
	mov	r3, r0
	strb	r3, [sp, #31]
	.loc 1 1264 7
	ldrb	r3, [sp, #31]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L88
	.loc 1 1264 29 discriminator 1
	ldrb	r3, [sp, #31]	@ zero_extendqisi2
	b	.L82
.L88:
	.loc 1 1266 9
	ldr	r3, [sp, #24]
	ldr	r2, [r3, #20]
	.loc 1 1266 25
	ldr	r3, [sp, #24]
	ldr	r3, [r3, #28]
	.loc 1 1266 36
	subs	r3, r3, #2
	.loc 1 1266 6
	cmp	r2, r3
	bcs	.L89
	.loc 1 1267 6
	ldr	r3, [sp, #24]
	ldr	r3, [r3, #20]
	.loc 1 1267 17
	adds	r2, r3, #1
	ldr	r3, [sp, #24]
	str	r2, [r3, #20]
	.loc 1 1268 17
	ldr	r3, [sp, #24]
	ldrb	r3, [r3, #4]	@ zero_extendqisi2
	orr	r3, r3, #1
	uxtb	r2, r3
	ldr	r3, [sp, #24]
	strb	r2, [r3, #4]
.L89:
	.loc 1 1288 8
	ldr	r3, [sp, #20]
	str	r3, [sp, #8]
	.loc 1 1289 20
	ldr	r3, [sp, #24]
	ldr	r3, [r3, #28]
	.loc 1 1289 2
	ldr	r2, [sp, #8]
	cmp	r2, r3
	bcc	.L90
	b	.L85
.L91:
	.loc 1 1259 3
	nop
.L85:
	.loc 1 1302 9
	movs	r3, #0
.L82:
	.loc 1 1303 1
	mov	r0, r3
	add	sp, sp, #36
.LCFI35:
	@ sp needed
	ldr	pc, [sp], #4
.LFE14:
	.size	remove_chain, .-remove_chain
	.section	.text.create_chain,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	create_chain, %function
create_chain:
.LFB15:
	.loc 1 1316 1
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI36:
	sub	sp, sp, #36
.LCFI37:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 1319 9
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	str	r3, [sp, #16]
	.loc 1 1322 5
	ldr	r3, [sp]
	cmp	r3, #0
	bne	.L93
	.loc 1 1323 7
	ldr	r3, [sp, #16]
	ldr	r3, [r3, #16]
	str	r3, [sp, #24]
	.loc 1 1324 6
	ldr	r3, [sp, #24]
	cmp	r3, #0
	beq	.L94
	.loc 1 1324 28 discriminator 2
	ldr	r3, [sp, #16]
	ldr	r3, [r3, #28]
	.loc 1 1324 16 discriminator 2
	ldr	r2, [sp, #24]
	cmp	r2, r3
	bcc	.L95
.L94:
	.loc 1 1324 44 discriminator 3
	movs	r3, #1
	str	r3, [sp, #24]
	b	.L95
.L93:
	.loc 1 1327 8
	ldr	r1, [sp]
	ldr	r0, [sp, #4]
	bl	get_fat
	str	r0, [sp, #12]
	.loc 1 1328 6
	ldr	r3, [sp, #12]
	cmp	r3, #1
	bhi	.L96
	.loc 1 1328 22 discriminator 1
	movs	r3, #1
	b	.L97
.L96:
	.loc 1 1329 6
	ldr	r3, [sp, #12]
	cmp	r3, #-1
	bne	.L98
	.loc 1 1329 32 discriminator 1
	ldr	r3, [sp, #12]
	b	.L97
.L98:
	.loc 1 1330 14
	ldr	r3, [sp, #16]
	ldr	r3, [r3, #28]
	.loc 1 1330 6
	ldr	r2, [sp, #12]
	cmp	r2, r3
	bcs	.L99
	.loc 1 1330 33 discriminator 1
	ldr	r3, [sp, #12]
	b	.L97
.L99:
	.loc 1 1331 7
	ldr	r3, [sp]
	str	r3, [sp, #24]
.L95:
	.loc 1 1352 7
	ldr	r3, [sp, #24]
	str	r3, [sp, #28]
.L106:
	.loc 1 1354 7
	ldr	r3, [sp, #28]
	adds	r3, r3, #1
	str	r3, [sp, #28]
	.loc 1 1355 17
	ldr	r3, [sp, #16]
	ldr	r3, [r3, #28]
	.loc 1 1355 7
	ldr	r2, [sp, #28]
	cmp	r2, r3
	bcc	.L100
	.loc 1 1356 9
	movs	r3, #2
	str	r3, [sp, #28]
	.loc 1 1357 8
	ldr	r2, [sp, #28]
	ldr	r3, [sp, #24]
	cmp	r2, r3
	bls	.L100
	.loc 1 1357 27 discriminator 1
	movs	r3, #0
	b	.L97
.L100:
	.loc 1 1359 9
	ldr	r1, [sp, #28]
	ldr	r0, [sp, #4]
	bl	get_fat
	str	r0, [sp, #12]
	.loc 1 1360 7
	ldr	r3, [sp, #12]
	cmp	r3, #0
	beq	.L115
	.loc 1 1361 7
	ldr	r3, [sp, #12]
	cmp	r3, #1
	beq	.L103
	.loc 1 1361 16 discriminator 2
	ldr	r3, [sp, #12]
	cmp	r3, #-1
	bne	.L104
.L103:
	.loc 1 1361 44 discriminator 3
	ldr	r3, [sp, #12]
	b	.L97
.L104:
	.loc 1 1362 7
	ldr	r2, [sp, #28]
	ldr	r3, [sp, #24]
	cmp	r2, r3
	bne	.L106
	.loc 1 1362 27 discriminator 1
	movs	r3, #0
	b	.L97
.L115:
	.loc 1 1360 4
	nop
	.loc 1 1369 9
	mov	r2, #-1
	ldr	r1, [sp, #28]
	ldr	r0, [sp, #16]
	bl	put_fat
	mov	r3, r0
	strb	r3, [sp, #23]
	.loc 1 1370 6
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L107
	.loc 1 1370 20 discriminator 1
	ldr	r3, [sp]
	cmp	r3, #0
	beq	.L107
	.loc 1 1371 10
	ldr	r2, [sp, #28]
	ldr	r1, [sp]
	ldr	r0, [sp, #16]
	bl	put_fat
	mov	r3, r0
	strb	r3, [sp, #23]
.L107:
	.loc 1 1375 5
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L108
	.loc 1 1376 17
	ldr	r3, [sp, #16]
	ldr	r2, [sp, #28]
	str	r2, [r3, #16]
	.loc 1 1377 9
	ldr	r3, [sp, #16]
	ldr	r2, [r3, #20]
	.loc 1 1377 25
	ldr	r3, [sp, #16]
	ldr	r3, [r3, #28]
	.loc 1 1377 36
	subs	r3, r3, #2
	.loc 1 1377 6
	cmp	r2, r3
	bcs	.L109
	.loc 1 1377 43 discriminator 1
	ldr	r3, [sp, #16]
	ldr	r3, [r3, #20]
	.loc 1 1377 54 discriminator 1
	subs	r2, r3, #1
	ldr	r3, [sp, #16]
	str	r2, [r3, #20]
.L109:
	.loc 1 1378 16
	ldr	r3, [sp, #16]
	ldrb	r3, [r3, #4]	@ zero_extendqisi2
	orr	r3, r3, #1
	uxtb	r2, r3
	ldr	r3, [sp, #16]
	strb	r2, [r3, #4]
	b	.L110
.L108:
	.loc 1 1380 43
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L111
	.loc 1 1380 43 is_stmt 0 discriminator 1
	mov	r3, #-1
	b	.L112
.L111:
	.loc 1 1380 43 discriminator 2
	movs	r3, #1
.L112:
	.loc 1 1380 7 is_stmt 1 discriminator 4
	str	r3, [sp, #28]
.L110:
	.loc 1 1383 9
	ldr	r3, [sp, #28]
.L97:
	.loc 1 1384 1
	mov	r0, r3
	add	sp, sp, #36
.LCFI38:
	@ sp needed
	ldr	pc, [sp], #4
.LFE15:
	.size	create_chain, .-create_chain
	.section	.text.dir_sdi,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	dir_sdi, %function
dir_sdi:
.LFB16:
	.loc 1 1431 1
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI39:
	sub	sp, sp, #28
.LCFI40:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 1433 9
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	str	r3, [sp, #16]
	.loc 1 1436 5
	ldr	r3, [sp]
	cmp	r3, #2097152
	bcs	.L117
	.loc 1 1436 91 discriminator 1
	ldr	r3, [sp]
	and	r3, r3, #31
	.loc 1 1436 84 discriminator 1
	cmp	r3, #0
	beq	.L118
.L117:
	.loc 1 1437 10
	movs	r3, #2
	b	.L119
.L118:
	.loc 1 1439 11
	ldr	r3, [sp, #4]
	ldr	r2, [sp]
	str	r2, [r3, #16]
	.loc 1 1440 7
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #8]
	str	r3, [sp, #20]
	.loc 1 1441 5
	ldr	r3, [sp, #20]
	cmp	r3, #0
	bne	.L120
	.loc 1 1441 21 discriminator 1
	ldr	r3, [sp, #16]
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 1441 16 discriminator 1
	cmp	r3, #2
	bls	.L120
	.loc 1 1442 8
	ldr	r3, [sp, #16]
	ldr	r3, [r3, #44]
	str	r3, [sp, #20]
.L120:
	.loc 1 1446 5
	ldr	r3, [sp, #20]
	cmp	r3, #0
	bne	.L121
	.loc 1 1447 11
	ldr	r3, [sp]
	lsrs	r3, r3, #5
	.loc 1 1447 25
	ldr	r2, [sp, #16]
	ldrh	r2, [r2, #8]
	.loc 1 1447 6
	cmp	r3, r2
	bcc	.L122
	.loc 1 1447 45 discriminator 1
	movs	r3, #2
	b	.L119
.L122:
	.loc 1 1448 16
	ldr	r3, [sp, #16]
	ldr	r2, [r3, #44]
	.loc 1 1448 12
	ldr	r3, [sp, #4]
	str	r2, [r3, #24]
	b	.L123
.L121:
	.loc 1 1451 18
	ldr	r3, [sp, #16]
	ldrh	r3, [r3, #10]
	.loc 1 1451 7
	lsls	r3, r3, #9
	str	r3, [sp, #12]
	.loc 1 1452 9
	b	.L124
.L128:
	.loc 1 1453 11
	ldr	r3, [sp, #4]
	ldr	r1, [sp, #20]
	mov	r0, r3
	bl	get_fat
	str	r0, [sp, #20]
	.loc 1 1454 7
	ldr	r3, [sp, #20]
	cmp	r3, #-1
	bne	.L125
	.loc 1 1454 35 discriminator 1
	movs	r3, #1
	b	.L119
.L125:
	.loc 1 1455 7
	ldr	r3, [sp, #20]
	cmp	r3, #1
	bls	.L126
	.loc 1 1455 30 discriminator 2
	ldr	r3, [sp, #16]
	ldr	r3, [r3, #28]
	.loc 1 1455 17 discriminator 2
	ldr	r2, [sp, #20]
	cmp	r2, r3
	bcc	.L127
.L126:
	.loc 1 1455 49 discriminator 3
	movs	r3, #2
	b	.L119
.L127:
	.loc 1 1456 8
	ldr	r2, [sp]
	ldr	r3, [sp, #12]
	subs	r3, r2, r3
	str	r3, [sp]
.L124:
	.loc 1 1452 9
	ldr	r2, [sp]
	ldr	r3, [sp, #12]
	cmp	r2, r3
	bcs	.L128
	.loc 1 1458 14
	ldr	r1, [sp, #20]
	ldr	r0, [sp, #16]
	bl	clust2sect
	mov	r2, r0
	.loc 1 1458 12
	ldr	r3, [sp, #4]
	str	r2, [r3, #24]
.L123:
	.loc 1 1460 12
	ldr	r3, [sp, #4]
	ldr	r2, [sp, #20]
	str	r2, [r3, #20]
	.loc 1 1461 9
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #24]
	.loc 1 1461 5
	cmp	r3, #0
	bne	.L129
	.loc 1 1461 24 discriminator 1
	movs	r3, #2
	b	.L119
.L129:
	.loc 1 1462 11
	ldr	r3, [sp, #4]
	ldr	r2, [r3, #24]
	.loc 1 1462 18
	ldr	r3, [sp]
	lsrs	r3, r3, #9
	.loc 1 1462 11
	add	r2, r2, r3
	ldr	r3, [sp, #4]
	str	r2, [r3, #24]
	.loc 1 1463 12
	ldr	r3, [sp, #16]
	add	r2, r3, #56
	.loc 1 1463 27
	ldr	r3, [sp]
	ubfx	r3, r3, #0, #9
	.loc 1 1463 20
	add	r2, r2, r3
	.loc 1 1463 10
	ldr	r3, [sp, #4]
	str	r2, [r3, #28]
	.loc 1 1465 9
	movs	r3, #0
.L119:
	.loc 1 1466 1
	mov	r0, r3
	add	sp, sp, #28
.LCFI41:
	@ sp needed
	ldr	pc, [sp], #4
.LFE16:
	.size	dir_sdi, .-dir_sdi
	.section	.text.dir_next,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	dir_next, %function
dir_next:
.LFB17:
	.loc 1 1480 1
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI42:
	sub	sp, sp, #28
.LCFI43:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 1482 9
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	str	r3, [sp, #12]
	.loc 1 1487 10
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #16]
	.loc 1 1487 6
	adds	r3, r3, #32
	str	r3, [sp, #8]
	.loc 1 1488 9
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #24]
	.loc 1 1488 5
	cmp	r3, #0
	beq	.L131
	.loc 1 1488 16 discriminator 2
	ldr	r3, [sp, #8]
	cmp	r3, #2097152
	bcc	.L132
.L131:
	.loc 1 1488 105 discriminator 3
	movs	r3, #4
	b	.L133
.L132:
	.loc 1 1490 10
	ldr	r3, [sp, #8]
	ubfx	r3, r3, #0, #9
	.loc 1 1490 5
	cmp	r3, #0
	bne	.L134
	.loc 1 1491 5
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #24]
	.loc 1 1491 11
	adds	r2, r3, #1
	ldr	r3, [sp, #4]
	str	r2, [r3, #24]
	.loc 1 1493 10
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #20]
	.loc 1 1493 6
	cmp	r3, #0
	bne	.L135
	.loc 1 1494 12
	ldr	r3, [sp, #8]
	lsrs	r3, r3, #5
	.loc 1 1494 26
	ldr	r2, [sp, #12]
	ldrh	r2, [r2, #8]
	.loc 1 1494 7
	cmp	r3, r2
	bcc	.L134
	.loc 1 1495 14
	ldr	r3, [sp, #4]
	movs	r2, #0
	str	r2, [r3, #24]
	.loc 1 1495 26
	movs	r3, #4
	b	.L133
.L135:
	.loc 1 1499 13
	ldr	r3, [sp, #8]
	lsrs	r3, r3, #9
	.loc 1 1499 27
	ldr	r2, [sp, #12]
	ldrh	r2, [r2, #10]
	.loc 1 1499 35
	subs	r2, r2, #1
	.loc 1 1499 22
	ands	r3, r3, r2
	.loc 1 1499 7
	cmp	r3, #0
	bne	.L134
	.loc 1 1500 12
	ldr	r2, [sp, #4]
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #20]
	mov	r1, r3
	mov	r0, r2
	bl	get_fat
	str	r0, [sp, #20]
	.loc 1 1501 8
	ldr	r3, [sp, #20]
	cmp	r3, #1
	bhi	.L136
	.loc 1 1501 27 discriminator 1
	movs	r3, #2
	b	.L133
.L136:
	.loc 1 1502 8
	ldr	r3, [sp, #20]
	cmp	r3, #-1
	bne	.L137
	.loc 1 1502 36 discriminator 1
	movs	r3, #1
	b	.L133
.L137:
	.loc 1 1503 19
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #28]
	.loc 1 1503 8
	ldr	r2, [sp, #20]
	cmp	r2, r3
	bcc	.L138
	.loc 1 1505 9
	ldr	r3, [sp]
	cmp	r3, #0
	bne	.L139
	.loc 1 1506 16
	ldr	r3, [sp, #4]
	movs	r2, #0
	str	r2, [r3, #24]
	.loc 1 1506 28
	movs	r3, #4
	b	.L133
.L139:
	.loc 1 1508 13
	ldr	r2, [sp, #4]
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #20]
	mov	r1, r3
	mov	r0, r2
	bl	create_chain
	str	r0, [sp, #20]
	.loc 1 1509 9
	ldr	r3, [sp, #20]
	cmp	r3, #0
	bne	.L140
	.loc 1 1509 28 discriminator 1
	movs	r3, #7
	b	.L133
.L140:
	.loc 1 1510 9
	ldr	r3, [sp, #20]
	cmp	r3, #1
	bne	.L141
	.loc 1 1510 28 discriminator 1
	movs	r3, #2
	b	.L133
.L141:
	.loc 1 1511 9
	ldr	r3, [sp, #20]
	cmp	r3, #-1
	bne	.L142
	.loc 1 1511 37 discriminator 1
	movs	r3, #1
	b	.L133
.L142:
	.loc 1 1514 10
	ldr	r0, [sp, #12]
	bl	sync_window
	mov	r3, r0
	.loc 1 1514 9
	cmp	r3, #0
	beq	.L143
	.loc 1 1514 43 discriminator 1
	movs	r3, #1
	b	.L133
.L143:
	.loc 1 1515 16
	ldr	r3, [sp, #12]
	adds	r3, r3, #56
	.loc 1 1515 6
	mov	r2, #512
	movs	r1, #0
	mov	r0, r3
	bl	mem_set
	.loc 1 1516 13
	movs	r3, #0
	str	r3, [sp, #16]
	.loc 1 1516 32
	ldr	r1, [sp, #20]
	ldr	r0, [sp, #12]
	bl	clust2sect
	mov	r2, r0
	.loc 1 1516 30
	ldr	r3, [sp, #12]
	str	r2, [r3, #52]
	.loc 1 1516 6
	b	.L144
.L146:
	.loc 1 1517 17
	ldr	r3, [sp, #12]
	movs	r2, #1
	strb	r2, [r3, #3]
	.loc 1 1518 11
	ldr	r0, [sp, #12]
	bl	sync_window
	mov	r3, r0
	.loc 1 1518 10
	cmp	r3, #0
	beq	.L145
	.loc 1 1518 44 discriminator 1
	movs	r3, #1
	b	.L133
.L145:
	.loc 1 1516 70 discriminator 2
	ldr	r3, [sp, #16]
	adds	r3, r3, #1
	str	r3, [sp, #16]
	.loc 1 1516 76 discriminator 2
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #52]
	.loc 1 1516 85 discriminator 2
	adds	r2, r3, #1
	ldr	r3, [sp, #12]
	str	r2, [r3, #52]
.L144:
	.loc 1 1516 60 discriminator 1
	ldr	r3, [sp, #12]
	ldrh	r3, [r3, #10]
	mov	r2, r3
	.loc 1 1516 6 discriminator 1
	ldr	r3, [sp, #16]
	cmp	r3, r2
	bcc	.L146
	.loc 1 1520 18
	ldr	r3, [sp, #12]
	ldr	r2, [r3, #52]
	ldr	r3, [sp, #16]
	subs	r2, r2, r3
	ldr	r3, [sp, #12]
	str	r2, [r3, #52]
.L138:
	.loc 1 1526 15
	ldr	r3, [sp, #4]
	ldr	r2, [sp, #20]
	str	r2, [r3, #20]
	.loc 1 1527 16
	ldr	r1, [sp, #20]
	ldr	r0, [sp, #12]
	bl	clust2sect
	mov	r2, r0
	.loc 1 1527 14
	ldr	r3, [sp, #4]
	str	r2, [r3, #24]
.L134:
	.loc 1 1531 11
	ldr	r3, [sp, #4]
	ldr	r2, [sp, #8]
	str	r2, [r3, #16]
	.loc 1 1532 12
	ldr	r3, [sp, #12]
	add	r2, r3, #56
	.loc 1 1532 26
	ldr	r3, [sp, #8]
	ubfx	r3, r3, #0, #9
	.loc 1 1532 20
	add	r2, r2, r3
	.loc 1 1532 10
	ldr	r3, [sp, #4]
	str	r2, [r3, #28]
	.loc 1 1534 9
	movs	r3, #0
.L133:
	.loc 1 1535 1
	mov	r0, r3
	add	sp, sp, #28
.LCFI44:
	@ sp needed
	ldr	pc, [sp], #4
.LFE17:
	.size	dir_next, .-dir_next
	.section	.text.dir_alloc,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	dir_alloc, %function
dir_alloc:
.LFB18:
	.loc 1 1550 1
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI45:
	sub	sp, sp, #28
.LCFI46:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 1553 9
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	str	r3, [sp, #12]
	.loc 1 1556 8
	movs	r1, #0
	ldr	r0, [sp, #4]
	bl	dir_sdi
	mov	r3, r0
	strb	r3, [sp, #23]
	.loc 1 1557 5
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L148
	.loc 1 1558 5
	movs	r3, #0
	str	r3, [sp, #16]
.L154:
	.loc 1 1560 10
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #24]
	mov	r1, r3
	ldr	r0, [sp, #12]
	bl	move_window
	mov	r3, r0
	strb	r3, [sp, #23]
	.loc 1 1561 7
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L157
	.loc 1 1565 10
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #28]
	.loc 1 1565 15
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 1565 7
	cmp	r3, #229
	beq	.L150
	.loc 1 1565 39 discriminator 1
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #28]
	.loc 1 1565 44 discriminator 1
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 1565 34 discriminator 1
	cmp	r3, #0
	bne	.L151
.L150:
	.loc 1 1567 8
	ldr	r3, [sp, #16]
	adds	r3, r3, #1
	str	r3, [sp, #16]
	ldr	r2, [sp, #16]
	ldr	r3, [sp]
	cmp	r2, r3
	bne	.L153
	.loc 1 1567 5
	b	.L148
.L151:
	.loc 1 1569 7
	movs	r3, #0
	str	r3, [sp, #16]
.L153:
	.loc 1 1571 10
	movs	r1, #1
	ldr	r0, [sp, #4]
	bl	dir_next
	mov	r3, r0
	strb	r3, [sp, #23]
	.loc 1 1572 3
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L154
	b	.L148
.L157:
	.loc 1 1561 4
	nop
.L148:
	.loc 1 1575 5
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #4
	bne	.L155
	.loc 1 1575 29 discriminator 1
	movs	r3, #7
	strb	r3, [sp, #23]
.L155:
	.loc 1 1576 9
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	.loc 1 1577 1
	mov	r0, r3
	add	sp, sp, #28
.LCFI47:
	@ sp needed
	ldr	pc, [sp], #4
.LFE18:
	.size	dir_alloc, .-dir_alloc
	.section	.text.ld_clust,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	ld_clust, %function
ld_clust:
.LFB19:
	.loc 1 1593 1
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI48:
	sub	sp, sp, #20
.LCFI49:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 1596 7
	ldr	r3, [sp]
	adds	r3, r3, #26
	mov	r0, r3
	bl	ld_word
	mov	r3, r0
	.loc 1 1596 5
	str	r3, [sp, #12]
	.loc 1 1597 8
	ldr	r3, [sp, #4]
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 1597 5
	cmp	r3, #3
	bne	.L159
	.loc 1 1598 16
	ldr	r3, [sp]
	adds	r3, r3, #20
	mov	r0, r3
	bl	ld_word
	mov	r3, r0
	.loc 1 1598 45
	lsls	r3, r3, #16
	.loc 1 1598 6
	ldr	r2, [sp, #12]
	orrs	r3, r3, r2
	str	r3, [sp, #12]
.L159:
	.loc 1 1601 9
	ldr	r3, [sp, #12]
	.loc 1 1602 1
	mov	r0, r3
	add	sp, sp, #20
.LCFI50:
	@ sp needed
	ldr	pc, [sp], #4
.LFE19:
	.size	ld_clust, .-ld_clust
	.section	.text.st_clust,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	st_clust, %function
st_clust:
.LFB20:
	.loc 1 1612 1
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI51:
	sub	sp, sp, #20
.LCFI52:
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	str	r2, [sp, #4]
	.loc 1 1613 2
	ldr	r3, [sp, #8]
	adds	r3, r3, #26
	ldr	r2, [sp, #4]
	uxth	r2, r2
	mov	r1, r2
	mov	r0, r3
	bl	st_word
	.loc 1 1614 8
	ldr	r3, [sp, #12]
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 1614 5
	cmp	r3, #3
	bne	.L163
	.loc 1 1615 3
	ldr	r3, [sp, #8]
	add	r2, r3, #20
	.loc 1 1615 42
	ldr	r3, [sp, #4]
	lsrs	r3, r3, #16
	.loc 1 1615 3
	uxth	r3, r3
	mov	r1, r3
	mov	r0, r2
	bl	st_word
.L163:
	.loc 1 1617 1
	nop
	add	sp, sp, #20
.LCFI53:
	@ sp needed
	ldr	pc, [sp], #4
.LFE20:
	.size	st_clust, .-st_clust
	.section	.rodata.LfnOfs,"a"
	.align	2
	.type	LfnOfs, %object
	.size	LfnOfs, 13
LfnOfs:
	.ascii	"\001\003\005\007\011\016\020\022\024\026\030\034\036"
	.section	.text.cmp_lfn,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	cmp_lfn, %function
cmp_lfn:
.LFB21:
	.loc 1 1638 1
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
.LCFI54:
	sub	sp, sp, #24
.LCFI55:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 1643 18
	ldr	r3, [sp]
	adds	r3, r3, #26
	.loc 1 1643 6
	mov	r0, r3
	bl	ld_word
	mov	r3, r0
	.loc 1 1643 5
	cmp	r3, #0
	beq	.L165
	.loc 1 1643 49 discriminator 1
	movs	r3, #0
	b	.L166
.L165:
	.loc 1 1645 11
	ldr	r3, [sp]
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 1645 22
	and	r3, r3, #63
	.loc 1 1645 30
	subs	r2, r3, #1
	.loc 1 1645 35
	mov	r3, r2
	lsls	r3, r3, #1
	add	r3, r3, r2
	lsls	r3, r3, #2
	add	r3, r3, r2
	.loc 1 1645 4
	str	r3, [sp, #20]
	.loc 1 1647 10
	movs	r3, #1
	strh	r3, [sp, #14]	@ movhi
	.loc 1 1647 17
	movs	r3, #0
	str	r3, [sp, #16]
	.loc 1 1647 2
	b	.L167
.L172:
	.loc 1 1648 28
	ldr	r2, .L174
	ldr	r3, [sp, #16]
	add	r3, r3, r2
	ldrb	r3, [r3]	@ zero_extendqisi2
	mov	r2, r3
	.loc 1 1648 20
	ldr	r3, [sp]
	add	r3, r3, r2
	.loc 1 1648 8
	mov	r0, r3
	bl	ld_word
	mov	r3, r0
	strh	r3, [sp, #12]	@ movhi
	.loc 1 1649 6
	ldrh	r3, [sp, #14]
	cmp	r3, #0
	beq	.L168
	.loc 1 1650 7
	ldr	r3, [sp, #20]
	cmp	r3, #254
	bhi	.L169
	.loc 1 1650 25 discriminator 1
	ldrh	r3, [sp, #12]
	mov	r0, r3
	bl	ff_wtoupper
	mov	r3, r0
	mov	r4, r3
	.loc 1 1650 64 discriminator 1
	ldr	r3, [sp, #20]
	adds	r2, r3, #1
	str	r2, [sp, #20]
	.loc 1 1650 62 discriminator 1
	lsls	r3, r3, #1
	ldr	r2, [sp, #4]
	add	r3, r3, r2
	.loc 1 1650 44 discriminator 1
	ldrh	r3, [r3]
	mov	r0, r3
	bl	ff_wtoupper
	mov	r3, r0
	.loc 1 1650 22 discriminator 1
	cmp	r4, r3
	beq	.L170
.L169:
	.loc 1 1651 12
	movs	r3, #0
	b	.L166
.L170:
	.loc 1 1653 7
	ldrh	r3, [sp, #12]	@ movhi
	strh	r3, [sp, #14]	@ movhi
	b	.L171
.L168:
	.loc 1 1655 7
	ldrh	r3, [sp, #12]
	movw	r2, #65535
	cmp	r3, r2
	beq	.L171
	.loc 1 1655 29 discriminator 1
	movs	r3, #0
	b	.L166
.L171:
	.loc 1 1647 31 discriminator 2
	ldr	r3, [sp, #16]
	adds	r3, r3, #1
	str	r3, [sp, #16]
.L167:
	.loc 1 1647 2 discriminator 1
	ldr	r3, [sp, #16]
	cmp	r3, #12
	bls	.L172
	.loc 1 1659 10
	ldr	r3, [sp]
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 1659 21
	and	r3, r3, #64
	.loc 1 1659 5
	cmp	r3, #0
	beq	.L173
	.loc 1 1659 29 discriminator 1
	ldrh	r3, [sp, #14]
	cmp	r3, #0
	beq	.L173
	.loc 1 1659 44 discriminator 2
	ldr	r3, [sp, #20]
	lsls	r3, r3, #1
	ldr	r2, [sp, #4]
	add	r3, r3, r2
	ldrh	r3, [r3]
	.loc 1 1659 35 discriminator 2
	cmp	r3, #0
	beq	.L173
	.loc 1 1659 56 discriminator 3
	movs	r3, #0
	b	.L166
.L173:
	.loc 1 1661 9
	movs	r3, #1
.L166:
	.loc 1 1662 1
	mov	r0, r3
	add	sp, sp, #24
.LCFI56:
	@ sp needed
	pop	{r4, pc}
.L175:
	.align	2
.L174:
	.word	LfnOfs
.LFE21:
	.size	cmp_lfn, .-cmp_lfn
	.section	.text.pick_lfn,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	pick_lfn, %function
pick_lfn:
.LFB22:
	.loc 1 1674 1
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI57:
	sub	sp, sp, #28
.LCFI58:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 1679 18
	ldr	r3, [sp]
	adds	r3, r3, #26
	.loc 1 1679 6
	mov	r0, r3
	bl	ld_word
	mov	r3, r0
	.loc 1 1679 5
	cmp	r3, #0
	beq	.L177
	.loc 1 1679 49 discriminator 1
	movs	r3, #0
	b	.L178
.L177:
	.loc 1 1681 11
	ldr	r3, [sp]
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 1681 22
	and	r3, r3, #63
	.loc 1 1681 30
	subs	r2, r3, #1
	.loc 1 1681 35
	mov	r3, r2
	lsls	r3, r3, #1
	add	r3, r3, r2
	lsls	r3, r3, #2
	add	r3, r3, r2
	.loc 1 1681 4
	str	r3, [sp, #20]
	.loc 1 1683 10
	movs	r3, #1
	strh	r3, [sp, #14]	@ movhi
	.loc 1 1683 17
	movs	r3, #0
	str	r3, [sp, #16]
	.loc 1 1683 2
	b	.L179
.L183:
	.loc 1 1684 28
	ldr	r2, .L186
	ldr	r3, [sp, #16]
	add	r3, r3, r2
	ldrb	r3, [r3]	@ zero_extendqisi2
	mov	r2, r3
	.loc 1 1684 20
	ldr	r3, [sp]
	add	r3, r3, r2
	.loc 1 1684 8
	mov	r0, r3
	bl	ld_word
	mov	r3, r0
	strh	r3, [sp, #12]	@ movhi
	.loc 1 1685 6
	ldrh	r3, [sp, #14]
	cmp	r3, #0
	beq	.L180
	.loc 1 1686 7
	ldr	r3, [sp, #20]
	cmp	r3, #254
	bls	.L181
	.loc 1 1686 30 discriminator 1
	movs	r3, #0
	b	.L178
.L181:
	.loc 1 1687 21
	ldrh	r3, [sp, #12]	@ movhi
	strh	r3, [sp, #14]	@ movhi
	.loc 1 1687 12
	ldr	r3, [sp, #20]
	adds	r2, r3, #1
	str	r2, [sp, #20]
	.loc 1 1687 10
	lsls	r3, r3, #1
	ldr	r2, [sp, #4]
	add	r3, r3, r2
	.loc 1 1687 16
	ldrh	r2, [sp, #14]	@ movhi
	strh	r2, [r3]	@ movhi
	b	.L182
.L180:
	.loc 1 1689 7
	ldrh	r3, [sp, #12]
	movw	r2, #65535
	cmp	r3, r2
	beq	.L182
	.loc 1 1689 29 discriminator 1
	movs	r3, #0
	b	.L178
.L182:
	.loc 1 1683 31 discriminator 2
	ldr	r3, [sp, #16]
	adds	r3, r3, #1
	str	r3, [sp, #16]
.L179:
	.loc 1 1683 2 discriminator 1
	ldr	r3, [sp, #16]
	cmp	r3, #12
	bls	.L183
	.loc 1 1693 9
	ldr	r3, [sp]
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 1693 20
	and	r3, r3, #64
	.loc 1 1693 5
	cmp	r3, #0
	beq	.L184
	.loc 1 1694 6
	ldr	r3, [sp, #20]
	cmp	r3, #254
	bls	.L185
	.loc 1 1694 29 discriminator 1
	movs	r3, #0
	b	.L178
.L185:
	.loc 1 1695 9
	ldr	r3, [sp, #20]
	lsls	r3, r3, #1
	ldr	r2, [sp, #4]
	add	r3, r3, r2
	.loc 1 1695 13
	movs	r2, #0
	strh	r2, [r3]	@ movhi
.L184:
	.loc 1 1698 9
	movs	r3, #1
.L178:
	.loc 1 1699 1
	mov	r0, r3
	add	sp, sp, #28
.LCFI59:
	@ sp needed
	ldr	pc, [sp], #4
.L187:
	.align	2
.L186:
	.word	LfnOfs
.LFE22:
	.size	pick_lfn, .-pick_lfn
	.section	.text.put_lfn,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	put_lfn, %function
put_lfn:
.LFB23:
	.loc 1 1714 1
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI60:
	sub	sp, sp, #36
.LCFI61:
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	mov	r1, r2
	mov	r2, r3
	mov	r3, r1
	strb	r3, [sp, #7]
	mov	r3, r2
	strb	r3, [sp, #6]
	.loc 1 1719 5
	ldr	r3, [sp, #8]
	adds	r3, r3, #13
	.loc 1 1719 19
	ldrb	r2, [sp, #6]
	strb	r2, [r3]
	.loc 1 1720 5
	ldr	r3, [sp, #8]
	adds	r3, r3, #11
	.loc 1 1720 17
	movs	r2, #15
	strb	r2, [r3]
	.loc 1 1721 5
	ldr	r3, [sp, #8]
	adds	r3, r3, #12
	.loc 1 1721 17
	movs	r2, #0
	strb	r2, [r3]
	.loc 1 1722 2
	ldr	r3, [sp, #8]
	adds	r3, r3, #26
	movs	r1, #0
	mov	r0, r3
	bl	st_word
	.loc 1 1724 11
	ldrb	r3, [sp, #7]	@ zero_extendqisi2
	subs	r2, r3, #1
	.loc 1 1724 16
	mov	r3, r2
	lsls	r3, r3, #1
	add	r3, r3, r2
	lsls	r3, r3, #2
	add	r3, r3, r2
	.loc 1 1724 4
	str	r3, [sp, #28]
	.loc 1 1725 9
	movs	r3, #0
	strh	r3, [sp, #22]	@ movhi
	.loc 1 1725 4
	movs	r3, #0
	str	r3, [sp, #24]
.L191:
	.loc 1 1727 6
	ldrh	r3, [sp, #22]
	movw	r2, #65535
	cmp	r3, r2
	beq	.L189
	.loc 1 1727 31 discriminator 1
	ldr	r3, [sp, #28]
	adds	r2, r3, #1
	str	r2, [sp, #28]
	.loc 1 1727 29 discriminator 1
	lsls	r3, r3, #1
	ldr	r2, [sp, #12]
	add	r3, r3, r2
	.loc 1 1727 24 discriminator 1
	ldrh	r3, [r3]	@ movhi
	strh	r3, [sp, #22]	@ movhi
.L189:
	.loc 1 1728 23
	ldr	r2, .L194
	ldr	r3, [sp, #24]
	add	r3, r3, r2
	ldrb	r3, [r3]	@ zero_extendqisi2
	mov	r2, r3
	.loc 1 1728 3
	ldr	r3, [sp, #8]
	add	r3, r3, r2
	ldrh	r2, [sp, #22]
	mov	r1, r2
	mov	r0, r3
	bl	st_word
	.loc 1 1729 6
	ldrh	r3, [sp, #22]
	cmp	r3, #0
	bne	.L190
	.loc 1 1729 19 discriminator 1
	movw	r3, #65535
	strh	r3, [sp, #22]	@ movhi
.L190:
	.loc 1 1730 2
	ldr	r3, [sp, #24]
	adds	r3, r3, #1
	str	r3, [sp, #24]
	ldr	r3, [sp, #24]
	cmp	r3, #12
	bls	.L191
	.loc 1 1731 5
	ldrh	r3, [sp, #22]
	movw	r2, #65535
	cmp	r3, r2
	beq	.L192
	.loc 1 1731 26 discriminator 2
	ldr	r3, [sp, #28]
	lsls	r3, r3, #1
	ldr	r2, [sp, #12]
	add	r3, r3, r2
	ldrh	r3, [r3]
	.loc 1 1731 19 discriminator 2
	cmp	r3, #0
	bne	.L193
.L192:
	.loc 1 1731 35 discriminator 3
	ldrb	r3, [sp, #7]
	orr	r3, r3, #64
	strb	r3, [sp, #7]
.L193:
	.loc 1 1732 16
	ldr	r3, [sp, #8]
	ldrb	r2, [sp, #7]
	strb	r2, [r3]
	.loc 1 1733 1
	nop
	add	sp, sp, #36
.LCFI62:
	@ sp needed
	ldr	pc, [sp], #4
.L195:
	.align	2
.L194:
	.word	LfnOfs
.LFE23:
	.size	put_lfn, .-put_lfn
	.section	.text.gen_numname,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	gen_numname, %function
gen_numname:
.LFB24:
	.loc 1 1752 1
	@ args = 0, pretend = 0, frame = 48
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI63:
	sub	sp, sp, #52
.LCFI64:
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	str	r2, [sp, #4]
	str	r3, [sp]
	.loc 1 1759 2
	movs	r2, #11
	ldr	r1, [sp, #8]
	ldr	r0, [sp, #12]
	bl	mem_cpy
	.loc 1 1761 5
	ldr	r3, [sp]
	cmp	r3, #5
	bls	.L197
	.loc 1 1762 6
	ldr	r3, [sp]
	str	r3, [sp, #28]
	.loc 1 1763 9
	b	.L198
.L202:
	.loc 1 1764 13
	ldr	r3, [sp, #4]
	adds	r2, r3, #2
	str	r2, [sp, #4]
	.loc 1 1764 7
	ldrh	r3, [r3]	@ movhi
	strh	r3, [sp, #34]	@ movhi
	.loc 1 1765 11
	movs	r3, #0
	str	r3, [sp, #40]
	.loc 1 1765 4
	b	.L199
.L201:
	.loc 1 1766 14
	ldr	r3, [sp, #28]
	lsls	r2, r3, #1
	.loc 1 1766 26
	ldrh	r3, [sp, #34]
	and	r3, r3, #1
	.loc 1 1766 8
	add	r3, r3, r2
	str	r3, [sp, #28]
	.loc 1 1767 8
	ldrh	r3, [sp, #34]
	lsrs	r3, r3, #1
	strh	r3, [sp, #34]	@ movhi
	.loc 1 1768 12
	ldr	r3, [sp, #28]
	and	r3, r3, #65536
	.loc 1 1768 8
	cmp	r3, #0
	beq	.L200
	.loc 1 1768 26 discriminator 1
	ldr	r3, [sp, #28]
	eor	r3, r3, #69632
	eor	r3, r3, #33
	str	r3, [sp, #28]
.L200:
	.loc 1 1765 25 discriminator 2
	ldr	r3, [sp, #40]
	adds	r3, r3, #1
	str	r3, [sp, #40]
.L199:
	.loc 1 1765 4 discriminator 1
	ldr	r3, [sp, #40]
	cmp	r3, #15
	bls	.L201
.L198:
	.loc 1 1763 10
	ldr	r3, [sp, #4]
	ldrh	r3, [r3]
	.loc 1 1763 9
	cmp	r3, #0
	bne	.L202
	.loc 1 1771 7
	ldr	r3, [sp, #28]
	str	r3, [sp]
.L197:
	.loc 1 1775 4
	movs	r3, #7
	str	r3, [sp, #40]
.L204:
	.loc 1 1777 7
	ldr	r3, [sp]
	uxtb	r3, r3
	and	r3, r3, #15
	uxtb	r3, r3
	.loc 1 1777 5
	adds	r3, r3, #48
	strb	r3, [sp, #47]
	.loc 1 1778 6
	ldrb	r3, [sp, #47]	@ zero_extendqisi2
	cmp	r3, #57
	bls	.L203
	.loc 1 1778 18 discriminator 1
	ldrb	r3, [sp, #47]
	adds	r3, r3, #7
	strb	r3, [sp, #47]
.L203:
	.loc 1 1779 7
	ldr	r3, [sp, #40]
	subs	r2, r3, #1
	str	r2, [sp, #40]
	.loc 1 1779 11
	add	r2, sp, #48
	add	r3, r3, r2
	ldrb	r2, [sp, #47]
	strb	r2, [r3, #-28]
	.loc 1 1780 7
	ldr	r3, [sp]
	lsrs	r3, r3, #4
	str	r3, [sp]
	.loc 1 1781 2
	ldr	r3, [sp]
	cmp	r3, #0
	bne	.L204
	.loc 1 1782 8
	add	r2, sp, #20
	ldr	r3, [sp, #40]
	add	r3, r3, r2
	movs	r2, #126
	strb	r2, [r3]
	.loc 1 1785 9
	movs	r3, #0
	str	r3, [sp, #36]
	.loc 1 1785 2
	b	.L205
.L211:
	.loc 1 1786 7
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #36]
	add	r3, r3, r2
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 1786 6
	cmp	r3, #128
	bls	.L206
	.loc 1 1786 7 discriminator 1
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #36]
	add	r3, r3, r2
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #159
	bls	.L207
.L206:
	.loc 1 1786 7 is_stmt 0 discriminator 3
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #36]
	add	r3, r3, r2
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #223
	bls	.L208
	.loc 1 1786 7 discriminator 4
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #36]
	add	r3, r3, r2
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #252
	bhi	.L208
.L207:
	.loc 1 1787 15 is_stmt 1
	ldr	r3, [sp, #40]
	subs	r3, r3, #1
	.loc 1 1787 7
	ldr	r2, [sp, #36]
	cmp	r2, r3
	beq	.L215
	.loc 1 1788 5
	ldr	r3, [sp, #36]
	adds	r3, r3, #1
	str	r3, [sp, #36]
.L208:
	.loc 1 1785 39 discriminator 2
	ldr	r3, [sp, #36]
	adds	r3, r3, #1
	str	r3, [sp, #36]
.L205:
	.loc 1 1785 2 discriminator 1
	ldr	r2, [sp, #36]
	ldr	r3, [sp, #40]
	cmp	r2, r3
	bcs	.L214
	.loc 1 1785 26 discriminator 3
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #36]
	add	r3, r3, r2
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 1785 20 discriminator 3
	cmp	r3, #32
	bne	.L211
	b	.L214
.L215:
	.loc 1 1787 4
	nop
.L214:
	.loc 1 1792 12
	ldr	r3, [sp, #40]
	cmp	r3, #7
	bhi	.L212
	.loc 1 1792 28 discriminator 1
	ldr	r3, [sp, #40]
	adds	r2, r3, #1
	str	r2, [sp, #40]
	.loc 1 1792 12 discriminator 1
	add	r2, sp, #48
	add	r3, r3, r2
	ldrb	r1, [r3, #-28]	@ zero_extendqisi2
	b	.L213
.L212:
	.loc 1 1792 12 is_stmt 0 discriminator 2
	movs	r1, #32
.L213:
	.loc 1 1792 8 is_stmt 1 discriminator 4
	ldr	r3, [sp, #36]
	adds	r2, r3, #1
	str	r2, [sp, #36]
	.loc 1 1792 6 discriminator 4
	ldr	r2, [sp, #12]
	add	r3, r3, r2
	.loc 1 1792 12 discriminator 4
	mov	r2, r1
	strb	r2, [r3]
	.loc 1 1793 2 discriminator 4
	ldr	r3, [sp, #36]
	cmp	r3, #7
	bls	.L214
	.loc 1 1794 1
	nop
	nop
	add	sp, sp, #52
.LCFI65:
	@ sp needed
	ldr	pc, [sp], #4
.LFE24:
	.size	gen_numname, .-gen_numname
	.section	.text.sum_sfn,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	sum_sfn, %function
sum_sfn:
.LFB25:
	.loc 1 1808 1
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #16
.LCFI66:
	str	r0, [sp, #4]
	.loc 1 1809 7
	movs	r3, #0
	strb	r3, [sp, #15]
	.loc 1 1810 7
	movs	r3, #11
	str	r3, [sp, #8]
.L217:
	.loc 1 1812 22 discriminator 1
	ldrb	r3, [sp, #15]
	uxtb	r2, r3
	lsrs	r2, r2, #1
	lsls	r3, r3, #7
	orrs	r3, r3, r2
	uxtb	r2, r3
	.loc 1 1812 41 discriminator 1
	ldr	r3, [sp, #4]
	adds	r1, r3, #1
	str	r1, [sp, #4]
	.loc 1 1812 37 discriminator 1
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 1812 9 discriminator 1
	add	r3, r3, r2
	strb	r3, [sp, #15]
	.loc 1 1812 2 discriminator 1
	ldr	r3, [sp, #8]
	subs	r3, r3, #1
	str	r3, [sp, #8]
	ldr	r3, [sp, #8]
	cmp	r3, #0
	bne	.L217
	.loc 1 1813 9
	ldrb	r3, [sp, #15]	@ zero_extendqisi2
	.loc 1 1814 1
	mov	r0, r3
	add	sp, sp, #16
.LCFI67:
	@ sp needed
	bx	lr
.LFE25:
	.size	sum_sfn, .-sum_sfn
	.section	.text.dir_read,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	dir_read, %function
dir_read:
.LFB26:
	.loc 1 2093 1
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI68:
	sub	sp, sp, #28
.LCFI69:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 2094 10
	movs	r3, #4
	strb	r3, [sp, #23]
	.loc 1 2095 9
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	str	r3, [sp, #16]
	.loc 1 2098 7
	movs	r3, #255
	strb	r3, [sp, #21]
	.loc 1 2098 19
	movs	r3, #255
	strb	r3, [sp, #20]
	.loc 1 2101 8
	b	.L220
.L233:
	.loc 1 2102 9
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #24]
	mov	r1, r3
	ldr	r0, [sp, #16]
	bl	move_window
	mov	r3, r0
	strb	r3, [sp, #23]
	.loc 1 2103 6
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L236
	.loc 1 2104 9
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #28]
	.loc 1 2104 5
	ldrb	r3, [r3]
	strb	r3, [sp, #22]
	.loc 1 2105 6
	ldrb	r3, [sp, #22]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L223
	.loc 1 2105 21 discriminator 1
	movs	r3, #4
	strb	r3, [sp, #23]
	.loc 1 2105 3 discriminator 1
	b	.L222
.L223:
	.loc 1 2123 25
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #28]
	.loc 1 2123 30
	adds	r3, r3, #11
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 2123 21
	and	r3, r3, #63
	strb	r3, [sp, #15]
	.loc 1 2123 17
	ldr	r3, [sp, #4]
	ldrb	r2, [sp, #15]
	strb	r2, [r3, #6]
	.loc 1 2125 7
	ldrb	r3, [sp, #22]	@ zero_extendqisi2
	cmp	r3, #229
	beq	.L224
	.loc 1 2125 18 discriminator 1
	ldrb	r3, [sp, #22]	@ zero_extendqisi2
	cmp	r3, #46
	beq	.L224
	.loc 1 2125 42 discriminator 2
	ldrb	r3, [sp, #15]	@ zero_extendqisi2
	bic	r3, r3, #32
	.loc 1 2125 33 discriminator 2
	cmp	r3, #8
	ite	eq
	moveq	r3, #1
	movne	r3, #0
	uxtb	r3, r3
	mov	r2, r3
	.loc 1 2125 30 discriminator 2
	ldr	r3, [sp]
	cmp	r3, r2
	beq	.L225
.L224:
	.loc 1 2126 9
	movs	r3, #255
	strb	r3, [sp, #21]
	b	.L226
.L225:
	.loc 1 2128 8
	ldrb	r3, [sp, #15]	@ zero_extendqisi2
	cmp	r3, #15
	bne	.L227
	.loc 1 2129 12
	ldrb	r3, [sp, #22]	@ zero_extendqisi2
	and	r3, r3, #64
	.loc 1 2129 9
	cmp	r3, #0
	beq	.L228
	.loc 1 2130 15
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #28]
	.loc 1 2130 11
	ldrb	r3, [r3, #13]
	strb	r3, [sp, #20]
	.loc 1 2131 9
	ldrb	r3, [sp, #22]
	bic	r3, r3, #64
	strb	r3, [sp, #22]
	.loc 1 2131 29
	ldrb	r3, [sp, #22]
	strb	r3, [sp, #21]
	.loc 1 2132 23
	ldr	r3, [sp, #4]
	ldr	r2, [r3, #16]
	.loc 1 2132 19
	ldr	r3, [sp, #4]
	str	r2, [r3, #44]
.L228:
	.loc 1 2135 10
	ldrb	r2, [sp, #22]	@ zero_extendqisi2
	ldrb	r3, [sp, #21]	@ zero_extendqisi2
	cmp	r2, r3
	bne	.L229
	.loc 1 2135 34 discriminator 1
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #28]
	.loc 1 2135 39 discriminator 1
	adds	r3, r3, #13
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 2135 22 discriminator 1
	ldrb	r2, [sp, #20]	@ zero_extendqisi2
	cmp	r2, r3
	bne	.L229
	.loc 1 2135 56 discriminator 3
	ldr	r3, [sp, #16]
	ldr	r2, [r3, #12]
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #28]
	mov	r1, r3
	mov	r0, r2
	bl	pick_lfn
	mov	r3, r0
	.loc 1 2135 53 discriminator 3
	cmp	r3, #0
	beq	.L229
	.loc 1 2135 10 discriminator 5
	ldrb	r3, [sp, #21]
	subs	r3, r3, #1
	uxtb	r3, r3
	b	.L230
.L229:
	.loc 1 2135 10 is_stmt 0 discriminator 6
	movs	r3, #255
.L230:
	.loc 1 2135 10 discriminator 8
	strb	r3, [sp, #21]
	b	.L226
.L227:
	.loc 1 2137 9 is_stmt 1
	ldrb	r3, [sp, #21]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L231
	.loc 1 2137 34 discriminator 1
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #28]
	.loc 1 2137 24 discriminator 1
	mov	r0, r3
	bl	sum_sfn
	mov	r3, r0
	mov	r2, r3
	.loc 1 2137 14 discriminator 1
	ldrb	r3, [sp, #20]	@ zero_extendqisi2
	cmp	r3, r2
	beq	.L237
.L231:
	.loc 1 2138 19
	ldr	r3, [sp, #4]
	mov	r2, #-1
	str	r2, [r3, #44]
	.loc 1 2140 6
	b	.L237
.L226:
	.loc 1 2149 9
	movs	r1, #0
	ldr	r0, [sp, #4]
	bl	dir_next
	mov	r3, r0
	strb	r3, [sp, #23]
	.loc 1 2150 6
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L238
.L220:
	.loc 1 2101 11
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #24]
	.loc 1 2101 8
	cmp	r3, #0
	bne	.L233
	b	.L222
.L236:
	.loc 1 2103 3
	nop
	b	.L222
.L237:
	.loc 1 2140 6
	nop
	b	.L222
.L238:
	.loc 1 2150 3
	nop
.L222:
	.loc 1 2153 5
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L234
	.loc 1 2153 29 discriminator 1
	ldr	r3, [sp, #4]
	movs	r2, #0
	str	r2, [r3, #24]
.L234:
	.loc 1 2154 9
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	.loc 1 2155 1
	mov	r0, r3
	add	sp, sp, #28
.LCFI70:
	@ sp needed
	ldr	pc, [sp], #4
.LFE26:
	.size	dir_read, .-dir_read
	.section	.text.dir_find,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	dir_find, %function
dir_find:
.LFB27:
	.loc 1 2169 1
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI71:
	sub	sp, sp, #28
.LCFI72:
	str	r0, [sp, #4]
	.loc 1 2171 9
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	str	r3, [sp, #16]
	.loc 1 2177 8
	movs	r1, #0
	ldr	r0, [sp, #4]
	bl	dir_sdi
	mov	r3, r0
	strb	r3, [sp, #23]
	.loc 1 2178 5
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L240
	.loc 1 2178 27 discriminator 1
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	b	.L241
.L240:
	.loc 1 2198 12
	movs	r3, #255
	strb	r3, [sp, #20]
	.loc 1 2198 6
	ldrb	r3, [sp, #20]
	strb	r3, [sp, #21]
	.loc 1 2198 32
	ldr	r3, [sp, #4]
	mov	r2, #-1
	str	r2, [r3, #44]
.L254:
	.loc 1 2201 9
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #24]
	mov	r1, r3
	ldr	r0, [sp, #16]
	bl	move_window
	mov	r3, r0
	strb	r3, [sp, #23]
	.loc 1 2202 6
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L255
	.loc 1 2203 9
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #28]
	.loc 1 2203 5
	ldrb	r3, [r3]
	strb	r3, [sp, #22]
	.loc 1 2204 6
	ldrb	r3, [sp, #22]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L244
	.loc 1 2204 21 discriminator 1
	movs	r3, #4
	strb	r3, [sp, #23]
	.loc 1 2204 3 discriminator 1
	b	.L243
.L244:
	.loc 1 2206 24
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #28]
	.loc 1 2206 29
	adds	r3, r3, #11
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 2206 20
	and	r3, r3, #63
	strb	r3, [sp, #15]
	.loc 1 2206 16
	ldr	r3, [sp, #4]
	ldrb	r2, [sp, #15]
	strb	r2, [r3, #6]
	.loc 1 2207 6
	ldrb	r3, [sp, #22]	@ zero_extendqisi2
	cmp	r3, #229
	beq	.L245
	.loc 1 2207 24 discriminator 1
	ldrb	r3, [sp, #15]	@ zero_extendqisi2
	and	r3, r3, #8
	.loc 1 2207 17 discriminator 1
	cmp	r3, #0
	beq	.L246
	.loc 1 2207 34 discriminator 2
	ldrb	r3, [sp, #15]	@ zero_extendqisi2
	cmp	r3, #15
	beq	.L246
.L245:
	.loc 1 2208 8
	movs	r3, #255
	strb	r3, [sp, #21]
	.loc 1 2208 28
	ldr	r3, [sp, #4]
	mov	r2, #-1
	str	r2, [r3, #44]
	b	.L247
.L246:
	.loc 1 2210 7
	ldrb	r3, [sp, #15]	@ zero_extendqisi2
	cmp	r3, #15
	bne	.L248
	.loc 1 2211 17
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #43]	@ zero_extendqisi2
	.loc 1 2211 26
	and	r3, r3, #64
	.loc 1 2211 8
	cmp	r3, #0
	bne	.L247
	.loc 1 2212 12
	ldrb	r3, [sp, #22]	@ zero_extendqisi2
	and	r3, r3, #64
	.loc 1 2212 9
	cmp	r3, #0
	beq	.L249
	.loc 1 2213 15
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #28]
	.loc 1 2213 11
	ldrb	r3, [r3, #13]
	strb	r3, [sp, #20]
	.loc 1 2214 9
	ldrb	r3, [sp, #22]
	bic	r3, r3, #64
	strb	r3, [sp, #22]
	.loc 1 2214 29
	ldrb	r3, [sp, #22]
	strb	r3, [sp, #21]
	.loc 1 2215 23
	ldr	r3, [sp, #4]
	ldr	r2, [r3, #16]
	.loc 1 2215 19
	ldr	r3, [sp, #4]
	str	r2, [r3, #44]
.L249:
	.loc 1 2218 10
	ldrb	r2, [sp, #22]	@ zero_extendqisi2
	ldrb	r3, [sp, #21]	@ zero_extendqisi2
	cmp	r2, r3
	bne	.L250
	.loc 1 2218 34 discriminator 1
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #28]
	.loc 1 2218 39 discriminator 1
	adds	r3, r3, #13
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 2218 22 discriminator 1
	ldrb	r2, [sp, #20]	@ zero_extendqisi2
	cmp	r2, r3
	bne	.L250
	.loc 1 2218 66 discriminator 3
	ldr	r3, [sp, #16]
	ldr	r2, [r3, #12]
	.loc 1 2218 56 discriminator 3
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #28]
	mov	r1, r3
	mov	r0, r2
	bl	cmp_lfn
	mov	r3, r0
	.loc 1 2218 53 discriminator 3
	cmp	r3, #0
	beq	.L250
	.loc 1 2218 10 discriminator 5
	ldrb	r3, [sp, #21]
	subs	r3, r3, #1
	uxtb	r3, r3
	b	.L251
.L250:
	.loc 1 2218 10 is_stmt 0 discriminator 6
	movs	r3, #255
.L251:
	.loc 1 2218 10 discriminator 8
	strb	r3, [sp, #21]
	b	.L247
.L248:
	.loc 1 2221 8 is_stmt 1
	ldrb	r3, [sp, #21]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L252
	.loc 1 2221 34 discriminator 1
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #28]
	.loc 1 2221 24 discriminator 1
	mov	r0, r3
	bl	sum_sfn
	mov	r3, r0
	mov	r2, r3
	.loc 1 2221 14 discriminator 1
	ldrb	r3, [sp, #20]	@ zero_extendqisi2
	cmp	r3, r2
	beq	.L256
.L252:
	.loc 1 2222 17
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #43]	@ zero_extendqisi2
	.loc 1 2222 26
	and	r3, r3, #1
	.loc 1 2222 8
	cmp	r3, #0
	bne	.L253
	.loc 1 2222 51 discriminator 1
	ldr	r3, [sp, #4]
	ldr	r0, [r3, #28]
	.loc 1 2222 60 discriminator 1
	ldr	r3, [sp, #4]
	adds	r3, r3, #32
	.loc 1 2222 41 discriminator 1
	movs	r2, #11
	mov	r1, r3
	bl	mem_cmp
	mov	r3, r0
	.loc 1 2222 37 discriminator 1
	cmp	r3, #0
	beq	.L257
.L253:
	.loc 1 2223 9
	movs	r3, #255
	strb	r3, [sp, #21]
	.loc 1 2223 29
	ldr	r3, [sp, #4]
	mov	r2, #-1
	str	r2, [r3, #44]
.L247:
	.loc 1 2230 9
	movs	r1, #0
	ldr	r0, [sp, #4]
	bl	dir_next
	mov	r3, r0
	strb	r3, [sp, #23]
	.loc 1 2231 2
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L254
	b	.L243
.L255:
	.loc 1 2202 3
	nop
	b	.L243
.L256:
	.loc 1 2221 5
	nop
	b	.L243
.L257:
	.loc 1 2222 5
	nop
.L243:
	.loc 1 2233 9
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
.L241:
	.loc 1 2234 1
	mov	r0, r3
	add	sp, sp, #28
.LCFI73:
	@ sp needed
	ldr	pc, [sp], #4
.LFE27:
	.size	dir_find, .-dir_find
	.section	.text.dir_register,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	dir_register, %function
dir_register:
.LFB28:
	.loc 1 2248 1
	@ args = 0, pretend = 0, frame = 48
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI74:
	sub	sp, sp, #52
.LCFI75:
	str	r0, [sp, #4]
	.loc 1 2250 9
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	str	r3, [sp, #28]
	.loc 1 2256 12
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #43]	@ zero_extendqisi2
	.loc 1 2256 21
	and	r3, r3, #160
	.loc 1 2256 5
	cmp	r3, #0
	beq	.L259
	.loc 1 2256 52 discriminator 1
	movs	r3, #6
	b	.L276
.L259:
	.loc 1 2257 12
	movs	r3, #0
	str	r3, [sp, #36]
	.loc 1 2257 2
	b	.L261
.L262:
	.loc 1 2257 39 discriminator 3
	ldr	r3, [sp, #36]
	adds	r3, r3, #1
	str	r3, [sp, #36]
.L261:
	.loc 1 2257 19 discriminator 1
	ldr	r3, [sp, #28]
	ldr	r2, [r3, #12]
	.loc 1 2257 27 discriminator 1
	ldr	r3, [sp, #36]
	lsls	r3, r3, #1
	add	r3, r3, r2
	ldrh	r3, [r3]
	.loc 1 2257 2 discriminator 1
	cmp	r3, #0
	bne	.L262
	.loc 1 2287 16
	ldr	r3, [sp, #4]
	add	r1, r3, #32
	.loc 1 2287 2
	add	r3, sp, #12
	movs	r2, #12
	mov	r0, r3
	bl	mem_cpy
	.loc 1 2288 8
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	.loc 1 2288 17
	and	r3, r3, #1
	.loc 1 2288 5
	cmp	r3, #0
	beq	.L263
	.loc 1 2289 18
	ldr	r3, [sp, #4]
	movs	r2, #64
	strb	r2, [r3, #43]
	.loc 1 2290 10
	movs	r3, #1
	str	r3, [sp, #40]
	.loc 1 2290 3
	b	.L264
.L267:
	.loc 1 2291 18
	ldr	r3, [sp, #4]
	add	r0, r3, #32
	.loc 1 2291 30
	ldr	r3, [sp, #28]
	ldr	r2, [r3, #12]
	.loc 1 2291 4
	add	r1, sp, #12
	ldr	r3, [sp, #40]
	bl	gen_numname
	.loc 1 2292 10
	ldr	r0, [sp, #4]
	bl	dir_find
	mov	r3, r0
	strb	r3, [sp, #47]
	.loc 1 2293 7
	ldrb	r3, [sp, #47]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L277
	.loc 1 2290 25 discriminator 2
	ldr	r3, [sp, #40]
	adds	r3, r3, #1
	str	r3, [sp, #40]
.L264:
	.loc 1 2290 3 discriminator 1
	ldr	r3, [sp, #40]
	cmp	r3, #99
	bls	.L267
	b	.L266
.L277:
	.loc 1 2293 4
	nop
.L266:
	.loc 1 2295 6
	ldr	r3, [sp, #40]
	cmp	r3, #100
	bne	.L268
	.loc 1 2295 24 discriminator 1
	movs	r3, #7
	b	.L276
.L268:
	.loc 1 2296 6
	ldrb	r3, [sp, #47]	@ zero_extendqisi2
	cmp	r3, #4
	beq	.L269
	.loc 1 2296 33 discriminator 1
	ldrb	r3, [sp, #47]	@ zero_extendqisi2
	b	.L276
.L269:
	.loc 1 2297 22
	ldrb	r2, [sp, #23]	@ zero_extendqisi2
	.loc 1 2297 18
	ldr	r3, [sp, #4]
	strb	r2, [r3, #43]
.L263:
	.loc 1 2301 12
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	.loc 1 2301 21
	and	r3, r3, #2
	.loc 1 2301 54
	cmp	r3, #0
	beq	.L270
	.loc 1 2301 39 discriminator 1
	ldr	r3, [sp, #36]
	adds	r3, r3, #12
	.loc 1 2301 45 discriminator 1
	ldr	r2, .L279
	umull	r2, r3, r2, r3
	lsrs	r3, r3, #2
	.loc 1 2301 54 discriminator 1
	adds	r3, r3, #1
	b	.L271
.L270:
	.loc 1 2301 54 is_stmt 0 discriminator 2
	movs	r3, #1
.L271:
	.loc 1 2301 7 is_stmt 1 discriminator 4
	str	r3, [sp, #32]
	.loc 1 2302 8 discriminator 4
	ldr	r1, [sp, #32]
	ldr	r0, [sp, #4]
	bl	dir_alloc
	mov	r3, r0
	strb	r3, [sp, #47]
	.loc 1 2303 5 discriminator 4
	ldrb	r3, [sp, #47]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L272
	.loc 1 2303 19 discriminator 1
	ldr	r3, [sp, #32]
	subs	r3, r3, #1
	str	r3, [sp, #32]
	ldr	r3, [sp, #32]
	cmp	r3, #0
	beq	.L272
	.loc 1 2304 23
	ldr	r3, [sp, #4]
	ldr	r2, [r3, #16]
	.loc 1 2304 37
	ldr	r3, [sp, #32]
	lsls	r3, r3, #5
	.loc 1 2304 9
	subs	r3, r2, r3
	mov	r1, r3
	ldr	r0, [sp, #4]
	bl	dir_sdi
	mov	r3, r0
	strb	r3, [sp, #47]
	.loc 1 2305 6
	ldrb	r3, [sp, #47]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L272
	.loc 1 2306 20
	ldr	r3, [sp, #4]
	adds	r3, r3, #32
	.loc 1 2306 10
	mov	r0, r3
	bl	sum_sfn
	mov	r3, r0
	strb	r3, [sp, #27]
.L274:
	.loc 1 2308 11
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #24]
	mov	r1, r3
	ldr	r0, [sp, #28]
	bl	move_window
	mov	r3, r0
	strb	r3, [sp, #47]
	.loc 1 2309 8
	ldrb	r3, [sp, #47]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L278
	.loc 1 2310 15
	ldr	r3, [sp, #28]
	ldr	r0, [r3, #12]
	.loc 1 2310 5
	ldr	r3, [sp, #4]
	ldr	r1, [r3, #28]
	ldr	r3, [sp, #32]
	uxtb	r2, r3
	ldrb	r3, [sp, #27]	@ zero_extendqisi2
	bl	put_lfn
	.loc 1 2311 15
	ldr	r3, [sp, #28]
	movs	r2, #1
	strb	r2, [r3, #3]
	.loc 1 2312 11
	movs	r1, #0
	ldr	r0, [sp, #4]
	bl	dir_next
	mov	r3, r0
	strb	r3, [sp, #47]
	.loc 1 2313 4
	ldrb	r3, [sp, #47]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L272
	.loc 1 2313 26 discriminator 1
	ldr	r3, [sp, #32]
	subs	r3, r3, #1
	str	r3, [sp, #32]
	ldr	r3, [sp, #32]
	cmp	r3, #0
	bne	.L274
	b	.L272
.L278:
	.loc 1 2309 5
	nop
.L272:
	.loc 1 2323 5
	ldrb	r3, [sp, #47]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L275
	.loc 1 2324 9
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #24]
	mov	r1, r3
	ldr	r0, [sp, #28]
	bl	move_window
	mov	r3, r0
	strb	r3, [sp, #47]
	.loc 1 2325 6
	ldrb	r3, [sp, #47]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L275
	.loc 1 2326 14
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #28]
	.loc 1 2326 4
	movs	r2, #32
	movs	r1, #0
	mov	r0, r3
	bl	mem_set
	.loc 1 2327 20
	ldr	r3, [sp, #4]
	ldr	r0, [r3, #28]
	.loc 1 2327 34
	ldr	r3, [sp, #4]
	adds	r3, r3, #32
	.loc 1 2327 4
	movs	r2, #11
	mov	r1, r3
	bl	mem_cpy
	.loc 1 2329 31
	ldr	r3, [sp, #4]
	ldrb	r2, [r3, #43]	@ zero_extendqisi2
	.loc 1 2329 6
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #28]
	.loc 1 2329 11
	adds	r3, r3, #12
	.loc 1 2329 40
	and	r2, r2, #24
	uxtb	r2, r2
	.loc 1 2329 23
	strb	r2, [r3]
	.loc 1 2331 14
	ldr	r3, [sp, #28]
	movs	r2, #1
	strb	r2, [r3, #3]
.L275:
	.loc 1 2335 9
	ldrb	r3, [sp, #47]	@ zero_extendqisi2
.L276:
	.loc 1 2336 1 discriminator 1
	mov	r0, r3
	add	sp, sp, #52
.LCFI76:
	@ sp needed
	ldr	pc, [sp], #4
.L280:
	.align	2
.L279:
	.word	1321528399
.LFE28:
	.size	dir_register, .-dir_register
	.section	.text.dir_remove,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	dir_remove, %function
dir_remove:
.LFB29:
	.loc 1 2351 1
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI77:
	sub	sp, sp, #28
.LCFI78:
	str	r0, [sp, #4]
	.loc 1 2353 9
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	str	r3, [sp, #16]
	.loc 1 2355 8
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #16]
	str	r3, [sp, #12]
	.loc 1 2357 11
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #44]
	.loc 1 2357 44
	cmp	r3, #-1
	beq	.L282
	.loc 1 2357 46 discriminator 1
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #44]
	mov	r1, r3
	ldr	r0, [sp, #4]
	bl	dir_sdi
	mov	r3, r0
	b	.L283
.L282:
	.loc 1 2357 44 discriminator 2
	movs	r3, #0
.L283:
	.loc 1 2357 6 discriminator 4
	strb	r3, [sp, #23]
	.loc 1 2358 5 discriminator 4
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L284
.L288:
	.loc 1 2360 10
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #24]
	mov	r1, r3
	ldr	r0, [sp, #16]
	bl	move_window
	mov	r3, r0
	strb	r3, [sp, #23]
	.loc 1 2361 7
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L290
	.loc 1 2366 7
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #28]
	.loc 1 2366 23
	movs	r2, #229
	strb	r2, [r3]
	.loc 1 2368 14
	ldr	r3, [sp, #16]
	movs	r2, #1
	strb	r2, [r3, #3]
	.loc 1 2369 10
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #16]
	.loc 1 2369 7
	ldr	r2, [sp, #12]
	cmp	r2, r3
	bls	.L291
	.loc 1 2370 10
	movs	r1, #0
	ldr	r0, [sp, #4]
	bl	dir_next
	mov	r3, r0
	strb	r3, [sp, #23]
	.loc 1 2371 3
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L288
	b	.L286
.L290:
	.loc 1 2361 4
	nop
	b	.L286
.L291:
	.loc 1 2369 4
	nop
.L286:
	.loc 1 2372 6
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #4
	bne	.L284
	.loc 1 2372 30 discriminator 1
	movs	r3, #2
	strb	r3, [sp, #23]
.L284:
	.loc 1 2383 9
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	.loc 1 2384 1
	mov	r0, r3
	add	sp, sp, #28
.LCFI79:
	@ sp needed
	ldr	pc, [sp], #4
.LFE29:
	.size	dir_remove, .-dir_remove
	.section	.text.get_fileinfo,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	get_fileinfo, %function
get_fileinfo:
.LFB30:
	.loc 1 2400 1
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI80:
	sub	sp, sp, #36
.LCFI81:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 2406 9
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	str	r3, [sp, #16]
	.loc 1 2410 16
	ldr	r3, [sp]
	movs	r2, #0
	strb	r2, [r3, #22]
	.loc 1 2411 9
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #24]
	.loc 1 2411 5
	cmp	r3, #0
	beq	.L313
	.loc 1 2421 9
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #44]
	.loc 1 2421 6
	cmp	r3, #-1
	beq	.L295
	.loc 1 2422 10
	movs	r3, #0
	str	r3, [sp, #24]
	.loc 1 2422 6
	ldr	r3, [sp, #24]
	str	r3, [sp, #28]
	.loc 1 2423 10
	b	.L296
.L301:
	.loc 1 2425 9
	ldrh	r3, [sp, #14]
	movs	r1, #0
	mov	r0, r3
	bl	ff_convert
	mov	r3, r0
	strh	r3, [sp, #14]	@ movhi
	.loc 1 2426 8
	ldrh	r3, [sp, #14]
	cmp	r3, #0
	bne	.L297
	.loc 1 2426 21 discriminator 1
	movs	r3, #0
	str	r3, [sp, #28]
	.loc 1 2426 5 discriminator 1
	b	.L298
.L297:
	.loc 1 2427 8
	ldrh	r3, [sp, #14]
	cmp	r3, #255
	bls	.L299
	.loc 1 2428 24
	ldrh	r3, [sp, #14]
	lsrs	r3, r3, #8
	uxth	r1, r3
	.loc 1 2428 18
	ldr	r3, [sp, #28]
	adds	r2, r3, #1
	str	r2, [sp, #28]
	.loc 1 2428 24
	uxtb	r1, r1
	.loc 1 2428 22
	ldr	r2, [sp]
	add	r3, r3, r2
	mov	r2, r1
	strb	r2, [r3, #22]
.L299:
	.loc 1 2431 8
	ldr	r3, [sp, #28]
	cmp	r3, #254
	bls	.L300
	.loc 1 2431 28 discriminator 1
	movs	r3, #0
	str	r3, [sp, #28]
	.loc 1 2431 5 discriminator 1
	b	.L298
.L300:
	.loc 1 2432 17
	ldr	r3, [sp, #28]
	adds	r2, r3, #1
	str	r2, [sp, #28]
	.loc 1 2432 23
	ldrh	r2, [sp, #14]	@ movhi
	uxtb	r1, r2
	.loc 1 2432 21
	ldr	r2, [sp]
	add	r3, r3, r2
	mov	r2, r1
	strb	r2, [r3, #22]
.L296:
	.loc 1 2423 18
	ldr	r3, [sp, #16]
	ldr	r2, [r3, #12]
	.loc 1 2423 28
	ldr	r3, [sp, #24]
	adds	r1, r3, #1
	str	r1, [sp, #24]
	.loc 1 2423 26
	lsls	r3, r3, #1
	add	r3, r3, r2
	.loc 1 2423 14
	ldrh	r3, [r3]	@ movhi
	strh	r3, [sp, #14]	@ movhi
	.loc 1 2423 10
	ldrh	r3, [sp, #14]
	cmp	r3, #0
	bne	.L301
.L298:
	.loc 1 2434 18
	ldr	r2, [sp]
	ldr	r3, [sp, #28]
	add	r3, r3, r2
	adds	r3, r3, #22
	movs	r2, #0
	strb	r2, [r3]
.L295:
	.loc 1 2438 8
	movs	r3, #0
	str	r3, [sp, #24]
	.loc 1 2438 4
	ldr	r3, [sp, #24]
	str	r3, [sp, #28]
	.loc 1 2439 18
	ldr	r2, [sp]
	ldr	r3, [sp, #28]
	add	r3, r3, r2
	adds	r3, r3, #22
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 2439 6
	strh	r3, [sp, #12]	@ movhi
	.loc 1 2440 8
	b	.L302
.L311:
	.loc 1 2441 16
	ldr	r3, [sp, #4]
	ldr	r2, [r3, #28]
	.loc 1 2441 23
	ldr	r3, [sp, #28]
	adds	r1, r3, #1
	str	r1, [sp, #28]
	.loc 1 2441 21
	add	r3, r3, r2
	.loc 1 2441 5
	ldrb	r3, [r3]
	strb	r3, [sp, #23]
	.loc 1 2442 6
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #32
	bne	.L303
	.loc 1 2442 3 discriminator 1
	b	.L302
.L303:
	.loc 1 2443 6
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #5
	bne	.L304
	.loc 1 2443 21 discriminator 1
	movs	r3, #229
	strb	r3, [sp, #23]
.L304:
	.loc 1 2444 6
	ldr	r3, [sp, #28]
	cmp	r3, #9
	bne	.L305
	.loc 1 2445 7
	ldrh	r3, [sp, #12]
	cmp	r3, #0
	bne	.L306
	.loc 1 2445 28 discriminator 1
	ldr	r2, [sp]
	ldr	r3, [sp, #24]
	add	r3, r3, r2
	adds	r3, r3, #22
	movs	r2, #46
	strb	r2, [r3]
.L306:
	.loc 1 2446 18
	ldr	r3, [sp, #24]
	adds	r2, r3, #1
	str	r2, [sp, #24]
	.loc 1 2446 22
	ldr	r2, [sp]
	add	r3, r3, r2
	movs	r2, #46
	strb	r2, [r3, #9]
.L305:
	.loc 1 2455 19
	ldr	r2, [sp]
	ldr	r3, [sp, #24]
	add	r3, r3, r2
	adds	r3, r3, #9
	ldrb	r2, [sp, #23]
	strb	r2, [r3]
	.loc 1 2456 6
	ldrh	r3, [sp, #12]
	cmp	r3, #0
	bne	.L307
	.loc 1 2457 7
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #64
	bls	.L308
	.loc 1 2457 8 discriminator 1
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #90
	bhi	.L308
	.loc 1 2457 25 discriminator 2
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #28]
	.loc 1 2457 30 discriminator 2
	adds	r3, r3, #12
	ldrb	r3, [r3]	@ zero_extendqisi2
	mov	r2, r3
	.loc 1 2457 61 discriminator 2
	ldr	r3, [sp, #28]
	cmp	r3, #8
	bls	.L309
	.loc 1 2457 61 is_stmt 0 discriminator 3
	movs	r3, #16
	b	.L310
.L309:
	.loc 1 2457 61 discriminator 4
	movs	r3, #8
.L310:
	.loc 1 2457 42 is_stmt 1 discriminator 6
	ands	r3, r3, r2
	.loc 1 2457 19 discriminator 6
	cmp	r3, #0
	beq	.L308
	.loc 1 2458 7
	ldrb	r3, [sp, #23]
	adds	r3, r3, #32
	strb	r3, [sp, #23]
.L308:
	.loc 1 2460 18
	ldr	r2, [sp]
	ldr	r3, [sp, #24]
	add	r3, r3, r2
	adds	r3, r3, #22
	ldrb	r2, [sp, #23]
	strb	r2, [r3]
.L307:
	.loc 1 2462 4
	ldr	r3, [sp, #24]
	adds	r3, r3, #1
	str	r3, [sp, #24]
.L302:
	.loc 1 2440 8
	ldr	r3, [sp, #28]
	cmp	r3, #10
	bls	.L311
	.loc 1 2464 5
	ldrh	r3, [sp, #12]
	cmp	r3, #0
	bne	.L312
	.loc 1 2465 17
	ldr	r2, [sp]
	ldr	r3, [sp, #24]
	add	r3, r3, r2
	adds	r3, r3, #22
	movs	r2, #0
	strb	r2, [r3]
	.loc 1 2466 10
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #28]
	.loc 1 2466 15
	adds	r3, r3, #12
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 2466 6
	cmp	r3, #0
	bne	.L312
	.loc 1 2466 30 discriminator 1
	movs	r3, #0
	str	r3, [sp, #24]
.L312:
	.loc 1 2468 18
	ldr	r2, [sp]
	ldr	r3, [sp, #24]
	add	r3, r3, r2
	adds	r3, r3, #9
	movs	r2, #0
	strb	r2, [r3]
	.loc 1 2482 19
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #28]
	.loc 1 2482 15
	ldrb	r2, [r3, #11]	@ zero_extendqisi2
	ldr	r3, [sp]
	strb	r2, [r3, #8]
	.loc 1 2483 26
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #28]
	.loc 1 2483 32
	adds	r3, r3, #28
	.loc 1 2483 15
	mov	r0, r3
	bl	ld_dword
	mov	r2, r0
	.loc 1 2483 13
	ldr	r3, [sp]
	str	r2, [r3]
	.loc 1 2484 18
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #28]
	.loc 1 2484 24
	adds	r3, r3, #22
	.loc 1 2484 7
	mov	r0, r3
	bl	ld_dword
	str	r0, [sp, #8]
	.loc 1 2485 15
	ldr	r3, [sp, #8]
	uxth	r2, r3
	.loc 1 2485 13
	ldr	r3, [sp]
	strh	r2, [r3, #6]	@ movhi
	.loc 1 2485 48
	ldr	r3, [sp, #8]
	lsrs	r3, r3, #16
	.loc 1 2485 38
	uxth	r2, r3
	.loc 1 2485 36
	ldr	r3, [sp]
	strh	r2, [r3, #4]	@ movhi
	b	.L292
.L313:
	.loc 1 2411 17
	nop
.L292:
	.loc 1 2486 1
	add	sp, sp, #36
.LCFI82:
	@ sp needed
	ldr	pc, [sp], #4
.LFE30:
	.size	get_fileinfo, .-get_fileinfo
	.section .rodata
	.align	2
.LC0:
	.ascii	"\"*:<>?|\177\000"
	.align	2
.LC1:
	.ascii	"+,;=[]\000"
	.section	.text.create_name,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	create_name, %function
create_name:
.LFB31:
	.loc 1 2574 1
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI83:
	sub	sp, sp, #44
.LCFI84:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 2582 4
	ldr	r3, [sp]
	ldr	r3, [r3]
	str	r3, [sp, #16]
	.loc 1 2582 26
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	.loc 1 2582 17
	ldr	r3, [r3, #12]
	str	r3, [sp, #12]
	.loc 1 2582 47
	movs	r3, #0
	str	r3, [sp, #20]
	.loc 1 2582 42
	ldr	r3, [sp, #20]
	str	r3, [sp, #24]
.L330:
	.loc 1 2584 11
	ldr	r3, [sp, #24]
	adds	r2, r3, #1
	str	r2, [sp, #24]
	.loc 1 2584 8
	ldr	r2, [sp, #16]
	add	r3, r3, r2
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 2584 5
	strh	r3, [sp, #36]	@ movhi
	.loc 1 2585 6
	ldrh	r3, [sp, #36]
	cmp	r3, #31
	bls	.L375
	.loc 1 2586 6
	ldrh	r3, [sp, #36]
	cmp	r3, #47
	beq	.L319
	.loc 1 2586 16 discriminator 1
	ldrh	r3, [sp, #36]
	cmp	r3, #92
	bne	.L318
	.loc 1 2587 10
	b	.L319
.L320:
	.loc 1 2587 44 discriminator 4
	ldr	r3, [sp, #24]
	adds	r3, r3, #1
	str	r3, [sp, #24]
.L319:
	.loc 1 2587 12 discriminator 1
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #24]
	add	r3, r3, r2
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 2587 10 discriminator 1
	cmp	r3, #47
	beq	.L320
	.loc 1 2587 28 discriminator 3
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #24]
	add	r3, r3, r2
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 2587 24 discriminator 3
	cmp	r3, #92
	beq	.L320
	.loc 1 2588 4
	b	.L316
.L318:
	.loc 1 2590 6
	ldr	r3, [sp, #20]
	cmp	r3, #254
	bls	.L321
	.loc 1 2590 30 discriminator 1
	movs	r3, #6
	b	.L322
.L321:
	.loc 1 2592 5
	ldrh	r3, [sp, #36]	@ movhi
	uxtb	r3, r3
	strh	r3, [sp, #36]	@ movhi
	.loc 1 2593 7
	ldrh	r3, [sp, #36]	@ movhi
	uxtb	r3, r3
	.loc 1 2593 6
	cmp	r3, #128
	bls	.L323
	.loc 1 2593 7 discriminator 1
	ldrh	r3, [sp, #36]	@ movhi
	uxtb	r3, r3
	cmp	r3, #159
	bls	.L324
.L323:
	.loc 1 2593 7 is_stmt 0 discriminator 3
	ldrh	r3, [sp, #36]	@ movhi
	uxtb	r3, r3
	cmp	r3, #223
	bls	.L325
	.loc 1 2593 7 discriminator 4
	ldrh	r3, [sp, #36]	@ movhi
	uxtb	r3, r3
	cmp	r3, #252
	bhi	.L325
.L324:
	.loc 1 2594 18 is_stmt 1
	ldr	r3, [sp, #24]
	adds	r2, r3, #1
	str	r2, [sp, #24]
	.loc 1 2594 15
	ldr	r2, [sp, #16]
	add	r3, r3, r2
	.loc 1 2594 6
	ldrb	r3, [r3]
	strb	r3, [sp, #39]
	.loc 1 2595 11
	ldrh	r3, [sp, #36]	@ movhi
	lsls	r3, r3, #8
	uxth	r2, r3
	.loc 1 2595 17
	ldrb	r3, [sp, #39]	@ zero_extendqisi2
	uxth	r3, r3
	.loc 1 2595 6
	add	r3, r3, r2
	strh	r3, [sp, #36]	@ movhi
	.loc 1 2596 7
	ldrb	r3, [sp, #39]	@ zero_extendqisi2
	cmp	r3, #63
	bls	.L326
	.loc 1 2596 9 discriminator 2
	ldrb	r3, [sp, #39]	@ zero_extendqisi2
	cmp	r3, #126
	bls	.L325
.L326:
	.loc 1 2596 9 is_stmt 0 discriminator 3
	ldrsb	r3, [sp, #39]
	.loc 1 2596 8 is_stmt 1 discriminator 3
	cmp	r3, #0
	bge	.L327
	.loc 1 2596 9 discriminator 5
	ldrb	r3, [sp, #39]	@ zero_extendqisi2
	cmp	r3, #252
	bls	.L325
.L327:
	.loc 1 2596 28 discriminator 6
	movs	r3, #6
	b	.L322
.L325:
	.loc 1 2598 7
	ldrh	r3, [sp, #36]
	movs	r1, #1
	mov	r0, r3
	bl	ff_convert
	mov	r3, r0
	strh	r3, [sp, #36]	@ movhi
	.loc 1 2599 6
	ldrh	r3, [sp, #36]
	cmp	r3, #0
	bne	.L328
	.loc 1 2599 18 discriminator 1
	movs	r3, #6
	b	.L322
.L328:
	.loc 1 2601 6
	ldrh	r3, [sp, #36]
	cmp	r3, #127
	bhi	.L329
	.loc 1 2601 19 discriminator 1
	ldrh	r3, [sp, #36]
	mov	r1, r3
	ldr	r0, .L379
	bl	chk_chr
	mov	r3, r0
	.loc 1 2601 16 discriminator 1
	cmp	r3, #0
	beq	.L329
	.loc 1 2601 55 discriminator 2
	movs	r3, #6
	b	.L322
.L329:
	.loc 1 2602 9
	ldr	r3, [sp, #20]
	adds	r2, r3, #1
	str	r2, [sp, #20]
	.loc 1 2602 6
	lsls	r3, r3, #1
	ldr	r2, [sp, #12]
	add	r3, r3, r2
	.loc 1 2602 13
	ldrh	r2, [sp, #36]	@ movhi
	strh	r2, [r3]	@ movhi
	.loc 1 2584 5
	b	.L330
.L375:
	.loc 1 2585 3
	nop
.L316:
	.loc 1 2604 10
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #24]
	add	r2, r2, r3
	.loc 1 2604 8
	ldr	r3, [sp]
	str	r2, [r3]
	.loc 1 2605 5
	ldrh	r3, [sp, #36]
	cmp	r3, #31
	bhi	.L331
	.loc 1 2605 5 is_stmt 0 discriminator 1
	movs	r3, #4
	b	.L332
.L331:
	.loc 1 2605 5 discriminator 2
	movs	r3, #0
.L332:
	.loc 1 2605 5 discriminator 4
	strb	r3, [sp, #38]
	.loc 1 2607 5 is_stmt 1 discriminator 4
	ldr	r3, [sp, #20]
	cmp	r3, #1
	bne	.L333
	.loc 1 2607 21 discriminator 1
	ldr	r3, [sp, #20]
	add	r3, r3, #-2147483648
	subs	r3, r3, #1
	lsls	r3, r3, #1
	ldr	r2, [sp, #12]
	add	r3, r3, r2
	ldrh	r3, [r3]
	.loc 1 2607 15 discriminator 1
	cmp	r3, #46
	beq	.L334
.L333:
	.loc 1 2607 38 discriminator 3
	ldr	r3, [sp, #20]
	cmp	r3, #2
	bne	.L340
	.loc 1 2608 18
	ldr	r3, [sp, #20]
	add	r3, r3, #-2147483648
	subs	r3, r3, #1
	lsls	r3, r3, #1
	ldr	r2, [sp, #12]
	add	r3, r3, r2
	ldrh	r3, [r3]
	.loc 1 2608 12
	cmp	r3, #46
	bne	.L340
	.loc 1 2608 40 discriminator 1
	ldr	r3, [sp, #20]
	add	r3, r3, #-2147483648
	subs	r3, r3, #2
	lsls	r3, r3, #1
	ldr	r2, [sp, #12]
	add	r3, r3, r2
	ldrh	r3, [r3]
	.loc 1 2608 34 discriminator 1
	cmp	r3, #46
	bne	.L340
.L334:
	.loc 1 2609 6
	ldr	r3, [sp, #20]
	lsls	r3, r3, #1
	ldr	r2, [sp, #12]
	add	r3, r3, r2
	.loc 1 2609 11
	movs	r2, #0
	strh	r2, [r3]	@ movhi
	.loc 1 2610 10
	movs	r3, #0
	str	r3, [sp, #32]
	.loc 1 2610 3
	b	.L336
.L339:
	.loc 1 2611 14
	ldr	r2, [sp, #32]
	ldr	r3, [sp, #20]
	cmp	r2, r3
	bcs	.L337
	.loc 1 2611 14 is_stmt 0 discriminator 1
	movs	r1, #46
	b	.L338
.L337:
	.loc 1 2611 14 discriminator 2
	movs	r1, #32
.L338:
	.loc 1 2611 14 discriminator 4
	ldr	r2, [sp, #4]
	ldr	r3, [sp, #32]
	add	r3, r3, r2
	adds	r3, r3, #32
	mov	r2, r1
	strb	r2, [r3]
	.loc 1 2610 24 is_stmt 1 discriminator 4
	ldr	r3, [sp, #32]
	adds	r3, r3, #1
	str	r3, [sp, #32]
.L336:
	.loc 1 2610 3 discriminator 2
	ldr	r3, [sp, #32]
	cmp	r3, #10
	bls	.L339
	.loc 1 2612 13
	ldrb	r3, [sp, #38]
	orr	r3, r3, #32
	uxtb	r1, r3
	ldr	r2, [sp, #4]
	ldr	r3, [sp, #32]
	add	r3, r3, r2
	adds	r3, r3, #32
	mov	r2, r1
	strb	r2, [r3]
	.loc 1 2613 10
	movs	r3, #0
	b	.L322
.L343:
	.loc 1 2617 10
	ldr	r3, [sp, #20]
	add	r3, r3, #-2147483648
	subs	r3, r3, #1
	lsls	r3, r3, #1
	ldr	r2, [sp, #12]
	add	r3, r3, r2
	.loc 1 2617 5
	ldrh	r3, [r3]	@ movhi
	strh	r3, [sp, #36]	@ movhi
	.loc 1 2618 6
	ldrh	r3, [sp, #36]
	cmp	r3, #32
	beq	.L341
	.loc 1 2618 16 discriminator 1
	ldrh	r3, [sp, #36]
	cmp	r3, #46
	bne	.L376
.L341:
	.loc 1 2619 5
	ldr	r3, [sp, #20]
	subs	r3, r3, #1
	str	r3, [sp, #20]
.L340:
	.loc 1 2616 8
	ldr	r3, [sp, #20]
	cmp	r3, #0
	bne	.L343
	b	.L342
.L376:
	.loc 1 2618 3
	nop
.L342:
	.loc 1 2621 5
	ldr	r3, [sp, #20]
	lsls	r3, r3, #1
	ldr	r2, [sp, #12]
	add	r3, r3, r2
	.loc 1 2621 10
	movs	r2, #0
	strh	r2, [r3]	@ movhi
	.loc 1 2622 5
	ldr	r3, [sp, #20]
	cmp	r3, #0
	bne	.L344
	.loc 1 2622 22 discriminator 1
	movs	r3, #6
	b	.L322
.L344:
	.loc 1 2625 12
	ldr	r3, [sp, #4]
	adds	r3, r3, #32
	.loc 1 2625 2
	movs	r2, #11
	movs	r1, #32
	mov	r0, r3
	bl	mem_set
	.loc 1 2626 10
	movs	r3, #0
	str	r3, [sp, #24]
	.loc 1 2626 2
	b	.L345
.L346:
	.loc 1 2626 51 discriminator 5
	ldr	r3, [sp, #24]
	adds	r3, r3, #1
	str	r3, [sp, #24]
.L345:
	.loc 1 2626 18 discriminator 1
	ldr	r3, [sp, #24]
	lsls	r3, r3, #1
	ldr	r2, [sp, #12]
	add	r3, r3, r2
	ldrh	r3, [r3]
	.loc 1 2626 2 discriminator 1
	cmp	r3, #32
	beq	.L346
	.loc 1 2626 36 discriminator 4
	ldr	r3, [sp, #24]
	lsls	r3, r3, #1
	ldr	r2, [sp, #12]
	add	r3, r3, r2
	ldrh	r3, [r3]
	.loc 1 2626 30 discriminator 4
	cmp	r3, #46
	beq	.L346
	.loc 1 2627 5
	ldr	r3, [sp, #24]
	cmp	r3, #0
	beq	.L348
	.loc 1 2627 13 discriminator 1
	ldrb	r3, [sp, #38]
	orr	r3, r3, #3
	strb	r3, [sp, #38]
	.loc 1 2628 8 discriminator 1
	b	.L348
.L350:
	.loc 1 2628 37 discriminator 3
	ldr	r3, [sp, #20]
	subs	r3, r3, #1
	str	r3, [sp, #20]
.L348:
	.loc 1 2628 8 discriminator 1
	ldr	r3, [sp, #20]
	cmp	r3, #0
	beq	.L349
	.loc 1 2628 18 discriminator 2
	ldr	r3, [sp, #20]
	add	r3, r3, #-2147483648
	subs	r3, r3, #1
	lsls	r3, r3, #1
	ldr	r2, [sp, #12]
	add	r3, r3, r2
	ldrh	r3, [r3]
	.loc 1 2628 12 discriminator 2
	cmp	r3, #46
	bne	.L350
.L349:
	.loc 1 2630 8
	movs	r3, #0
	strb	r3, [sp, #39]
	.loc 1 2630 4
	movs	r3, #0
	str	r3, [sp, #32]
	.loc 1 2630 16
	movs	r3, #8
	str	r3, [sp, #28]
.L368:
	.loc 1 2632 13
	ldr	r3, [sp, #24]
	adds	r2, r3, #1
	str	r2, [sp, #24]
	.loc 1 2632 10
	lsls	r3, r3, #1
	ldr	r2, [sp, #12]
	add	r3, r3, r2
	.loc 1 2632 5
	ldrh	r3, [r3]	@ movhi
	strh	r3, [sp, #36]	@ movhi
	.loc 1 2633 6
	ldrh	r3, [sp, #36]
	cmp	r3, #0
	beq	.L377
	.loc 1 2634 6
	ldrh	r3, [sp, #36]
	cmp	r3, #32
	beq	.L353
	.loc 1 2634 16 discriminator 1
	ldrh	r3, [sp, #36]
	cmp	r3, #46
	bne	.L354
	.loc 1 2634 29 discriminator 2
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #20]
	cmp	r2, r3
	beq	.L354
.L353:
	.loc 1 2635 7
	ldrb	r3, [sp, #38]
	orr	r3, r3, #3
	strb	r3, [sp, #38]
	.loc 1 2635 4
	b	.L355
.L354:
	.loc 1 2638 6
	ldr	r2, [sp, #32]
	ldr	r3, [sp, #28]
	cmp	r2, r3
	bcs	.L356
	.loc 1 2638 15 discriminator 1
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #20]
	cmp	r2, r3
	bne	.L357
.L356:
	.loc 1 2639 7
	ldr	r3, [sp, #28]
	cmp	r3, #11
	bne	.L358
	.loc 1 2640 8
	ldrb	r3, [sp, #38]
	orr	r3, r3, #3
	strb	r3, [sp, #38]
	.loc 1 2640 5
	b	.L352
.L358:
	.loc 1 2642 7
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #20]
	cmp	r2, r3
	beq	.L359
	.loc 1 2642 21 discriminator 1
	ldrb	r3, [sp, #38]
	orr	r3, r3, #3
	strb	r3, [sp, #38]
.L359:
	.loc 1 2643 7
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #20]
	cmp	r2, r3
	bhi	.L378
	.loc 1 2644 7
	ldr	r3, [sp, #20]
	str	r3, [sp, #24]
	.loc 1 2644 15
	movs	r3, #8
	str	r3, [sp, #32]
	.loc 1 2644 23
	movs	r3, #11
	str	r3, [sp, #28]
	.loc 1 2645 6
	ldrb	r3, [sp, #39]
	lsls	r3, r3, #2
	strb	r3, [sp, #39]
	.loc 1 2645 4
	b	.L355
.L357:
	.loc 1 2648 6
	ldrh	r3, [sp, #36]
	cmp	r3, #127
	bls	.L361
	.loc 1 2653 8
	ldrh	r3, [sp, #36]
	mov	r0, r3
	bl	ff_wtoupper
	mov	r3, r0
	movs	r1, #0
	mov	r0, r3
	bl	ff_convert
	mov	r3, r0
	strh	r3, [sp, #36]	@ movhi
	.loc 1 2655 7
	ldrb	r3, [sp, #38]
	orr	r3, r3, #2
	strb	r3, [sp, #38]
.L361:
	.loc 1 2658 6
	ldrh	r3, [sp, #36]
	cmp	r3, #255
	bls	.L362
	.loc 1 2659 16
	ldr	r3, [sp, #28]
	subs	r3, r3, #1
	.loc 1 2659 7
	ldr	r2, [sp, #32]
	cmp	r2, r3
	bcc	.L363
	.loc 1 2660 8
	ldrb	r3, [sp, #38]
	orr	r3, r3, #3
	strb	r3, [sp, #38]
	.loc 1 2660 31
	ldr	r3, [sp, #28]
	str	r3, [sp, #32]
	.loc 1 2660 5
	b	.L355
.L363:
	.loc 1 2662 18
	ldrh	r3, [sp, #36]
	lsrs	r3, r3, #8
	uxth	r1, r3
	.loc 1 2662 12
	ldr	r3, [sp, #32]
	adds	r2, r3, #1
	str	r2, [sp, #32]
	.loc 1 2662 18
	uxtb	r1, r1
	.loc 1 2662 16
	ldr	r2, [sp, #4]
	add	r3, r3, r2
	mov	r2, r1
	strb	r2, [r3, #32]
	b	.L364
.L380:
	.align	2
.L379:
	.word	.LC0
.L362:
	.loc 1 2664 7
	ldrh	r3, [sp, #36]
	cmp	r3, #0
	beq	.L365
	.loc 1 2664 14 discriminator 1
	ldrh	r3, [sp, #36]
	mov	r1, r3
	ldr	r0, .L381
	bl	chk_chr
	mov	r3, r0
	.loc 1 2664 11 discriminator 1
	cmp	r3, #0
	beq	.L366
.L365:
	.loc 1 2665 7
	movs	r3, #95
	strh	r3, [sp, #36]	@ movhi
	.loc 1 2665 17
	ldrb	r3, [sp, #38]
	orr	r3, r3, #3
	strb	r3, [sp, #38]
	b	.L364
.L366:
	.loc 1 2667 8
	ldrh	r3, [sp, #36]
	cmp	r3, #64
	bls	.L367
	.loc 1 2667 9 discriminator 1
	ldrh	r3, [sp, #36]
	cmp	r3, #90
	bhi	.L367
	.loc 1 2668 8
	ldrb	r3, [sp, #39]
	orr	r3, r3, #2
	strb	r3, [sp, #39]
	b	.L364
.L367:
	.loc 1 2670 9
	ldrh	r3, [sp, #36]
	cmp	r3, #96
	bls	.L364
	.loc 1 2670 10 discriminator 1
	ldrh	r3, [sp, #36]
	cmp	r3, #122
	bhi	.L364
	.loc 1 2671 9
	ldrb	r3, [sp, #39]
	orr	r3, r3, #1
	strb	r3, [sp, #39]
	.loc 1 2671 17
	ldrh	r3, [sp, #36]	@ movhi
	subs	r3, r3, #32
	strh	r3, [sp, #36]	@ movhi
.L364:
	.loc 1 2676 11
	ldr	r3, [sp, #32]
	adds	r2, r3, #1
	str	r2, [sp, #32]
	.loc 1 2676 17
	ldrh	r2, [sp, #36]	@ movhi
	uxtb	r1, r2
	.loc 1 2676 15
	ldr	r2, [sp, #4]
	add	r3, r3, r2
	mov	r2, r1
	strb	r2, [r3, #32]
.L355:
	.loc 1 2632 5
	b	.L368
.L377:
	.loc 1 2633 3
	nop
	b	.L352
.L378:
	.loc 1 2643 4
	nop
.L352:
	.loc 1 2679 12
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #32]	@ zero_extendqisi2
	.loc 1 2679 5
	cmp	r3, #229
	bne	.L369
	.loc 1 2679 35 discriminator 1
	ldr	r3, [sp, #4]
	movs	r2, #5
	strb	r2, [r3, #32]
.L369:
	.loc 1 2681 5
	ldr	r3, [sp, #28]
	cmp	r3, #8
	bne	.L370
	.loc 1 2681 17 discriminator 1
	ldrb	r3, [sp, #39]
	lsls	r3, r3, #2
	strb	r3, [sp, #39]
.L370:
	.loc 1 2682 9
	ldrb	r3, [sp, #39]	@ zero_extendqisi2
	and	r3, r3, #12
	.loc 1 2682 5
	cmp	r3, #12
	beq	.L371
	.loc 1 2682 31 discriminator 2
	ldrb	r3, [sp, #39]	@ zero_extendqisi2
	and	r3, r3, #3
	.loc 1 2682 25 discriminator 2
	cmp	r3, #3
	bne	.L372
.L371:
	.loc 1 2682 51 discriminator 3
	ldrb	r3, [sp, #38]
	orr	r3, r3, #2
	strb	r3, [sp, #38]
.L372:
	.loc 1 2683 11
	ldrb	r3, [sp, #38]	@ zero_extendqisi2
	and	r3, r3, #2
	.loc 1 2683 5
	cmp	r3, #0
	bne	.L373
	.loc 1 2684 10
	ldrb	r3, [sp, #39]	@ zero_extendqisi2
	and	r3, r3, #3
	.loc 1 2684 6
	cmp	r3, #1
	bne	.L374
	.loc 1 2684 30 discriminator 1
	ldrb	r3, [sp, #38]
	orr	r3, r3, #16
	strb	r3, [sp, #38]
.L374:
	.loc 1 2685 10
	ldrb	r3, [sp, #39]	@ zero_extendqisi2
	and	r3, r3, #12
	.loc 1 2685 6
	cmp	r3, #4
	bne	.L373
	.loc 1 2685 30 discriminator 1
	ldrb	r3, [sp, #38]
	orr	r3, r3, #8
	strb	r3, [sp, #38]
.L373:
	.loc 1 2688 17
	ldr	r3, [sp, #4]
	ldrb	r2, [sp, #38]
	strb	r2, [r3, #43]
	.loc 1 2690 9
	movs	r3, #0
.L322:
	.loc 1 2755 1
	mov	r0, r3
	add	sp, sp, #44
.LCFI85:
	@ sp needed
	ldr	pc, [sp], #4
.L382:
	.align	2
.L381:
	.word	.LC1
.LFE31:
	.size	create_name, .-create_name
	.section	.text.follow_path,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	follow_path, %function
follow_path:
.LFB32:
	.loc 1 2769 1
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI86:
	sub	sp, sp, #28
.LCFI87:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 2772 9
	ldr	r3, [sp, #4]
	str	r3, [sp, #16]
	.loc 1 2773 9
	ldr	r3, [sp, #16]
	ldr	r3, [r3]
	str	r3, [sp, #12]
	.loc 1 2777 6
	ldr	r3, [sp]
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 2777 5
	cmp	r3, #47
	beq	.L386
	.loc 1 2777 22 discriminator 1
	ldr	r3, [sp]
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 2777 19 discriminator 1
	cmp	r3, #92
	beq	.L386
	.loc 1 2778 19
	ldr	r3, [sp, #12]
	ldr	r2, [r3, #24]
	.loc 1 2778 15
	ldr	r3, [sp, #16]
	str	r2, [r3, #8]
	b	.L385
.L387:
	.loc 1 2782 45 discriminator 4
	ldr	r3, [sp]
	adds	r3, r3, #1
	str	r3, [sp]
.L386:
	.loc 1 2782 10 discriminator 1
	ldr	r3, [sp]
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 2782 9 discriminator 1
	cmp	r3, #47
	beq	.L387
	.loc 1 2782 26 discriminator 3
	ldr	r3, [sp]
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 2782 23 discriminator 3
	cmp	r3, #92
	beq	.L387
	.loc 1 2783 15
	ldr	r3, [sp, #16]
	movs	r2, #0
	str	r2, [r3, #8]
.L385:
	.loc 1 2799 12
	ldr	r3, [sp]
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 2799 5
	cmp	r3, #31
	bhi	.L388
	.loc 1 2800 18
	ldr	r3, [sp, #4]
	movs	r2, #128
	strb	r2, [r3, #43]
	.loc 1 2801 9
	movs	r1, #0
	ldr	r0, [sp, #4]
	bl	dir_sdi
	mov	r3, r0
	strb	r3, [sp, #23]
	b	.L389
.L388:
	.loc 1 2805 10
	mov	r3, sp
	mov	r1, r3
	ldr	r0, [sp, #4]
	bl	create_name
	mov	r3, r0
	strb	r3, [sp, #23]
	.loc 1 2806 7
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L399
	.loc 1 2807 10
	ldr	r0, [sp, #4]
	bl	dir_find
	mov	r3, r0
	strb	r3, [sp, #23]
	.loc 1 2808 7
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #43]
	strb	r3, [sp, #11]
	.loc 1 2809 7
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L391
	.loc 1 2810 8
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #4
	bne	.L400
	.loc 1 2811 27
	ldrb	r3, [sp, #11]	@ zero_extendqisi2
	and	r3, r3, #32
	.loc 1 2811 9
	cmp	r3, #0
	beq	.L393
	.loc 1 2812 16
	ldrb	r3, [sp, #11]	@ zero_extendqisi2
	and	r3, r3, #4
	.loc 1 2812 10
	cmp	r3, #0
	beq	.L401
	.loc 1 2813 22
	ldr	r3, [sp, #4]
	movs	r2, #128
	strb	r2, [r3, #43]
	.loc 1 2814 11
	movs	r3, #0
	strb	r3, [sp, #23]
	.loc 1 2819 5
	b	.L400
.L393:
	.loc 1 2816 16
	ldrb	r3, [sp, #11]	@ zero_extendqisi2
	and	r3, r3, #4
	.loc 1 2816 10
	cmp	r3, #0
	bne	.L400
	.loc 1 2816 32 discriminator 1
	movs	r3, #5
	strb	r3, [sp, #23]
	.loc 1 2819 5 discriminator 1
	b	.L400
.L391:
	.loc 1 2821 11
	ldrb	r3, [sp, #11]	@ zero_extendqisi2
	and	r3, r3, #4
	.loc 1 2821 7
	cmp	r3, #0
	bne	.L402
	.loc 1 2823 13
	ldr	r3, [sp, #16]
	ldrb	r3, [r3, #6]	@ zero_extendqisi2
	.loc 1 2823 20
	and	r3, r3, #16
	.loc 1 2823 7
	cmp	r3, #0
	bne	.L397
	.loc 1 2824 9
	movs	r3, #5
	strb	r3, [sp, #23]
	.loc 1 2824 5
	b	.L389
.L397:
	.loc 1 2837 32
	ldr	r3, [sp, #12]
	add	r2, r3, #56
	.loc 1 2837 44
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #16]
	.loc 1 2837 51
	ubfx	r3, r3, #0, #9
	.loc 1 2837 19
	add	r3, r3, r2
	mov	r1, r3
	ldr	r0, [sp, #12]
	bl	ld_clust
	mov	r2, r0
	.loc 1 2837 17
	ldr	r3, [sp, #16]
	str	r2, [r3, #8]
	b	.L388
.L401:
	.loc 1 2812 7
	nop
	.loc 1 2805 8
	b	.L388
.L399:
	.loc 1 2806 4
	nop
	b	.L389
.L400:
	.loc 1 2819 5
	nop
	b	.L389
.L402:
	.loc 1 2821 4
	nop
.L389:
	.loc 1 2842 9
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	.loc 1 2843 1
	mov	r0, r3
	add	sp, sp, #28
.LCFI88:
	@ sp needed
	ldr	pc, [sp], #4
.LFE32:
	.size	follow_path, .-follow_path
	.section	.text.get_ldnumber,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	get_ldnumber, %function
get_ldnumber:
.LFB33:
	.loc 1 2856 1
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #24
.LCFI89:
	str	r0, [sp, #4]
	.loc 1 2859 6
	mov	r3, #-1
	str	r3, [sp, #16]
	.loc 1 2868 6
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	.loc 1 2868 5
	cmp	r3, #0
	beq	.L404
	.loc 1 2869 11
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	str	r3, [sp, #20]
	.loc 1 2869 3
	b	.L405
.L407:
	.loc 1 2869 73 discriminator 4
	ldr	r3, [sp, #20]
	adds	r3, r3, #1
	str	r3, [sp, #20]
.L405:
	.loc 1 2869 26 discriminator 1
	ldr	r3, [sp, #20]
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 2869 3 discriminator 1
	cmp	r3, #31
	bls	.L406
	.loc 1 2869 59 discriminator 3
	ldr	r3, [sp, #20]
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 2869 56 discriminator 3
	cmp	r3, #58
	bne	.L407
.L406:
	.loc 1 2870 7
	ldr	r3, [sp, #20]
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 2870 6
	cmp	r3, #58
	bne	.L408
	.loc 1 2871 7
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	str	r3, [sp, #12]
	.loc 1 2872 11
	ldr	r3, [sp, #12]
	adds	r2, r3, #1
	str	r2, [sp, #12]
	.loc 1 2872 8
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 2872 14
	subs	r3, r3, #48
	.loc 1 2872 6
	str	r3, [sp, #8]
	.loc 1 2873 7
	ldr	r3, [sp, #8]
	cmp	r3, #9
	bhi	.L409
	.loc 1 2873 15 discriminator 1
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #20]
	cmp	r2, r3
	bne	.L409
	.loc 1 2874 8
	ldr	r3, [sp, #8]
	cmp	r3, #0
	bne	.L409
	.loc 1 2875 10
	ldr	r3, [sp, #8]
	str	r3, [sp, #16]
	.loc 1 2876 12
	ldr	r3, [sp, #20]
	adds	r3, r3, #1
	str	r3, [sp, #20]
	ldr	r3, [sp, #4]
	ldr	r2, [sp, #20]
	str	r2, [r3]
.L409:
	.loc 1 2895 11
	ldr	r3, [sp, #16]
	b	.L410
.L408:
	.loc 1 2900 7
	movs	r3, #0
	str	r3, [sp, #16]
.L404:
	.loc 1 2903 9
	ldr	r3, [sp, #16]
.L410:
	.loc 1 2904 1
	mov	r0, r3
	add	sp, sp, #24
.LCFI90:
	@ sp needed
	bx	lr
.LFE33:
	.size	get_ldnumber, .-get_ldnumber
	.section	.text.check_fs,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	check_fs, %function
check_fs:
.LFB34:
	.loc 1 2918 1
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI91:
	sub	sp, sp, #12
.LCFI92:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 2919 12
	ldr	r3, [sp, #4]
	movs	r2, #0
	strb	r2, [r3, #3]
	.loc 1 2919 29
	ldr	r3, [sp, #4]
	mov	r2, #-1
	str	r2, [r3, #52]
	.loc 1 2920 6
	ldr	r1, [sp]
	ldr	r0, [sp, #4]
	bl	move_window
	mov	r3, r0
	.loc 1 2920 5
	cmp	r3, #0
	beq	.L412
	.loc 1 2920 45 discriminator 1
	movs	r3, #4
	b	.L413
.L412:
	.loc 1 2922 14
	ldr	r3, [sp, #4]
	adds	r3, r3, #56
	.loc 1 2922 6
	add	r3, r3, #510
	mov	r0, r3
	bl	ld_word
	mov	r3, r0
	mov	r2, r3
	.loc 1 2922 5
	movw	r3, #43605
	cmp	r2, r3
	beq	.L414
	.loc 1 2922 51 discriminator 1
	movs	r3, #3
	b	.L413
.L414:
	.loc 1 2924 13
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #56]	@ zero_extendqisi2
	.loc 1 2924 5
	cmp	r3, #233
	beq	.L415
	.loc 1 2924 45 discriminator 1
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #56]	@ zero_extendqisi2
	.loc 1 2924 34 discriminator 1
	cmp	r3, #235
	bne	.L416
	.loc 1 2924 76 discriminator 2
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #58]	@ zero_extendqisi2
	.loc 1 2924 66 discriminator 2
	cmp	r3, #144
	bne	.L416
.L415:
	.loc 1 2925 17
	ldr	r3, [sp, #4]
	adds	r3, r3, #56
	.loc 1 2925 8
	adds	r3, r3, #54
	mov	r0, r3
	bl	ld_dword
	mov	r3, r0
	.loc 1 2925 42
	bic	r3, r3, #-16777216
	.loc 1 2925 6
	ldr	r2, .L418
	cmp	r3, r2
	bne	.L417
	.loc 1 2925 74 discriminator 1
	movs	r3, #0
	b	.L413
.L417:
	.loc 1 2926 16
	ldr	r3, [sp, #4]
	adds	r3, r3, #56
	.loc 1 2926 7
	adds	r3, r3, #82
	mov	r0, r3
	bl	ld_dword
	mov	r3, r0
	.loc 1 2926 6
	ldr	r2, .L418+4
	cmp	r3, r2
	bne	.L416
	.loc 1 2926 65 discriminator 1
	movs	r3, #0
	b	.L413
.L416:
	.loc 1 2931 9
	movs	r3, #2
.L413:
	.loc 1 2932 1
	mov	r0, r3
	add	sp, sp, #12
.LCFI93:
	@ sp needed
	ldr	pc, [sp], #4
.L419:
	.align	2
.L418:
	.word	5521734
	.word	861159750
.LFE34:
	.size	check_fs, .-check_fs
	.section	.text.find_volume,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	find_volume, %function
find_volume:
.LFB35:
	.loc 1 2947 1
	@ args = 0, pretend = 0, frame = 88
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI94:
	sub	sp, sp, #92
.LCFI95:
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	mov	r3, r2
	strb	r3, [sp, #7]
	.loc 1 2958 7
	ldr	r3, [sp, #8]
	movs	r2, #0
	str	r2, [r3]
	.loc 1 2959 8
	ldr	r0, [sp, #12]
	bl	get_ldnumber
	str	r0, [sp, #60]
	.loc 1 2960 5
	ldr	r3, [sp, #60]
	cmp	r3, #0
	bge	.L421
	.loc 1 2960 22 discriminator 1
	movs	r3, #11
	b	.L460
.L421:
	.loc 1 2963 5
	ldr	r2, .L461
	ldr	r3, [sp, #60]
	ldr	r3, [r2, r3, lsl #2]
	str	r3, [sp, #56]
	.loc 1 2964 5
	ldr	r3, [sp, #56]
	cmp	r3, #0
	bne	.L423
	.loc 1 2964 18 discriminator 1
	movs	r3, #12
	b	.L460
.L423:
	.loc 1 2967 7
	ldr	r3, [sp, #8]
	ldr	r2, [sp, #56]
	str	r2, [r3]
	.loc 1 2969 7
	ldrb	r3, [sp, #7]
	bic	r3, r3, #1
	strb	r3, [sp, #7]
	.loc 1 2970 8
	ldr	r3, [sp, #56]
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 2970 5
	cmp	r3, #0
	beq	.L424
	.loc 1 2971 10
	ldr	r3, [sp, #56]
	ldrb	r3, [r3, #1]	@ zero_extendqisi2
	mov	r0, r3
	bl	disk_status
	mov	r3, r0
	strb	r3, [sp, #55]
	.loc 1 2972 14
	ldrb	r3, [sp, #55]	@ zero_extendqisi2
	and	r3, r3, #1
	.loc 1 2972 6
	cmp	r3, #0
	bne	.L424
	.loc 1 2973 7
	ldrb	r3, [sp, #7]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L425
	.loc 1 2973 39 discriminator 1
	ldrb	r3, [sp, #55]	@ zero_extendqisi2
	and	r3, r3, #4
	.loc 1 2973 30 discriminator 1
	cmp	r3, #0
	beq	.L425
	.loc 1 2974 12
	movs	r3, #10
	b	.L460
.L425:
	.loc 1 2976 11
	movs	r3, #0
	b	.L460
.L424:
	.loc 1 2983 14
	ldr	r3, [sp, #56]
	movs	r2, #0
	strb	r2, [r3]
	.loc 1 2984 12
	ldr	r3, [sp, #60]
	uxtb	r2, r3
	.loc 1 2984 10
	ldr	r3, [sp, #56]
	strb	r2, [r3, #1]
	.loc 1 2985 9
	ldr	r3, [sp, #56]
	ldrb	r3, [r3, #1]	@ zero_extendqisi2
	mov	r0, r3
	bl	disk_initialize
	mov	r3, r0
	strb	r3, [sp, #55]
	.loc 1 2986 11
	ldrb	r3, [sp, #55]	@ zero_extendqisi2
	and	r3, r3, #1
	.loc 1 2986 5
	cmp	r3, #0
	beq	.L426
	.loc 1 2987 10
	movs	r3, #3
	b	.L460
.L426:
	.loc 1 2989 5
	ldrb	r3, [sp, #7]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L427
	.loc 1 2989 37 discriminator 1
	ldrb	r3, [sp, #55]	@ zero_extendqisi2
	and	r3, r3, #4
	.loc 1 2989 28 discriminator 1
	cmp	r3, #0
	beq	.L427
	.loc 1 2990 10
	movs	r3, #10
	b	.L460
.L427:
	.loc 1 2997 8
	movs	r3, #0
	str	r3, [sp, #80]
	.loc 1 2998 8
	ldr	r1, [sp, #80]
	ldr	r0, [sp, #56]
	bl	check_fs
	mov	r3, r0
	strb	r3, [sp, #87]
	.loc 1 2999 5
	ldrb	r3, [sp, #87]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L428
	.loc 1 3000 10
	movs	r3, #0
	str	r3, [sp, #64]
	.loc 1 3000 3
	b	.L429
.L432:
	.loc 1 3001 9
	ldr	r3, [sp, #56]
	add	r2, r3, #56
	.loc 1 3001 34
	ldr	r3, [sp, #64]
	lsls	r3, r3, #4
	.loc 1 3001 30
	add	r3, r3, #446
	.loc 1 3001 7
	add	r3, r3, r2
	str	r3, [sp, #48]
	.loc 1 3002 14
	ldr	r3, [sp, #48]
	adds	r3, r3, #4
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 3002 54
	cmp	r3, #0
	beq	.L430
	.loc 1 3002 41 discriminator 1
	ldr	r3, [sp, #48]
	adds	r3, r3, #8
	.loc 1 3002 29 discriminator 1
	mov	r0, r3
	bl	ld_dword
	mov	r2, r0
	b	.L431
.L430:
	.loc 1 3002 54 discriminator 2
	movs	r2, #0
.L431:
	.loc 1 3002 10 discriminator 4
	ldr	r3, [sp, #64]
	lsls	r3, r3, #2
	add	r1, sp, #88
	add	r3, r3, r1
	str	r2, [r3, #-68]
	.loc 1 3000 23 discriminator 4
	ldr	r3, [sp, #64]
	adds	r3, r3, #1
	str	r3, [sp, #64]
.L429:
	.loc 1 3000 3 discriminator 2
	ldr	r3, [sp, #64]
	cmp	r3, #3
	bls	.L432
	.loc 1 3004 5
	movs	r3, #0
	str	r3, [sp, #64]
	.loc 1 3005 6
	ldr	r3, [sp, #64]
	cmp	r3, #0
	beq	.L436
	.loc 1 3005 11 discriminator 1
	ldr	r3, [sp, #64]
	subs	r3, r3, #1
	str	r3, [sp, #64]
.L436:
	.loc 1 3007 10
	ldr	r3, [sp, #64]
	lsls	r3, r3, #2
	add	r2, sp, #88
	add	r3, r3, r2
	ldr	r3, [r3, #-68]
	str	r3, [sp, #80]
	.loc 1 3008 8
	ldr	r3, [sp, #80]
	cmp	r3, #0
	beq	.L434
	.loc 1 3008 18 discriminator 1
	ldr	r1, [sp, #80]
	ldr	r0, [sp, #56]
	bl	check_fs
	mov	r3, r0
	b	.L435
.L434:
	.loc 1 3008 8 discriminator 2
	movs	r3, #3
.L435:
	.loc 1 3008 8 is_stmt 0 discriminator 4
	strb	r3, [sp, #87]
	.loc 1 3009 3 is_stmt 1 discriminator 4
	ldrb	r3, [sp, #87]	@ zero_extendqisi2
	cmp	r3, #1
	bls	.L428
	.loc 1 3009 36 discriminator 1
	ldr	r3, [sp, #64]
	adds	r3, r3, #1
	str	r3, [sp, #64]
	ldr	r3, [sp, #64]
	cmp	r3, #3
	bls	.L436
.L428:
	.loc 1 3011 5
	ldrb	r3, [sp, #87]	@ zero_extendqisi2
	cmp	r3, #4
	bne	.L437
	.loc 1 3011 23 discriminator 1
	movs	r3, #1
	b	.L460
.L437:
	.loc 1 3012 5
	ldrb	r3, [sp, #87]	@ zero_extendqisi2
	cmp	r3, #1
	bls	.L438
	.loc 1 3012 23 discriminator 1
	movs	r3, #13
	b	.L460
.L438:
	.loc 1 3063 15
	ldr	r3, [sp, #56]
	adds	r3, r3, #56
	.loc 1 3063 7
	adds	r3, r3, #11
	mov	r0, r3
	bl	ld_word
	mov	r3, r0
	.loc 1 3063 6
	cmp	r3, #512
	beq	.L439
	.loc 1 3063 59 discriminator 1
	movs	r3, #13
	b	.L460
.L439:
	.loc 1 3065 20
	ldr	r3, [sp, #56]
	adds	r3, r3, #56
	.loc 1 3065 12
	adds	r3, r3, #22
	mov	r0, r3
	bl	ld_word
	mov	r3, r0
	.loc 1 3065 10
	str	r3, [sp, #76]
	.loc 1 3066 6
	ldr	r3, [sp, #76]
	cmp	r3, #0
	bne	.L440
	.loc 1 3066 38 discriminator 1
	ldr	r3, [sp, #56]
	adds	r3, r3, #56
	.loc 1 3066 29 discriminator 1
	adds	r3, r3, #36
	mov	r0, r3
	bl	ld_dword
	str	r0, [sp, #76]
.L440:
	.loc 1 3067 13
	ldr	r3, [sp, #56]
	ldr	r2, [sp, #76]
	str	r2, [r3, #32]
	.loc 1 3069 23
	ldr	r3, [sp, #56]
	ldrb	r2, [r3, #72]	@ zero_extendqisi2
	.loc 1 3069 14
	ldr	r3, [sp, #56]
	strb	r2, [r3, #2]
	.loc 1 3070 9
	ldr	r3, [sp, #56]
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	.loc 1 3070 6
	cmp	r3, #1
	beq	.L441
	.loc 1 3070 28 discriminator 1
	ldr	r3, [sp, #56]
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	.loc 1 3070 23 discriminator 1
	cmp	r3, #2
	beq	.L441
	.loc 1 3070 50 discriminator 2
	movs	r3, #13
	b	.L460
.L441:
	.loc 1 3071 15
	ldr	r3, [sp, #56]
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	mov	r2, r3
	.loc 1 3071 10
	ldr	r3, [sp, #76]
	mul	r3, r2, r3
	str	r3, [sp, #76]
	.loc 1 3073 22
	ldr	r3, [sp, #56]
	ldrb	r3, [r3, #69]	@ zero_extendqisi2
	uxth	r2, r3
	.loc 1 3073 13
	ldr	r3, [sp, #56]
	strh	r2, [r3, #10]	@ movhi
	.loc 1 3074 9
	ldr	r3, [sp, #56]
	ldrh	r3, [r3, #10]
	.loc 1 3074 6
	cmp	r3, #0
	beq	.L442
	.loc 1 3074 28 discriminator 2
	ldr	r3, [sp, #56]
	ldrh	r3, [r3, #10]
	mov	r2, r3
	.loc 1 3074 41 discriminator 2
	ldr	r3, [sp, #56]
	ldrh	r3, [r3, #10]
	.loc 1 3074 49 discriminator 2
	subs	r3, r3, #1
	.loc 1 3074 36 discriminator 2
	ands	r3, r3, r2
	.loc 1 3074 22 discriminator 2
	cmp	r3, #0
	beq	.L443
.L442:
	.loc 1 3074 63 discriminator 3
	movs	r3, #13
	b	.L460
.L443:
	.loc 1 3076 27
	ldr	r3, [sp, #56]
	adds	r3, r3, #56
	.loc 1 3076 19
	adds	r3, r3, #17
	mov	r0, r3
	bl	ld_word
	mov	r3, r0
	mov	r2, r3
	.loc 1 3076 17
	ldr	r3, [sp, #56]
	strh	r2, [r3, #8]	@ movhi
	.loc 1 3077 9
	ldr	r3, [sp, #56]
	ldrh	r3, [r3, #8]
	.loc 1 3077 7
	and	r3, r3, #15
	uxth	r3, r3
	.loc 1 3077 6
	cmp	r3, #0
	beq	.L444
	.loc 1 3077 49 discriminator 1
	movs	r3, #13
	b	.L460
.L444:
	.loc 1 3079 19
	ldr	r3, [sp, #56]
	adds	r3, r3, #56
	.loc 1 3079 11
	adds	r3, r3, #19
	mov	r0, r3
	bl	ld_word
	mov	r3, r0
	.loc 1 3079 9
	str	r3, [sp, #72]
	.loc 1 3080 6
	ldr	r3, [sp, #72]
	cmp	r3, #0
	bne	.L445
	.loc 1 3080 36 discriminator 1
	ldr	r3, [sp, #56]
	adds	r3, r3, #56
	.loc 1 3080 27 discriminator 1
	adds	r3, r3, #32
	mov	r0, r3
	bl	ld_dword
	str	r0, [sp, #72]
.L445:
	.loc 1 3082 18
	ldr	r3, [sp, #56]
	adds	r3, r3, #56
	.loc 1 3082 10
	adds	r3, r3, #14
	mov	r0, r3
	bl	ld_word
	mov	r3, r0
	strh	r3, [sp, #46]	@ movhi
	.loc 1 3083 6
	ldrh	r3, [sp, #46]
	cmp	r3, #0
	bne	.L446
	.loc 1 3083 25 discriminator 1
	movs	r3, #13
	b	.L460
.L446:
	.loc 1 3086 17
	ldrh	r2, [sp, #46]
	ldr	r3, [sp, #76]
	add	r3, r3, r2
	.loc 1 3086 30
	ldr	r2, [sp, #56]
	ldrh	r2, [r2, #8]
	.loc 1 3086 42
	lsrs	r2, r2, #4
	uxth	r2, r2
	.loc 1 3086 10
	add	r3, r3, r2
	str	r3, [sp, #40]
	.loc 1 3087 6
	ldr	r2, [sp, #72]
	ldr	r3, [sp, #40]
	cmp	r2, r3
	bcs	.L447
	.loc 1 3087 30 discriminator 1
	movs	r3, #13
	b	.L460
.L462:
	.align	2
.L461:
	.word	FatFs
.L447:
	.loc 1 3088 18
	ldr	r2, [sp, #72]
	ldr	r3, [sp, #40]
	subs	r3, r2, r3
	.loc 1 3088 32
	ldr	r2, [sp, #56]
	ldrh	r2, [r2, #10]
	.loc 1 3088 9
	udiv	r3, r3, r2
	str	r3, [sp, #36]
	.loc 1 3089 6
	ldr	r3, [sp, #36]
	cmp	r3, #0
	bne	.L448
	.loc 1 3089 26 discriminator 1
	movs	r3, #13
	b	.L460
.L448:
	.loc 1 3090 7
	movs	r3, #3
	strb	r3, [sp, #87]
	.loc 1 3091 6
	ldr	r3, [sp, #36]
	movw	r2, #65525
	cmp	r3, r2
	bhi	.L449
	.loc 1 3091 31 discriminator 1
	movs	r3, #2
	strb	r3, [sp, #87]
.L449:
	.loc 1 3092 6
	ldr	r3, [sp, #36]
	movw	r2, #4085
	cmp	r3, r2
	bhi	.L450
	.loc 1 3092 31 discriminator 1
	movs	r3, #1
	strb	r3, [sp, #87]
.L450:
	.loc 1 3095 24
	ldr	r3, [sp, #36]
	adds	r2, r3, #2
	.loc 1 3095 16
	ldr	r3, [sp, #56]
	str	r2, [r3, #28]
	.loc 1 3096 15
	ldr	r3, [sp, #56]
	ldr	r2, [sp, #80]
	str	r2, [r3, #36]
	.loc 1 3097 23
	ldrh	r2, [sp, #46]
	ldr	r3, [sp, #80]
	add	r2, r2, r3
	.loc 1 3097 15
	ldr	r3, [sp, #56]
	str	r2, [r3, #40]
	.loc 1 3098 24
	ldr	r2, [sp, #80]
	ldr	r3, [sp, #40]
	add	r2, r2, r3
	.loc 1 3098 16
	ldr	r3, [sp, #56]
	str	r2, [r3, #48]
	.loc 1 3099 6
	ldrb	r3, [sp, #87]	@ zero_extendqisi2
	cmp	r3, #3
	bne	.L451
	.loc 1 3100 16
	ldr	r3, [sp, #56]
	adds	r3, r3, #56
	.loc 1 3100 8
	adds	r3, r3, #42
	mov	r0, r3
	bl	ld_word
	mov	r3, r0
	.loc 1 3100 7
	cmp	r3, #0
	beq	.L452
	.loc 1 3100 52 discriminator 1
	movs	r3, #13
	b	.L460
.L452:
	.loc 1 3101 10
	ldr	r3, [sp, #56]
	ldrh	r3, [r3, #8]
	.loc 1 3101 7
	cmp	r3, #0
	beq	.L453
	.loc 1 3101 30 discriminator 1
	movs	r3, #13
	b	.L460
.L453:
	.loc 1 3102 27
	ldr	r3, [sp, #56]
	adds	r3, r3, #56
	.loc 1 3102 18
	adds	r3, r3, #44
	mov	r0, r3
	bl	ld_dword
	mov	r2, r0
	.loc 1 3102 16
	ldr	r3, [sp, #56]
	str	r2, [r3, #44]
	.loc 1 3103 15
	ldr	r3, [sp, #56]
	ldr	r3, [r3, #28]
	.loc 1 3103 11
	lsls	r3, r3, #2
	str	r3, [sp, #68]
	b	.L454
.L451:
	.loc 1 3105 10
	ldr	r3, [sp, #56]
	ldrh	r3, [r3, #8]
	.loc 1 3105 7
	cmp	r3, #0
	bne	.L455
	.loc 1 3105 35 discriminator 1
	movs	r3, #13
	b	.L460
.L455:
	.loc 1 3106 20
	ldr	r3, [sp, #56]
	ldr	r2, [r3, #40]
	.loc 1 3106 30
	ldr	r3, [sp, #76]
	add	r2, r2, r3
	.loc 1 3106 16
	ldr	r3, [sp, #56]
	str	r2, [r3, #44]
	.loc 1 3108 22
	ldrb	r3, [sp, #87]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L456
	.loc 1 3108 7 discriminator 1
	ldr	r3, [sp, #56]
	ldr	r3, [r3, #28]
	.loc 1 3108 22 discriminator 1
	lsls	r3, r3, #1
	b	.L457
.L456:
	.loc 1 3108 26 discriminator 2
	ldr	r3, [sp, #56]
	ldr	r2, [r3, #28]
	.loc 1 3108 37 discriminator 2
	mov	r3, r2
	lsls	r3, r3, #1
	add	r3, r3, r2
	.loc 1 3108 41 discriminator 2
	lsrs	r2, r3, #1
	.loc 1 3108 50 discriminator 2
	ldr	r3, [sp, #56]
	ldr	r3, [r3, #28]
	.loc 1 3108 61 discriminator 2
	and	r3, r3, #1
	.loc 1 3108 22 discriminator 2
	add	r3, r3, r2
.L457:
	.loc 1 3107 11
	str	r3, [sp, #68]
.L454:
	.loc 1 3110 9
	ldr	r3, [sp, #56]
	ldr	r2, [r3, #32]
	.loc 1 3110 27
	ldr	r3, [sp, #68]
	addw	r3, r3, #511
	.loc 1 3110 43
	lsrs	r3, r3, #9
	.loc 1 3110 6
	cmp	r2, r3
	bcs	.L458
	.loc 1 3110 60 discriminator 1
	movs	r3, #13
	b	.L460
.L458:
	.loc 1 3114 33
	ldr	r3, [sp, #56]
	mov	r2, #-1
	str	r2, [r3, #20]
	.loc 1 3114 21
	ldr	r3, [sp, #56]
	ldr	r2, [r3, #20]
	.loc 1 3114 17
	ldr	r3, [sp, #56]
	str	r2, [r3, #16]
	.loc 1 3115 16
	ldr	r3, [sp, #56]
	movs	r2, #128
	strb	r2, [r3, #4]
	.loc 1 3117 6
	ldrb	r3, [sp, #87]	@ zero_extendqisi2
	cmp	r3, #3
	bne	.L459
	.loc 1 3118 15
	ldr	r3, [sp, #56]
	adds	r3, r3, #56
	.loc 1 3118 7
	adds	r3, r3, #48
	mov	r0, r3
	bl	ld_word
	mov	r3, r0
	.loc 1 3118 4
	cmp	r3, #1
	bne	.L459
	.loc 1 3119 7
	ldr	r3, [sp, #80]
	adds	r3, r3, #1
	mov	r1, r3
	ldr	r0, [sp, #56]
	bl	move_window
	mov	r3, r0
	.loc 1 3119 4
	cmp	r3, #0
	bne	.L459
	.loc 1 3121 17
	ldr	r3, [sp, #56]
	movs	r2, #0
	strb	r2, [r3, #4]
	.loc 1 3122 16
	ldr	r3, [sp, #56]
	adds	r3, r3, #56
	.loc 1 3122 8
	add	r3, r3, #510
	mov	r0, r3
	bl	ld_word
	mov	r3, r0
	mov	r2, r3
	.loc 1 3122 7
	movw	r3, #43605
	cmp	r2, r3
	bne	.L459
	.loc 1 3123 17
	ldr	r3, [sp, #56]
	adds	r3, r3, #56
	.loc 1 3123 8
	mov	r0, r3
	bl	ld_dword
	mov	r3, r0
	.loc 1 3123 5
	ldr	r2, .L463
	cmp	r3, r2
	bne	.L459
	.loc 1 3124 17
	ldr	r3, [sp, #56]
	adds	r3, r3, #56
	.loc 1 3124 8
	add	r3, r3, #484
	mov	r0, r3
	bl	ld_dword
	mov	r3, r0
	.loc 1 3124 5
	ldr	r2, .L463+4
	cmp	r3, r2
	bne	.L459
	.loc 1 3127 30
	ldr	r3, [sp, #56]
	adds	r3, r3, #56
	.loc 1 3127 21
	add	r3, r3, #488
	mov	r0, r3
	bl	ld_dword
	mov	r2, r0
	.loc 1 3127 19
	ldr	r3, [sp, #56]
	str	r2, [r3, #20]
	.loc 1 3130 30
	ldr	r3, [sp, #56]
	adds	r3, r3, #56
	.loc 1 3130 21
	add	r3, r3, #492
	mov	r0, r3
	bl	ld_dword
	mov	r2, r0
	.loc 1 3130 19
	ldr	r3, [sp, #56]
	str	r2, [r3, #16]
.L459:
	.loc 1 3138 14
	ldr	r3, [sp, #56]
	ldrb	r2, [sp, #87]
	strb	r2, [r3]
	.loc 1 3139 11
	ldr	r3, .L463+8
	ldrh	r3, [r3]
	adds	r3, r3, #1
	uxth	r2, r3
	.loc 1 3139 9
	ldr	r3, .L463+8
	strh	r2, [r3]	@ movhi
	ldr	r3, .L463+8
	ldrh	r2, [r3]
	ldr	r3, [sp, #56]
	strh	r2, [r3, #6]	@ movhi
	.loc 1 3141 13
	ldr	r3, [sp, #56]
	ldr	r2, .L463+12
	str	r2, [r3, #12]
	.loc 1 3147 11
	ldr	r3, [sp, #56]
	movs	r2, #0
	str	r2, [r3, #24]
	.loc 1 3152 9
	movs	r3, #0
.L460:
	.loc 1 3153 1 discriminator 1
	mov	r0, r3
	add	sp, sp, #92
.LCFI96:
	@ sp needed
	ldr	pc, [sp], #4
.L464:
	.align	2
.L463:
	.word	1096897106
	.word	1631679090
	.word	Fsid
	.word	LfnBuf
.LFE35:
	.size	find_volume, .-find_volume
	.section	.text.validate,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	validate, %function
validate:
.LFB36:
	.loc 1 3167 1
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI97:
	sub	sp, sp, #20
.LCFI98:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 3171 5
	ldr	r3, [sp, #4]
	cmp	r3, #0
	beq	.L466
	.loc 1 3171 18 discriminator 1
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	.loc 1 3171 11 discriminator 1
	cmp	r3, #0
	beq	.L466
	.loc 1 3171 30 discriminator 2
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	.loc 1 3171 34 discriminator 2
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 3171 23 discriminator 2
	cmp	r3, #0
	beq	.L466
	.loc 1 3171 50 discriminator 3
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	.loc 1 3171 54 discriminator 3
	ldrh	r2, [r3, #6]
	.loc 1 3171 65 discriminator 3
	ldr	r3, [sp, #4]
	ldrh	r3, [r3, #4]
	.loc 1 3171 44 discriminator 3
	cmp	r2, r3
	bne	.L466
	.loc 1 3171 89 discriminator 4
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	.loc 1 3171 74 discriminator 4
	ldrb	r3, [r3, #1]	@ zero_extendqisi2
	mov	r0, r3
	bl	disk_status
	mov	r3, r0
	.loc 1 3171 100 discriminator 4
	and	r3, r3, #1
	.loc 1 3171 70 discriminator 4
	cmp	r3, #0
	beq	.L467
.L466:
	.loc 1 3172 7
	ldr	r3, [sp]
	movs	r2, #0
	str	r2, [r3]
	.loc 1 3173 7
	movs	r3, #9
	strb	r3, [sp, #15]
	b	.L468
.L467:
	.loc 1 3175 12
	ldr	r3, [sp, #4]
	ldr	r2, [r3]
	.loc 1 3175 7
	ldr	r3, [sp]
	str	r2, [r3]
	.loc 1 3177 7
	movs	r3, #0
	strb	r3, [sp, #15]
.L468:
	.loc 1 3179 9
	ldrb	r3, [sp, #15]	@ zero_extendqisi2
	.loc 1 3180 1
	mov	r0, r3
	add	sp, sp, #20
.LCFI99:
	@ sp needed
	ldr	pc, [sp], #4
.LFE36:
	.size	validate, .-validate
	.section	.text.f_mount,"ax",%progbits
	.align	1
	.global	f_mount
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	f_mount, %function
f_mount:
.LFB37:
	.loc 1 3202 1
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI100:
	sub	sp, sp, #36
.LCFI101:
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	mov	r3, r2
	strb	r3, [sp, #7]
	.loc 1 3206 15
	ldr	r3, [sp, #8]
	str	r3, [sp, #16]
	.loc 1 3210 8
	add	r3, sp, #16
	mov	r0, r3
	bl	get_ldnumber
	str	r0, [sp, #28]
	.loc 1 3211 5
	ldr	r3, [sp, #28]
	cmp	r3, #0
	bge	.L471
	.loc 1 3211 22 discriminator 1
	movs	r3, #11
	b	.L477
.L471:
	.loc 1 3212 6
	ldr	r2, .L478
	ldr	r3, [sp, #28]
	ldr	r3, [r2, r3, lsl #2]
	str	r3, [sp, #24]
	.loc 1 3214 5
	ldr	r3, [sp, #24]
	cmp	r3, #0
	beq	.L473
	.loc 1 3221 16
	ldr	r3, [sp, #24]
	movs	r2, #0
	strb	r2, [r3]
.L473:
	.loc 1 3224 6
	ldr	r3, [sp, #12]
	.loc 1 3224 5
	cmp	r3, #0
	beq	.L474
	.loc 1 3225 5
	ldr	r3, [sp, #12]
	.loc 1 3225 15
	movs	r2, #0
	strb	r2, [r3]
.L474:
	.loc 1 3230 13
	ldr	r2, [sp, #12]
	ldr	r1, .L478
	ldr	r3, [sp, #28]
	str	r2, [r1, r3, lsl #2]
	.loc 1 3232 6
	ldr	r3, [sp, #12]
	.loc 1 3232 5
	cmp	r3, #0
	beq	.L475
	.loc 1 3232 10 discriminator 2
	ldrb	r3, [sp, #7]	@ zero_extendqisi2
	cmp	r3, #1
	beq	.L476
.L475:
	.loc 1 3232 30 discriminator 3
	movs	r3, #0
	b	.L477
.L476:
	.loc 1 3234 8
	add	r1, sp, #12
	add	r3, sp, #8
	movs	r2, #0
	mov	r0, r3
	bl	find_volume
	mov	r3, r0
	strb	r3, [sp, #23]
	.loc 1 3235 2
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
.L477:
	.loc 1 3236 1 discriminator 1
	mov	r0, r3
	add	sp, sp, #36
.LCFI102:
	@ sp needed
	ldr	pc, [sp], #4
.L479:
	.align	2
.L478:
	.word	FatFs
.LFE37:
	.size	f_mount, .-f_mount
	.section	.text.f_open,"ax",%progbits
	.align	1
	.global	f_open
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	f_open, %function
f_open:
.LFB38:
	.loc 1 3250 1
	@ args = 0, pretend = 0, frame = 96
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI103:
	sub	sp, sp, #100
.LCFI104:
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	mov	r3, r2
	strb	r3, [sp, #7]
	.loc 1 3261 5
	ldr	r3, [sp, #12]
	cmp	r3, #0
	bne	.L481
	.loc 1 3261 18 discriminator 1
	movs	r3, #9
	b	.L501
.L481:
	.loc 1 3264 7
	ldrb	r3, [sp, #7]
	and	r3, r3, #63
	strb	r3, [sp, #7]
	.loc 1 3265 8
	ldrb	r2, [sp, #7]	@ zero_extendqisi2
	add	r1, sp, #16
	add	r3, sp, #8
	mov	r0, r3
	bl	find_volume
	mov	r3, r0
	strb	r3, [sp, #95]
	.loc 1 3266 5
	ldrb	r3, [sp, #95]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L483
	.loc 1 3267 13
	ldr	r3, [sp, #16]
	str	r3, [sp, #20]
	.loc 1 3269 9
	ldr	r2, [sp, #8]
	add	r3, sp, #20
	mov	r1, r2
	mov	r0, r3
	bl	follow_path
	mov	r3, r0
	strb	r3, [sp, #95]
	.loc 1 3271 6
	ldrb	r3, [sp, #95]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L484
	.loc 1 3272 13
	ldrb	r3, [sp, #63]	@ zero_extendqisi2
	.loc 1 3272 8
	sxtb	r3, r3
	.loc 1 3272 7
	cmp	r3, #0
	bge	.L484
	.loc 1 3273 9
	movs	r3, #6
	strb	r3, [sp, #95]
.L484:
	.loc 1 3282 12
	ldrb	r3, [sp, #7]	@ zero_extendqisi2
	and	r3, r3, #28
	.loc 1 3282 6
	cmp	r3, #0
	beq	.L485
	.loc 1 3283 7
	ldrb	r3, [sp, #95]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L486
	.loc 1 3284 8
	ldrb	r3, [sp, #95]	@ zero_extendqisi2
	cmp	r3, #4
	bne	.L487
	.loc 1 3288 12
	add	r3, sp, #20
	mov	r0, r3
	bl	dir_register
	mov	r3, r0
	strb	r3, [sp, #95]
.L487:
	.loc 1 3290 10
	ldrb	r3, [sp, #7]
	orr	r3, r3, #8
	strb	r3, [sp, #7]
	b	.L488
.L486:
	.loc 1 3293 15
	ldrb	r3, [sp, #26]	@ zero_extendqisi2
	.loc 1 3293 21
	and	r3, r3, #17
	.loc 1 3293 8
	cmp	r3, #0
	beq	.L489
	.loc 1 3294 10
	movs	r3, #7
	strb	r3, [sp, #95]
	b	.L488
.L489:
	.loc 1 3296 15
	ldrb	r3, [sp, #7]	@ zero_extendqisi2
	and	r3, r3, #4
	.loc 1 3296 9
	cmp	r3, #0
	beq	.L488
	.loc 1 3296 36 discriminator 1
	movs	r3, #8
	strb	r3, [sp, #95]
.L488:
	.loc 1 3299 7
	ldrb	r3, [sp, #95]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L490
	.loc 1 3299 30 discriminator 1
	ldrb	r3, [sp, #7]	@ zero_extendqisi2
	and	r3, r3, #8
	.loc 1 3299 21 discriminator 1
	cmp	r3, #0
	beq	.L490
	.loc 1 3300 8
	ldr	r3, .L502
	str	r3, [sp, #80]
	.loc 1 3327 17
	ldr	r3, [sp, #48]
	.loc 1 3327 6
	adds	r3, r3, #14
	ldr	r1, [sp, #80]
	mov	r0, r3
	bl	st_dword
	.loc 1 3328 17
	ldr	r3, [sp, #48]
	.loc 1 3328 6
	adds	r3, r3, #22
	ldr	r1, [sp, #80]
	mov	r0, r3
	bl	st_dword
	.loc 1 3329 8
	ldr	r3, [sp, #48]
	.loc 1 3329 12
	adds	r3, r3, #11
	.loc 1 3329 23
	movs	r2, #32
	strb	r2, [r3]
	.loc 1 3330 11
	ldr	r3, [sp, #16]
	.loc 1 3330 26
	ldr	r2, [sp, #48]
	.loc 1 3330 11
	mov	r1, r2
	mov	r0, r3
	bl	ld_clust
	str	r0, [sp, #76]
	.loc 1 3331 6
	ldr	r3, [sp, #16]
	ldr	r1, [sp, #48]
	movs	r2, #0
	mov	r0, r3
	bl	st_clust
	.loc 1 3332 17
	ldr	r3, [sp, #48]
	.loc 1 3332 6
	adds	r3, r3, #28
	movs	r1, #0
	mov	r0, r3
	bl	st_dword
	.loc 1 3333 8
	ldr	r3, [sp, #16]
	.loc 1 3333 16
	movs	r2, #1
	strb	r2, [r3, #3]
	.loc 1 3335 9
	ldr	r3, [sp, #76]
	cmp	r3, #0
	beq	.L490
	.loc 1 3336 14
	ldr	r3, [sp, #16]
	.loc 1 3336 10
	ldr	r3, [r3, #52]
	str	r3, [sp, #80]
	.loc 1 3337 13
	add	r3, sp, #20
	movs	r2, #0
	ldr	r1, [sp, #76]
	mov	r0, r3
	bl	remove_chain
	mov	r3, r0
	strb	r3, [sp, #95]
	.loc 1 3338 10
	ldrb	r3, [sp, #95]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L490
	.loc 1 3339 14
	ldr	r3, [sp, #16]
	ldr	r1, [sp, #80]
	mov	r0, r3
	bl	move_window
	mov	r3, r0
	strb	r3, [sp, #95]
	.loc 1 3340 10
	ldr	r3, [sp, #16]
	.loc 1 3340 27
	ldr	r2, [sp, #76]
	subs	r2, r2, #1
	.loc 1 3340 22
	str	r2, [r3, #16]
	b	.L490
.L485:
	.loc 1 3347 7
	ldrb	r3, [sp, #95]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L490
	.loc 1 3348 15
	ldrb	r3, [sp, #26]	@ zero_extendqisi2
	.loc 1 3348 21
	and	r3, r3, #16
	.loc 1 3348 8
	cmp	r3, #0
	beq	.L491
	.loc 1 3349 10
	movs	r3, #4
	strb	r3, [sp, #95]
	b	.L490
.L491:
	.loc 1 3351 16
	ldrb	r3, [sp, #7]	@ zero_extendqisi2
	and	r3, r3, #2
	.loc 1 3351 9
	cmp	r3, #0
	beq	.L490
	.loc 1 3351 38 discriminator 1
	ldrb	r3, [sp, #26]	@ zero_extendqisi2
	.loc 1 3351 44 discriminator 1
	and	r3, r3, #1
	.loc 1 3351 28 discriminator 1
	cmp	r3, #0
	beq	.L490
	.loc 1 3352 11
	movs	r3, #7
	strb	r3, [sp, #95]
.L490:
	.loc 1 3357 6
	ldrb	r3, [sp, #95]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L492
	.loc 1 3358 13
	ldrb	r3, [sp, #7]	@ zero_extendqisi2
	and	r3, r3, #8
	.loc 1 3358 7
	cmp	r3, #0
	beq	.L493
	.loc 1 3359 10
	ldrb	r3, [sp, #7]
	orr	r3, r3, #64
	strb	r3, [sp, #7]
.L493:
	.loc 1 3360 21
	ldr	r3, [sp, #16]
	ldr	r2, [r3, #52]
	.loc 1 3360 17
	ldr	r3, [sp, #12]
	str	r2, [r3, #32]
	.loc 1 3361 20
	ldr	r2, [sp, #48]
	.loc 1 3361 16
	ldr	r3, [sp, #12]
	str	r2, [r3, #36]
.L492:
	.loc 1 3379 6
	ldrb	r3, [sp, #95]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L483
	.loc 1 3391 22
	ldr	r3, [sp, #16]
	.loc 1 3391 37
	ldr	r2, [sp, #48]
	.loc 1 3391 22
	mov	r1, r2
	mov	r0, r3
	bl	ld_clust
	mov	r2, r0
	.loc 1 3391 20
	ldr	r3, [sp, #12]
	str	r2, [r3, #8]
	.loc 1 3392 34
	ldr	r3, [sp, #48]
	.loc 1 3392 39
	adds	r3, r3, #28
	.loc 1 3392 23
	mov	r0, r3
	bl	ld_dword
	mov	r2, r0
	.loc 1 3392 21
	ldr	r3, [sp, #12]
	str	r2, [r3, #12]
	.loc 1 3397 15
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #12]
	str	r2, [r3]
	.loc 1 3398 19
	ldr	r3, [sp, #16]
	ldrh	r2, [r3, #6]
	.loc 1 3398 15
	ldr	r3, [sp, #12]
	strh	r2, [r3, #4]	@ movhi
	.loc 1 3399 13
	ldr	r3, [sp, #12]
	ldrb	r2, [sp, #7]
	strb	r2, [r3, #16]
	.loc 1 3400 12
	ldr	r3, [sp, #12]
	movs	r2, #0
	strb	r2, [r3, #17]
	.loc 1 3401 13
	ldr	r3, [sp, #12]
	movs	r2, #0
	str	r2, [r3, #28]
	.loc 1 3402 13
	ldr	r3, [sp, #12]
	movs	r2, #0
	str	r2, [r3, #20]
	.loc 1 3405 14
	ldr	r3, [sp, #12]
	adds	r3, r3, #40
	.loc 1 3405 4
	mov	r2, #512
	movs	r1, #0
	mov	r0, r3
	bl	mem_set
	.loc 1 3407 14
	ldrb	r3, [sp, #7]	@ zero_extendqisi2
	and	r3, r3, #32
	.loc 1 3407 7
	cmp	r3, #0
	beq	.L483
	.loc 1 3407 38 discriminator 1
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #12]
	.loc 1 3407 28 discriminator 1
	cmp	r3, #0
	beq	.L483
	.loc 1 3408 23
	ldr	r3, [sp, #12]
	ldr	r2, [r3, #12]
	.loc 1 3408 14
	ldr	r3, [sp, #12]
	str	r2, [r3, #20]
	.loc 1 3409 20
	ldr	r3, [sp, #16]
	ldrh	r3, [r3, #10]
	.loc 1 3409 9
	lsls	r3, r3, #9
	str	r3, [sp, #72]
	.loc 1 3410 10
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #8]
	str	r3, [sp, #88]
	.loc 1 3411 14
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #12]
	str	r3, [sp, #84]
	.loc 1 3411 5
	b	.L494
.L498:
	.loc 1 3412 13
	ldr	r3, [sp, #12]
	ldr	r1, [sp, #88]
	mov	r0, r3
	bl	get_fat
	str	r0, [sp, #88]
	.loc 1 3413 9
	ldr	r3, [sp, #88]
	cmp	r3, #1
	bhi	.L495
	.loc 1 3413 25 discriminator 1
	movs	r3, #2
	strb	r3, [sp, #95]
.L495:
	.loc 1 3414 9
	ldr	r3, [sp, #88]
	cmp	r3, #-1
	bne	.L496
	.loc 1 3414 34 discriminator 1
	movs	r3, #1
	strb	r3, [sp, #95]
.L496:
	.loc 1 3411 64 discriminator 2
	ldr	r2, [sp, #84]
	ldr	r3, [sp, #72]
	subs	r3, r2, r3
	str	r3, [sp, #84]
.L494:
	.loc 1 3411 5 discriminator 1
	ldrb	r3, [sp, #95]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L497
	.loc 1 3411 46 discriminator 3
	ldr	r2, [sp, #84]
	ldr	r3, [sp, #72]
	cmp	r2, r3
	bhi	.L498
.L497:
	.loc 1 3416 15
	ldr	r3, [sp, #12]
	ldr	r2, [sp, #88]
	str	r2, [r3, #24]
	.loc 1 3417 8
	ldrb	r3, [sp, #95]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L483
	.loc 1 3417 29 discriminator 1
	ldr	r3, [sp, #84]
	ubfx	r3, r3, #0, #9
	.loc 1 3417 22 discriminator 1
	cmp	r3, #0
	beq	.L483
	.loc 1 3418 16
	ldr	r3, [sp, #16]
	ldr	r1, [sp, #88]
	mov	r0, r3
	bl	clust2sect
	str	r0, [sp, #68]
	.loc 1 3418 9
	ldr	r3, [sp, #68]
	cmp	r3, #0
	bne	.L499
	.loc 1 3419 11
	movs	r3, #2
	strb	r3, [sp, #95]
	b	.L483
.L499:
	.loc 1 3421 23
	ldr	r3, [sp, #84]
	lsrs	r2, r3, #9
	.loc 1 3421 21
	ldr	r3, [sp, #68]
	add	r2, r2, r3
	.loc 1 3421 16
	ldr	r3, [sp, #12]
	str	r2, [r3, #28]
	.loc 1 3423 23
	ldr	r3, [sp, #16]
	.loc 1 3423 11
	ldrb	r0, [r3, #1]	@ zero_extendqisi2
	.loc 1 3423 32
	ldr	r3, [sp, #12]
	add	r1, r3, #40
	.loc 1 3423 11
	ldr	r3, [sp, #12]
	ldr	r2, [r3, #28]
	movs	r3, #1
	bl	disk_read
	mov	r3, r0
	.loc 1 3423 10
	cmp	r3, #0
	beq	.L483
	.loc 1 3423 67 discriminator 1
	movs	r3, #1
	strb	r3, [sp, #95]
.L483:
	.loc 1 3434 5
	ldrb	r3, [sp, #95]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L500
	.loc 1 3434 31 discriminator 1
	ldr	r3, [sp, #12]
	movs	r2, #0
	str	r2, [r3]
.L500:
	.loc 1 3436 2
	ldrb	r3, [sp, #95]	@ zero_extendqisi2
.L501:
	.loc 1 3437 1 discriminator 1
	mov	r0, r3
	add	sp, sp, #100
.LCFI105:
	@ sp needed
	ldr	pc, [sp], #4
.L503:
	.align	2
.L502:
	.word	1210122240
.LFE38:
	.size	f_open, .-f_open
	.section	.text.f_read,"ax",%progbits
	.align	1
	.global	f_read
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	f_read, %function
f_read:
.LFB39:
	.loc 1 3452 1
	@ args = 0, pretend = 0, frame = 56
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI106:
	sub	sp, sp, #60
.LCFI107:
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	str	r2, [sp, #4]
	str	r3, [sp]
	.loc 1 3458 8
	ldr	r3, [sp, #8]
	str	r3, [sp, #36]
	.loc 1 3461 6
	ldr	r3, [sp]
	movs	r2, #0
	str	r2, [r3]
	.loc 1 3462 8
	ldr	r3, [sp, #12]
	add	r2, sp, #20
	mov	r1, r2
	mov	r0, r3
	bl	validate
	mov	r3, r0
	strb	r3, [sp, #55]
	.loc 1 3463 5
	ldrb	r3, [sp, #55]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L505
	.loc 1 3463 27 discriminator 2
	ldr	r3, [sp, #12]
	ldrb	r3, [r3, #17]
	strb	r3, [sp, #55]
	.loc 1 3463 19 discriminator 2
	ldrb	r3, [sp, #55]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L506
.L505:
	.loc 1 3463 57 discriminator 3
	ldrb	r3, [sp, #55]	@ zero_extendqisi2
	b	.L528
.L506:
	.loc 1 3464 10
	ldr	r3, [sp, #12]
	ldrb	r3, [r3, #16]	@ zero_extendqisi2
	.loc 1 3464 17
	and	r3, r3, #1
	.loc 1 3464 5
	cmp	r3, #0
	bne	.L508
	.loc 1 3464 29 discriminator 1
	movs	r3, #7
	b	.L528
.L508:
	.loc 1 3465 18
	ldr	r3, [sp, #12]
	ldr	r2, [r3, #12]
	.loc 1 3465 31
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #20]
	.loc 1 3465 9
	subs	r3, r2, r3
	str	r3, [sp, #32]
	.loc 1 3466 5
	ldr	r2, [sp, #4]
	ldr	r3, [sp, #32]
	cmp	r2, r3
	bls	.L510
	.loc 1 3466 24 discriminator 1
	ldr	r3, [sp, #32]
	str	r3, [sp, #4]
	.loc 1 3468 2 discriminator 1
	b	.L510
.L527:
	.loc 1 3470 9
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #20]
	.loc 1 3470 16
	ubfx	r3, r3, #0, #9
	.loc 1 3470 6
	cmp	r3, #0
	bne	.L511
	.loc 1 3471 21
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #20]
	.loc 1 3471 28
	lsrs	r3, r3, #9
	.loc 1 3471 42
	ldr	r2, [sp, #20]
	ldrh	r2, [r2, #10]
	.loc 1 3471 50
	subs	r2, r2, #1
	.loc 1 3471 10
	ands	r3, r3, r2
	str	r3, [sp, #28]
	.loc 1 3472 7
	ldr	r3, [sp, #28]
	cmp	r3, #0
	bne	.L512
	.loc 1 3473 11
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #20]
	.loc 1 3473 8
	cmp	r3, #0
	bne	.L513
	.loc 1 3474 11
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #8]
	str	r3, [sp, #48]
	b	.L514
.L513:
	.loc 1 3482 14
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #24]
	mov	r1, r3
	mov	r0, r2
	bl	get_fat
	str	r0, [sp, #48]
.L514:
	.loc 1 3485 8
	ldr	r3, [sp, #48]
	cmp	r3, #1
	bhi	.L515
	.loc 1 3485 19 discriminator 1
	ldr	r3, [sp, #12]
	movs	r2, #2
	strb	r2, [r3, #17]
	movs	r3, #2
	b	.L528
.L515:
	.loc 1 3486 8
	ldr	r3, [sp, #48]
	cmp	r3, #-1
	bne	.L516
	.loc 1 3486 29 discriminator 1
	ldr	r3, [sp, #12]
	movs	r2, #1
	strb	r2, [r3, #17]
	movs	r3, #1
	b	.L528
.L516:
	.loc 1 3487 15
	ldr	r3, [sp, #12]
	ldr	r2, [sp, #48]
	str	r2, [r3, #24]
.L512:
	.loc 1 3489 11
	ldr	r2, [sp, #20]
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #24]
	mov	r1, r3
	mov	r0, r2
	bl	clust2sect
	str	r0, [sp, #24]
	.loc 1 3490 7
	ldr	r3, [sp, #24]
	cmp	r3, #0
	bne	.L517
	.loc 1 3490 15 discriminator 1
	ldr	r3, [sp, #12]
	movs	r2, #2
	strb	r2, [r3, #17]
	movs	r3, #2
	b	.L528
.L517:
	.loc 1 3491 9
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #28]
	add	r3, r3, r2
	str	r3, [sp, #24]
	.loc 1 3492 7
	ldr	r3, [sp, #4]
	lsrs	r3, r3, #9
	str	r3, [sp, #40]
	.loc 1 3493 7
	ldr	r3, [sp, #40]
	cmp	r3, #0
	beq	.L518
	.loc 1 3494 15
	ldr	r2, [sp, #28]
	ldr	r3, [sp, #40]
	add	r3, r3, r2
	.loc 1 3494 24
	ldr	r2, [sp, #20]
	ldrh	r2, [r2, #10]
	.loc 1 3494 8
	cmp	r3, r2
	bls	.L519
	.loc 1 3495 13
	ldr	r3, [sp, #20]
	ldrh	r3, [r3, #10]
	mov	r2, r3
	.loc 1 3495 9
	ldr	r3, [sp, #28]
	subs	r3, r2, r3
	str	r3, [sp, #40]
.L519:
	.loc 1 3497 21
	ldr	r3, [sp, #20]
	.loc 1 3497 9
	ldrb	r0, [r3, #1]	@ zero_extendqisi2
	ldr	r3, [sp, #40]
	ldr	r2, [sp, #24]
	ldr	r1, [sp, #36]
	bl	disk_read
	mov	r3, r0
	.loc 1 3497 8
	cmp	r3, #0
	beq	.L520
	.loc 1 3497 56 discriminator 1
	ldr	r3, [sp, #12]
	movs	r2, #1
	strb	r2, [r3, #17]
	movs	r3, #1
	b	.L528
.L520:
	.loc 1 3504 12
	ldr	r3, [sp, #12]
	ldrb	r3, [r3, #16]	@ zero_extendqisi2
	.loc 1 3504 9
	sxtb	r3, r3
	.loc 1 3504 8
	cmp	r3, #0
	bge	.L521
	.loc 1 3504 36 discriminator 1
	ldr	r3, [sp, #12]
	ldr	r2, [r3, #28]
	.loc 1 3504 43 discriminator 1
	ldr	r3, [sp, #24]
	subs	r3, r2, r3
	.loc 1 3504 31 discriminator 1
	ldr	r2, [sp, #40]
	cmp	r2, r3
	bls	.L521
	.loc 1 3505 26
	ldr	r3, [sp, #12]
	ldr	r2, [r3, #28]
	.loc 1 3505 33
	ldr	r3, [sp, #24]
	subs	r3, r2, r3
	.loc 1 3505 41
	lsls	r3, r3, #9
	.loc 1 3505 20
	ldr	r2, [sp, #36]
	adds	r0, r2, r3
	.loc 1 3505 54
	ldr	r3, [sp, #12]
	adds	r3, r3, #40
	.loc 1 3505 6
	mov	r2, #512
	mov	r1, r3
	bl	mem_cpy
.L521:
	.loc 1 3509 10
	ldr	r3, [sp, #40]
	lsls	r3, r3, #9
	str	r3, [sp, #44]
	.loc 1 3510 5
	b	.L522
.L518:
	.loc 1 3513 10
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #28]
	.loc 1 3513 7
	ldr	r2, [sp, #24]
	cmp	r2, r3
	beq	.L523
	.loc 1 3515 11
	ldr	r3, [sp, #12]
	ldrb	r3, [r3, #16]	@ zero_extendqisi2
	.loc 1 3515 9
	sxtb	r3, r3
	.loc 1 3515 8
	cmp	r3, #0
	bge	.L524
	.loc 1 3516 23
	ldr	r3, [sp, #20]
	.loc 1 3516 10
	ldrb	r0, [r3, #1]	@ zero_extendqisi2
	.loc 1 3516 32
	ldr	r3, [sp, #12]
	add	r1, r3, #40
	.loc 1 3516 10
	ldr	r3, [sp, #12]
	ldr	r2, [r3, #28]
	movs	r3, #1
	bl	disk_write
	mov	r3, r0
	.loc 1 3516 9
	cmp	r3, #0
	beq	.L525
	.loc 1 3516 63 discriminator 1
	ldr	r3, [sp, #12]
	movs	r2, #1
	strb	r2, [r3, #17]
	movs	r3, #1
	b	.L528
.L525:
	.loc 1 3517 15
	ldr	r3, [sp, #12]
	ldrb	r3, [r3, #16]	@ zero_extendqisi2
	and	r3, r3, #127
	uxtb	r2, r3
	ldr	r3, [sp, #12]
	strb	r2, [r3, #16]
.L524:
	.loc 1 3520 21
	ldr	r3, [sp, #20]
	.loc 1 3520 9
	ldrb	r0, [r3, #1]	@ zero_extendqisi2
	.loc 1 3520 30
	ldr	r3, [sp, #12]
	add	r1, r3, #40
	.loc 1 3520 9
	movs	r3, #1
	ldr	r2, [sp, #24]
	bl	disk_read
	mov	r3, r0
	.loc 1 3520 8
	cmp	r3, #0
	beq	.L523
	.loc 1 3520 57 discriminator 1
	ldr	r3, [sp, #12]
	movs	r2, #1
	strb	r2, [r3, #17]
	movs	r3, #1
	b	.L528
.L523:
	.loc 1 3523 13
	ldr	r3, [sp, #12]
	ldr	r2, [sp, #24]
	str	r2, [r3, #28]
.L511:
	.loc 1 3525 27
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #20]
	.loc 1 3525 34
	ubfx	r3, r3, #0, #9
	.loc 1 3525 8
	rsb	r3, r3, #512
	str	r3, [sp, #44]
	.loc 1 3526 6
	ldr	r2, [sp, #44]
	ldr	r3, [sp, #4]
	cmp	r2, r3
	bls	.L526
	.loc 1 3526 24 discriminator 1
	ldr	r3, [sp, #4]
	str	r3, [sp, #44]
.L526:
	.loc 1 3531 18
	ldr	r3, [sp, #12]
	add	r2, r3, #40
	.loc 1 3531 30
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #20]
	.loc 1 3531 37
	ubfx	r3, r3, #0, #9
	.loc 1 3531 3
	add	r3, r3, r2
	ldr	r2, [sp, #44]
	mov	r1, r3
	ldr	r0, [sp, #36]
	bl	mem_cpy
.L522:
	.loc 1 3469 9
	ldr	r2, [sp, #36]
	ldr	r3, [sp, #44]
	add	r3, r3, r2
	str	r3, [sp, #36]
	.loc 1 3469 27
	ldr	r3, [sp, #12]
	ldr	r2, [r3, #20]
	ldr	r3, [sp, #44]
	add	r2, r2, r3
	ldr	r3, [sp, #12]
	str	r2, [r3, #20]
	.loc 1 3469 40
	ldr	r3, [sp]
	ldr	r2, [r3]
	ldr	r3, [sp, #44]
	add	r2, r2, r3
	ldr	r3, [sp]
	str	r2, [r3]
	.loc 1 3469 53
	ldr	r2, [sp, #4]
	ldr	r3, [sp, #44]
	subs	r3, r2, r3
	str	r3, [sp, #4]
.L510:
	.loc 1 3468 2
	ldr	r3, [sp, #4]
	cmp	r3, #0
	bne	.L527
	.loc 1 3535 2
	movs	r3, #0
.L528:
	.loc 1 3536 1 discriminator 1
	mov	r0, r3
	add	sp, sp, #60
.LCFI108:
	@ sp needed
	ldr	pc, [sp], #4
.LFE39:
	.size	f_read, .-f_read
	.section	.text.f_write,"ax",%progbits
	.align	1
	.global	f_write
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	f_write, %function
f_write:
.LFB40:
	.loc 1 3552 1
	@ args = 0, pretend = 0, frame = 48
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI109:
	sub	sp, sp, #52
.LCFI110:
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	str	r2, [sp, #4]
	str	r3, [sp]
	.loc 1 3557 14
	ldr	r3, [sp, #8]
	str	r3, [sp, #28]
	.loc 1 3560 6
	ldr	r3, [sp]
	movs	r2, #0
	str	r2, [r3]
	.loc 1 3561 8
	ldr	r3, [sp, #12]
	add	r2, sp, #16
	mov	r1, r2
	mov	r0, r3
	bl	validate
	mov	r3, r0
	strb	r3, [sp, #47]
	.loc 1 3562 5
	ldrb	r3, [sp, #47]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L530
	.loc 1 3562 27 discriminator 2
	ldr	r3, [sp, #12]
	ldrb	r3, [r3, #17]
	strb	r3, [sp, #47]
	.loc 1 3562 19 discriminator 2
	ldrb	r3, [sp, #47]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L531
.L530:
	.loc 1 3562 57 discriminator 3
	ldrb	r3, [sp, #47]	@ zero_extendqisi2
	b	.L555
.L531:
	.loc 1 3563 10
	ldr	r3, [sp, #12]
	ldrb	r3, [r3, #16]	@ zero_extendqisi2
	.loc 1 3563 17
	and	r3, r3, #2
	.loc 1 3563 5
	cmp	r3, #0
	bne	.L533
	.loc 1 3563 30 discriminator 1
	movs	r3, #7
	b	.L555
.L533:
	.loc 1 3566 59
	ldr	r3, [sp, #12]
	ldr	r2, [r3, #20]
	.loc 1 3566 49
	ldr	r3, [sp, #4]
	add	r2, r2, r3
	.loc 1 3566 84
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #20]
	.loc 1 3566 5
	cmp	r2, r3
	bcs	.L535
	.loc 1 3567 38
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #20]
	.loc 1 3567 7
	mvns	r3, r3
	str	r3, [sp, #4]
	.loc 1 3570 2
	b	.L535
.L554:
	.loc 1 3572 9
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #20]
	.loc 1 3572 16
	ubfx	r3, r3, #0, #9
	.loc 1 3572 6
	cmp	r3, #0
	bne	.L536
	.loc 1 3573 21
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #20]
	.loc 1 3573 28
	lsrs	r3, r3, #9
	.loc 1 3573 43
	ldr	r2, [sp, #16]
	ldrh	r2, [r2, #10]
	.loc 1 3573 51
	subs	r2, r2, #1
	.loc 1 3573 10
	ands	r3, r3, r2
	str	r3, [sp, #24]
	.loc 1 3574 7
	ldr	r3, [sp, #24]
	cmp	r3, #0
	bne	.L537
	.loc 1 3575 11
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #20]
	.loc 1 3575 8
	cmp	r3, #0
	bne	.L538
	.loc 1 3576 11
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #8]
	str	r3, [sp, #40]
	.loc 1 3577 9
	ldr	r3, [sp, #40]
	cmp	r3, #0
	bne	.L539
	.loc 1 3578 14
	ldr	r3, [sp, #12]
	movs	r1, #0
	mov	r0, r3
	bl	create_chain
	str	r0, [sp, #40]
	b	.L539
.L538:
	.loc 1 3587 14
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #24]
	mov	r1, r3
	mov	r0, r2
	bl	create_chain
	str	r0, [sp, #40]
.L539:
	.loc 1 3590 8
	ldr	r3, [sp, #40]
	cmp	r3, #0
	beq	.L556
	.loc 1 3591 8
	ldr	r3, [sp, #40]
	cmp	r3, #1
	bne	.L542
	.loc 1 3591 20 discriminator 1
	ldr	r3, [sp, #12]
	movs	r2, #2
	strb	r2, [r3, #17]
	movs	r3, #2
	b	.L555
.L542:
	.loc 1 3592 8
	ldr	r3, [sp, #40]
	cmp	r3, #-1
	bne	.L543
	.loc 1 3592 29 discriminator 1
	ldr	r3, [sp, #12]
	movs	r2, #1
	strb	r2, [r3, #17]
	movs	r3, #1
	b	.L555
.L543:
	.loc 1 3593 15
	ldr	r3, [sp, #12]
	ldr	r2, [sp, #40]
	str	r2, [r3, #24]
	.loc 1 3594 16
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #8]
	.loc 1 3594 8
	cmp	r3, #0
	bne	.L537
	.loc 1 3594 45 discriminator 1
	ldr	r3, [sp, #12]
	ldr	r2, [sp, #40]
	str	r2, [r3, #8]
.L537:
	.loc 1 3599 10
	ldr	r3, [sp, #12]
	ldrb	r3, [r3, #16]	@ zero_extendqisi2
	.loc 1 3599 8
	sxtb	r3, r3
	.loc 1 3599 7
	cmp	r3, #0
	bge	.L544
	.loc 1 3600 22
	ldr	r3, [sp, #16]
	.loc 1 3600 9
	ldrb	r0, [r3, #1]	@ zero_extendqisi2
	.loc 1 3600 31
	ldr	r3, [sp, #12]
	add	r1, r3, #40
	.loc 1 3600 9
	ldr	r3, [sp, #12]
	ldr	r2, [r3, #28]
	movs	r3, #1
	bl	disk_write
	mov	r3, r0
	.loc 1 3600 8
	cmp	r3, #0
	beq	.L545
	.loc 1 3600 62 discriminator 1
	ldr	r3, [sp, #12]
	movs	r2, #1
	strb	r2, [r3, #17]
	movs	r3, #1
	b	.L555
.L545:
	.loc 1 3601 14
	ldr	r3, [sp, #12]
	ldrb	r3, [r3, #16]	@ zero_extendqisi2
	and	r3, r3, #127
	uxtb	r2, r3
	ldr	r3, [sp, #12]
	strb	r2, [r3, #16]
.L544:
	.loc 1 3604 11
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #24]
	mov	r1, r3
	mov	r0, r2
	bl	clust2sect
	str	r0, [sp, #20]
	.loc 1 3605 7
	ldr	r3, [sp, #20]
	cmp	r3, #0
	bne	.L546
	.loc 1 3605 15 discriminator 1
	ldr	r3, [sp, #12]
	movs	r2, #2
	strb	r2, [r3, #17]
	movs	r3, #2
	b	.L555
.L546:
	.loc 1 3606 9
	ldr	r2, [sp, #20]
	ldr	r3, [sp, #24]
	add	r3, r3, r2
	str	r3, [sp, #20]
	.loc 1 3607 7
	ldr	r3, [sp, #4]
	lsrs	r3, r3, #9
	str	r3, [sp, #32]
	.loc 1 3608 7
	ldr	r3, [sp, #32]
	cmp	r3, #0
	beq	.L547
	.loc 1 3609 15
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #32]
	add	r3, r3, r2
	.loc 1 3609 24
	ldr	r2, [sp, #16]
	ldrh	r2, [r2, #10]
	.loc 1 3609 8
	cmp	r3, r2
	bls	.L548
	.loc 1 3610 13
	ldr	r3, [sp, #16]
	ldrh	r3, [r3, #10]
	mov	r2, r3
	.loc 1 3610 9
	ldr	r3, [sp, #24]
	subs	r3, r2, r3
	str	r3, [sp, #32]
.L548:
	.loc 1 3612 22
	ldr	r3, [sp, #16]
	.loc 1 3612 9
	ldrb	r0, [r3, #1]	@ zero_extendqisi2
	ldr	r3, [sp, #32]
	ldr	r2, [sp, #20]
	ldr	r1, [sp, #28]
	bl	disk_write
	mov	r3, r0
	.loc 1 3612 8
	cmp	r3, #0
	beq	.L549
	.loc 1 3612 57 discriminator 1
	ldr	r3, [sp, #12]
	movs	r2, #1
	strb	r2, [r3, #17]
	movs	r3, #1
	b	.L555
.L549:
	.loc 1 3620 11
	ldr	r3, [sp, #12]
	ldr	r2, [r3, #28]
	.loc 1 3620 18
	ldr	r3, [sp, #20]
	subs	r3, r2, r3
	.loc 1 3620 8
	ldr	r2, [sp, #32]
	cmp	r2, r3
	bls	.L550
	.loc 1 3621 16
	ldr	r3, [sp, #12]
	add	r0, r3, #40
	.loc 1 3621 35
	ldr	r3, [sp, #12]
	ldr	r2, [r3, #28]
	.loc 1 3621 42
	ldr	r3, [sp, #20]
	subs	r3, r2, r3
	.loc 1 3621 50
	lsls	r3, r3, #9
	.loc 1 3621 29
	ldr	r2, [sp, #28]
	add	r3, r3, r2
	.loc 1 3621 6
	mov	r2, #512
	mov	r1, r3
	bl	mem_cpy
	.loc 1 3622 15
	ldr	r3, [sp, #12]
	ldrb	r3, [r3, #16]	@ zero_extendqisi2
	and	r3, r3, #127
	uxtb	r2, r3
	ldr	r3, [sp, #12]
	strb	r2, [r3, #16]
.L550:
	.loc 1 3626 10
	ldr	r3, [sp, #32]
	lsls	r3, r3, #9
	str	r3, [sp, #36]
	.loc 1 3627 5
	b	.L551
.L547:
	.loc 1 3635 10
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #28]
	.loc 1 3635 7
	ldr	r2, [sp, #20]
	cmp	r2, r3
	beq	.L552
	.loc 1 3636 7 discriminator 1
	ldr	r3, [sp, #12]
	ldr	r2, [r3, #20]
	.loc 1 3636 23 discriminator 1
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #12]
	.loc 1 3635 25 discriminator 1
	cmp	r2, r3
	bcs	.L552
	.loc 1 3637 17
	ldr	r3, [sp, #16]
	.loc 1 3637 5
	ldrb	r0, [r3, #1]	@ zero_extendqisi2
	.loc 1 3637 26
	ldr	r3, [sp, #12]
	add	r1, r3, #40
	.loc 1 3637 5
	movs	r3, #1
	ldr	r2, [sp, #20]
	bl	disk_read
	mov	r3, r0
	.loc 1 3636 32
	cmp	r3, #0
	beq	.L552
	.loc 1 3638 6
	ldr	r3, [sp, #12]
	movs	r2, #1
	strb	r2, [r3, #17]
	movs	r3, #1
	b	.L555
.L552:
	.loc 1 3641 13
	ldr	r3, [sp, #12]
	ldr	r2, [sp, #20]
	str	r2, [r3, #28]
.L536:
	.loc 1 3643 27
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #20]
	.loc 1 3643 34
	ubfx	r3, r3, #0, #9
	.loc 1 3643 8
	rsb	r3, r3, #512
	str	r3, [sp, #36]
	.loc 1 3644 6
	ldr	r2, [sp, #36]
	ldr	r3, [sp, #4]
	cmp	r2, r3
	bls	.L553
	.loc 1 3644 24 discriminator 1
	ldr	r3, [sp, #4]
	str	r3, [sp, #36]
.L553:
	.loc 1 3650 11
	ldr	r3, [sp, #12]
	add	r2, r3, #40
	.loc 1 3650 23
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #20]
	.loc 1 3650 30
	ubfx	r3, r3, #0, #9
	.loc 1 3650 3
	add	r3, r3, r2
	ldr	r2, [sp, #36]
	ldr	r1, [sp, #28]
	mov	r0, r3
	bl	mem_cpy
	.loc 1 3651 12
	ldr	r3, [sp, #12]
	ldrb	r3, [r3, #16]	@ zero_extendqisi2
	orn	r3, r3, #127
	uxtb	r2, r3
	ldr	r3, [sp, #12]
	strb	r2, [r3, #16]
.L551:
	.loc 1 3571 9
	ldr	r2, [sp, #28]
	ldr	r3, [sp, #36]
	add	r3, r3, r2
	str	r3, [sp, #28]
	.loc 1 3571 27
	ldr	r3, [sp, #12]
	ldr	r2, [r3, #20]
	ldr	r3, [sp, #36]
	add	r2, r2, r3
	ldr	r3, [sp, #12]
	str	r2, [r3, #20]
	.loc 1 3571 73
	ldr	r3, [sp, #12]
	ldr	r2, [r3, #12]
	.loc 1 3571 57
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #20]
	.loc 1 3571 94
	cmp	r2, r3
	it	cc
	movcc	r2, r3
	.loc 1 3571 52
	ldr	r3, [sp, #12]
	str	r2, [r3, #12]
	.loc 1 3571 117
	ldr	r3, [sp]
	ldr	r2, [r3]
	ldr	r3, [sp, #36]
	add	r2, r2, r3
	ldr	r3, [sp]
	str	r2, [r3]
	.loc 1 3571 130
	ldr	r2, [sp, #4]
	ldr	r3, [sp, #36]
	subs	r3, r2, r3
	str	r3, [sp, #4]
.L535:
	.loc 1 3570 2
	ldr	r3, [sp, #4]
	cmp	r3, #0
	bne	.L554
	b	.L541
.L556:
	.loc 1 3590 5
	nop
.L541:
	.loc 1 3655 11
	ldr	r3, [sp, #12]
	ldrb	r3, [r3, #16]	@ zero_extendqisi2
	orr	r3, r3, #64
	uxtb	r2, r3
	ldr	r3, [sp, #12]
	strb	r2, [r3, #16]
	.loc 1 3657 2
	movs	r3, #0
.L555:
	.loc 1 3658 1 discriminator 1
	mov	r0, r3
	add	sp, sp, #52
.LCFI111:
	@ sp needed
	ldr	pc, [sp], #4
.LFE40:
	.size	f_write, .-f_write
	.section	.text.f_sync,"ax",%progbits
	.align	1
	.global	f_sync
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	f_sync, %function
f_sync:
.LFB41:
	.loc 1 3670 1
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI112:
	sub	sp, sp, #28
.LCFI113:
	str	r0, [sp, #4]
	.loc 1 3678 8
	ldr	r3, [sp, #4]
	add	r2, sp, #8
	mov	r1, r2
	mov	r0, r3
	bl	validate
	mov	r3, r0
	strb	r3, [sp, #23]
	.loc 1 3679 5
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L558
	.loc 1 3680 9
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #16]	@ zero_extendqisi2
	.loc 1 3680 16
	and	r3, r3, #64
	.loc 1 3680 6
	cmp	r3, #0
	beq	.L558
	.loc 1 3682 10
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #16]	@ zero_extendqisi2
	.loc 1 3682 8
	sxtb	r3, r3
	.loc 1 3682 7
	cmp	r3, #0
	bge	.L559
	.loc 1 3683 22
	ldr	r3, [sp, #8]
	.loc 1 3683 9
	ldrb	r0, [r3, #1]	@ zero_extendqisi2
	.loc 1 3683 31
	ldr	r3, [sp, #4]
	add	r1, r3, #40
	.loc 1 3683 9
	ldr	r3, [sp, #4]
	ldr	r2, [r3, #28]
	movs	r3, #1
	bl	disk_write
	mov	r3, r0
	.loc 1 3683 8
	cmp	r3, #0
	beq	.L560
	.loc 1 3683 62 discriminator 1
	movs	r3, #1
	b	.L562
.L560:
	.loc 1 3684 14
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #16]	@ zero_extendqisi2
	and	r3, r3, #127
	uxtb	r2, r3
	ldr	r3, [sp, #4]
	strb	r2, [r3, #16]
.L559:
	.loc 1 3688 7
	ldr	r3, .L563
	str	r3, [sp, #16]
	.loc 1 3717 11
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #32]
	mov	r1, r3
	mov	r0, r2
	bl	move_window
	mov	r3, r0
	strb	r3, [sp, #23]
	.loc 1 3718 8
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L558
	.loc 1 3719 10
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #36]
	str	r3, [sp, #12]
	.loc 1 3720 20
	ldr	r3, [sp, #12]
	adds	r3, r3, #11
	ldrb	r2, [r3]	@ zero_extendqisi2
	ldr	r3, [sp, #12]
	adds	r3, r3, #11
	orr	r2, r2, #32
	uxtb	r2, r2
	strb	r2, [r3]
	.loc 1 3721 6
	ldr	r3, [sp, #4]
	ldr	r0, [r3]
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #8]
	mov	r2, r3
	ldr	r1, [sp, #12]
	bl	st_clust
	.loc 1 3722 6
	ldr	r3, [sp, #12]
	add	r2, r3, #28
	.loc 1 3722 49
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #12]
	.loc 1 3722 6
	mov	r1, r3
	mov	r0, r2
	bl	st_dword
	.loc 1 3723 6
	ldr	r3, [sp, #12]
	adds	r3, r3, #22
	ldr	r1, [sp, #16]
	mov	r0, r3
	bl	st_dword
	.loc 1 3724 6
	ldr	r3, [sp, #12]
	adds	r3, r3, #18
	movs	r1, #0
	mov	r0, r3
	bl	st_word
	.loc 1 3725 8
	ldr	r3, [sp, #8]
	.loc 1 3725 16
	movs	r2, #1
	strb	r2, [r3, #3]
	.loc 1 3726 12
	ldr	r3, [sp, #8]
	mov	r0, r3
	bl	sync_fs
	mov	r3, r0
	strb	r3, [sp, #23]
	.loc 1 3727 15
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #16]	@ zero_extendqisi2
	bic	r3, r3, #64
	uxtb	r2, r3
	ldr	r3, [sp, #4]
	strb	r2, [r3, #16]
.L558:
	.loc 1 3733 2
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
.L562:
	.loc 1 3734 1 discriminator 1
	mov	r0, r3
	add	sp, sp, #28
.LCFI114:
	@ sp needed
	ldr	pc, [sp], #4
.L564:
	.align	2
.L563:
	.word	1210122240
.LFE41:
	.size	f_sync, .-f_sync
	.section	.text.f_close,"ax",%progbits
	.align	1
	.global	f_close
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	f_close, %function
f_close:
.LFB42:
	.loc 1 3748 1
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI115:
	sub	sp, sp, #20
.LCFI116:
	str	r0, [sp, #4]
	.loc 1 3753 8
	ldr	r0, [sp, #4]
	bl	f_sync
	mov	r3, r0
	strb	r3, [sp, #15]
	.loc 1 3754 5
	ldrb	r3, [sp, #15]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L566
	.loc 1 3757 9
	ldr	r3, [sp, #4]
	add	r2, sp, #8
	mov	r1, r2
	mov	r0, r3
	bl	validate
	mov	r3, r0
	strb	r3, [sp, #15]
	.loc 1 3758 6
	ldrb	r3, [sp, #15]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L566
	.loc 1 3764 16
	ldr	r3, [sp, #4]
	movs	r2, #0
	str	r2, [r3]
.L566:
	.loc 1 3771 9
	ldrb	r3, [sp, #15]	@ zero_extendqisi2
	.loc 1 3772 1
	mov	r0, r3
	add	sp, sp, #20
.LCFI117:
	@ sp needed
	ldr	pc, [sp], #4
.LFE42:
	.size	f_close, .-f_close
	.section	.text.f_chdir,"ax",%progbits
	.align	1
	.global	f_chdir
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	f_chdir, %function
f_chdir:
.LFB43:
	.loc 1 3804 1
	@ args = 0, pretend = 0, frame = 64
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
.LCFI118:
	sub	sp, sp, #64
.LCFI119:
	str	r0, [sp, #4]
	.loc 1 3811 8
	add	r1, sp, #8
	add	r3, sp, #4
	movs	r2, #0
	mov	r0, r3
	bl	find_volume
	mov	r3, r0
	strb	r3, [sp, #63]
	.loc 1 3812 5
	ldrb	r3, [sp, #63]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L569
	.loc 1 3813 13
	ldr	r3, [sp, #8]
	str	r3, [sp, #12]
	.loc 1 3815 9
	ldr	r2, [sp, #4]
	add	r3, sp, #12
	mov	r1, r2
	mov	r0, r3
	bl	follow_path
	mov	r3, r0
	strb	r3, [sp, #63]
	.loc 1 3816 6
	ldrb	r3, [sp, #63]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L570
	.loc 1 3817 13
	ldrb	r3, [sp, #55]	@ zero_extendqisi2
	.loc 1 3817 8
	sxtb	r3, r3
	.loc 1 3817 7
	cmp	r3, #0
	bge	.L571
	.loc 1 3818 7
	ldr	r3, [sp, #8]
	.loc 1 3818 22
	ldr	r2, [sp, #20]
	.loc 1 3818 14
	str	r2, [r3, #24]
	b	.L570
.L571:
	.loc 1 3827 15
	ldrb	r3, [sp, #18]	@ zero_extendqisi2
	.loc 1 3827 21
	and	r3, r3, #16
	.loc 1 3827 8
	cmp	r3, #0
	beq	.L572
	.loc 1 3837 18
	ldr	r3, [sp, #8]
	.loc 1 3837 33
	ldr	r2, [sp, #40]
	.loc 1 3837 9
	ldr	r4, [sp, #8]
	.loc 1 3837 18
	mov	r1, r2
	mov	r0, r3
	bl	ld_clust
	mov	r3, r0
	.loc 1 3837 16
	str	r3, [r4, #24]
	b	.L570
.L572:
	.loc 1 3840 10
	movs	r3, #5
	strb	r3, [sp, #63]
.L570:
	.loc 1 3845 6
	ldrb	r3, [sp, #63]	@ zero_extendqisi2
	cmp	r3, #4
	bne	.L569
	.loc 1 3845 30 discriminator 1
	movs	r3, #5
	strb	r3, [sp, #63]
.L569:
	.loc 1 3848 2
	ldrb	r3, [sp, #63]	@ zero_extendqisi2
	.loc 1 3849 1
	mov	r0, r3
	add	sp, sp, #64
.LCFI120:
	@ sp needed
	pop	{r4, pc}
.LFE43:
	.size	f_chdir, .-f_chdir
	.section	.text.f_lseek,"ax",%progbits
	.align	1
	.global	f_lseek
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	f_lseek, %function
f_lseek:
.LFB44:
	.loc 1 3937 1
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI121:
	sub	sp, sp, #36
.LCFI122:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 3946 8
	ldr	r3, [sp, #4]
	add	r2, sp, #8
	mov	r1, r2
	mov	r0, r3
	bl	validate
	mov	r3, r0
	strb	r3, [sp, #31]
	.loc 1 3947 5
	ldrb	r3, [sp, #31]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L575
	.loc 1 3947 27 discriminator 2
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #17]
	strb	r3, [sp, #31]
	.loc 1 3947 19 discriminator 2
	ldrb	r3, [sp, #31]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L576
.L575:
	.loc 1 3947 57 discriminator 3
	ldrb	r3, [sp, #31]	@ zero_extendqisi2
	b	.L599
.L576:
	.loc 1 4005 20
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #12]
	.loc 1 4005 6
	ldr	r2, [sp]
	cmp	r2, r3
	bls	.L578
	.loc 1 4005 53 discriminator 1
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #16]	@ zero_extendqisi2
	.loc 1 4005 60 discriminator 1
	and	r3, r3, #2
	.loc 1 4005 29 discriminator 1
	cmp	r3, #0
	bne	.L578
	.loc 1 4006 8
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #12]
	str	r3, [sp]
.L578:
	.loc 1 4008 9
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #20]
	str	r3, [sp, #16]
	.loc 1 4009 20
	movs	r3, #0
	str	r3, [sp, #20]
	.loc 1 4009 12
	ldr	r3, [sp, #4]
	ldr	r2, [sp, #20]
	str	r2, [r3, #20]
	.loc 1 4010 6
	ldr	r3, [sp]
	cmp	r3, #0
	beq	.L579
	.loc 1 4011 19
	ldr	r3, [sp, #8]
	ldrh	r3, [r3, #10]
	.loc 1 4011 8
	lsls	r3, r3, #9
	str	r3, [sp, #12]
	.loc 1 4012 7
	ldr	r3, [sp, #16]
	cmp	r3, #0
	beq	.L580
	.loc 1 4013 10 discriminator 1
	ldr	r3, [sp]
	subs	r2, r3, #1
	.loc 1 4013 15 discriminator 1
	ldr	r3, [sp, #12]
	udiv	r2, r2, r3
	.loc 1 4013 31 discriminator 1
	ldr	r3, [sp, #16]
	subs	r1, r3, #1
	.loc 1 4013 36 discriminator 1
	ldr	r3, [sp, #12]
	udiv	r3, r1, r3
	.loc 1 4012 18 discriminator 1
	cmp	r2, r3
	bcc	.L580
	.loc 1 4014 23
	ldr	r3, [sp, #16]
	subs	r2, r3, #1
	.loc 1 4014 30
	ldr	r3, [sp, #12]
	rsbs	r3, r3, #0
	.loc 1 4014 28
	ands	r2, r2, r3
	.loc 1 4014 14
	ldr	r3, [sp, #4]
	str	r2, [r3, #20]
	.loc 1 4015 14
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #20]
	.loc 1 4015 9
	ldr	r2, [sp]
	subs	r3, r2, r3
	str	r3, [sp]
	.loc 1 4016 10
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #24]
	str	r3, [sp, #24]
	b	.L581
.L580:
	.loc 1 4018 10
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #8]
	str	r3, [sp, #24]
	.loc 1 4020 8
	ldr	r3, [sp, #24]
	cmp	r3, #0
	bne	.L582
	.loc 1 4021 13
	ldr	r3, [sp, #4]
	movs	r1, #0
	mov	r0, r3
	bl	create_chain
	str	r0, [sp, #24]
	.loc 1 4022 9
	ldr	r3, [sp, #24]
	cmp	r3, #1
	bne	.L583
	.loc 1 4022 21 discriminator 1
	ldr	r3, [sp, #4]
	movs	r2, #2
	strb	r2, [r3, #17]
	movs	r3, #2
	b	.L599
.L583:
	.loc 1 4023 9
	ldr	r3, [sp, #24]
	cmp	r3, #-1
	bne	.L584
	.loc 1 4023 30 discriminator 1
	ldr	r3, [sp, #4]
	movs	r2, #1
	strb	r2, [r3, #17]
	movs	r3, #1
	b	.L599
.L584:
	.loc 1 4024 21
	ldr	r3, [sp, #4]
	ldr	r2, [sp, #24]
	str	r2, [r3, #8]
.L582:
	.loc 1 4027 15
	ldr	r3, [sp, #4]
	ldr	r2, [sp, #24]
	str	r2, [r3, #24]
.L581:
	.loc 1 4029 7
	ldr	r3, [sp, #24]
	cmp	r3, #0
	beq	.L579
	.loc 1 4030 11
	b	.L585
.L592:
	.loc 1 4031 10
	ldr	r2, [sp]
	ldr	r3, [sp, #12]
	subs	r3, r2, r3
	str	r3, [sp]
	.loc 1 4031 27
	ldr	r3, [sp, #4]
	ldr	r2, [r3, #20]
	ldr	r3, [sp, #12]
	add	r2, r2, r3
	ldr	r3, [sp, #4]
	str	r2, [r3, #20]
	.loc 1 4033 12
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #16]	@ zero_extendqisi2
	.loc 1 4033 19
	and	r3, r3, #2
	.loc 1 4033 9
	cmp	r3, #0
	beq	.L586
	.loc 1 4038 14
	ldr	r3, [sp, #4]
	ldr	r1, [sp, #24]
	mov	r0, r3
	bl	create_chain
	str	r0, [sp, #24]
	.loc 1 4039 10
	ldr	r3, [sp, #24]
	cmp	r3, #0
	bne	.L587
	.loc 1 4040 12
	movs	r3, #0
	str	r3, [sp]
	.loc 1 4040 8
	b	.L588
.L586:
	.loc 1 4045 14
	ldr	r3, [sp, #4]
	ldr	r1, [sp, #24]
	mov	r0, r3
	bl	get_fat
	str	r0, [sp, #24]
.L587:
	.loc 1 4047 9
	ldr	r3, [sp, #24]
	cmp	r3, #-1
	bne	.L589
	.loc 1 4047 30 discriminator 1
	ldr	r3, [sp, #4]
	movs	r2, #1
	strb	r2, [r3, #17]
	movs	r3, #1
	b	.L599
.L589:
	.loc 1 4048 9
	ldr	r3, [sp, #24]
	cmp	r3, #1
	bls	.L590
	.loc 1 4048 33 discriminator 2
	ldr	r3, [sp, #8]
	ldr	r3, [r3, #28]
	.loc 1 4048 20 discriminator 2
	ldr	r2, [sp, #24]
	cmp	r2, r3
	bcc	.L591
.L590:
	.loc 1 4048 45 discriminator 3
	ldr	r3, [sp, #4]
	movs	r2, #2
	strb	r2, [r3, #17]
	movs	r3, #2
	b	.L599
.L591:
	.loc 1 4049 16
	ldr	r3, [sp, #4]
	ldr	r2, [sp, #24]
	str	r2, [r3, #24]
.L585:
	.loc 1 4030 11
	ldr	r2, [sp]
	ldr	r3, [sp, #12]
	cmp	r2, r3
	bhi	.L592
.L588:
	.loc 1 4051 14
	ldr	r3, [sp, #4]
	ldr	r2, [r3, #20]
	ldr	r3, [sp]
	add	r2, r2, r3
	ldr	r3, [sp, #4]
	str	r2, [r3, #20]
	.loc 1 4052 13
	ldr	r3, [sp]
	ubfx	r3, r3, #0, #9
	.loc 1 4052 8
	cmp	r3, #0
	beq	.L579
	.loc 1 4053 14
	ldr	r3, [sp, #8]
	ldr	r1, [sp, #24]
	mov	r0, r3
	bl	clust2sect
	str	r0, [sp, #20]
	.loc 1 4054 9
	ldr	r3, [sp, #20]
	cmp	r3, #0
	bne	.L593
	.loc 1 4054 18 discriminator 1
	ldr	r3, [sp, #4]
	movs	r2, #2
	strb	r2, [r3, #17]
	movs	r3, #2
	b	.L599
.L593:
	.loc 1 4055 15
	ldr	r3, [sp]
	lsrs	r3, r3, #9
	.loc 1 4055 12
	ldr	r2, [sp, #20]
	add	r3, r3, r2
	str	r3, [sp, #20]
.L579:
	.loc 1 4059 26
	ldr	r3, [sp, #4]
	ldr	r2, [r3, #20]
	.loc 1 4059 42
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #12]
	.loc 1 4059 6
	cmp	r2, r3
	bls	.L594
	.loc 1 4060 24
	ldr	r3, [sp, #4]
	ldr	r2, [r3, #20]
	.loc 1 4060 20
	ldr	r3, [sp, #4]
	str	r2, [r3, #12]
	.loc 1 4061 13
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #16]	@ zero_extendqisi2
	orr	r3, r3, #64
	uxtb	r2, r3
	ldr	r3, [sp, #4]
	strb	r2, [r3, #16]
.L594:
	.loc 1 4063 9
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #20]
	.loc 1 4063 16
	ubfx	r3, r3, #0, #9
	.loc 1 4063 6
	cmp	r3, #0
	beq	.L595
	.loc 1 4063 39 discriminator 1
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #28]
	.loc 1 4063 25 discriminator 1
	ldr	r2, [sp, #20]
	cmp	r2, r3
	beq	.L595
	.loc 1 4066 10
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #16]	@ zero_extendqisi2
	.loc 1 4066 8
	sxtb	r3, r3
	.loc 1 4066 7
	cmp	r3, #0
	bge	.L596
	.loc 1 4067 22
	ldr	r3, [sp, #8]
	.loc 1 4067 9
	ldrb	r0, [r3, #1]	@ zero_extendqisi2
	.loc 1 4067 31
	ldr	r3, [sp, #4]
	add	r1, r3, #40
	.loc 1 4067 9
	ldr	r3, [sp, #4]
	ldr	r2, [r3, #28]
	movs	r3, #1
	bl	disk_write
	mov	r3, r0
	.loc 1 4067 8
	cmp	r3, #0
	beq	.L597
	.loc 1 4067 62 discriminator 1
	ldr	r3, [sp, #4]
	movs	r2, #1
	strb	r2, [r3, #17]
	movs	r3, #1
	b	.L599
.L597:
	.loc 1 4068 14
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #16]	@ zero_extendqisi2
	and	r3, r3, #127
	uxtb	r2, r3
	ldr	r3, [sp, #4]
	strb	r2, [r3, #16]
.L596:
	.loc 1 4071 20
	ldr	r3, [sp, #8]
	.loc 1 4071 8
	ldrb	r0, [r3, #1]	@ zero_extendqisi2
	.loc 1 4071 29
	ldr	r3, [sp, #4]
	add	r1, r3, #40
	.loc 1 4071 8
	movs	r3, #1
	ldr	r2, [sp, #20]
	bl	disk_read
	mov	r3, r0
	.loc 1 4071 7
	cmp	r3, #0
	beq	.L598
	.loc 1 4071 57 discriminator 1
	ldr	r3, [sp, #4]
	movs	r2, #1
	strb	r2, [r3, #17]
	movs	r3, #1
	b	.L599
.L598:
	.loc 1 4073 13
	ldr	r3, [sp, #4]
	ldr	r2, [sp, #20]
	str	r2, [r3, #28]
.L595:
	.loc 1 4077 2
	ldrb	r3, [sp, #31]	@ zero_extendqisi2
.L599:
	.loc 1 4078 1 discriminator 1
	mov	r0, r3
	add	sp, sp, #36
.LCFI123:
	@ sp needed
	ldr	pc, [sp], #4
.LFE44:
	.size	f_lseek, .-f_lseek
	.section	.text.f_opendir,"ax",%progbits
	.align	1
	.global	f_opendir
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	f_opendir, %function
f_opendir:
.LFB45:
	.loc 1 4091 1
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI124:
	sub	sp, sp, #28
.LCFI125:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 4098 5
	ldr	r3, [sp, #4]
	cmp	r3, #0
	bne	.L601
	.loc 1 4098 18 discriminator 1
	movs	r3, #9
	b	.L608
.L601:
	.loc 1 4101 6
	ldr	r3, [sp, #4]
	str	r3, [sp, #16]
	.loc 1 4102 8
	add	r1, sp, #12
	mov	r3, sp
	movs	r2, #0
	mov	r0, r3
	bl	find_volume
	mov	r3, r0
	strb	r3, [sp, #23]
	.loc 1 4103 5
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L603
	.loc 1 4104 11
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #16]
	str	r2, [r3]
	.loc 1 4106 9
	ldr	r3, [sp]
	mov	r1, r3
	ldr	r0, [sp, #4]
	bl	follow_path
	mov	r3, r0
	strb	r3, [sp, #23]
	.loc 1 4107 6
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L604
	.loc 1 4108 16
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #43]	@ zero_extendqisi2
	.loc 1 4108 8
	sxtb	r3, r3
	.loc 1 4108 7
	cmp	r3, #0
	blt	.L605
	.loc 1 4109 12
	ldr	r3, [sp, #16]
	ldrb	r3, [r3, #6]	@ zero_extendqisi2
	.loc 1 4109 19
	and	r3, r3, #16
	.loc 1 4109 8
	cmp	r3, #0
	beq	.L606
	.loc 1 4121 21
	ldr	r2, [sp, #12]
	.loc 1 4121 36
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #28]
	.loc 1 4121 21
	mov	r1, r3
	mov	r0, r2
	bl	ld_clust
	mov	r2, r0
	.loc 1 4121 19
	ldr	r3, [sp, #16]
	str	r2, [r3, #8]
	b	.L605
.L606:
	.loc 1 4124 10
	movs	r3, #5
	strb	r3, [sp, #23]
.L605:
	.loc 1 4127 7
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L604
	.loc 1 4128 17
	ldr	r3, [sp, #12]
	ldrh	r2, [r3, #6]
	.loc 1 4128 13
	ldr	r3, [sp, #16]
	strh	r2, [r3, #4]	@ movhi
	.loc 1 4129 11
	movs	r1, #0
	ldr	r0, [sp, #4]
	bl	dir_sdi
	mov	r3, r0
	strb	r3, [sp, #23]
.L604:
	.loc 1 4143 6
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #4
	bne	.L603
	.loc 1 4143 30 discriminator 1
	movs	r3, #5
	strb	r3, [sp, #23]
.L603:
	.loc 1 4145 5
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L607
	.loc 1 4145 28 discriminator 1
	ldr	r3, [sp, #16]
	movs	r2, #0
	str	r2, [r3]
.L607:
	.loc 1 4147 2
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
.L608:
	.loc 1 4148 1 discriminator 1
	mov	r0, r3
	add	sp, sp, #28
.LCFI126:
	@ sp needed
	ldr	pc, [sp], #4
.LFE45:
	.size	f_opendir, .-f_opendir
	.section	.text.f_closedir,"ax",%progbits
	.align	1
	.global	f_closedir
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	f_closedir, %function
f_closedir:
.LFB46:
	.loc 1 4160 1
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI127:
	sub	sp, sp, #20
.LCFI128:
	str	r0, [sp, #4]
	.loc 1 4165 8
	ldr	r3, [sp, #4]
	add	r2, sp, #8
	mov	r1, r2
	mov	r0, r3
	bl	validate
	mov	r3, r0
	strb	r3, [sp, #15]
	.loc 1 4166 5
	ldrb	r3, [sp, #15]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L610
	.loc 1 4174 15
	ldr	r3, [sp, #4]
	movs	r2, #0
	str	r2, [r3]
.L610:
	.loc 1 4180 9
	ldrb	r3, [sp, #15]	@ zero_extendqisi2
	.loc 1 4181 1
	mov	r0, r3
	add	sp, sp, #20
.LCFI129:
	@ sp needed
	ldr	pc, [sp], #4
.LFE46:
	.size	f_closedir, .-f_closedir
	.section	.text.f_readdir,"ax",%progbits
	.align	1
	.global	f_readdir
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	f_readdir, %function
f_readdir:
.LFB47:
	.loc 1 4194 1
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI130:
	sub	sp, sp, #20
.LCFI131:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 4200 8
	ldr	r3, [sp, #4]
	add	r2, sp, #8
	mov	r1, r2
	mov	r0, r3
	bl	validate
	mov	r3, r0
	strb	r3, [sp, #15]
	.loc 1 4201 5
	ldrb	r3, [sp, #15]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L613
	.loc 1 4202 6
	ldr	r3, [sp]
	cmp	r3, #0
	bne	.L614
	.loc 1 4203 10
	movs	r1, #0
	ldr	r0, [sp, #4]
	bl	dir_sdi
	mov	r3, r0
	strb	r3, [sp, #15]
	b	.L613
.L614:
	.loc 1 4206 10
	movs	r1, #0
	ldr	r0, [sp, #4]
	bl	dir_read
	mov	r3, r0
	strb	r3, [sp, #15]
	.loc 1 4207 7
	ldrb	r3, [sp, #15]	@ zero_extendqisi2
	cmp	r3, #4
	bne	.L615
	.loc 1 4207 31 discriminator 1
	movs	r3, #0
	strb	r3, [sp, #15]
.L615:
	.loc 1 4208 7
	ldrb	r3, [sp, #15]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L613
	.loc 1 4209 5
	ldr	r1, [sp]
	ldr	r0, [sp, #4]
	bl	get_fileinfo
	.loc 1 4210 11
	movs	r1, #0
	ldr	r0, [sp, #4]
	bl	dir_next
	mov	r3, r0
	strb	r3, [sp, #15]
	.loc 1 4211 8
	ldrb	r3, [sp, #15]	@ zero_extendqisi2
	cmp	r3, #4
	bne	.L613
	.loc 1 4211 32 discriminator 1
	movs	r3, #0
	strb	r3, [sp, #15]
.L613:
	.loc 1 4216 2
	ldrb	r3, [sp, #15]	@ zero_extendqisi2
	.loc 1 4217 1
	mov	r0, r3
	add	sp, sp, #20
.LCFI132:
	@ sp needed
	ldr	pc, [sp], #4
.LFE47:
	.size	f_readdir, .-f_readdir
	.section	.text.f_stat,"ax",%progbits
	.align	1
	.global	f_stat
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	f_stat, %function
f_stat:
.LFB48:
	.loc 1 4282 1
	@ args = 0, pretend = 0, frame = 64
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI133:
	sub	sp, sp, #68
.LCFI134:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 4289 8
	add	r1, sp, #12
	add	r3, sp, #4
	movs	r2, #0
	mov	r0, r3
	bl	find_volume
	mov	r3, r0
	strb	r3, [sp, #63]
	.loc 1 4290 5
	ldrb	r3, [sp, #63]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L618
	.loc 1 4292 9
	ldr	r2, [sp, #4]
	add	r3, sp, #12
	mov	r1, r2
	mov	r0, r3
	bl	follow_path
	mov	r3, r0
	strb	r3, [sp, #63]
	.loc 1 4293 6
	ldrb	r3, [sp, #63]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L618
	.loc 1 4294 13
	ldrb	r3, [sp, #55]	@ zero_extendqisi2
	.loc 1 4294 8
	sxtb	r3, r3
	.loc 1 4294 7
	cmp	r3, #0
	bge	.L619
	.loc 1 4295 9
	movs	r3, #6
	strb	r3, [sp, #63]
	b	.L618
.L619:
	.loc 1 4297 8
	ldr	r3, [sp]
	cmp	r3, #0
	beq	.L618
	.loc 1 4297 14 discriminator 1
	add	r3, sp, #12
	ldr	r1, [sp]
	mov	r0, r3
	bl	get_fileinfo
.L618:
	.loc 1 4303 2
	ldrb	r3, [sp, #63]	@ zero_extendqisi2
	.loc 1 4304 1
	mov	r0, r3
	add	sp, sp, #68
.LCFI135:
	@ sp needed
	ldr	pc, [sp], #4
.LFE48:
	.size	f_stat, .-f_stat
	.section	.text.f_getfree,"ax",%progbits
	.align	1
	.global	f_getfree
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	f_getfree, %function
f_getfree:
.LFB49:
	.loc 1 4318 1
	@ args = 0, pretend = 0, frame = 64
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI136:
	sub	sp, sp, #68
.LCFI137:
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	str	r2, [sp, #4]
	.loc 1 4328 8
	add	r1, sp, #32
	add	r3, sp, #12
	movs	r2, #0
	mov	r0, r3
	bl	find_volume
	mov	r3, r0
	strb	r3, [sp, #63]
	.loc 1 4329 5
	ldrb	r3, [sp, #63]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L622
	.loc 1 4330 10
	ldr	r2, [sp, #32]
	ldr	r3, [sp, #4]
	str	r2, [r3]
	.loc 1 4332 9
	ldr	r3, [sp, #32]
	ldr	r2, [r3, #20]
	.loc 1 4332 26
	ldr	r3, [sp, #32]
	ldr	r3, [r3, #28]
	.loc 1 4332 37
	subs	r3, r3, #2
	.loc 1 4332 6
	cmp	r2, r3
	bhi	.L623
	.loc 1 4333 15
	ldr	r3, [sp, #32]
	ldr	r2, [r3, #20]
	.loc 1 4333 11
	ldr	r3, [sp, #8]
	str	r2, [r3]
	b	.L622
.L623:
	.loc 1 4336 10
	movs	r3, #0
	str	r3, [sp, #56]
	.loc 1 4337 10
	ldr	r3, [sp, #32]
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 4337 7
	cmp	r3, #1
	bne	.L624
	.loc 1 4338 10
	movs	r3, #2
	str	r3, [sp, #52]
	.loc 1 4338 22
	ldr	r3, [sp, #32]
	str	r3, [sp, #16]
.L629:
	.loc 1 4340 13
	add	r3, sp, #16
	ldr	r1, [sp, #52]
	mov	r0, r3
	bl	get_fat
	str	r0, [sp, #36]
	.loc 1 4341 9
	ldr	r3, [sp, #36]
	cmp	r3, #-1
	bne	.L625
	.loc 1 4341 36 discriminator 1
	movs	r3, #1
	strb	r3, [sp, #63]
	.loc 1 4341 6 discriminator 1
	b	.L630
.L625:
	.loc 1 4342 9
	ldr	r3, [sp, #36]
	cmp	r3, #1
	bne	.L627
	.loc 1 4342 27 discriminator 1
	movs	r3, #2
	strb	r3, [sp, #63]
	.loc 1 4342 6 discriminator 1
	b	.L630
.L627:
	.loc 1 4343 9
	ldr	r3, [sp, #36]
	cmp	r3, #0
	bne	.L628
	.loc 1 4343 26 discriminator 1
	ldr	r3, [sp, #56]
	adds	r3, r3, #1
	str	r3, [sp, #56]
.L628:
	.loc 1 4344 5
	ldr	r3, [sp, #52]
	adds	r3, r3, #1
	str	r3, [sp, #52]
	.loc 1 4344 25
	ldr	r3, [sp, #32]
	ldr	r3, [r3, #28]
	.loc 1 4344 5
	ldr	r2, [sp, #52]
	cmp	r2, r3
	bcc	.L629
	b	.L630
.L624:
	.loc 1 4365 15
	ldr	r3, [sp, #32]
	.loc 1 4365 11
	ldr	r3, [r3, #28]
	str	r3, [sp, #52]
	.loc 1 4365 36
	ldr	r3, [sp, #32]
	.loc 1 4365 32
	ldr	r3, [r3, #40]
	str	r3, [sp, #48]
	.loc 1 4366 8
	movs	r3, #0
	str	r3, [sp, #44]
	.loc 1 4366 15
	movs	r3, #0
	str	r3, [sp, #40]
.L637:
	.loc 1 4368 10
	ldr	r3, [sp, #44]
	cmp	r3, #0
	bne	.L631
	.loc 1 4369 14
	ldr	r0, [sp, #32]
	ldr	r3, [sp, #48]
	adds	r2, r3, #1
	str	r2, [sp, #48]
	mov	r1, r3
	bl	move_window
	mov	r3, r0
	strb	r3, [sp, #63]
	.loc 1 4370 11
	ldrb	r3, [sp, #63]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L639
	.loc 1 4371 14
	ldr	r3, [sp, #32]
	.loc 1 4371 10
	adds	r3, r3, #56
	str	r3, [sp, #40]
	.loc 1 4372 10
	mov	r3, #512
	str	r3, [sp, #44]
.L631:
	.loc 1 4374 13
	ldr	r3, [sp, #32]
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 4374 10
	cmp	r3, #2
	bne	.L633
	.loc 1 4375 12
	ldr	r0, [sp, #40]
	bl	ld_word
	mov	r3, r0
	.loc 1 4375 11
	cmp	r3, #0
	bne	.L634
	.loc 1 4375 34 discriminator 1
	ldr	r3, [sp, #56]
	adds	r3, r3, #1
	str	r3, [sp, #56]
.L634:
	.loc 1 4376 10
	ldr	r3, [sp, #40]
	adds	r3, r3, #2
	str	r3, [sp, #40]
	.loc 1 4376 18
	ldr	r3, [sp, #44]
	subs	r3, r3, #2
	str	r3, [sp, #44]
	b	.L635
.L633:
	.loc 1 4378 13
	ldr	r0, [sp, #40]
	bl	ld_dword
	mov	r3, r0
	.loc 1 4378 25
	bic	r3, r3, #-268435456
	.loc 1 4378 11
	cmp	r3, #0
	bne	.L636
	.loc 1 4378 50 discriminator 1
	ldr	r3, [sp, #56]
	adds	r3, r3, #1
	str	r3, [sp, #56]
.L636:
	.loc 1 4379 10
	ldr	r3, [sp, #40]
	adds	r3, r3, #4
	str	r3, [sp, #40]
	.loc 1 4379 18
	ldr	r3, [sp, #44]
	subs	r3, r3, #4
	str	r3, [sp, #44]
.L635:
	.loc 1 4381 6
	ldr	r3, [sp, #52]
	subs	r3, r3, #1
	str	r3, [sp, #52]
	ldr	r3, [sp, #52]
	cmp	r3, #0
	bne	.L637
	b	.L630
.L639:
	.loc 1 4370 8
	nop
.L630:
	.loc 1 4384 11
	ldr	r3, [sp, #8]
	ldr	r2, [sp, #56]
	str	r2, [r3]
	.loc 1 4385 6
	ldr	r3, [sp, #32]
	.loc 1 4385 18
	ldr	r2, [sp, #56]
	str	r2, [r3, #20]
	.loc 1 4386 17
	ldr	r3, [sp, #32]
	ldrb	r2, [r3, #4]	@ zero_extendqisi2
	ldr	r3, [sp, #32]
	orr	r2, r2, #1
	uxtb	r2, r2
	strb	r2, [r3, #4]
.L622:
	.loc 1 4390 2
	ldrb	r3, [sp, #63]	@ zero_extendqisi2
	.loc 1 4391 1
	mov	r0, r3
	add	sp, sp, #68
.LCFI138:
	@ sp needed
	ldr	pc, [sp], #4
.LFE49:
	.size	f_getfree, .-f_getfree
	.section	.text.f_truncate,"ax",%progbits
	.align	1
	.global	f_truncate
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	f_truncate, %function
f_truncate:
.LFB50:
	.loc 1 4403 1
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI139:
	sub	sp, sp, #28
.LCFI140:
	str	r0, [sp, #4]
	.loc 1 4409 8
	ldr	r3, [sp, #4]
	add	r2, sp, #12
	mov	r1, r2
	mov	r0, r3
	bl	validate
	mov	r3, r0
	strb	r3, [sp, #23]
	.loc 1 4410 5
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L641
	.loc 1 4410 27 discriminator 2
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #17]
	strb	r3, [sp, #23]
	.loc 1 4410 19 discriminator 2
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L642
.L641:
	.loc 1 4410 57 discriminator 3
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	b	.L652
.L642:
	.loc 1 4411 10
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #16]	@ zero_extendqisi2
	.loc 1 4411 17
	and	r3, r3, #2
	.loc 1 4411 5
	cmp	r3, #0
	bne	.L644
	.loc 1 4411 30 discriminator 1
	movs	r3, #7
	b	.L652
.L644:
	.loc 1 4413 13
	ldr	r3, [sp, #4]
	ldr	r2, [r3, #12]
	.loc 1 4413 26
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #20]
	.loc 1 4413 5
	cmp	r2, r3
	bls	.L645
	.loc 1 4414 9
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #20]
	.loc 1 4414 6
	cmp	r3, #0
	bne	.L646
	.loc 1 4415 10
	ldr	r0, [sp, #4]
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #8]
	movs	r2, #0
	mov	r1, r3
	bl	remove_chain
	mov	r3, r0
	strb	r3, [sp, #23]
	.loc 1 4416 19
	ldr	r3, [sp, #4]
	movs	r2, #0
	str	r2, [r3, #8]
	b	.L647
.L646:
	.loc 1 4418 10
	ldr	r2, [sp, #4]
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #24]
	mov	r1, r3
	mov	r0, r2
	bl	get_fat
	str	r0, [sp, #16]
	.loc 1 4419 8
	movs	r3, #0
	strb	r3, [sp, #23]
	.loc 1 4420 7
	ldr	r3, [sp, #16]
	cmp	r3, #-1
	bne	.L648
	.loc 1 4420 31 discriminator 1
	movs	r3, #1
	strb	r3, [sp, #23]
.L648:
	.loc 1 4421 7
	ldr	r3, [sp, #16]
	cmp	r3, #1
	bne	.L649
	.loc 1 4421 22 discriminator 1
	movs	r3, #2
	strb	r3, [sp, #23]
.L649:
	.loc 1 4422 7
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L647
	.loc 1 4422 32 discriminator 1
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #28]
	.loc 1 4422 21 discriminator 1
	ldr	r2, [sp, #16]
	cmp	r2, r3
	bcs	.L647
	.loc 1 4423 11
	ldr	r0, [sp, #4]
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #24]
	mov	r2, r3
	ldr	r1, [sp, #16]
	bl	remove_chain
	mov	r3, r0
	strb	r3, [sp, #23]
.L647:
	.loc 1 4426 23
	ldr	r3, [sp, #4]
	ldr	r2, [r3, #20]
	.loc 1 4426 19
	ldr	r3, [sp, #4]
	str	r2, [r3, #12]
	.loc 1 4427 12
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #16]	@ zero_extendqisi2
	orr	r3, r3, #64
	uxtb	r2, r3
	ldr	r3, [sp, #4]
	strb	r2, [r3, #16]
	.loc 1 4429 6
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L650
	.loc 1 4429 26 discriminator 1
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #16]	@ zero_extendqisi2
	.loc 1 4429 20 discriminator 1
	sxtb	r3, r3
	cmp	r3, #0
	bge	.L650
	.loc 1 4430 21
	ldr	r3, [sp, #12]
	.loc 1 4430 8
	ldrb	r0, [r3, #1]	@ zero_extendqisi2
	.loc 1 4430 30
	ldr	r3, [sp, #4]
	add	r1, r3, #40
	.loc 1 4430 8
	ldr	r3, [sp, #4]
	ldr	r2, [r3, #28]
	movs	r3, #1
	bl	disk_write
	mov	r3, r0
	.loc 1 4430 7
	cmp	r3, #0
	beq	.L651
	.loc 1 4431 9
	movs	r3, #1
	strb	r3, [sp, #23]
	b	.L650
.L651:
	.loc 1 4433 14
	ldr	r3, [sp, #4]
	ldrb	r3, [r3, #16]	@ zero_extendqisi2
	and	r3, r3, #127
	uxtb	r2, r3
	ldr	r3, [sp, #4]
	strb	r2, [r3, #16]
.L650:
	.loc 1 4437 6
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L645
	.loc 1 4437 21 discriminator 1
	ldr	r3, [sp, #4]
	ldrb	r2, [sp, #23]
	strb	r2, [r3, #17]
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	b	.L652
.L645:
	.loc 1 4440 2
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
.L652:
	.loc 1 4441 1 discriminator 1
	mov	r0, r3
	add	sp, sp, #28
.LCFI141:
	@ sp needed
	ldr	pc, [sp], #4
.LFE50:
	.size	f_truncate, .-f_truncate
	.section	.text.f_unlink,"ax",%progbits
	.align	1
	.global	f_unlink
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	f_unlink, %function
f_unlink:
.LFB51:
	.loc 1 4453 1
	@ args = 0, pretend = 0, frame = 120
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI142:
	sub	sp, sp, #124
.LCFI143:
	str	r0, [sp, #4]
	.loc 1 4456 8
	movs	r3, #0
	str	r3, [sp, #112]
	.loc 1 4465 8
	add	r1, sp, #12
	add	r3, sp, #4
	movs	r2, #2
	mov	r0, r3
	bl	find_volume
	mov	r3, r0
	strb	r3, [sp, #119]
	.loc 1 4466 12
	ldr	r3, [sp, #12]
	str	r3, [sp, #64]
	.loc 1 4467 5
	ldrb	r3, [sp, #119]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L654
	.loc 1 4469 9
	ldr	r2, [sp, #4]
	add	r3, sp, #64
	mov	r1, r2
	mov	r0, r3
	bl	follow_path
	mov	r3, r0
	strb	r3, [sp, #119]
	.loc 1 4470 6
	ldrb	r3, [sp, #119]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L655
	.loc 1 4470 42 discriminator 1
	ldrb	r3, [sp, #107]	@ zero_extendqisi2
	.loc 1 4470 51 discriminator 1
	and	r3, r3, #32
	.loc 1 4470 33 discriminator 1
	cmp	r3, #0
	beq	.L655
	.loc 1 4471 8
	movs	r3, #6
	strb	r3, [sp, #119]
.L655:
	.loc 1 4476 6
	ldrb	r3, [sp, #119]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L654
	.loc 1 4477 13
	ldrb	r3, [sp, #107]	@ zero_extendqisi2
	.loc 1 4477 8
	sxtb	r3, r3
	.loc 1 4477 7
	cmp	r3, #0
	bge	.L656
	.loc 1 4478 9
	movs	r3, #6
	strb	r3, [sp, #119]
	b	.L657
.L656:
	.loc 1 4480 15
	ldrb	r3, [sp, #70]	@ zero_extendqisi2
	.loc 1 4480 21
	and	r3, r3, #1
	.loc 1 4480 8
	cmp	r3, #0
	beq	.L657
	.loc 1 4481 10
	movs	r3, #7
	strb	r3, [sp, #119]
.L657:
	.loc 1 4484 7
	ldrb	r3, [sp, #119]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L658
	.loc 1 4494 14
	ldr	r3, [sp, #12]
	.loc 1 4494 29
	ldr	r2, [sp, #92]
	.loc 1 4494 14
	mov	r1, r2
	mov	r0, r3
	bl	ld_clust
	str	r0, [sp, #112]
	.loc 1 4496 15
	ldrb	r3, [sp, #70]	@ zero_extendqisi2
	.loc 1 4496 21
	and	r3, r3, #16
	.loc 1 4496 8
	cmp	r3, #0
	beq	.L658
	.loc 1 4498 21
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #24]
	.loc 1 4498 9
	ldr	r2, [sp, #112]
	cmp	r2, r3
	bne	.L659
	.loc 1 4499 11
	movs	r3, #7
	strb	r3, [sp, #119]
	b	.L658
.L659:
	.loc 1 4503 18
	ldr	r3, [sp, #12]
	str	r3, [sp, #16]
	.loc 1 4504 22
	ldr	r3, [sp, #112]
	str	r3, [sp, #24]
	.loc 1 4511 13
	add	r3, sp, #16
	movs	r1, #0
	mov	r0, r3
	bl	dir_sdi
	mov	r3, r0
	strb	r3, [sp, #119]
	.loc 1 4512 10
	ldrb	r3, [sp, #119]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L658
	.loc 1 4513 14
	add	r3, sp, #16
	movs	r1, #0
	mov	r0, r3
	bl	dir_read
	mov	r3, r0
	strb	r3, [sp, #119]
	.loc 1 4514 11
	ldrb	r3, [sp, #119]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L660
	.loc 1 4514 30 discriminator 1
	movs	r3, #7
	strb	r3, [sp, #119]
.L660:
	.loc 1 4515 11
	ldrb	r3, [sp, #119]	@ zero_extendqisi2
	cmp	r3, #4
	bne	.L658
	.loc 1 4515 35 discriminator 1
	movs	r3, #0
	strb	r3, [sp, #119]
.L658:
	.loc 1 4520 7
	ldrb	r3, [sp, #119]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L654
	.loc 1 4521 11
	add	r3, sp, #64
	mov	r0, r3
	bl	dir_remove
	mov	r3, r0
	strb	r3, [sp, #119]
	.loc 1 4522 8
	ldrb	r3, [sp, #119]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L661
	.loc 1 4522 22 discriminator 1
	ldr	r3, [sp, #112]
	cmp	r3, #0
	beq	.L661
	.loc 1 4526 12
	add	r3, sp, #64
	movs	r2, #0
	ldr	r1, [sp, #112]
	mov	r0, r3
	bl	remove_chain
	mov	r3, r0
	strb	r3, [sp, #119]
.L661:
	.loc 1 4529 8
	ldrb	r3, [sp, #119]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L654
	.loc 1 4529 29 discriminator 1
	ldr	r3, [sp, #12]
	mov	r0, r3
	bl	sync_fs
	mov	r3, r0
	strb	r3, [sp, #119]
.L654:
	.loc 1 4535 2
	ldrb	r3, [sp, #119]	@ zero_extendqisi2
	.loc 1 4536 1
	mov	r0, r3
	add	sp, sp, #124
.LCFI144:
	@ sp needed
	ldr	pc, [sp], #4
.LFE51:
	.size	f_unlink, .-f_unlink
	.section	.text.f_mkdir,"ax",%progbits
	.align	1
	.global	f_mkdir
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	f_mkdir, %function
f_mkdir:
.LFB52:
	.loc 1 4548 1
	@ args = 0, pretend = 0, frame = 88
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI145:
	sub	sp, sp, #92
.LCFI146:
	str	r0, [sp, #4]
	.loc 1 4559 8
	add	r1, sp, #8
	add	r3, sp, #4
	movs	r2, #2
	mov	r0, r3
	bl	find_volume
	mov	r3, r0
	strb	r3, [sp, #87]
	.loc 1 4560 12
	ldr	r3, [sp, #8]
	str	r3, [sp, #12]
	.loc 1 4561 5
	ldrb	r3, [sp, #87]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L664
	.loc 1 4563 9
	ldr	r2, [sp, #4]
	add	r3, sp, #12
	mov	r1, r2
	mov	r0, r3
	bl	follow_path
	mov	r3, r0
	strb	r3, [sp, #87]
	.loc 1 4564 6
	ldrb	r3, [sp, #87]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L665
	.loc 1 4564 25 discriminator 1
	movs	r3, #8
	strb	r3, [sp, #87]
.L665:
	.loc 1 4565 6
	ldrb	r3, [sp, #87]	@ zero_extendqisi2
	cmp	r3, #4
	bne	.L666
	.loc 1 4565 47 discriminator 1
	ldrb	r3, [sp, #55]	@ zero_extendqisi2
	.loc 1 4565 56 discriminator 1
	and	r3, r3, #32
	.loc 1 4565 38 discriminator 1
	cmp	r3, #0
	beq	.L666
	.loc 1 4566 8
	movs	r3, #6
	strb	r3, [sp, #87]
.L666:
	.loc 1 4568 6
	ldrb	r3, [sp, #87]	@ zero_extendqisi2
	cmp	r3, #4
	bne	.L664
	.loc 1 4569 10
	add	r3, sp, #12
	movs	r1, #0
	mov	r0, r3
	bl	create_chain
	str	r0, [sp, #68]
	.loc 1 4570 30
	ldr	r3, [sp, #8]
	ldrh	r3, [r3, #10]
	.loc 1 4570 38
	lsls	r3, r3, #9
	.loc 1 4570 19
	str	r3, [sp, #24]
	.loc 1 4571 8
	movs	r3, #0
	strb	r3, [sp, #87]
	.loc 1 4572 7
	ldr	r3, [sp, #68]
	cmp	r3, #0
	bne	.L667
	.loc 1 4572 22 discriminator 1
	movs	r3, #7
	strb	r3, [sp, #87]
.L667:
	.loc 1 4573 7
	ldr	r3, [sp, #68]
	cmp	r3, #1
	bne	.L668
	.loc 1 4573 22 discriminator 1
	movs	r3, #2
	strb	r3, [sp, #87]
.L668:
	.loc 1 4574 7
	ldr	r3, [sp, #68]
	cmp	r3, #-1
	bne	.L669
	.loc 1 4574 31 discriminator 1
	movs	r3, #1
	strb	r3, [sp, #87]
.L669:
	.loc 1 4575 7
	ldrb	r3, [sp, #87]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L670
	.loc 1 4575 28 discriminator 1
	ldr	r3, [sp, #8]
	mov	r0, r3
	bl	sync_window
	mov	r3, r0
	strb	r3, [sp, #87]
.L670:
	.loc 1 4576 7
	ldr	r3, .L680
	str	r3, [sp, #64]
	.loc 1 4577 7
	ldrb	r3, [sp, #87]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L671
	.loc 1 4578 11
	ldr	r3, [sp, #8]
	ldr	r1, [sp, #68]
	mov	r0, r3
	bl	clust2sect
	str	r0, [sp, #76]
	.loc 1 4579 13
	ldr	r3, [sp, #8]
	.loc 1 4579 9
	adds	r3, r3, #56
	str	r3, [sp, #60]
	.loc 1 4580 5
	mov	r2, #512
	movs	r1, #0
	ldr	r0, [sp, #60]
	bl	mem_set
	.loc 1 4582 6
	movs	r2, #11
	movs	r1, #32
	ldr	r0, [sp, #60]
	bl	mem_set
	.loc 1 4583 20
	ldr	r3, [sp, #60]
	movs	r2, #46
	strb	r2, [r3]
	.loc 1 4584 9
	ldr	r3, [sp, #60]
	adds	r3, r3, #11
	.loc 1 4584 20
	movs	r2, #16
	strb	r2, [r3]
	.loc 1 4585 6
	ldr	r3, [sp, #60]
	adds	r3, r3, #22
	ldr	r1, [sp, #64]
	mov	r0, r3
	bl	st_dword
	.loc 1 4586 6
	ldr	r3, [sp, #8]
	ldr	r2, [sp, #68]
	ldr	r1, [sp, #60]
	mov	r0, r3
	bl	st_clust
	.loc 1 4587 18
	ldr	r3, [sp, #60]
	adds	r3, r3, #32
	.loc 1 4587 6
	movs	r2, #32
	ldr	r1, [sp, #60]
	mov	r0, r3
	bl	mem_cpy
	.loc 1 4588 9
	ldr	r3, [sp, #60]
	adds	r3, r3, #33
	.loc 1 4588 22
	movs	r2, #46
	strb	r2, [r3]
	.loc 1 4588 33
	ldr	r3, [sp, #20]
	str	r3, [sp, #72]
	.loc 1 4589 12
	ldr	r3, [sp, #8]
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 4589 9
	cmp	r3, #3
	bne	.L672
	.loc 1 4589 46 discriminator 1
	ldr	r3, [sp, #8]
	ldr	r3, [r3, #44]
	.loc 1 4589 34 discriminator 1
	ldr	r2, [sp, #72]
	cmp	r2, r3
	bne	.L672
	.loc 1 4589 61 discriminator 2
	movs	r3, #0
	str	r3, [sp, #72]
.L672:
	.loc 1 4590 6
	ldr	r0, [sp, #8]
	ldr	r3, [sp, #60]
	adds	r3, r3, #32
	ldr	r2, [sp, #72]
	mov	r1, r3
	bl	st_clust
	.loc 1 4592 16
	ldr	r3, [sp, #8]
	ldrh	r3, [r3, #10]
	.loc 1 4592 12
	str	r3, [sp, #80]
	.loc 1 4592 5
	b	.L673
.L675:
	.loc 1 4593 23
	ldr	r3, [sp, #76]
	adds	r2, r3, #1
	str	r2, [sp, #76]
	.loc 1 4593 8
	ldr	r2, [sp, #8]
	.loc 1 4593 18
	str	r3, [r2, #52]
	.loc 1 4594 8
	ldr	r3, [sp, #8]
	.loc 1 4594 16
	movs	r2, #1
	strb	r2, [r3, #3]
	.loc 1 4595 12
	ldr	r3, [sp, #8]
	mov	r0, r3
	bl	sync_window
	mov	r3, r0
	strb	r3, [sp, #87]
	.loc 1 4596 9
	ldrb	r3, [sp, #87]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L679
	.loc 1 4597 6 discriminator 2
	mov	r2, #512
	movs	r1, #0
	ldr	r0, [sp, #60]
	bl	mem_set
	.loc 1 4592 29 discriminator 2
	ldr	r3, [sp, #80]
	subs	r3, r3, #1
	str	r3, [sp, #80]
.L673:
	.loc 1 4592 5 discriminator 1
	ldr	r3, [sp, #80]
	cmp	r3, #0
	bne	.L675
	b	.L671
.L679:
	.loc 1 4596 6
	nop
.L671:
	.loc 1 4600 7
	ldrb	r3, [sp, #87]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L676
	.loc 1 4600 28 discriminator 1
	add	r3, sp, #12
	mov	r0, r3
	bl	dir_register
	mov	r3, r0
	strb	r3, [sp, #87]
.L676:
	.loc 1 4601 7
	ldrb	r3, [sp, #87]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L677
	.loc 1 4614 10
	ldr	r3, [sp, #40]
	str	r3, [sp, #60]
	.loc 1 4615 6
	ldr	r3, [sp, #60]
	adds	r3, r3, #22
	ldr	r1, [sp, #64]
	mov	r0, r3
	bl	st_dword
	.loc 1 4616 6
	ldr	r3, [sp, #8]
	ldr	r2, [sp, #68]
	ldr	r1, [sp, #60]
	mov	r0, r3
	bl	st_clust
	.loc 1 4617 9
	ldr	r3, [sp, #60]
	adds	r3, r3, #11
	.loc 1 4617 20
	movs	r2, #16
	strb	r2, [r3]
	.loc 1 4618 8
	ldr	r3, [sp, #8]
	.loc 1 4618 16
	movs	r2, #1
	strb	r2, [r3, #3]
	.loc 1 4620 8
	ldrb	r3, [sp, #87]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L664
	.loc 1 4620 29 discriminator 1
	ldr	r3, [sp, #8]
	mov	r0, r3
	bl	sync_fs
	mov	r3, r0
	strb	r3, [sp, #87]
	b	.L664
.L677:
	.loc 1 4622 5
	add	r3, sp, #12
	movs	r2, #0
	ldr	r1, [sp, #68]
	mov	r0, r3
	bl	remove_chain
.L664:
	.loc 1 4628 2
	ldrb	r3, [sp, #87]	@ zero_extendqisi2
	.loc 1 4629 1
	mov	r0, r3
	add	sp, sp, #92
.LCFI147:
	@ sp needed
	ldr	pc, [sp], #4
.L681:
	.align	2
.L680:
	.word	1210122240
.LFE52:
	.size	f_mkdir, .-f_mkdir
	.section	.text.f_rename,"ax",%progbits
	.align	1
	.global	f_rename
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	f_rename, %function
f_rename:
.LFB53:
	.loc 1 4642 1
	@ args = 0, pretend = 0, frame = 144
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
.LCFI148:
	sub	sp, sp, #144
.LCFI149:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 4651 2
	mov	r3, sp
	mov	r0, r3
	bl	get_ldnumber
	.loc 1 4652 8
	add	r1, sp, #32
	add	r3, sp, #4
	movs	r2, #2
	mov	r0, r3
	bl	find_volume
	mov	r3, r0
	strb	r3, [sp, #143]
	.loc 1 4653 5
	ldrb	r3, [sp, #143]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L683
	.loc 1 4654 14
	ldr	r3, [sp, #32]
	str	r3, [sp, #84]
	.loc 1 4656 9
	ldr	r2, [sp, #4]
	add	r3, sp, #84
	mov	r1, r2
	mov	r0, r3
	bl	follow_path
	mov	r3, r0
	strb	r3, [sp, #143]
	.loc 1 4657 6
	ldrb	r3, [sp, #143]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L684
	.loc 1 4657 30 discriminator 1
	ldrb	r3, [sp, #127]	@ zero_extendqisi2
	.loc 1 4657 39 discriminator 1
	and	r3, r3, #160
	.loc 1 4657 20 discriminator 1
	cmp	r3, #0
	beq	.L684
	.loc 1 4657 68 discriminator 2
	movs	r3, #6
	strb	r3, [sp, #143]
.L684:
	.loc 1 4661 6
	ldrb	r3, [sp, #143]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L683
	.loc 1 4688 21
	ldr	r3, [sp, #112]
	.loc 1 4688 26
	add	r1, r3, #11
	.loc 1 4688 5
	add	r3, sp, #8
	movs	r2, #21
	mov	r0, r3
	bl	mem_cpy
	.loc 1 4689 5
	add	r1, sp, #84
	add	r3, sp, #36
	movs	r2, #48
	mov	r0, r3
	bl	mem_cpy
	.loc 1 4690 11
	ldr	r2, [sp]
	add	r3, sp, #36
	mov	r1, r2
	mov	r0, r3
	bl	follow_path
	mov	r3, r0
	strb	r3, [sp, #143]
	.loc 1 4691 8
	ldrb	r3, [sp, #143]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L685
	.loc 1 4692 20
	ldr	r2, [sp, #44]
	.loc 1 4692 38
	ldr	r3, [sp, #92]
	.loc 1 4692 84
	cmp	r2, r3
	bne	.L686
	.loc 1 4692 52 discriminator 1
	ldr	r2, [sp, #52]
	.loc 1 4692 64 discriminator 1
	ldr	r3, [sp, #100]
	.loc 1 4692 46 discriminator 1
	cmp	r2, r3
	bne	.L686
	.loc 1 4692 84 discriminator 3
	movs	r3, #4
	b	.L687
.L686:
	.loc 1 4692 84 is_stmt 0 discriminator 4
	movs	r3, #8
.L687:
	.loc 1 4692 10 is_stmt 1 discriminator 6
	strb	r3, [sp, #143]
.L685:
	.loc 1 4694 8
	ldrb	r3, [sp, #143]	@ zero_extendqisi2
	cmp	r3, #4
	bne	.L688
	.loc 1 4695 12
	add	r3, sp, #36
	mov	r0, r3
	bl	dir_register
	mov	r3, r0
	strb	r3, [sp, #143]
	.loc 1 4696 9
	ldrb	r3, [sp, #143]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L688
	.loc 1 4697 11
	ldr	r3, [sp, #64]
	str	r3, [sp, #136]
	.loc 1 4698 19
	ldr	r3, [sp, #136]
	add	r0, r3, #13
	.loc 1 4698 7
	add	r3, sp, #8
	adds	r3, r3, #2
	movs	r2, #19
	mov	r1, r3
	bl	mem_cpy
	.loc 1 4699 26
	ldrb	r2, [sp, #8]	@ zero_extendqisi2
	.loc 1 4699 10
	ldr	r3, [sp, #136]
	adds	r3, r3, #11
	.loc 1 4699 21
	orr	r2, r2, #32
	uxtb	r2, r2
	strb	r2, [r3]
	.loc 1 4700 9
	ldr	r3, [sp, #32]
	.loc 1 4700 17
	movs	r2, #1
	strb	r2, [r3, #3]
	.loc 1 4701 15
	ldr	r3, [sp, #136]
	adds	r3, r3, #11
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 4701 26
	and	r3, r3, #16
	.loc 1 4701 10
	cmp	r3, #0
	beq	.L688
	.loc 1 4701 46 discriminator 1
	ldr	r2, [sp, #92]
	.loc 1 4701 64 discriminator 1
	ldr	r3, [sp, #44]
	.loc 1 4701 36 discriminator 1
	cmp	r2, r3
	beq	.L688
	.loc 1 4702 13
	ldr	r4, [sp, #32]
	ldr	r3, [sp, #32]
	ldr	r1, [sp, #136]
	mov	r0, r3
	bl	ld_clust
	mov	r3, r0
	mov	r1, r3
	mov	r0, r4
	bl	clust2sect
	str	r0, [sp, #132]
	.loc 1 4703 11
	ldr	r3, [sp, #132]
	cmp	r3, #0
	bne	.L689
	.loc 1 4704 13
	movs	r3, #2
	strb	r3, [sp, #143]
	b	.L688
.L689:
	.loc 1 4707 15
	ldr	r3, [sp, #32]
	ldr	r1, [sp, #132]
	mov	r0, r3
	bl	move_window
	mov	r3, r0
	strb	r3, [sp, #143]
	.loc 1 4708 17
	ldr	r3, [sp, #32]
	.loc 1 4708 15
	adds	r3, r3, #56
	.loc 1 4708 13
	adds	r3, r3, #32
	str	r3, [sp, #136]
	.loc 1 4709 12
	ldrb	r3, [sp, #143]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L688
	.loc 1 4709 32 discriminator 1
	ldr	r3, [sp, #136]
	adds	r3, r3, #1
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 4709 26 discriminator 1
	cmp	r3, #46
	bne	.L688
	.loc 1 4710 10
	ldr	r3, [sp, #32]
	ldr	r2, [sp, #44]
	ldr	r1, [sp, #136]
	mov	r0, r3
	bl	st_clust
	.loc 1 4711 12
	ldr	r3, [sp, #32]
	.loc 1 4711 20
	movs	r2, #1
	strb	r2, [r3, #3]
.L688:
	.loc 1 4718 7
	ldrb	r3, [sp, #143]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L683
	.loc 1 4719 11
	add	r3, sp, #84
	mov	r0, r3
	bl	dir_remove
	mov	r3, r0
	strb	r3, [sp, #143]
	.loc 1 4720 8
	ldrb	r3, [sp, #143]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L683
	.loc 1 4721 12
	ldr	r3, [sp, #32]
	mov	r0, r3
	bl	sync_fs
	mov	r3, r0
	strb	r3, [sp, #143]
.L683:
	.loc 1 4729 2
	ldrb	r3, [sp, #143]	@ zero_extendqisi2
	.loc 1 4730 1
	mov	r0, r3
	add	sp, sp, #144
.LCFI150:
	@ sp needed
	pop	{r4, pc}
.LFE53:
	.size	f_rename, .-f_rename
	.section .rodata
	.align	2
.LC2:
	.ascii	"\353\376\220MSDOS5.0\000"
	.align	2
.LC3:
	.ascii	"NO NAME    FAT32   \000"
	.align	2
.LC4:
	.ascii	"NO NAME    FAT     \000"
	.section	.text.f_mkfs,"ax",%progbits
	.align	1
	.global	f_mkfs
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	f_mkfs, %function
f_mkfs:
.LFB54:
	.loc 1 5218 1
	@ args = 4, pretend = 0, frame = 112
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI151:
	sub	sp, sp, #116
.LCFI152:
	str	r0, [sp, #12]
	str	r2, [sp, #4]
	str	r3, [sp]
	mov	r3, r1
	strb	r3, [sp, #11]
	.loc 1 5219 13
	movs	r3, #1
	str	r3, [sp, #68]
	.loc 1 5220 13
	mov	r3, #512
	str	r3, [sp, #64]
	.loc 1 5237 8
	add	r3, sp, #12
	mov	r0, r3
	bl	get_ldnumber
	str	r0, [sp, #60]
	.loc 1 5238 5
	ldr	r3, [sp, #60]
	cmp	r3, #0
	bge	.L692
	.loc 1 5238 22 discriminator 1
	movs	r3, #11
	b	.L766
.L692:
	.loc 1 5239 11
	ldr	r2, .L771
	ldr	r3, [sp, #60]
	ldr	r3, [r2, r3, lsl #2]
	.loc 1 5239 5
	cmp	r3, #0
	beq	.L694
	.loc 1 5239 23 discriminator 1
	ldr	r2, .L771
	ldr	r3, [sp, #60]
	ldr	r3, [r2, r3, lsl #2]
	.loc 1 5239 38 discriminator 1
	movs	r2, #0
	strb	r2, [r3]
.L694:
	.loc 1 5240 7
	ldr	r3, [sp, #60]
	strb	r3, [sp, #59]
	.loc 1 5241 7
	movs	r3, #0
	strb	r3, [sp, #58]
	.loc 1 5244 9
	ldrb	r3, [sp, #59]	@ zero_extendqisi2
	mov	r0, r3
	bl	disk_initialize
	mov	r3, r0
	strb	r3, [sp, #57]
	.loc 1 5245 11
	ldrb	r3, [sp, #57]	@ zero_extendqisi2
	and	r3, r3, #1
	.loc 1 5245 5
	cmp	r3, #0
	beq	.L695
	.loc 1 5245 32 discriminator 1
	movs	r3, #3
	b	.L766
.L695:
	.loc 1 5246 11
	ldrb	r3, [sp, #57]	@ zero_extendqisi2
	and	r3, r3, #4
	.loc 1 5246 5
	cmp	r3, #0
	beq	.L696
	.loc 1 5246 33 discriminator 1
	movs	r3, #10
	b	.L766
.L696:
	.loc 1 5247 6
	add	r2, sp, #20
	ldrb	r3, [sp, #59]	@ zero_extendqisi2
	movs	r1, #3
	mov	r0, r3
	bl	disk_ioctl
	mov	r3, r0
	.loc 1 5247 5
	cmp	r3, #0
	bne	.L697
	.loc 1 5247 61 discriminator 2
	ldr	r3, [sp, #20]
	.loc 1 5247 58 discriminator 2
	cmp	r3, #0
	beq	.L697
	.loc 1 5247 79 discriminator 4
	ldr	r3, [sp, #20]
	.loc 1 5247 69 discriminator 4
	cmp	r3, #32768
	bhi	.L697
	.loc 1 5247 108 discriminator 6
	ldr	r3, [sp, #20]
	subs	r2, r3, #1
	.loc 1 5247 98 discriminator 6
	ldr	r3, [sp, #20]
	ands	r3, r3, r2
	.loc 1 5247 87 discriminator 6
	cmp	r3, #0
	beq	.L698
.L697:
	.loc 1 5247 122 discriminator 7
	movs	r3, #1
	str	r3, [sp, #20]
.L698:
	.loc 1 5252 5
	mov	r3, #512
	strh	r3, [sp, #54]	@ movhi
	.loc 1 5254 5
	ldr	r3, [sp, #4]
	cmp	r3, #0
	beq	.L699
	.loc 1 5254 21 discriminator 1
	ldrh	r3, [sp, #54]
	.loc 1 5254 15 discriminator 1
	ldr	r2, [sp, #4]
	cmp	r2, r3
	bcc	.L700
.L699:
	.loc 1 5254 27 discriminator 4
	ldr	r3, [sp, #4]
	cmp	r3, #16777216
	bhi	.L700
	.loc 1 5254 58 discriminator 6
	ldr	r3, [sp, #4]
	subs	r2, r3, #1
	.loc 1 5254 52 discriminator 6
	ldr	r3, [sp, #4]
	ands	r3, r3, r2
	.loc 1 5254 45 discriminator 6
	cmp	r3, #0
	beq	.L701
.L700:
	.loc 1 5254 72 discriminator 7
	movs	r3, #19
	b	.L766
.L701:
	.loc 1 5255 5
	ldrh	r3, [sp, #54]
	ldr	r2, [sp, #4]
	udiv	r3, r2, r3
	str	r3, [sp, #4]
	.loc 1 5258 6
	ldr	r3, [sp]
	str	r3, [sp, #48]
	.loc 1 5259 15
	ldrh	r3, [sp, #54]
	.loc 1 5259 9
	ldr	r2, [sp, #120]
	udiv	r3, r2, r3
	str	r3, [sp, #44]
	.loc 1 5260 19
	ldrh	r2, [sp, #54]
	.loc 1 5260 10
	ldr	r3, [sp, #44]
	mul	r3, r2, r3
	str	r3, [sp, #40]
	.loc 1 5261 5
	ldr	r3, [sp, #40]
	cmp	r3, #0
	bne	.L702
	.loc 1 5261 23 discriminator 1
	movs	r3, #14
	b	.L766
.L702:
	.loc 1 5274 7
	add	r2, sp, #16
	ldrb	r3, [sp, #59]	@ zero_extendqisi2
	movs	r1, #1
	mov	r0, r3
	bl	disk_ioctl
	mov	r3, r0
	.loc 1 5274 6
	cmp	r3, #0
	beq	.L703
	.loc 1 5274 69 discriminator 1
	movs	r3, #1
	b	.L766
.L703:
	.loc 1 5275 16
	ldrb	r3, [sp, #11]	@ zero_extendqisi2
	and	r3, r3, #8
	.loc 1 5275 30
	cmp	r3, #0
	beq	.L704
	.loc 1 5275 30 is_stmt 0 discriminator 1
	movs	r3, #0
	b	.L705
.L704:
	.loc 1 5275 30 discriminator 2
	movs	r3, #63
.L705:
	.loc 1 5275 9 is_stmt 1 discriminator 4
	str	r3, [sp, #36]
	.loc 1 5276 14 discriminator 4
	ldr	r3, [sp, #16]
	.loc 1 5276 6 discriminator 4
	ldr	r2, [sp, #36]
	cmp	r2, r3
	bls	.L706
	.loc 1 5276 30 discriminator 1
	movs	r3, #14
	b	.L766
.L706:
	.loc 1 5277 10
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #36]
	subs	r3, r2, r3
	str	r3, [sp, #16]
	.loc 1 5279 13
	ldr	r3, [sp, #16]
	.loc 1 5279 5
	cmp	r3, #127
	bhi	.L707
	.loc 1 5279 27 discriminator 1
	movs	r3, #14
	b	.L766
.L707:
	.loc 1 5288 6
	ldr	r3, [sp, #4]
	cmp	r3, #128
	bls	.L708
	.loc 1 5288 24 discriminator 1
	movs	r3, #19
	b	.L766
.L708:
	.loc 1 5289 11
	ldrb	r3, [sp, #11]	@ zero_extendqisi2
	and	r3, r3, #2
	.loc 1 5289 6
	cmp	r3, #0
	beq	.L709
	.loc 1 5290 13
	ldrb	r3, [sp, #11]	@ zero_extendqisi2
	and	r3, r3, #7
	.loc 1 5290 7
	cmp	r3, #2
	beq	.L710
	.loc 1 5290 44 discriminator 1
	ldrb	r3, [sp, #11]	@ zero_extendqisi2
	and	r3, r3, #1
	.loc 1 5290 35 discriminator 1
	cmp	r3, #0
	bne	.L709
.L710:
	.loc 1 5291 9
	movs	r3, #3
	strb	r3, [sp, #79]
	.loc 1 5291 5
	b	.L738
.L709:
	.loc 1 5294 13
	ldrb	r3, [sp, #11]	@ zero_extendqisi2
	and	r3, r3, #1
	.loc 1 5294 6
	cmp	r3, #0
	bne	.L712
	.loc 1 5294 31 discriminator 1
	movs	r3, #19
	b	.L766
.L712:
	.loc 1 5295 7
	movs	r3, #2
	strb	r3, [sp, #79]
.L738:
	.loc 1 5462 8
	ldr	r3, [sp, #4]
	str	r3, [sp, #80]
	.loc 1 5464 7
	ldrb	r3, [sp, #79]	@ zero_extendqisi2
	cmp	r3, #3
	bne	.L713
	.loc 1 5465 8
	ldr	r3, [sp, #80]
	cmp	r3, #0
	bne	.L714
	.loc 1 5466 17
	ldr	r3, [sp, #16]
	.loc 1 5466 8
	lsrs	r3, r3, #17
	str	r3, [sp, #92]
	.loc 1 5467 13
	movs	r3, #0
	str	r3, [sp, #72]
	.loc 1 5467 22
	movs	r3, #1
	str	r3, [sp, #80]
	.loc 1 5467 6
	b	.L715
.L716:
	.loc 1 5467 55 discriminator 4
	ldr	r3, [sp, #72]
	adds	r3, r3, #1
	str	r3, [sp, #72]
	.loc 1 5467 63 discriminator 4
	ldr	r3, [sp, #80]
	lsls	r3, r3, #1
	str	r3, [sp, #80]
.L715:
	.loc 1 5467 32 discriminator 1
	ldr	r2, .L771+4
	ldr	r3, [sp, #72]
	ldrh	r3, [r2, r3, lsl #1]
	.loc 1 5467 6 discriminator 1
	cmp	r3, #0
	beq	.L714
	.loc 1 5467 44 discriminator 3
	ldr	r2, .L771+4
	ldr	r3, [sp, #72]
	ldrh	r3, [r2, r3, lsl #1]
	mov	r2, r3
	.loc 1 5467 36 discriminator 3
	ldr	r3, [sp, #92]
	cmp	r3, r2
	bcs	.L716
.L714:
	.loc 1 5469 21
	ldr	r2, [sp, #16]
	.loc 1 5469 12
	ldr	r3, [sp, #80]
	udiv	r3, r2, r3
	str	r3, [sp, #32]
	.loc 1 5470 26
	ldr	r3, [sp, #32]
	adds	r3, r3, #2
	lsls	r2, r3, #2
	.loc 1 5470 30
	ldrh	r3, [sp, #54]
	add	r3, r3, r2
	.loc 1 5470 35
	subs	r2, r3, #1
	.loc 1 5470 40
	ldrh	r3, [sp, #54]
	.loc 1 5470 12
	udiv	r3, r2, r3
	str	r3, [sp, #104]
	.loc 1 5471 12
	movs	r3, #32
	str	r3, [sp, #84]
	.loc 1 5472 12
	movs	r3, #0
	str	r3, [sp, #108]
	.loc 1 5473 8
	ldr	r3, [sp, #32]
	movw	r2, #65525
	cmp	r3, r2
	bls	.L717
	.loc 1 5473 29 discriminator 2
	ldr	r3, [sp, #32]
	ldr	r2, .L771+8
	cmp	r3, r2
	bls	.L718
.L717:
	.loc 1 5473 59 discriminator 3
	movs	r3, #14
	b	.L766
.L713:
	.loc 1 5475 8
	ldr	r3, [sp, #80]
	cmp	r3, #0
	bne	.L719
	.loc 1 5476 17
	ldr	r3, [sp, #16]
	.loc 1 5476 8
	lsrs	r3, r3, #12
	str	r3, [sp, #92]
	.loc 1 5477 13
	movs	r3, #0
	str	r3, [sp, #72]
	.loc 1 5477 22
	movs	r3, #1
	str	r3, [sp, #80]
	.loc 1 5477 6
	b	.L720
.L721:
	.loc 1 5477 51 discriminator 4
	ldr	r3, [sp, #72]
	adds	r3, r3, #1
	str	r3, [sp, #72]
	.loc 1 5477 59 discriminator 4
	ldr	r3, [sp, #80]
	lsls	r3, r3, #1
	str	r3, [sp, #80]
.L720:
	.loc 1 5477 30 discriminator 1
	ldr	r2, .L771+12
	ldr	r3, [sp, #72]
	ldrh	r3, [r2, r3, lsl #1]
	.loc 1 5477 6 discriminator 1
	cmp	r3, #0
	beq	.L719
	.loc 1 5477 40 discriminator 3
	ldr	r2, .L771+12
	ldr	r3, [sp, #72]
	ldrh	r3, [r2, r3, lsl #1]
	mov	r2, r3
	.loc 1 5477 34 discriminator 3
	ldr	r3, [sp, #92]
	cmp	r3, r2
	bcs	.L721
.L719:
	.loc 1 5479 21
	ldr	r2, [sp, #16]
	.loc 1 5479 12
	ldr	r3, [sp, #80]
	udiv	r3, r2, r3
	str	r3, [sp, #32]
	.loc 1 5480 8
	ldr	r3, [sp, #32]
	movw	r2, #4085
	cmp	r3, r2
	bls	.L722
	.loc 1 5481 21
	ldr	r3, [sp, #32]
	adds	r3, r3, #2
	.loc 1 5481 8
	lsls	r3, r3, #1
	str	r3, [sp, #92]
	b	.L723
.L772:
	.align	2
.L771:
	.word	FatFs
	.word	cst32.1
	.word	268435445
	.word	cst.0
.L722:
	.loc 1 5483 10
	movs	r3, #1
	strb	r3, [sp, #79]
	.loc 1 5484 18
	ldr	r2, [sp, #32]
	mov	r3, r2
	lsls	r3, r3, #1
	add	r3, r3, r2
	.loc 1 5484 22
	adds	r3, r3, #1
	.loc 1 5484 27
	lsrs	r3, r3, #1
	.loc 1 5484 8
	adds	r3, r3, #3
	str	r3, [sp, #92]
.L723:
	.loc 1 5486 17
	ldrh	r2, [sp, #54]
	ldr	r3, [sp, #92]
	add	r3, r3, r2
	.loc 1 5486 22
	subs	r2, r3, #1
	.loc 1 5486 27
	ldrh	r3, [sp, #54]
	.loc 1 5486 12
	udiv	r3, r2, r3
	str	r3, [sp, #104]
	.loc 1 5487 12
	movs	r3, #1
	str	r3, [sp, #84]
	.loc 1 5488 31
	ldr	r3, [sp, #64]
	lsls	r2, r3, #5
	.loc 1 5488 40
	ldrh	r3, [sp, #54]
	.loc 1 5488 12
	udiv	r3, r2, r3
	str	r3, [sp, #108]
.L718:
	.loc 1 5490 10
	ldr	r2, [sp, #36]
	ldr	r3, [sp, #84]
	add	r3, r3, r2
	str	r3, [sp, #88]
	.loc 1 5491 28
	ldr	r3, [sp, #104]
	ldr	r2, [sp, #68]
	mul	r2, r3, r2
	.loc 1 5491 19
	ldr	r3, [sp, #88]
	add	r3, r3, r2
	.loc 1 5491 11
	ldr	r2, [sp, #108]
	add	r3, r3, r2
	str	r3, [sp, #28]
	.loc 1 5494 17
	ldr	r2, [sp, #20]
	ldr	r3, [sp, #28]
	add	r3, r3, r2
	.loc 1 5494 26
	subs	r2, r3, #1
	.loc 1 5494 33
	ldr	r3, [sp, #20]
	rsbs	r3, r3, #0
	.loc 1 5494 31
	ands	r2, r2, r3
	.loc 1 5494 6
	ldr	r3, [sp, #28]
	subs	r3, r2, r3
	str	r3, [sp, #92]
	.loc 1 5495 7
	ldrb	r3, [sp, #79]	@ zero_extendqisi2
	cmp	r3, #3
	bne	.L724
	.loc 1 5496 12
	ldr	r2, [sp, #84]
	ldr	r3, [sp, #92]
	add	r3, r3, r2
	str	r3, [sp, #84]
	.loc 1 5496 24
	ldr	r2, [sp, #88]
	ldr	r3, [sp, #92]
	add	r3, r3, r2
	str	r3, [sp, #88]
	b	.L725
.L724:
	.loc 1 5498 17
	ldr	r2, [sp, #92]
	ldr	r3, [sp, #68]
	udiv	r3, r2, r3
	.loc 1 5498 12
	ldr	r2, [sp, #104]
	add	r3, r3, r2
	str	r3, [sp, #104]
.L725:
	.loc 1 5502 30
	ldr	r3, [sp, #80]
	lsls	r2, r3, #4
	.loc 1 5502 24
	ldr	r3, [sp, #28]
	add	r2, r2, r3
	.loc 1 5502 35
	ldr	r3, [sp, #36]
	subs	r2, r2, r3
	.loc 1 5502 15
	ldr	r3, [sp, #16]
	.loc 1 5502 7
	cmp	r2, r3
	bls	.L726
	.loc 1 5502 51 discriminator 1
	movs	r3, #14
	b	.L766
.L726:
	.loc 1 5503 21
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #84]
	subs	r2, r2, r3
	.loc 1 5503 39
	ldr	r3, [sp, #104]
	ldr	r1, [sp, #68]
	mul	r3, r1, r3
	.loc 1 5503 30
	subs	r2, r2, r3
	.loc 1 5503 48
	ldr	r3, [sp, #108]
	subs	r2, r2, r3
	.loc 1 5503 11
	ldr	r3, [sp, #80]
	udiv	r3, r2, r3
	str	r3, [sp, #32]
	.loc 1 5504 7
	ldrb	r3, [sp, #79]	@ zero_extendqisi2
	cmp	r3, #3
	bne	.L727
	.loc 1 5505 8
	ldr	r3, [sp, #32]
	movw	r2, #65525
	cmp	r3, r2
	bhi	.L727
	.loc 1 5506 9
	ldr	r3, [sp, #4]
	cmp	r3, #0
	bne	.L728
	.loc 1 5506 21 discriminator 1
	ldr	r3, [sp, #80]
	lsrs	r3, r3, #1
	str	r3, [sp, #4]
	.loc 1 5506 14 discriminator 1
	ldr	r3, [sp, #4]
	cmp	r3, #0
	bne	.L767
.L728:
	.loc 1 5507 13
	movs	r3, #14
	b	.L766
.L727:
	.loc 1 5510 7
	ldrb	r3, [sp, #79]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L730
	.loc 1 5511 8
	ldr	r3, [sp, #32]
	movw	r2, #65525
	cmp	r3, r2
	bls	.L731
	.loc 1 5512 9
	ldr	r3, [sp, #4]
	cmp	r3, #0
	bne	.L732
	.loc 1 5512 22 discriminator 1
	ldr	r3, [sp, #80]
	lsls	r3, r3, #1
	.loc 1 5512 14 discriminator 1
	cmp	r3, #64
	bhi	.L732
	.loc 1 5513 10
	ldr	r3, [sp, #80]
	lsls	r3, r3, #1
	str	r3, [sp, #4]
	.loc 1 5513 7
	b	.L729
.L732:
	.loc 1 5515 15
	ldrb	r3, [sp, #11]	@ zero_extendqisi2
	and	r3, r3, #2
	.loc 1 5515 9
	cmp	r3, #0
	beq	.L733
	.loc 1 5516 11
	movs	r3, #3
	strb	r3, [sp, #79]
	.loc 1 5516 7
	b	.L729
.L733:
	.loc 1 5518 9
	ldr	r3, [sp, #4]
	cmp	r3, #0
	bne	.L734
	.loc 1 5518 21 discriminator 1
	ldr	r3, [sp, #80]
	lsls	r3, r3, #1
	str	r3, [sp, #4]
	.loc 1 5518 14 discriminator 1
	ldr	r3, [sp, #4]
	cmp	r3, #128
	bls	.L768
.L734:
	.loc 1 5519 13
	movs	r3, #14
	b	.L766
.L731:
	.loc 1 5521 9
	ldr	r3, [sp, #32]
	movw	r2, #4085
	cmp	r3, r2
	bhi	.L730
	.loc 1 5522 9
	ldr	r3, [sp, #4]
	cmp	r3, #0
	bne	.L735
	.loc 1 5522 21 discriminator 1
	ldr	r3, [sp, #80]
	lsls	r3, r3, #1
	str	r3, [sp, #4]
	.loc 1 5522 14 discriminator 1
	ldr	r3, [sp, #4]
	cmp	r3, #128
	bls	.L769
.L735:
	.loc 1 5523 13
	movs	r3, #14
	b	.L766
.L730:
	.loc 1 5526 7
	ldrb	r3, [sp, #79]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L770
	.loc 1 5526 24 discriminator 1
	ldr	r3, [sp, #32]
	movw	r2, #4085
	cmp	r3, r2
	bls	.L770
	.loc 1 5526 54 discriminator 2
	movs	r3, #14
	b	.L766
.L767:
	.loc 1 5506 6
	nop
	b	.L738
.L768:
	.loc 1 5518 6
	nop
	b	.L738
.L769:
	.loc 1 5522 6
	nop
.L729:
	.loc 1 5462 8
	b	.L738
.L770:
	.loc 1 5529 4
	nop
	.loc 1 5537 3
	ldrh	r3, [sp, #54]
	mov	r2, r3
	movs	r1, #0
	ldr	r0, [sp, #48]
	bl	mem_set
	.loc 1 5538 3
	movs	r2, #11
	ldr	r1, .L773
	ldr	r0, [sp, #48]
	bl	mem_cpy
	.loc 1 5539 3
	ldr	r3, [sp, #48]
	adds	r3, r3, #11
	ldrh	r2, [sp, #54]
	mov	r1, r2
	mov	r0, r3
	bl	st_word
	.loc 1 5540 6
	ldr	r3, [sp, #48]
	adds	r3, r3, #13
	.loc 1 5540 25
	ldr	r2, [sp, #80]
	uxtb	r2, r2
	.loc 1 5540 23
	strb	r2, [r3]
	.loc 1 5541 3
	ldr	r3, [sp, #48]
	adds	r3, r3, #14
	ldr	r2, [sp, #84]
	uxth	r2, r2
	mov	r1, r2
	mov	r0, r3
	bl	st_word
	.loc 1 5542 6
	ldr	r3, [sp, #48]
	adds	r3, r3, #16
	.loc 1 5542 22
	ldr	r2, [sp, #68]
	uxtb	r2, r2
	.loc 1 5542 20
	strb	r2, [r3]
	.loc 1 5543 3
	ldr	r3, [sp, #48]
	add	r2, r3, #17
	ldrb	r3, [sp, #79]	@ zero_extendqisi2
	cmp	r3, #3
	beq	.L739
	.loc 1 5543 3 is_stmt 0 discriminator 1
	ldr	r3, [sp, #64]
	uxth	r3, r3
	b	.L740
.L739:
	.loc 1 5543 3 discriminator 2
	movs	r3, #0
.L740:
	.loc 1 5543 3 discriminator 4
	mov	r1, r3
	mov	r0, r2
	bl	st_word
	.loc 1 5544 14 is_stmt 1 discriminator 4
	ldr	r3, [sp, #16]
	.loc 1 5544 6 discriminator 4
	cmp	r3, #65536
	bcs	.L741
	.loc 1 5545 4
	ldr	r3, [sp, #48]
	adds	r3, r3, #19
	ldr	r2, [sp, #16]
	uxth	r2, r2
	mov	r1, r2
	mov	r0, r3
	bl	st_word
	b	.L742
.L741:
	.loc 1 5547 4
	ldr	r3, [sp, #48]
	adds	r3, r3, #32
	ldr	r2, [sp, #16]
	mov	r1, r2
	mov	r0, r3
	bl	st_dword
.L742:
	.loc 1 5549 6
	ldr	r3, [sp, #48]
	adds	r3, r3, #21
	.loc 1 5549 18
	movs	r2, #248
	strb	r2, [r3]
	.loc 1 5550 3
	ldr	r3, [sp, #48]
	adds	r3, r3, #24
	movs	r1, #63
	mov	r0, r3
	bl	st_word
	.loc 1 5551 3
	ldr	r3, [sp, #48]
	adds	r3, r3, #26
	movs	r1, #255
	mov	r0, r3
	bl	st_word
	.loc 1 5552 3
	ldr	r3, [sp, #48]
	adds	r3, r3, #28
	ldr	r1, [sp, #36]
	mov	r0, r3
	bl	st_dword
	.loc 1 5553 6
	ldrb	r3, [sp, #79]	@ zero_extendqisi2
	cmp	r3, #3
	bne	.L743
	.loc 1 5554 4
	ldr	r3, [sp, #48]
	adds	r3, r3, #67
	ldr	r1, .L773+4
	mov	r0, r3
	bl	st_dword
	.loc 1 5555 4
	ldr	r3, [sp, #48]
	adds	r3, r3, #36
	ldr	r1, [sp, #104]
	mov	r0, r3
	bl	st_dword
	.loc 1 5556 4
	ldr	r3, [sp, #48]
	adds	r3, r3, #44
	movs	r1, #2
	mov	r0, r3
	bl	st_dword
	.loc 1 5557 4
	ldr	r3, [sp, #48]
	adds	r3, r3, #48
	movs	r1, #1
	mov	r0, r3
	bl	st_word
	.loc 1 5558 4
	ldr	r3, [sp, #48]
	adds	r3, r3, #50
	movs	r1, #6
	mov	r0, r3
	bl	st_word
	.loc 1 5559 7
	ldr	r3, [sp, #48]
	adds	r3, r3, #64
	.loc 1 5559 21
	movs	r2, #128
	strb	r2, [r3]
	.loc 1 5560 7
	ldr	r3, [sp, #48]
	adds	r3, r3, #66
	.loc 1 5560 22
	movs	r2, #41
	strb	r2, [r3]
	.loc 1 5561 16
	ldr	r3, [sp, #48]
	adds	r3, r3, #71
	.loc 1 5561 4
	movs	r2, #19
	ldr	r1, .L773+8
	mov	r0, r3
	bl	mem_cpy
	b	.L744
.L743:
	.loc 1 5563 4
	ldr	r3, [sp, #48]
	adds	r3, r3, #39
	ldr	r1, .L773+4
	mov	r0, r3
	bl	st_dword
	.loc 1 5564 4
	ldr	r3, [sp, #48]
	adds	r3, r3, #22
	ldr	r2, [sp, #104]
	uxth	r2, r2
	mov	r1, r2
	mov	r0, r3
	bl	st_word
	.loc 1 5565 7
	ldr	r3, [sp, #48]
	adds	r3, r3, #36
	.loc 1 5565 19
	movs	r2, #128
	strb	r2, [r3]
	.loc 1 5566 7
	ldr	r3, [sp, #48]
	adds	r3, r3, #38
	.loc 1 5566 20
	movs	r2, #41
	strb	r2, [r3]
	.loc 1 5567 16
	ldr	r3, [sp, #48]
	adds	r3, r3, #43
	.loc 1 5567 4
	movs	r2, #19
	ldr	r1, .L773+12
	mov	r0, r3
	bl	mem_cpy
.L744:
	.loc 1 5569 3
	ldr	r3, [sp, #48]
	add	r3, r3, #510
	movw	r1, #43605
	mov	r0, r3
	bl	st_word
	.loc 1 5570 7
	ldrb	r0, [sp, #59]	@ zero_extendqisi2
	movs	r3, #1
	ldr	r2, [sp, #36]
	ldr	r1, [sp, #48]
	bl	disk_write
	mov	r3, r0
	.loc 1 5570 6
	cmp	r3, #0
	beq	.L745
	.loc 1 5570 57 discriminator 1
	movs	r3, #1
	b	.L766
.L745:
	.loc 1 5573 6
	ldrb	r3, [sp, #79]	@ zero_extendqisi2
	cmp	r3, #3
	bne	.L746
	.loc 1 5574 4
	ldr	r3, [sp, #36]
	adds	r2, r3, #6
	ldrb	r0, [sp, #59]	@ zero_extendqisi2
	movs	r3, #1
	ldr	r1, [sp, #48]
	bl	disk_write
	.loc 1 5575 4
	ldrh	r3, [sp, #54]
	mov	r2, r3
	movs	r1, #0
	ldr	r0, [sp, #48]
	bl	mem_set
	.loc 1 5576 4
	ldr	r1, .L773+16
	ldr	r0, [sp, #48]
	bl	st_dword
	.loc 1 5577 4
	ldr	r3, [sp, #48]
	add	r3, r3, #484
	ldr	r1, .L773+20
	mov	r0, r3
	bl	st_dword
	.loc 1 5578 4
	ldr	r3, [sp, #48]
	add	r2, r3, #488
	ldr	r3, [sp, #32]
	subs	r3, r3, #1
	mov	r1, r3
	mov	r0, r2
	bl	st_dword
	.loc 1 5579 4
	ldr	r3, [sp, #48]
	add	r3, r3, #492
	movs	r1, #2
	mov	r0, r3
	bl	st_dword
	.loc 1 5580 4
	ldr	r3, [sp, #48]
	add	r3, r3, #510
	movw	r1, #43605
	mov	r0, r3
	bl	st_word
	.loc 1 5581 4
	ldr	r3, [sp, #36]
	adds	r2, r3, #7
	ldrb	r0, [sp, #59]	@ zero_extendqisi2
	movs	r3, #1
	ldr	r1, [sp, #48]
	bl	disk_write
	.loc 1 5582 4
	ldr	r3, [sp, #36]
	adds	r2, r3, #1
	ldrb	r0, [sp, #59]	@ zero_extendqisi2
	movs	r3, #1
	ldr	r1, [sp, #48]
	bl	disk_write
.L746:
	.loc 1 5586 3
	ldr	r2, [sp, #40]
	movs	r1, #0
	ldr	r0, [sp, #48]
	bl	mem_set
	.loc 1 5587 8
	ldr	r3, [sp, #88]
	str	r3, [sp, #100]
	.loc 1 5588 10
	movs	r3, #0
	str	r3, [sp, #72]
	.loc 1 5588 3
	b	.L747
.L754:
	.loc 1 5589 7
	ldrb	r3, [sp, #79]	@ zero_extendqisi2
	cmp	r3, #3
	bne	.L748
	.loc 1 5590 5
	mvn	r1, #7
	ldr	r0, [sp, #48]
	bl	st_dword
	.loc 1 5591 5
	ldr	r3, [sp, #48]
	adds	r3, r3, #4
	mov	r1, #-1
	mov	r0, r3
	bl	st_dword
	.loc 1 5592 5
	ldr	r3, [sp, #48]
	adds	r3, r3, #8
	mvn	r1, #-268435456
	mov	r0, r3
	bl	st_dword
	b	.L749
.L748:
	.loc 1 5594 5
	ldrb	r3, [sp, #79]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L750
	.loc 1 5594 5 is_stmt 0 discriminator 1
	ldr	r3, .L773+24
	b	.L751
.L750:
	.loc 1 5594 5 discriminator 2
	mvn	r3, #7
.L751:
	.loc 1 5594 5 discriminator 4
	mov	r1, r3
	ldr	r0, [sp, #48]
	bl	st_dword
.L749:
	.loc 1 5596 10 is_stmt 1
	ldr	r3, [sp, #104]
	str	r3, [sp, #96]
.L753:
	.loc 1 5598 7
	ldr	r2, [sp, #96]
	ldr	r3, [sp, #44]
	cmp	r3, r2
	it	cs
	movcs	r3, r2
	str	r3, [sp, #92]
	.loc 1 5599 9
	ldrb	r0, [sp, #59]	@ zero_extendqisi2
	ldr	r3, [sp, #92]
	ldr	r2, [sp, #100]
	ldr	r1, [sp, #48]
	bl	disk_write
	mov	r3, r0
	.loc 1 5599 8
	cmp	r3, #0
	beq	.L752
	.loc 1 5599 64 discriminator 1
	movs	r3, #1
	b	.L766
.L752:
	.loc 1 5600 5
	ldrh	r3, [sp, #54]
	mov	r2, r3
	movs	r1, #0
	ldr	r0, [sp, #48]
	bl	mem_set
	.loc 1 5601 10
	ldr	r2, [sp, #100]
	ldr	r3, [sp, #92]
	add	r3, r3, r2
	str	r3, [sp, #100]
	.loc 1 5601 22
	ldr	r2, [sp, #96]
	ldr	r3, [sp, #92]
	subs	r3, r2, r3
	str	r3, [sp, #96]
	.loc 1 5602 4
	ldr	r3, [sp, #96]
	cmp	r3, #0
	bne	.L753
	.loc 1 5588 28 discriminator 2
	ldr	r3, [sp, #72]
	adds	r3, r3, #1
	str	r3, [sp, #72]
.L747:
	.loc 1 5588 3 discriminator 1
	ldr	r2, [sp, #72]
	ldr	r3, [sp, #68]
	cmp	r2, r3
	bcc	.L754
	.loc 1 5606 35
	ldrb	r3, [sp, #79]	@ zero_extendqisi2
	cmp	r3, #3
	bne	.L755
	.loc 1 5606 35 is_stmt 0 discriminator 1
	ldr	r3, [sp, #80]
	b	.L756
.L774:
	.align	2
.L773:
	.word	.LC2
	.word	1210122240
	.word	.LC3
	.word	.LC4
	.word	1096897106
	.word	1631679090
	.word	16777208
.L755:
	.loc 1 5606 35 discriminator 2
	ldr	r3, [sp, #108]
.L756:
	.loc 1 5606 9 is_stmt 1 discriminator 4
	str	r3, [sp, #96]
.L758:
	.loc 1 5608 6
	ldr	r2, [sp, #96]
	ldr	r3, [sp, #44]
	cmp	r3, r2
	it	cs
	movcs	r3, r2
	str	r3, [sp, #92]
	.loc 1 5609 8
	ldrb	r0, [sp, #59]	@ zero_extendqisi2
	ldr	r3, [sp, #92]
	ldr	r2, [sp, #100]
	ldr	r1, [sp, #48]
	bl	disk_write
	mov	r3, r0
	.loc 1 5609 7
	cmp	r3, #0
	beq	.L757
	.loc 1 5609 63 discriminator 1
	movs	r3, #1
	b	.L766
.L757:
	.loc 1 5610 9
	ldr	r2, [sp, #100]
	ldr	r3, [sp, #92]
	add	r3, r3, r2
	str	r3, [sp, #100]
	.loc 1 5610 21
	ldr	r2, [sp, #96]
	ldr	r3, [sp, #92]
	subs	r3, r2, r3
	str	r3, [sp, #96]
	.loc 1 5611 3
	ldr	r3, [sp, #96]
	cmp	r3, #0
	bne	.L758
	.loc 1 5618 6
	ldrb	r3, [sp, #79]	@ zero_extendqisi2
	cmp	r3, #3
	bne	.L759
	.loc 1 5619 8
	movs	r3, #12
	strb	r3, [sp, #78]
	b	.L760
.L759:
	.loc 1 5621 15
	ldr	r3, [sp, #16]
	.loc 1 5621 7
	cmp	r3, #65536
	bcc	.L761
	.loc 1 5622 9
	movs	r3, #6
	strb	r3, [sp, #78]
	b	.L760
.L761:
	.loc 1 5624 9
	ldrb	r3, [sp, #79]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L762
	.loc 1 5624 9 is_stmt 0 discriminator 1
	movs	r3, #4
	b	.L763
.L762:
	.loc 1 5624 9 discriminator 2
	movs	r3, #1
.L763:
	.loc 1 5624 9 discriminator 4
	strb	r3, [sp, #78]
.L760:
	.loc 1 5635 13 is_stmt 1
	ldrb	r3, [sp, #11]	@ zero_extendqisi2
	and	r3, r3, #8
	.loc 1 5635 6
	cmp	r3, #0
	bne	.L764
	.loc 1 5637 4
	ldrh	r3, [sp, #54]
	mov	r2, r3
	movs	r1, #0
	ldr	r0, [sp, #48]
	bl	mem_set
	.loc 1 5638 4
	ldr	r3, [sp, #48]
	add	r3, r3, #510
	movw	r1, #43605
	mov	r0, r3
	bl	st_word
	.loc 1 5639 8
	ldr	r3, [sp, #48]
	add	r3, r3, #446
	str	r3, [sp, #24]
	.loc 1 5640 18
	ldr	r3, [sp, #24]
	movs	r2, #0
	strb	r2, [r3]
	.loc 1 5641 7
	ldr	r3, [sp, #24]
	adds	r3, r3, #1
	.loc 1 5641 20
	movs	r2, #1
	strb	r2, [r3]
	.loc 1 5642 7
	ldr	r3, [sp, #24]
	adds	r3, r3, #2
	.loc 1 5642 19
	movs	r2, #1
	strb	r2, [r3]
	.loc 1 5643 7
	ldr	r3, [sp, #24]
	adds	r3, r3, #3
	.loc 1 5643 19
	movs	r2, #0
	strb	r2, [r3]
	.loc 1 5644 7
	ldr	r3, [sp, #24]
	adds	r3, r3, #4
	.loc 1 5644 20
	ldrb	r2, [sp, #78]
	strb	r2, [r3]
	.loc 1 5645 15
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #36]
	add	r2, r2, r3
	.loc 1 5645 6
	ldr	r3, .L775
	umull	r1, r3, r3, r2
	subs	r2, r2, r3
	lsrs	r2, r2, #1
	add	r3, r3, r2
	lsrs	r3, r3, #13
	str	r3, [sp, #92]
	.loc 1 5646 7
	ldr	r3, [sp, #24]
	adds	r3, r3, #5
	.loc 1 5646 20
	movs	r2, #254
	strb	r2, [r3]
	.loc 1 5647 30
	ldr	r3, [sp, #92]
	lsrs	r3, r3, #2
	.loc 1 5647 21
	uxtb	r2, r3
	.loc 1 5647 7
	ldr	r3, [sp, #24]
	adds	r3, r3, #6
	.loc 1 5647 21
	orr	r2, r2, #63
	uxtb	r2, r2
	.loc 1 5647 19
	strb	r2, [r3]
	.loc 1 5648 7
	ldr	r3, [sp, #24]
	adds	r3, r3, #7
	.loc 1 5648 21
	ldr	r2, [sp, #92]
	uxtb	r2, r2
	.loc 1 5648 19
	strb	r2, [r3]
	.loc 1 5649 4
	ldr	r3, [sp, #24]
	adds	r3, r3, #8
	ldr	r1, [sp, #36]
	mov	r0, r3
	bl	st_dword
	.loc 1 5650 4
	ldr	r3, [sp, #24]
	adds	r3, r3, #12
	ldr	r2, [sp, #16]
	mov	r1, r2
	mov	r0, r3
	bl	st_dword
	.loc 1 5651 8
	ldrb	r0, [sp, #59]	@ zero_extendqisi2
	movs	r3, #1
	movs	r2, #0
	ldr	r1, [sp, #48]
	bl	disk_write
	mov	r3, r0
	.loc 1 5651 7
	cmp	r3, #0
	beq	.L764
	.loc 1 5651 54 discriminator 1
	movs	r3, #1
	b	.L766
.L764:
	.loc 1 5655 6
	ldrb	r3, [sp, #59]	@ zero_extendqisi2
	movs	r2, #0
	movs	r1, #0
	mov	r0, r3
	bl	disk_ioctl
	mov	r3, r0
	.loc 1 5655 5
	cmp	r3, #0
	beq	.L765
	.loc 1 5655 55 discriminator 1
	movs	r3, #1
	b	.L766
.L765:
	.loc 1 5657 9
	movs	r3, #0
.L766:
	.loc 1 5658 1 discriminator 1
	mov	r0, r3
	add	sp, sp, #116
.LCFI153:
	@ sp needed
	ldr	pc, [sp], #4
.L776:
	.align	2
.L775:
	.word	85284443
.LFE54:
	.size	f_mkfs, .-f_mkfs
	.section	.text.f_gets,"ax",%progbits
	.align	1
	.global	f_gets
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	f_gets, %function
f_gets:
.LFB55:
	.loc 1 5744 1
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI154:
	sub	sp, sp, #36
.LCFI155:
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	str	r2, [sp, #4]
	.loc 1 5745 6
	movs	r3, #0
	str	r3, [sp, #28]
	.loc 1 5746 12
	ldr	r3, [sp, #12]
	str	r3, [sp, #24]
	.loc 1 5751 8
	b	.L778
.L781:
	.loc 1 5796 3
	add	r3, sp, #16
	add	r1, sp, #20
	movs	r2, #1
	ldr	r0, [sp, #4]
	bl	f_read
	.loc 1 5797 10
	ldr	r3, [sp, #16]
	.loc 1 5797 6
	cmp	r3, #1
	bne	.L785
	.loc 1 5798 5
	ldrb	r3, [sp, #20]
	strb	r3, [sp, #23]
	.loc 1 5801 5
	ldr	r3, [sp, #24]
	adds	r2, r3, #1
	str	r2, [sp, #24]
	.loc 1 5801 8
	ldrb	r2, [sp, #23]
	strb	r2, [r3]
	.loc 1 5802 4
	ldr	r3, [sp, #28]
	adds	r3, r3, #1
	str	r3, [sp, #28]
	.loc 1 5803 6
	ldrb	r3, [sp, #23]	@ zero_extendqisi2
	cmp	r3, #10
	beq	.L786
.L778:
	.loc 1 5751 17
	ldr	r3, [sp, #8]
	subs	r3, r3, #1
	.loc 1 5751 8
	ldr	r2, [sp, #28]
	cmp	r2, r3
	blt	.L781
	b	.L780
.L785:
	.loc 1 5797 3
	nop
	b	.L780
.L786:
	.loc 1 5803 3
	nop
.L780:
	.loc 1 5805 5
	ldr	r3, [sp, #24]
	movs	r2, #0
	strb	r2, [r3]
	.loc 1 5806 18
	ldr	r3, [sp, #28]
	cmp	r3, #0
	beq	.L782
	.loc 1 5806 18 is_stmt 0 discriminator 1
	ldr	r3, [sp, #12]
	b	.L784
.L782:
	.loc 1 5806 18 discriminator 2
	movs	r3, #0
.L784:
	.loc 1 5807 1 is_stmt 1
	mov	r0, r3
	add	sp, sp, #36
.LCFI156:
	@ sp needed
	ldr	pc, [sp], #4
.LFE55:
	.size	f_gets, .-f_gets
	.section	.text.putc_bfd,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	putc_bfd, %function
putc_bfd:
.LFB56:
	.loc 1 5830 1
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI157:
	sub	sp, sp, #20
.LCFI158:
	str	r0, [sp, #4]
	mov	r3, r1
	strb	r3, [sp, #3]
	.loc 1 5839 4
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #4]
	str	r3, [sp, #12]
	.loc 1 5840 5
	ldr	r3, [sp, #12]
	cmp	r3, #0
	blt	.L793
	.loc 1 5869 11
	ldr	r3, [sp, #12]
	adds	r2, r3, #1
	str	r2, [sp, #12]
	.loc 1 5869 15
	ldr	r2, [sp, #4]
	add	r3, r3, r2
	ldrb	r2, [sp, #3]
	strb	r2, [r3, #12]
	.loc 1 5872 5
	ldr	r3, [sp, #12]
	cmp	r3, #60
	ble	.L790
	.loc 1 5873 3
	ldr	r3, [sp, #4]
	ldr	r0, [r3]
	.loc 1 5873 21
	ldr	r3, [sp, #4]
	add	r1, r3, #12
	.loc 1 5873 3
	ldr	r2, [sp, #12]
	add	r3, sp, #8
	bl	f_write
	.loc 1 5874 11
	ldr	r2, [sp, #8]
	.loc 1 5874 14
	ldr	r3, [sp, #12]
	.loc 1 5874 27
	cmp	r2, r3
	bne	.L791
	.loc 1 5874 27 is_stmt 0 discriminator 1
	movs	r3, #0
	b	.L792
.L791:
	.loc 1 5874 27 discriminator 2
	mov	r3, #-1
.L792:
	.loc 1 5874 5 is_stmt 1 discriminator 4
	str	r3, [sp, #12]
.L790:
	.loc 1 5876 10
	ldr	r3, [sp, #4]
	ldr	r2, [sp, #12]
	str	r2, [r3, #4]
	.loc 1 5877 4
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #8]
	.loc 1 5877 10
	adds	r2, r3, #1
	ldr	r3, [sp, #4]
	str	r2, [r3, #8]
	b	.L787
.L793:
	.loc 1 5840 13
	nop
.L787:
	.loc 1 5878 1
	add	sp, sp, #20
.LCFI159:
	@ sp needed
	ldr	pc, [sp], #4
.LFE56:
	.size	putc_bfd, .-putc_bfd
	.section	.text.putc_flush,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	putc_flush, %function
putc_flush:
.LFB57:
	.loc 1 5885 1
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI160:
	sub	sp, sp, #20
.LCFI161:
	str	r0, [sp, #4]
	.loc 1 5888 11
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #4]
	.loc 1 5888 5
	cmp	r3, #0
	blt	.L795
	.loc 1 5889 6
	ldr	r3, [sp, #4]
	ldr	r0, [r3]
	.loc 1 5889 24
	ldr	r3, [sp, #4]
	add	r1, r3, #12
	.loc 1 5889 39
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #4]
	.loc 1 5889 6
	mov	r2, r3
	add	r3, sp, #12
	bl	f_write
	mov	r3, r0
	.loc 1 5889 3
	cmp	r3, #0
	bne	.L795
	.loc 1 5890 14
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #4]
	.loc 1 5890 20
	ldr	r2, [sp, #12]
	.loc 1 5890 3
	cmp	r3, r2
	bne	.L795
	.loc 1 5890 36 discriminator 1
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #8]
	b	.L797
.L795:
	.loc 1 5891 9
	mov	r3, #-1
.L797:
	.loc 1 5892 1 discriminator 1
	mov	r0, r3
	add	sp, sp, #20
.LCFI162:
	@ sp needed
	ldr	pc, [sp], #4
.LFE57:
	.size	putc_flush, .-putc_flush
	.section	.text.putc_init,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	putc_init, %function
putc_init:
.LFB58:
	.loc 1 5900 1
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #8
.LCFI163:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 5901 9
	ldr	r3, [sp, #4]
	ldr	r2, [sp]
	str	r2, [r3]
	.loc 1 5902 21
	ldr	r3, [sp, #4]
	movs	r2, #0
	str	r2, [r3, #4]
	.loc 1 5902 15
	ldr	r3, [sp, #4]
	ldr	r2, [r3, #4]
	.loc 1 5902 11
	ldr	r3, [sp, #4]
	str	r2, [r3, #8]
	.loc 1 5903 1
	nop
	add	sp, sp, #8
.LCFI164:
	@ sp needed
	bx	lr
.LFE58:
	.size	putc_init, .-putc_init
	.section	.text.f_putc,"ax",%progbits
	.align	1
	.global	f_putc
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	f_putc, %function
f_putc:
.LFB59:
	.loc 1 5911 1
	@ args = 0, pretend = 0, frame = 88
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI165:
	sub	sp, sp, #92
.LCFI166:
	mov	r3, r0
	str	r1, [sp]
	strb	r3, [sp, #7]
	.loc 1 5915 2
	add	r3, sp, #12
	ldr	r1, [sp]
	mov	r0, r3
	bl	putc_init
	.loc 1 5916 2
	ldrb	r2, [sp, #7]	@ zero_extendqisi2
	add	r3, sp, #12
	mov	r1, r2
	mov	r0, r3
	bl	putc_bfd
	.loc 1 5917 9
	add	r3, sp, #12
	mov	r0, r3
	bl	putc_flush
	mov	r3, r0
	.loc 1 5918 1
	mov	r0, r3
	add	sp, sp, #92
.LCFI167:
	@ sp needed
	ldr	pc, [sp], #4
.LFE59:
	.size	f_putc, .-f_putc
	.section	.text.f_puts,"ax",%progbits
	.align	1
	.global	f_puts
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	f_puts, %function
f_puts:
.LFB60:
	.loc 1 5931 1
	@ args = 0, pretend = 0, frame = 88
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI168:
	sub	sp, sp, #92
.LCFI169:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 5935 2
	add	r3, sp, #12
	ldr	r1, [sp]
	mov	r0, r3
	bl	putc_init
	.loc 1 5936 8
	b	.L802
.L803:
	.loc 1 5936 33 discriminator 2
	ldr	r3, [sp, #4]
	adds	r2, r3, #1
	str	r2, [sp, #4]
	.loc 1 5936 15 discriminator 2
	ldrb	r2, [r3]	@ zero_extendqisi2
	add	r3, sp, #12
	mov	r1, r2
	mov	r0, r3
	bl	putc_bfd
.L802:
	.loc 1 5936 9 discriminator 1
	ldr	r3, [sp, #4]
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 5936 8 discriminator 1
	cmp	r3, #0
	bne	.L803
	.loc 1 5937 9
	add	r3, sp, #12
	mov	r0, r3
	bl	putc_flush
	mov	r3, r0
	.loc 1 5938 1
	mov	r0, r3
	add	sp, sp, #92
.LCFI170:
	@ sp needed
	ldr	pc, [sp], #4
.LFE60:
	.size	f_puts, .-f_puts
	.section	.text.f_printf,"ax",%progbits
	.align	1
	.global	f_printf
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	f_printf, %function
f_printf:
.LFB61:
	.loc 1 5952 1
	@ args = 4, pretend = 12, frame = 152
	@ frame_needed = 0, uses_anonymous_args = 1
	push	{r1, r2, r3}
.LCFI171:
	push	{lr}
.LCFI172:
	sub	sp, sp, #152
.LCFI173:
	str	r0, [sp, #4]
	.loc 1 5961 2
	add	r3, sp, #44
	ldr	r1, [sp, #4]
	mov	r0, r3
	bl	putc_init
	.loc 1 5963 2
	add	r3, sp, #160
	str	r3, [sp, #120]
.L856:
	.loc 1 5966 11
	ldr	r3, [sp, #156]
	adds	r2, r3, #1
	str	r2, [sp, #156]
	.loc 1 5966 5
	ldrb	r3, [r3]
	strb	r3, [sp, #131]
	.loc 1 5967 6
	ldrb	r3, [sp, #131]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L858
	.loc 1 5968 6
	ldrb	r3, [sp, #131]	@ zero_extendqisi2
	cmp	r3, #37
	beq	.L808
	.loc 1 5969 4
	ldrb	r2, [sp, #131]	@ zero_extendqisi2
	add	r3, sp, #44
	mov	r1, r2
	mov	r0, r3
	bl	putc_bfd
	.loc 1 5970 4
	b	.L809
.L808:
	.loc 1 5972 9
	movs	r3, #0
	strb	r3, [sp, #151]
	.loc 1 5972 5
	movs	r3, #0
	str	r3, [sp, #136]
	.loc 1 5973 11
	ldr	r3, [sp, #156]
	adds	r2, r3, #1
	str	r2, [sp, #156]
	.loc 1 5973 5
	ldrb	r3, [r3]
	strb	r3, [sp, #131]
	.loc 1 5974 6
	ldrb	r3, [sp, #131]	@ zero_extendqisi2
	cmp	r3, #48
	bne	.L810
	.loc 1 5975 6
	movs	r3, #1
	strb	r3, [sp, #151]
	.loc 1 5975 19
	ldr	r3, [sp, #156]
	adds	r2, r3, #1
	str	r2, [sp, #156]
	.loc 1 5975 13
	ldrb	r3, [r3]
	strb	r3, [sp, #131]
	b	.L812
.L810:
	.loc 1 5977 7
	ldrb	r3, [sp, #131]	@ zero_extendqisi2
	cmp	r3, #45
	bne	.L812
	.loc 1 5978 7
	movs	r3, #2
	strb	r3, [sp, #151]
	.loc 1 5978 20
	ldr	r3, [sp, #156]
	adds	r2, r3, #1
	str	r2, [sp, #156]
	.loc 1 5978 14
	ldrb	r3, [r3]
	strb	r3, [sp, #131]
	.loc 1 5981 9
	b	.L812
.L814:
	.loc 1 5982 10
	ldr	r2, [sp, #136]
	mov	r3, r2
	lsls	r3, r3, #2
	add	r3, r3, r2
	lsls	r3, r3, #1
	mov	r2, r3
	.loc 1 5982 15
	ldrb	r3, [sp, #131]	@ zero_extendqisi2
	add	r3, r3, r2
	.loc 1 5982 6
	subs	r3, r3, #48
	str	r3, [sp, #136]
	.loc 1 5983 12
	ldr	r3, [sp, #156]
	adds	r2, r3, #1
	str	r2, [sp, #156]
	.loc 1 5983 6
	ldrb	r3, [r3]
	strb	r3, [sp, #131]
.L812:
	.loc 1 5981 9
	ldrb	r3, [sp, #131]	@ zero_extendqisi2
	cmp	r3, #47
	bls	.L813
	.loc 1 5981 10 discriminator 1
	ldrb	r3, [sp, #131]	@ zero_extendqisi2
	cmp	r3, #57
	bls	.L814
.L813:
	.loc 1 5985 6
	ldrb	r3, [sp, #131]	@ zero_extendqisi2
	cmp	r3, #108
	beq	.L815
	.loc 1 5985 16 discriminator 1
	ldrb	r3, [sp, #131]	@ zero_extendqisi2
	cmp	r3, #76
	bne	.L816
.L815:
	.loc 1 5986 6
	ldrb	r3, [sp, #151]
	orr	r3, r3, #4
	strb	r3, [sp, #151]
	.loc 1 5986 20
	ldr	r3, [sp, #156]
	adds	r2, r3, #1
	str	r2, [sp, #156]
	.loc 1 5986 14
	ldrb	r3, [r3]
	strb	r3, [sp, #131]
.L816:
	.loc 1 5988 6
	ldrb	r3, [sp, #131]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L859
	.loc 1 5989 5
	ldrb	r3, [sp, #131]
	strb	r3, [sp, #130]
	.loc 1 5990 6
	ldrb	r3, [sp, #130]	@ zero_extendqisi2
	cmp	r3, #96
	bls	.L818
	.loc 1 5990 7 discriminator 1
	ldrb	r3, [sp, #130]	@ zero_extendqisi2
	cmp	r3, #122
	bhi	.L818
	.loc 1 5990 21 discriminator 2
	ldrb	r3, [sp, #130]
	subs	r3, r3, #32
	strb	r3, [sp, #130]
.L818:
	.loc 1 5991 3
	ldrb	r3, [sp, #130]	@ zero_extendqisi2
	subs	r3, r3, #66
	cmp	r3, #22
	bhi	.L819
	adr	r2, .L821
	ldr	pc, [r2, r3, lsl #2]
	.p2align 2
.L821:
	.word	.L826+1
	.word	.L825+1
	.word	.L822+1
	.word	.L819+1
	.word	.L819+1
	.word	.L819+1
	.word	.L819+1
	.word	.L819+1
	.word	.L819+1
	.word	.L819+1
	.word	.L819+1
	.word	.L819+1
	.word	.L819+1
	.word	.L824+1
	.word	.L819+1
	.word	.L819+1
	.word	.L819+1
	.word	.L823+1
	.word	.L819+1
	.word	.L822+1
	.word	.L819+1
	.word	.L819+1
	.word	.L820+1
	.p2align 1
.L823:
	.loc 1 5993 6
	ldr	r3, [sp, #120]
	adds	r2, r3, #4
	str	r2, [sp, #120]
	ldr	r3, [r3]
	str	r3, [sp, #124]
	.loc 1 5994 11
	movs	r3, #0
	str	r3, [sp, #140]
	.loc 1 5994 4
	b	.L827
.L828:
	.loc 1 5994 23 discriminator 3
	ldr	r3, [sp, #140]
	adds	r3, r3, #1
	str	r3, [sp, #140]
.L827:
	.loc 1 5994 17 discriminator 1
	ldr	r2, [sp, #124]
	ldr	r3, [sp, #140]
	add	r3, r3, r2
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 5994 4 discriminator 1
	cmp	r3, #0
	bne	.L828
	.loc 1 5995 12
	ldrb	r3, [sp, #151]	@ zero_extendqisi2
	and	r3, r3, #2
	.loc 1 5995 7
	cmp	r3, #0
	bne	.L832
	.loc 1 5996 11
	b	.L830
.L831:
	.loc 1 5996 21 discriminator 2
	add	r3, sp, #44
	movs	r1, #32
	mov	r0, r3
	bl	putc_bfd
.L830:
	.loc 1 5996 13 discriminator 1
	ldr	r3, [sp, #140]
	adds	r2, r3, #1
	str	r2, [sp, #140]
	.loc 1 5996 11 discriminator 1
	ldr	r2, [sp, #136]
	cmp	r2, r3
	bhi	.L831
	.loc 1 5998 10
	b	.L832
.L833:
	.loc 1 5998 31 discriminator 2
	ldr	r3, [sp, #124]
	adds	r2, r3, #1
	str	r2, [sp, #124]
	.loc 1 5998 15 discriminator 2
	ldrb	r2, [r3]	@ zero_extendqisi2
	add	r3, sp, #44
	mov	r1, r2
	mov	r0, r3
	bl	putc_bfd
.L832:
	.loc 1 5998 11 discriminator 1
	ldr	r3, [sp, #124]
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 5998 10 discriminator 1
	cmp	r3, #0
	bne	.L833
	.loc 1 5999 10
	b	.L834
.L835:
	.loc 1 5999 20 discriminator 2
	add	r3, sp, #44
	movs	r1, #32
	mov	r0, r3
	bl	putc_bfd
.L834:
	.loc 1 5999 12 discriminator 1
	ldr	r3, [sp, #140]
	adds	r2, r3, #1
	str	r2, [sp, #140]
	.loc 1 5999 10 discriminator 1
	ldr	r2, [sp, #136]
	cmp	r2, r3
	bhi	.L835
	.loc 1 6000 4
	b	.L809
.L825:
	.loc 1 6002 37
	ldr	r3, [sp, #120]
	adds	r2, r3, #4
	str	r2, [sp, #120]
	ldr	r3, [r3]
	.loc 1 6002 4
	uxtb	r2, r3
	add	r3, sp, #44
	mov	r1, r2
	mov	r0, r3
	bl	putc_bfd
	b	.L809
.L826:
	.loc 1 6004 6
	movs	r3, #2
	strb	r3, [sp, #150]
	.loc 1 6004 4
	b	.L836
.L824:
	.loc 1 6006 6
	movs	r3, #8
	strb	r3, [sp, #150]
	.loc 1 6006 4
	b	.L836
.L822:
	.loc 1 6009 6
	movs	r3, #10
	strb	r3, [sp, #150]
	.loc 1 6009 4
	b	.L836
.L820:
	.loc 1 6011 6
	movs	r3, #16
	strb	r3, [sp, #150]
	.loc 1 6011 4
	b	.L836
.L819:
	.loc 1 6013 4
	ldrb	r2, [sp, #131]	@ zero_extendqisi2
	add	r3, sp, #44
	mov	r1, r2
	mov	r0, r3
	bl	putc_bfd
	b	.L809
.L836:
	.loc 1 6017 10
	ldrb	r3, [sp, #151]	@ zero_extendqisi2
	and	r3, r3, #4
	.loc 1 6017 42
	cmp	r3, #0
	beq	.L837
	.loc 1 6017 36
	ldr	r3, [sp, #120]
	adds	r2, r3, #4
	str	r2, [sp, #120]
	ldr	r3, [r3]
	b	.L838
.L837:
	.loc 1 6017 88 discriminator 2
	ldrb	r3, [sp, #130]	@ zero_extendqisi2
	cmp	r3, #68
	bne	.L839
	.loc 1 6017 83
	ldr	r3, [sp, #120]
	adds	r2, r3, #4
	str	r2, [sp, #120]
	ldr	r3, [r3]
	b	.L838
.L839:
	.loc 1 6017 88
	ldr	r3, [sp, #120]
	adds	r2, r3, #4
	str	r2, [sp, #120]
	ldr	r3, [r3]
.L838:
	.loc 1 6017 5 discriminator 8
	str	r3, [sp, #132]
	.loc 1 6018 6 discriminator 8
	ldrb	r3, [sp, #130]	@ zero_extendqisi2
	cmp	r3, #68
	bne	.L841
	.loc 1 6018 16 discriminator 1
	ldr	r3, [sp, #132]
	cmp	r3, #0
	bge	.L841
	.loc 1 6019 6
	ldr	r3, [sp, #132]
	rsbs	r3, r3, #0
	str	r3, [sp, #132]
	.loc 1 6020 6
	ldrb	r3, [sp, #151]
	orr	r3, r3, #8
	strb	r3, [sp, #151]
.L841:
	.loc 1 6022 5
	movs	r3, #0
	str	r3, [sp, #144]
.L846:
	.loc 1 6024 18
	ldrb	r2, [sp, #150]	@ zero_extendqisi2
	ldr	r3, [sp, #132]
	udiv	r1, r3, r2
	mul	r2, r1, r2
	subs	r3, r3, r2
	.loc 1 6024 6
	strb	r3, [sp, #130]
	.loc 1 6024 26
	ldrb	r3, [sp, #150]	@ zero_extendqisi2
	ldr	r2, [sp, #132]
	udiv	r3, r2, r3
	str	r3, [sp, #132]
	.loc 1 6025 7
	ldrb	r3, [sp, #130]	@ zero_extendqisi2
	cmp	r3, #9
	bls	.L842
	.loc 1 6025 17 discriminator 1
	ldrb	r3, [sp, #131]	@ zero_extendqisi2
	cmp	r3, #120
	bne	.L843
	.loc 1 6025 17 is_stmt 0 discriminator 2
	movs	r2, #39
	b	.L844
.L843:
	.loc 1 6025 17 discriminator 3
	movs	r2, #7
.L844:
	.loc 1 6025 17 discriminator 5
	ldrb	r3, [sp, #130]
	add	r3, r3, r2
	strb	r3, [sp, #130]
.L842:
	.loc 1 6026 9 is_stmt 1
	ldr	r3, [sp, #144]
	adds	r2, r3, #1
	str	r2, [sp, #144]
	.loc 1 6026 17
	ldrb	r2, [sp, #130]
	adds	r2, r2, #48
	uxtb	r2, r2
	.loc 1 6026 13
	add	r1, sp, #152
	add	r3, r3, r1
	strb	r2, [r3, #-140]
	.loc 1 6027 3
	ldr	r3, [sp, #132]
	cmp	r3, #0
	beq	.L845
	.loc 1 6027 14 discriminator 1
	ldr	r3, [sp, #144]
	cmp	r3, #31
	bls	.L846
.L845:
	.loc 1 6028 9
	ldrb	r3, [sp, #151]	@ zero_extendqisi2
	and	r3, r3, #8
	.loc 1 6028 6
	cmp	r3, #0
	beq	.L847
	.loc 1 6028 19 discriminator 1
	ldr	r3, [sp, #144]
	adds	r2, r3, #1
	str	r2, [sp, #144]
	.loc 1 6028 23 discriminator 1
	add	r2, sp, #152
	add	r3, r3, r2
	movs	r2, #45
	strb	r2, [r3, #-140]
.L847:
	.loc 1 6029 5
	ldr	r3, [sp, #144]
	str	r3, [sp, #140]
	.loc 1 6029 17
	ldrb	r3, [sp, #151]	@ zero_extendqisi2
	and	r3, r3, #1
	.loc 1 6029 12
	cmp	r3, #0
	beq	.L848
	.loc 1 6029 12 is_stmt 0 discriminator 1
	movs	r3, #48
	b	.L849
.L848:
	.loc 1 6029 12 discriminator 2
	movs	r3, #32
.L849:
	.loc 1 6029 12 discriminator 4
	strb	r3, [sp, #130]
	.loc 1 6030 9 is_stmt 1 discriminator 4
	b	.L850
.L852:
	.loc 1 6030 31 discriminator 3
	ldrb	r2, [sp, #130]	@ zero_extendqisi2
	add	r3, sp, #44
	mov	r1, r2
	mov	r0, r3
	bl	putc_bfd
.L850:
	.loc 1 6030 14 discriminator 1
	ldrb	r3, [sp, #151]	@ zero_extendqisi2
	and	r3, r3, #2
	.loc 1 6030 9 discriminator 1
	cmp	r3, #0
	bne	.L853
	.loc 1 6030 23 discriminator 2
	ldr	r3, [sp, #140]
	adds	r2, r3, #1
	str	r2, [sp, #140]
	.loc 1 6030 19 discriminator 2
	ldr	r2, [sp, #136]
	cmp	r2, r3
	bhi	.L852
.L853:
	.loc 1 6031 6 discriminator 1
	ldr	r3, [sp, #144]
	subs	r3, r3, #1
	str	r3, [sp, #144]
	add	r2, sp, #12
	ldr	r3, [sp, #144]
	add	r3, r3, r2
	ldrb	r2, [r3]	@ zero_extendqisi2
	add	r3, sp, #44
	mov	r1, r2
	mov	r0, r3
	bl	putc_bfd
	.loc 1 6031 3 discriminator 1
	ldr	r3, [sp, #144]
	cmp	r3, #0
	bne	.L853
	.loc 1 6032 9
	b	.L854
.L855:
	.loc 1 6032 19 discriminator 2
	ldrb	r2, [sp, #130]	@ zero_extendqisi2
	add	r3, sp, #44
	mov	r1, r2
	mov	r0, r3
	bl	putc_bfd
.L854:
	.loc 1 6032 11 discriminator 1
	ldr	r3, [sp, #140]
	adds	r2, r3, #1
	str	r2, [sp, #140]
	.loc 1 6032 9 discriminator 1
	ldr	r2, [sp, #136]
	cmp	r2, r3
	bhi	.L855
.L809:
	.loc 1 5966 5
	b	.L856
.L858:
	.loc 1 5967 3
	nop
	b	.L807
.L859:
	.loc 1 5988 3
	nop
.L807:
	.loc 1 6037 9
	add	r3, sp, #44
	mov	r0, r3
	bl	putc_flush
	mov	r3, r0
	.loc 1 6038 1
	mov	r0, r3
	add	sp, sp, #152
.LCFI174:
	@ sp needed
	ldr	lr, [sp], #4
.LCFI175:
	add	sp, sp, #12
.LCFI176:
	bx	lr
.LFE61:
	.size	f_printf, .-f_printf
	.section	.rodata.cst32.1,"a"
	.align	2
	.type	cst32.1, %object
	.size	cst32.1, 14
cst32.1:
	.short	1
	.short	2
	.short	4
	.short	8
	.short	16
	.short	32
	.short	0
	.section	.rodata.cst.0,"a"
	.align	2
	.type	cst.0, %object
	.size	cst.0, 14
cst.0:
	.short	1
	.short	4
	.short	16
	.short	64
	.short	256
	.short	512
	.short	0
	.section	.debug_frame,"",%progbits
.Lframe0:
	.4byte	.LECIE0-.LSCIE0
.LSCIE0:
	.4byte	0xffffffff
	.byte	0x3
	.ascii	"\000"
	.uleb128 0x1
	.sleb128 -4
	.uleb128 0xe
	.byte	0xc
	.uleb128 0xd
	.uleb128 0
	.align	2
.LECIE0:
.LSFDE0:
	.4byte	.LEFDE0-.LASFDE0
.LASFDE0:
	.4byte	.Lframe0
	.4byte	.LFB0
	.4byte	.LFE0-.LFB0
	.byte	0x4
	.4byte	.LCFI0-.LFB0
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.4byte	.LCFI1-.LCFI0
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE0:
.LSFDE2:
	.4byte	.LEFDE2-.LASFDE2
.LASFDE2:
	.4byte	.Lframe0
	.4byte	.LFB1
	.4byte	.LFE1-.LFB1
	.byte	0x4
	.4byte	.LCFI2-.LFB1
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.4byte	.LCFI3-.LCFI2
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE2:
.LSFDE4:
	.4byte	.LEFDE4-.LASFDE4
.LASFDE4:
	.4byte	.Lframe0
	.4byte	.LFB2
	.4byte	.LFE2-.LFB2
	.byte	0x4
	.4byte	.LCFI4-.LFB2
	.byte	0xe
	.uleb128 0x8
	.byte	0x4
	.4byte	.LCFI5-.LCFI4
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE4:
.LSFDE6:
	.4byte	.LEFDE6-.LASFDE6
.LASFDE6:
	.4byte	.Lframe0
	.4byte	.LFB3
	.4byte	.LFE3-.LFB3
	.byte	0x4
	.4byte	.LCFI6-.LFB3
	.byte	0xe
	.uleb128 0x8
	.byte	0x4
	.4byte	.LCFI7-.LCFI6
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE6:
.LSFDE8:
	.4byte	.LEFDE8-.LASFDE8
.LASFDE8:
	.4byte	.Lframe0
	.4byte	.LFB4
	.4byte	.LFE4-.LFB4
	.byte	0x4
	.4byte	.LCFI8-.LFB4
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.4byte	.LCFI9-.LCFI8
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE8:
.LSFDE10:
	.4byte	.LEFDE10-.LASFDE10
.LASFDE10:
	.4byte	.Lframe0
	.4byte	.LFB5
	.4byte	.LFE5-.LFB5
	.byte	0x4
	.4byte	.LCFI10-.LFB5
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.4byte	.LCFI11-.LCFI10
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE10:
.LSFDE12:
	.4byte	.LEFDE12-.LASFDE12
.LASFDE12:
	.4byte	.Lframe0
	.4byte	.LFB6
	.4byte	.LFE6-.LFB6
	.byte	0x4
	.4byte	.LCFI12-.LFB6
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.4byte	.LCFI13-.LCFI12
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE12:
.LSFDE14:
	.4byte	.LEFDE14-.LASFDE14
.LASFDE14:
	.4byte	.Lframe0
	.4byte	.LFB7
	.4byte	.LFE7-.LFB7
	.byte	0x4
	.4byte	.LCFI14-.LFB7
	.byte	0xe
	.uleb128 0x8
	.byte	0x4
	.4byte	.LCFI15-.LCFI14
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE14:
.LSFDE16:
	.4byte	.LEFDE16-.LASFDE16
.LASFDE16:
	.4byte	.Lframe0
	.4byte	.LFB8
	.4byte	.LFE8-.LFB8
	.byte	0x4
	.4byte	.LCFI16-.LFB8
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI17-.LCFI16
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.4byte	.LCFI18-.LCFI17
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE16:
.LSFDE18:
	.4byte	.LEFDE18-.LASFDE18
.LASFDE18:
	.4byte	.Lframe0
	.4byte	.LFB9
	.4byte	.LFE9-.LFB9
	.byte	0x4
	.4byte	.LCFI19-.LFB9
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI20-.LCFI19
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.4byte	.LCFI21-.LCFI20
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE18:
.LSFDE20:
	.4byte	.LEFDE20-.LASFDE20
.LASFDE20:
	.4byte	.Lframe0
	.4byte	.LFB10
	.4byte	.LFE10-.LFB10
	.byte	0x4
	.4byte	.LCFI22-.LFB10
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI23-.LCFI22
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.4byte	.LCFI24-.LCFI23
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE20:
.LSFDE22:
	.4byte	.LEFDE22-.LASFDE22
.LASFDE22:
	.4byte	.Lframe0
	.4byte	.LFB11
	.4byte	.LFE11-.LFB11
	.byte	0x4
	.4byte	.LCFI25-.LFB11
	.byte	0xe
	.uleb128 0x8
	.byte	0x4
	.4byte	.LCFI26-.LCFI25
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE22:
.LSFDE24:
	.4byte	.LEFDE24-.LASFDE24
.LASFDE24:
	.4byte	.Lframe0
	.4byte	.LFB12
	.4byte	.LFE12-.LFB12
	.byte	0x4
	.4byte	.LCFI27-.LFB12
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI28-.LCFI27
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.4byte	.LCFI29-.LCFI28
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE24:
.LSFDE26:
	.4byte	.LEFDE26-.LASFDE26
.LASFDE26:
	.4byte	.Lframe0
	.4byte	.LFB13
	.4byte	.LFE13-.LFB13
	.byte	0x4
	.4byte	.LCFI30-.LFB13
	.byte	0xe
	.uleb128 0x8
	.byte	0x84
	.uleb128 0x2
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI31-.LCFI30
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.4byte	.LCFI32-.LCFI31
	.byte	0xe
	.uleb128 0x8
	.align	2
.LEFDE26:
.LSFDE28:
	.4byte	.LEFDE28-.LASFDE28
.LASFDE28:
	.4byte	.Lframe0
	.4byte	.LFB14
	.4byte	.LFE14-.LFB14
	.byte	0x4
	.4byte	.LCFI33-.LFB14
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI34-.LCFI33
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.4byte	.LCFI35-.LCFI34
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE28:
.LSFDE30:
	.4byte	.LEFDE30-.LASFDE30
.LASFDE30:
	.4byte	.Lframe0
	.4byte	.LFB15
	.4byte	.LFE15-.LFB15
	.byte	0x4
	.4byte	.LCFI36-.LFB15
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI37-.LCFI36
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.4byte	.LCFI38-.LCFI37
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE30:
.LSFDE32:
	.4byte	.LEFDE32-.LASFDE32
.LASFDE32:
	.4byte	.Lframe0
	.4byte	.LFB16
	.4byte	.LFE16-.LFB16
	.byte	0x4
	.4byte	.LCFI39-.LFB16
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI40-.LCFI39
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.4byte	.LCFI41-.LCFI40
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE32:
.LSFDE34:
	.4byte	.LEFDE34-.LASFDE34
.LASFDE34:
	.4byte	.Lframe0
	.4byte	.LFB17
	.4byte	.LFE17-.LFB17
	.byte	0x4
	.4byte	.LCFI42-.LFB17
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI43-.LCFI42
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.4byte	.LCFI44-.LCFI43
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE34:
.LSFDE36:
	.4byte	.LEFDE36-.LASFDE36
.LASFDE36:
	.4byte	.Lframe0
	.4byte	.LFB18
	.4byte	.LFE18-.LFB18
	.byte	0x4
	.4byte	.LCFI45-.LFB18
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI46-.LCFI45
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.4byte	.LCFI47-.LCFI46
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE36:
.LSFDE38:
	.4byte	.LEFDE38-.LASFDE38
.LASFDE38:
	.4byte	.Lframe0
	.4byte	.LFB19
	.4byte	.LFE19-.LFB19
	.byte	0x4
	.4byte	.LCFI48-.LFB19
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI49-.LCFI48
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.4byte	.LCFI50-.LCFI49
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE38:
.LSFDE40:
	.4byte	.LEFDE40-.LASFDE40
.LASFDE40:
	.4byte	.Lframe0
	.4byte	.LFB20
	.4byte	.LFE20-.LFB20
	.byte	0x4
	.4byte	.LCFI51-.LFB20
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI52-.LCFI51
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.4byte	.LCFI53-.LCFI52
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE40:
.LSFDE42:
	.4byte	.LEFDE42-.LASFDE42
.LASFDE42:
	.4byte	.Lframe0
	.4byte	.LFB21
	.4byte	.LFE21-.LFB21
	.byte	0x4
	.4byte	.LCFI54-.LFB21
	.byte	0xe
	.uleb128 0x8
	.byte	0x84
	.uleb128 0x2
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI55-.LCFI54
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.4byte	.LCFI56-.LCFI55
	.byte	0xe
	.uleb128 0x8
	.align	2
.LEFDE42:
.LSFDE44:
	.4byte	.LEFDE44-.LASFDE44
.LASFDE44:
	.4byte	.Lframe0
	.4byte	.LFB22
	.4byte	.LFE22-.LFB22
	.byte	0x4
	.4byte	.LCFI57-.LFB22
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI58-.LCFI57
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.4byte	.LCFI59-.LCFI58
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE44:
.LSFDE46:
	.4byte	.LEFDE46-.LASFDE46
.LASFDE46:
	.4byte	.Lframe0
	.4byte	.LFB23
	.4byte	.LFE23-.LFB23
	.byte	0x4
	.4byte	.LCFI60-.LFB23
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI61-.LCFI60
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.4byte	.LCFI62-.LCFI61
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE46:
.LSFDE48:
	.4byte	.LEFDE48-.LASFDE48
.LASFDE48:
	.4byte	.Lframe0
	.4byte	.LFB24
	.4byte	.LFE24-.LFB24
	.byte	0x4
	.4byte	.LCFI63-.LFB24
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI64-.LCFI63
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.4byte	.LCFI65-.LCFI64
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE48:
.LSFDE50:
	.4byte	.LEFDE50-.LASFDE50
.LASFDE50:
	.4byte	.Lframe0
	.4byte	.LFB25
	.4byte	.LFE25-.LFB25
	.byte	0x4
	.4byte	.LCFI66-.LFB25
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.4byte	.LCFI67-.LCFI66
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE50:
.LSFDE52:
	.4byte	.LEFDE52-.LASFDE52
.LASFDE52:
	.4byte	.Lframe0
	.4byte	.LFB26
	.4byte	.LFE26-.LFB26
	.byte	0x4
	.4byte	.LCFI68-.LFB26
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI69-.LCFI68
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.4byte	.LCFI70-.LCFI69
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE52:
.LSFDE54:
	.4byte	.LEFDE54-.LASFDE54
.LASFDE54:
	.4byte	.Lframe0
	.4byte	.LFB27
	.4byte	.LFE27-.LFB27
	.byte	0x4
	.4byte	.LCFI71-.LFB27
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI72-.LCFI71
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.4byte	.LCFI73-.LCFI72
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE54:
.LSFDE56:
	.4byte	.LEFDE56-.LASFDE56
.LASFDE56:
	.4byte	.Lframe0
	.4byte	.LFB28
	.4byte	.LFE28-.LFB28
	.byte	0x4
	.4byte	.LCFI74-.LFB28
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI75-.LCFI74
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.4byte	.LCFI76-.LCFI75
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE56:
.LSFDE58:
	.4byte	.LEFDE58-.LASFDE58
.LASFDE58:
	.4byte	.Lframe0
	.4byte	.LFB29
	.4byte	.LFE29-.LFB29
	.byte	0x4
	.4byte	.LCFI77-.LFB29
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI78-.LCFI77
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.4byte	.LCFI79-.LCFI78
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE58:
.LSFDE60:
	.4byte	.LEFDE60-.LASFDE60
.LASFDE60:
	.4byte	.Lframe0
	.4byte	.LFB30
	.4byte	.LFE30-.LFB30
	.byte	0x4
	.4byte	.LCFI80-.LFB30
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI81-.LCFI80
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.4byte	.LCFI82-.LCFI81
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE60:
.LSFDE62:
	.4byte	.LEFDE62-.LASFDE62
.LASFDE62:
	.4byte	.Lframe0
	.4byte	.LFB31
	.4byte	.LFE31-.LFB31
	.byte	0x4
	.4byte	.LCFI83-.LFB31
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI84-.LCFI83
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.4byte	.LCFI85-.LCFI84
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE62:
.LSFDE64:
	.4byte	.LEFDE64-.LASFDE64
.LASFDE64:
	.4byte	.Lframe0
	.4byte	.LFB32
	.4byte	.LFE32-.LFB32
	.byte	0x4
	.4byte	.LCFI86-.LFB32
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI87-.LCFI86
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.4byte	.LCFI88-.LCFI87
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE64:
.LSFDE66:
	.4byte	.LEFDE66-.LASFDE66
.LASFDE66:
	.4byte	.Lframe0
	.4byte	.LFB33
	.4byte	.LFE33-.LFB33
	.byte	0x4
	.4byte	.LCFI89-.LFB33
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.4byte	.LCFI90-.LCFI89
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE66:
.LSFDE68:
	.4byte	.LEFDE68-.LASFDE68
.LASFDE68:
	.4byte	.Lframe0
	.4byte	.LFB34
	.4byte	.LFE34-.LFB34
	.byte	0x4
	.4byte	.LCFI91-.LFB34
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI92-.LCFI91
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.4byte	.LCFI93-.LCFI92
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE68:
.LSFDE70:
	.4byte	.LEFDE70-.LASFDE70
.LASFDE70:
	.4byte	.Lframe0
	.4byte	.LFB35
	.4byte	.LFE35-.LFB35
	.byte	0x4
	.4byte	.LCFI94-.LFB35
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI95-.LCFI94
	.byte	0xe
	.uleb128 0x60
	.byte	0x4
	.4byte	.LCFI96-.LCFI95
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE70:
.LSFDE72:
	.4byte	.LEFDE72-.LASFDE72
.LASFDE72:
	.4byte	.Lframe0
	.4byte	.LFB36
	.4byte	.LFE36-.LFB36
	.byte	0x4
	.4byte	.LCFI97-.LFB36
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI98-.LCFI97
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.4byte	.LCFI99-.LCFI98
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE72:
.LSFDE74:
	.4byte	.LEFDE74-.LASFDE74
.LASFDE74:
	.4byte	.Lframe0
	.4byte	.LFB37
	.4byte	.LFE37-.LFB37
	.byte	0x4
	.4byte	.LCFI100-.LFB37
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI101-.LCFI100
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.4byte	.LCFI102-.LCFI101
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE74:
.LSFDE76:
	.4byte	.LEFDE76-.LASFDE76
.LASFDE76:
	.4byte	.Lframe0
	.4byte	.LFB38
	.4byte	.LFE38-.LFB38
	.byte	0x4
	.4byte	.LCFI103-.LFB38
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI104-.LCFI103
	.byte	0xe
	.uleb128 0x68
	.byte	0x4
	.4byte	.LCFI105-.LCFI104
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE76:
.LSFDE78:
	.4byte	.LEFDE78-.LASFDE78
.LASFDE78:
	.4byte	.Lframe0
	.4byte	.LFB39
	.4byte	.LFE39-.LFB39
	.byte	0x4
	.4byte	.LCFI106-.LFB39
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI107-.LCFI106
	.byte	0xe
	.uleb128 0x40
	.byte	0x4
	.4byte	.LCFI108-.LCFI107
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE78:
.LSFDE80:
	.4byte	.LEFDE80-.LASFDE80
.LASFDE80:
	.4byte	.Lframe0
	.4byte	.LFB40
	.4byte	.LFE40-.LFB40
	.byte	0x4
	.4byte	.LCFI109-.LFB40
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI110-.LCFI109
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.4byte	.LCFI111-.LCFI110
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE80:
.LSFDE82:
	.4byte	.LEFDE82-.LASFDE82
.LASFDE82:
	.4byte	.Lframe0
	.4byte	.LFB41
	.4byte	.LFE41-.LFB41
	.byte	0x4
	.4byte	.LCFI112-.LFB41
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI113-.LCFI112
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.4byte	.LCFI114-.LCFI113
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE82:
.LSFDE84:
	.4byte	.LEFDE84-.LASFDE84
.LASFDE84:
	.4byte	.Lframe0
	.4byte	.LFB42
	.4byte	.LFE42-.LFB42
	.byte	0x4
	.4byte	.LCFI115-.LFB42
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI116-.LCFI115
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.4byte	.LCFI117-.LCFI116
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE84:
.LSFDE86:
	.4byte	.LEFDE86-.LASFDE86
.LASFDE86:
	.4byte	.Lframe0
	.4byte	.LFB43
	.4byte	.LFE43-.LFB43
	.byte	0x4
	.4byte	.LCFI118-.LFB43
	.byte	0xe
	.uleb128 0x8
	.byte	0x84
	.uleb128 0x2
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI119-.LCFI118
	.byte	0xe
	.uleb128 0x48
	.byte	0x4
	.4byte	.LCFI120-.LCFI119
	.byte	0xe
	.uleb128 0x8
	.align	2
.LEFDE86:
.LSFDE88:
	.4byte	.LEFDE88-.LASFDE88
.LASFDE88:
	.4byte	.Lframe0
	.4byte	.LFB44
	.4byte	.LFE44-.LFB44
	.byte	0x4
	.4byte	.LCFI121-.LFB44
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI122-.LCFI121
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.4byte	.LCFI123-.LCFI122
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE88:
.LSFDE90:
	.4byte	.LEFDE90-.LASFDE90
.LASFDE90:
	.4byte	.Lframe0
	.4byte	.LFB45
	.4byte	.LFE45-.LFB45
	.byte	0x4
	.4byte	.LCFI124-.LFB45
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI125-.LCFI124
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.4byte	.LCFI126-.LCFI125
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE90:
.LSFDE92:
	.4byte	.LEFDE92-.LASFDE92
.LASFDE92:
	.4byte	.Lframe0
	.4byte	.LFB46
	.4byte	.LFE46-.LFB46
	.byte	0x4
	.4byte	.LCFI127-.LFB46
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI128-.LCFI127
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.4byte	.LCFI129-.LCFI128
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE92:
.LSFDE94:
	.4byte	.LEFDE94-.LASFDE94
.LASFDE94:
	.4byte	.Lframe0
	.4byte	.LFB47
	.4byte	.LFE47-.LFB47
	.byte	0x4
	.4byte	.LCFI130-.LFB47
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI131-.LCFI130
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.4byte	.LCFI132-.LCFI131
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE94:
.LSFDE96:
	.4byte	.LEFDE96-.LASFDE96
.LASFDE96:
	.4byte	.Lframe0
	.4byte	.LFB48
	.4byte	.LFE48-.LFB48
	.byte	0x4
	.4byte	.LCFI133-.LFB48
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI134-.LCFI133
	.byte	0xe
	.uleb128 0x48
	.byte	0x4
	.4byte	.LCFI135-.LCFI134
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE96:
.LSFDE98:
	.4byte	.LEFDE98-.LASFDE98
.LASFDE98:
	.4byte	.Lframe0
	.4byte	.LFB49
	.4byte	.LFE49-.LFB49
	.byte	0x4
	.4byte	.LCFI136-.LFB49
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI137-.LCFI136
	.byte	0xe
	.uleb128 0x48
	.byte	0x4
	.4byte	.LCFI138-.LCFI137
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE98:
.LSFDE100:
	.4byte	.LEFDE100-.LASFDE100
.LASFDE100:
	.4byte	.Lframe0
	.4byte	.LFB50
	.4byte	.LFE50-.LFB50
	.byte	0x4
	.4byte	.LCFI139-.LFB50
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI140-.LCFI139
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.4byte	.LCFI141-.LCFI140
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE100:
.LSFDE102:
	.4byte	.LEFDE102-.LASFDE102
.LASFDE102:
	.4byte	.Lframe0
	.4byte	.LFB51
	.4byte	.LFE51-.LFB51
	.byte	0x4
	.4byte	.LCFI142-.LFB51
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI143-.LCFI142
	.byte	0xe
	.uleb128 0x80
	.byte	0x4
	.4byte	.LCFI144-.LCFI143
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE102:
.LSFDE104:
	.4byte	.LEFDE104-.LASFDE104
.LASFDE104:
	.4byte	.Lframe0
	.4byte	.LFB52
	.4byte	.LFE52-.LFB52
	.byte	0x4
	.4byte	.LCFI145-.LFB52
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI146-.LCFI145
	.byte	0xe
	.uleb128 0x60
	.byte	0x4
	.4byte	.LCFI147-.LCFI146
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE104:
.LSFDE106:
	.4byte	.LEFDE106-.LASFDE106
.LASFDE106:
	.4byte	.Lframe0
	.4byte	.LFB53
	.4byte	.LFE53-.LFB53
	.byte	0x4
	.4byte	.LCFI148-.LFB53
	.byte	0xe
	.uleb128 0x8
	.byte	0x84
	.uleb128 0x2
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI149-.LCFI148
	.byte	0xe
	.uleb128 0x98
	.byte	0x4
	.4byte	.LCFI150-.LCFI149
	.byte	0xe
	.uleb128 0x8
	.align	2
.LEFDE106:
.LSFDE108:
	.4byte	.LEFDE108-.LASFDE108
.LASFDE108:
	.4byte	.Lframe0
	.4byte	.LFB54
	.4byte	.LFE54-.LFB54
	.byte	0x4
	.4byte	.LCFI151-.LFB54
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI152-.LCFI151
	.byte	0xe
	.uleb128 0x78
	.byte	0x4
	.4byte	.LCFI153-.LCFI152
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE108:
.LSFDE110:
	.4byte	.LEFDE110-.LASFDE110
.LASFDE110:
	.4byte	.Lframe0
	.4byte	.LFB55
	.4byte	.LFE55-.LFB55
	.byte	0x4
	.4byte	.LCFI154-.LFB55
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI155-.LCFI154
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.4byte	.LCFI156-.LCFI155
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE110:
.LSFDE112:
	.4byte	.LEFDE112-.LASFDE112
.LASFDE112:
	.4byte	.Lframe0
	.4byte	.LFB56
	.4byte	.LFE56-.LFB56
	.byte	0x4
	.4byte	.LCFI157-.LFB56
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI158-.LCFI157
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.4byte	.LCFI159-.LCFI158
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE112:
.LSFDE114:
	.4byte	.LEFDE114-.LASFDE114
.LASFDE114:
	.4byte	.Lframe0
	.4byte	.LFB57
	.4byte	.LFE57-.LFB57
	.byte	0x4
	.4byte	.LCFI160-.LFB57
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI161-.LCFI160
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.4byte	.LCFI162-.LCFI161
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE114:
.LSFDE116:
	.4byte	.LEFDE116-.LASFDE116
.LASFDE116:
	.4byte	.Lframe0
	.4byte	.LFB58
	.4byte	.LFE58-.LFB58
	.byte	0x4
	.4byte	.LCFI163-.LFB58
	.byte	0xe
	.uleb128 0x8
	.byte	0x4
	.4byte	.LCFI164-.LCFI163
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE116:
.LSFDE118:
	.4byte	.LEFDE118-.LASFDE118
.LASFDE118:
	.4byte	.Lframe0
	.4byte	.LFB59
	.4byte	.LFE59-.LFB59
	.byte	0x4
	.4byte	.LCFI165-.LFB59
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI166-.LCFI165
	.byte	0xe
	.uleb128 0x60
	.byte	0x4
	.4byte	.LCFI167-.LCFI166
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE118:
.LSFDE120:
	.4byte	.LEFDE120-.LASFDE120
.LASFDE120:
	.4byte	.Lframe0
	.4byte	.LFB60
	.4byte	.LFE60-.LFB60
	.byte	0x4
	.4byte	.LCFI168-.LFB60
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI169-.LCFI168
	.byte	0xe
	.uleb128 0x60
	.byte	0x4
	.4byte	.LCFI170-.LCFI169
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE120:
.LSFDE122:
	.4byte	.LEFDE122-.LASFDE122
.LASFDE122:
	.4byte	.Lframe0
	.4byte	.LFB61
	.4byte	.LFE61-.LFB61
	.byte	0x4
	.4byte	.LCFI171-.LFB61
	.byte	0xe
	.uleb128 0xc
	.byte	0x81
	.uleb128 0x3
	.byte	0x82
	.uleb128 0x2
	.byte	0x83
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI172-.LCFI171
	.byte	0xe
	.uleb128 0x10
	.byte	0x8e
	.uleb128 0x4
	.byte	0x4
	.4byte	.LCFI173-.LCFI172
	.byte	0xe
	.uleb128 0xa8
	.byte	0x4
	.4byte	.LCFI174-.LCFI173
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.4byte	.LCFI175-.LCFI174
	.byte	0xce
	.byte	0xe
	.uleb128 0xc
	.byte	0x4
	.4byte	.LCFI176-.LCFI175
	.byte	0xc3
	.byte	0xc2
	.byte	0xc1
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE122:
	.text
.Letext0:
	.file 2 "C:\\Nordic\\NAATOS_fw_src\\application\\sdk\\external\\fatfs\\src\\integer.h"
	.file 3 "C:\\Nordic\\NAATOS_fw_src\\application\\sdk\\external\\fatfs\\src\\ff.h"
	.file 4 "C:\\Nordic\\NAATOS_fw_src\\application\\sdk\\external\\fatfs\\src\\diskio.h"
	.file 5 "C:/Program Files/SEGGER/SEGGER Embedded Studio for ARM 5.34/include/__crossworks.h"
	.file 6 "C:/Program Files/SEGGER/SEGGER Embedded Studio for ARM 5.34/include/stdarg.h"
	.file 7 "<built-in>"
	.section	.debug_info,"",%progbits
.Ldebug_info0:
	.4byte	0x24b3
	.2byte	0x4
	.4byte	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF924
	.byte	0xc
	.4byte	.LASF925
	.4byte	.LASF926
	.4byte	.Ldebug_ranges0+0
	.4byte	0
	.4byte	.Ldebug_line0
	.4byte	.Ldebug_macro0
	.uleb128 0x2
	.byte	0x4
	.byte	0x5
	.ascii	"int\000"
	.uleb128 0x3
	.4byte	.LASF730
	.byte	0x2
	.byte	0x13
	.byte	0x16
	.4byte	0x41
	.uleb128 0x4
	.4byte	0x30
	.uleb128 0x5
	.byte	0x4
	.byte	0x7
	.4byte	.LASF729
	.uleb128 0x3
	.4byte	.LASF731
	.byte	0x2
	.byte	0x16
	.byte	0x17
	.4byte	0x59
	.uleb128 0x4
	.4byte	0x48
	.uleb128 0x5
	.byte	0x1
	.byte	0x8
	.4byte	.LASF732
	.uleb128 0x5
	.byte	0x2
	.byte	0x5
	.4byte	.LASF733
	.uleb128 0x3
	.4byte	.LASF734
	.byte	0x2
	.byte	0x1a
	.byte	0x18
	.4byte	0x78
	.uleb128 0x4
	.4byte	0x67
	.uleb128 0x5
	.byte	0x2
	.byte	0x7
	.4byte	.LASF735
	.uleb128 0x3
	.4byte	.LASF736
	.byte	0x2
	.byte	0x1b
	.byte	0x18
	.4byte	0x78
	.uleb128 0x4
	.4byte	0x7f
	.uleb128 0x5
	.byte	0x4
	.byte	0x5
	.4byte	.LASF737
	.uleb128 0x3
	.4byte	.LASF738
	.byte	0x2
	.byte	0x1f
	.byte	0x17
	.4byte	0xa3
	.uleb128 0x5
	.byte	0x4
	.byte	0x7
	.4byte	.LASF739
	.uleb128 0x5
	.byte	0x8
	.byte	0x7
	.4byte	.LASF740
	.uleb128 0x3
	.4byte	.LASF741
	.byte	0x3
	.byte	0x45
	.byte	0xe
	.4byte	0xc2
	.uleb128 0x4
	.4byte	0xb1
	.uleb128 0x5
	.byte	0x1
	.byte	0x8
	.4byte	.LASF742
	.uleb128 0x4
	.4byte	0xc2
	.uleb128 0x3
	.4byte	.LASF743
	.byte	0x3
	.byte	0x55
	.byte	0xf
	.4byte	0x97
	.uleb128 0x6
	.2byte	0x238
	.byte	0x3
	.byte	0x5c
	.byte	0x9
	.4byte	0x1e8
	.uleb128 0x7
	.4byte	.LASF744
	.byte	0x3
	.byte	0x5d
	.byte	0x7
	.4byte	0x48
	.byte	0
	.uleb128 0x8
	.ascii	"drv\000"
	.byte	0x3
	.byte	0x5e
	.byte	0x7
	.4byte	0x48
	.byte	0x1
	.uleb128 0x7
	.4byte	.LASF745
	.byte	0x3
	.byte	0x5f
	.byte	0x7
	.4byte	0x48
	.byte	0x2
	.uleb128 0x7
	.4byte	.LASF746
	.byte	0x3
	.byte	0x60
	.byte	0x7
	.4byte	0x48
	.byte	0x3
	.uleb128 0x7
	.4byte	.LASF747
	.byte	0x3
	.byte	0x61
	.byte	0x7
	.4byte	0x48
	.byte	0x4
	.uleb128 0x8
	.ascii	"id\000"
	.byte	0x3
	.byte	0x62
	.byte	0x7
	.4byte	0x67
	.byte	0x6
	.uleb128 0x7
	.4byte	.LASF748
	.byte	0x3
	.byte	0x63
	.byte	0x7
	.4byte	0x67
	.byte	0x8
	.uleb128 0x7
	.4byte	.LASF749
	.byte	0x3
	.byte	0x64
	.byte	0x7
	.4byte	0x67
	.byte	0xa
	.uleb128 0x7
	.4byte	.LASF750
	.byte	0x3
	.byte	0x69
	.byte	0x9
	.4byte	0x1e8
	.byte	0xc
	.uleb128 0x7
	.4byte	.LASF751
	.byte	0x3
	.byte	0x72
	.byte	0x8
	.4byte	0x97
	.byte	0x10
	.uleb128 0x7
	.4byte	.LASF752
	.byte	0x3
	.byte	0x73
	.byte	0x8
	.4byte	0x97
	.byte	0x14
	.uleb128 0x7
	.4byte	.LASF753
	.byte	0x3
	.byte	0x76
	.byte	0x8
	.4byte	0x97
	.byte	0x18
	.uleb128 0x7
	.4byte	.LASF754
	.byte	0x3
	.byte	0x7d
	.byte	0x8
	.4byte	0x97
	.byte	0x1c
	.uleb128 0x7
	.4byte	.LASF755
	.byte	0x3
	.byte	0x7e
	.byte	0x8
	.4byte	0x97
	.byte	0x20
	.uleb128 0x7
	.4byte	.LASF756
	.byte	0x3
	.byte	0x7f
	.byte	0x8
	.4byte	0x97
	.byte	0x24
	.uleb128 0x7
	.4byte	.LASF757
	.byte	0x3
	.byte	0x80
	.byte	0x8
	.4byte	0x97
	.byte	0x28
	.uleb128 0x7
	.4byte	.LASF758
	.byte	0x3
	.byte	0x81
	.byte	0x8
	.4byte	0x97
	.byte	0x2c
	.uleb128 0x7
	.4byte	.LASF759
	.byte	0x3
	.byte	0x82
	.byte	0x8
	.4byte	0x97
	.byte	0x30
	.uleb128 0x7
	.4byte	.LASF760
	.byte	0x3
	.byte	0x83
	.byte	0x8
	.4byte	0x97
	.byte	0x34
	.uleb128 0x8
	.ascii	"win\000"
	.byte	0x3
	.byte	0x84
	.byte	0x7
	.4byte	0x1ee
	.byte	0x38
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.4byte	0x7f
	.uleb128 0xa
	.4byte	0x48
	.4byte	0x1ff
	.uleb128 0xb
	.4byte	0x41
	.2byte	0x1ff
	.byte	0
	.uleb128 0x3
	.4byte	.LASF761
	.byte	0x3
	.byte	0x85
	.byte	0x3
	.4byte	0xda
	.uleb128 0xc
	.byte	0x10
	.byte	0x3
	.byte	0x8b
	.byte	0x9
	.4byte	0x261
	.uleb128 0x8
	.ascii	"fs\000"
	.byte	0x3
	.byte	0x8c
	.byte	0x9
	.4byte	0x261
	.byte	0
	.uleb128 0x8
	.ascii	"id\000"
	.byte	0x3
	.byte	0x8d
	.byte	0x7
	.4byte	0x67
	.byte	0x4
	.uleb128 0x7
	.4byte	.LASF762
	.byte	0x3
	.byte	0x8e
	.byte	0x7
	.4byte	0x48
	.byte	0x6
	.uleb128 0x7
	.4byte	.LASF763
	.byte	0x3
	.byte	0x8f
	.byte	0x7
	.4byte	0x48
	.byte	0x7
	.uleb128 0x7
	.4byte	.LASF764
	.byte	0x3
	.byte	0x90
	.byte	0x8
	.4byte	0x97
	.byte	0x8
	.uleb128 0x7
	.4byte	.LASF765
	.byte	0x3
	.byte	0x91
	.byte	0xa
	.4byte	0xce
	.byte	0xc
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.4byte	0x1ff
	.uleb128 0x3
	.4byte	.LASF766
	.byte	0x3
	.byte	0x9b
	.byte	0x3
	.4byte	0x20b
	.uleb128 0x6
	.2byte	0x228
	.byte	0x3
	.byte	0xa1
	.byte	0x9
	.4byte	0x2f3
	.uleb128 0x8
	.ascii	"obj\000"
	.byte	0x3
	.byte	0xa2
	.byte	0x8
	.4byte	0x267
	.byte	0
	.uleb128 0x7
	.4byte	.LASF767
	.byte	0x3
	.byte	0xa3
	.byte	0x7
	.4byte	0x48
	.byte	0x10
	.uleb128 0x8
	.ascii	"err\000"
	.byte	0x3
	.byte	0xa4
	.byte	0x7
	.4byte	0x48
	.byte	0x11
	.uleb128 0x7
	.4byte	.LASF768
	.byte	0x3
	.byte	0xa5
	.byte	0xa
	.4byte	0xce
	.byte	0x14
	.uleb128 0x7
	.4byte	.LASF769
	.byte	0x3
	.byte	0xa6
	.byte	0x8
	.4byte	0x97
	.byte	0x18
	.uleb128 0x7
	.4byte	.LASF770
	.byte	0x3
	.byte	0xa7
	.byte	0x8
	.4byte	0x97
	.byte	0x1c
	.uleb128 0x7
	.4byte	.LASF771
	.byte	0x3
	.byte	0xa9
	.byte	0x8
	.4byte	0x97
	.byte	0x20
	.uleb128 0x7
	.4byte	.LASF772
	.byte	0x3
	.byte	0xaa
	.byte	0x8
	.4byte	0x2f3
	.byte	0x24
	.uleb128 0x8
	.ascii	"buf\000"
	.byte	0x3
	.byte	0xb0
	.byte	0x7
	.4byte	0x1ee
	.byte	0x28
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.4byte	0x48
	.uleb128 0xd
	.ascii	"FIL\000"
	.byte	0x3
	.byte	0xb2
	.byte	0x3
	.4byte	0x273
	.uleb128 0xc
	.byte	0x30
	.byte	0x3
	.byte	0xb8
	.byte	0x9
	.4byte	0x369
	.uleb128 0x8
	.ascii	"obj\000"
	.byte	0x3
	.byte	0xb9
	.byte	0x8
	.4byte	0x267
	.byte	0
	.uleb128 0x7
	.4byte	.LASF773
	.byte	0x3
	.byte	0xba
	.byte	0x8
	.4byte	0x97
	.byte	0x10
	.uleb128 0x7
	.4byte	.LASF769
	.byte	0x3
	.byte	0xbb
	.byte	0x8
	.4byte	0x97
	.byte	0x14
	.uleb128 0x7
	.4byte	.LASF770
	.byte	0x3
	.byte	0xbc
	.byte	0x8
	.4byte	0x97
	.byte	0x18
	.uleb128 0x8
	.ascii	"dir\000"
	.byte	0x3
	.byte	0xbd
	.byte	0x8
	.4byte	0x2f3
	.byte	0x1c
	.uleb128 0x8
	.ascii	"fn\000"
	.byte	0x3
	.byte	0xbe
	.byte	0x7
	.4byte	0x369
	.byte	0x20
	.uleb128 0x7
	.4byte	.LASF774
	.byte	0x3
	.byte	0xc0
	.byte	0x8
	.4byte	0x97
	.byte	0x2c
	.byte	0
	.uleb128 0xa
	.4byte	0x48
	.4byte	0x379
	.uleb128 0xe
	.4byte	0x41
	.byte	0xb
	.byte	0
	.uleb128 0xd
	.ascii	"DIR\000"
	.byte	0x3
	.byte	0xc5
	.byte	0x3
	.4byte	0x305
	.uleb128 0x6
	.2byte	0x118
	.byte	0x3
	.byte	0xcb
	.byte	0x9
	.4byte	0x3de
	.uleb128 0x7
	.4byte	.LASF755
	.byte	0x3
	.byte	0xcc
	.byte	0xa
	.4byte	0xce
	.byte	0
	.uleb128 0x7
	.4byte	.LASF775
	.byte	0x3
	.byte	0xcd
	.byte	0x7
	.4byte	0x67
	.byte	0x4
	.uleb128 0x7
	.4byte	.LASF776
	.byte	0x3
	.byte	0xce
	.byte	0x7
	.4byte	0x67
	.byte	0x6
	.uleb128 0x7
	.4byte	.LASF777
	.byte	0x3
	.byte	0xcf
	.byte	0x7
	.4byte	0x48
	.byte	0x8
	.uleb128 0x7
	.4byte	.LASF778
	.byte	0x3
	.byte	0xd1
	.byte	0x8
	.4byte	0x3de
	.byte	0x9
	.uleb128 0x7
	.4byte	.LASF779
	.byte	0x3
	.byte	0xd2
	.byte	0x8
	.4byte	0x3ee
	.byte	0x16
	.byte	0
	.uleb128 0xa
	.4byte	0xb1
	.4byte	0x3ee
	.uleb128 0xe
	.4byte	0x41
	.byte	0xc
	.byte	0
	.uleb128 0xa
	.4byte	0xb1
	.4byte	0x3fe
	.uleb128 0xe
	.4byte	0x41
	.byte	0xff
	.byte	0
	.uleb128 0x3
	.4byte	.LASF780
	.byte	0x3
	.byte	0xd6
	.byte	0x3
	.4byte	0x385
	.uleb128 0xf
	.byte	0x7
	.byte	0x1
	.4byte	0x59
	.byte	0x3
	.byte	0xdc
	.byte	0xe
	.4byte	0x491
	.uleb128 0x10
	.4byte	.LASF781
	.byte	0
	.uleb128 0x10
	.4byte	.LASF782
	.byte	0x1
	.uleb128 0x10
	.4byte	.LASF783
	.byte	0x2
	.uleb128 0x10
	.4byte	.LASF784
	.byte	0x3
	.uleb128 0x10
	.4byte	.LASF785
	.byte	0x4
	.uleb128 0x10
	.4byte	.LASF786
	.byte	0x5
	.uleb128 0x10
	.4byte	.LASF787
	.byte	0x6
	.uleb128 0x10
	.4byte	.LASF788
	.byte	0x7
	.uleb128 0x10
	.4byte	.LASF789
	.byte	0x8
	.uleb128 0x10
	.4byte	.LASF790
	.byte	0x9
	.uleb128 0x10
	.4byte	.LASF791
	.byte	0xa
	.uleb128 0x10
	.4byte	.LASF792
	.byte	0xb
	.uleb128 0x10
	.4byte	.LASF793
	.byte	0xc
	.uleb128 0x10
	.4byte	.LASF794
	.byte	0xd
	.uleb128 0x10
	.4byte	.LASF795
	.byte	0xe
	.uleb128 0x10
	.4byte	.LASF796
	.byte	0xf
	.uleb128 0x10
	.4byte	.LASF797
	.byte	0x10
	.uleb128 0x10
	.4byte	.LASF798
	.byte	0x11
	.uleb128 0x10
	.4byte	.LASF799
	.byte	0x12
	.uleb128 0x10
	.4byte	.LASF800
	.byte	0x13
	.byte	0
	.uleb128 0x3
	.4byte	.LASF801
	.byte	0x3
	.byte	0xf1
	.byte	0x3
	.4byte	0x40a
	.uleb128 0x3
	.4byte	.LASF802
	.byte	0x4
	.byte	0x10
	.byte	0xe
	.4byte	0x48
	.uleb128 0xf
	.byte	0x7
	.byte	0x1
	.4byte	0x59
	.byte	0x4
	.byte	0x13
	.byte	0xe
	.4byte	0x4d6
	.uleb128 0x10
	.4byte	.LASF803
	.byte	0
	.uleb128 0x10
	.4byte	.LASF804
	.byte	0x1
	.uleb128 0x10
	.4byte	.LASF805
	.byte	0x2
	.uleb128 0x10
	.4byte	.LASF806
	.byte	0x3
	.uleb128 0x10
	.4byte	.LASF807
	.byte	0x4
	.byte	0
	.uleb128 0xa
	.4byte	0x261
	.4byte	0x4e6
	.uleb128 0xe
	.4byte	0x41
	.byte	0
	.byte	0
	.uleb128 0x11
	.4byte	.LASF808
	.byte	0x1
	.2byte	0x20d
	.byte	0xf
	.4byte	0x4d6
	.uleb128 0x5
	.byte	0x3
	.4byte	FatFs
	.uleb128 0x11
	.4byte	.LASF809
	.byte	0x1
	.2byte	0x20e
	.byte	0xd
	.4byte	0x67
	.uleb128 0x5
	.byte	0x3
	.4byte	Fsid
	.uleb128 0xa
	.4byte	0x7f
	.4byte	0x51c
	.uleb128 0xe
	.4byte	0x41
	.byte	0xff
	.byte	0
	.uleb128 0x11
	.4byte	.LASF810
	.byte	0x1
	.2byte	0x225
	.byte	0xe
	.4byte	0x50c
	.uleb128 0x5
	.byte	0x3
	.4byte	LfnBuf
	.uleb128 0xa
	.4byte	0x54
	.4byte	0x53f
	.uleb128 0xe
	.4byte	0x41
	.byte	0xc
	.byte	0
	.uleb128 0x4
	.4byte	0x52f
	.uleb128 0x11
	.4byte	.LASF811
	.byte	0x1
	.2byte	0x65b
	.byte	0xc
	.4byte	0x53f
	.uleb128 0x5
	.byte	0x3
	.4byte	LfnOfs
	.uleb128 0x3
	.4byte	.LASF812
	.byte	0x5
	.byte	0x46
	.byte	0x1b
	.4byte	0x563
	.uleb128 0x12
	.4byte	.LASF812
	.byte	0x4
	.byte	0x7
	.byte	0
	.4byte	0x57a
	.uleb128 0x13
	.4byte	.LASF927
	.4byte	0x57a
	.byte	0
	.byte	0
	.uleb128 0x14
	.byte	0x4
	.uleb128 0x9
	.byte	0x4
	.4byte	0xc9
	.uleb128 0x3
	.4byte	.LASF813
	.byte	0x6
	.byte	0x3f
	.byte	0x13
	.4byte	0x557
	.uleb128 0x15
	.byte	0x4c
	.byte	0x1
	.2byte	0x16ba
	.byte	0x9
	.4byte	0x5d0
	.uleb128 0x16
	.ascii	"fp\000"
	.byte	0x1
	.2byte	0x16bb
	.byte	0x7
	.4byte	0x5d0
	.byte	0
	.uleb128 0x16
	.ascii	"idx\000"
	.byte	0x1
	.2byte	0x16bc
	.byte	0x6
	.4byte	0x29
	.byte	0x4
	.uleb128 0x17
	.4byte	.LASF814
	.byte	0x1
	.2byte	0x16bc
	.byte	0xb
	.4byte	0x29
	.byte	0x8
	.uleb128 0x16
	.ascii	"buf\000"
	.byte	0x1
	.2byte	0x16bd
	.byte	0x7
	.4byte	0x5d6
	.byte	0xc
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.4byte	0x2f9
	.uleb128 0xa
	.4byte	0x48
	.4byte	0x5e6
	.uleb128 0xe
	.4byte	0x41
	.byte	0x3f
	.byte	0
	.uleb128 0x18
	.4byte	.LASF815
	.byte	0x1
	.2byte	0x16be
	.byte	0x3
	.4byte	0x58e
	.uleb128 0x19
	.4byte	.LASF816
	.byte	0x1
	.2byte	0x173b
	.byte	0x5
	.4byte	0x29
	.4byte	.LFB61
	.4byte	.LFE61-.LFB61
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x6df
	.uleb128 0x1a
	.ascii	"fp\000"
	.byte	0x1
	.2byte	0x173c
	.byte	0x7
	.4byte	0x5d0
	.uleb128 0x3
	.byte	0x91
	.sleb128 -164
	.uleb128 0x1a
	.ascii	"fmt\000"
	.byte	0x1
	.2byte	0x173d
	.byte	0xf
	.4byte	0x6df
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x1b
	.uleb128 0x1c
	.ascii	"arp\000"
	.byte	0x1
	.2byte	0x1741
	.byte	0xa
	.4byte	0x582
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x1c
	.ascii	"pb\000"
	.byte	0x1
	.2byte	0x1742
	.byte	0xa
	.4byte	0x5e6
	.uleb128 0x3
	.byte	0x91
	.sleb128 -124
	.uleb128 0x1c
	.ascii	"f\000"
	.byte	0x1
	.2byte	0x1743
	.byte	0x7
	.4byte	0x48
	.uleb128 0x2
	.byte	0x91
	.sleb128 -17
	.uleb128 0x1c
	.ascii	"r\000"
	.byte	0x1
	.2byte	0x1743
	.byte	0xa
	.4byte	0x48
	.uleb128 0x2
	.byte	0x91
	.sleb128 -18
	.uleb128 0x1c
	.ascii	"i\000"
	.byte	0x1
	.2byte	0x1744
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x1c
	.ascii	"j\000"
	.byte	0x1
	.2byte	0x1744
	.byte	0xa
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x1c
	.ascii	"w\000"
	.byte	0x1
	.2byte	0x1744
	.byte	0xd
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x1c
	.ascii	"v\000"
	.byte	0x1
	.2byte	0x1745
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x1c
	.ascii	"c\000"
	.byte	0x1
	.2byte	0x1746
	.byte	0x8
	.4byte	0xb1
	.uleb128 0x2
	.byte	0x91
	.sleb128 -37
	.uleb128 0x1c
	.ascii	"d\000"
	.byte	0x1
	.2byte	0x1746
	.byte	0xb
	.4byte	0xb1
	.uleb128 0x2
	.byte	0x91
	.sleb128 -38
	.uleb128 0x1c
	.ascii	"str\000"
	.byte	0x1
	.2byte	0x1746
	.byte	0xe
	.4byte	0x6e5
	.uleb128 0x3
	.byte	0x91
	.sleb128 -156
	.uleb128 0x1c
	.ascii	"p\000"
	.byte	0x1
	.2byte	0x1746
	.byte	0x18
	.4byte	0x6f5
	.uleb128 0x2
	.byte	0x91
	.sleb128 -44
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.4byte	0xbd
	.uleb128 0xa
	.4byte	0xb1
	.4byte	0x6f5
	.uleb128 0xe
	.4byte	0x41
	.byte	0x1f
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.4byte	0xb1
	.uleb128 0x19
	.4byte	.LASF817
	.byte	0x1
	.2byte	0x1727
	.byte	0x5
	.4byte	0x29
	.4byte	.LFB60
	.4byte	.LFE60-.LFB60
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x748
	.uleb128 0x1a
	.ascii	"str\000"
	.byte	0x1
	.2byte	0x1728
	.byte	0xf
	.4byte	0x6df
	.uleb128 0x3
	.byte	0x91
	.sleb128 -92
	.uleb128 0x1a
	.ascii	"fp\000"
	.byte	0x1
	.2byte	0x1729
	.byte	0x7
	.4byte	0x5d0
	.uleb128 0x3
	.byte	0x91
	.sleb128 -96
	.uleb128 0x1c
	.ascii	"pb\000"
	.byte	0x1
	.2byte	0x172c
	.byte	0xa
	.4byte	0x5e6
	.uleb128 0x3
	.byte	0x91
	.sleb128 -84
	.byte	0
	.uleb128 0x19
	.4byte	.LASF818
	.byte	0x1
	.2byte	0x1713
	.byte	0x5
	.4byte	0x29
	.4byte	.LFB59
	.4byte	.LFE59-.LFB59
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x793
	.uleb128 0x1a
	.ascii	"c\000"
	.byte	0x1
	.2byte	0x1714
	.byte	0x8
	.4byte	0xb1
	.uleb128 0x3
	.byte	0x91
	.sleb128 -89
	.uleb128 0x1a
	.ascii	"fp\000"
	.byte	0x1
	.2byte	0x1715
	.byte	0x7
	.4byte	0x5d0
	.uleb128 0x3
	.byte	0x91
	.sleb128 -96
	.uleb128 0x1c
	.ascii	"pb\000"
	.byte	0x1
	.2byte	0x1718
	.byte	0xa
	.4byte	0x5e6
	.uleb128 0x3
	.byte	0x91
	.sleb128 -84
	.byte	0
	.uleb128 0x1d
	.4byte	.LASF819
	.byte	0x1
	.2byte	0x1708
	.byte	0x6
	.4byte	.LFB58
	.4byte	.LFE58-.LFB58
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x7c9
	.uleb128 0x1a
	.ascii	"pb\000"
	.byte	0x1
	.2byte	0x1709
	.byte	0xb
	.4byte	0x7c9
	.uleb128 0x2
	.byte	0x91
	.sleb128 -4
	.uleb128 0x1a
	.ascii	"fp\000"
	.byte	0x1
	.2byte	0x170a
	.byte	0x7
	.4byte	0x5d0
	.uleb128 0x2
	.byte	0x91
	.sleb128 -8
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.4byte	0x5e6
	.uleb128 0x1e
	.4byte	.LASF873
	.byte	0x1
	.2byte	0x16fa
	.byte	0x5
	.4byte	0x29
	.4byte	.LFB57
	.4byte	.LFE57-.LFB57
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x809
	.uleb128 0x1a
	.ascii	"pb\000"
	.byte	0x1
	.2byte	0x16fb
	.byte	0xb
	.4byte	0x7c9
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x1c
	.ascii	"nw\000"
	.byte	0x1
	.2byte	0x16fe
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.byte	0
	.uleb128 0x1f
	.4byte	.LASF820
	.byte	0x1
	.2byte	0x16c2
	.byte	0x6
	.4byte	.LFB56
	.4byte	.LFE56-.LFB56
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x85b
	.uleb128 0x1a
	.ascii	"pb\000"
	.byte	0x1
	.2byte	0x16c3
	.byte	0xb
	.4byte	0x7c9
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x1a
	.ascii	"c\000"
	.byte	0x1
	.2byte	0x16c4
	.byte	0x8
	.4byte	0xb1
	.uleb128 0x2
	.byte	0x91
	.sleb128 -21
	.uleb128 0x1c
	.ascii	"bw\000"
	.byte	0x1
	.2byte	0x16c7
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x1c
	.ascii	"i\000"
	.byte	0x1
	.2byte	0x16c8
	.byte	0x6
	.4byte	0x29
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.byte	0
	.uleb128 0x19
	.4byte	.LASF821
	.byte	0x1
	.2byte	0x166b
	.byte	0x8
	.4byte	0x6f5
	.4byte	.LFB55
	.4byte	.LFE55-.LFB55
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x8ed
	.uleb128 0x20
	.4byte	.LASF822
	.byte	0x1
	.2byte	0x166c
	.byte	0x9
	.4byte	0x6f5
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x1a
	.ascii	"len\000"
	.byte	0x1
	.2byte	0x166d
	.byte	0x6
	.4byte	0x29
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x1a
	.ascii	"fp\000"
	.byte	0x1
	.2byte	0x166e
	.byte	0x7
	.4byte	0x5d0
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x1c
	.ascii	"n\000"
	.byte	0x1
	.2byte	0x1671
	.byte	0x6
	.4byte	0x29
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x1c
	.ascii	"c\000"
	.byte	0x1
	.2byte	0x1672
	.byte	0x8
	.4byte	0xb1
	.uleb128 0x2
	.byte	0x91
	.sleb128 -17
	.uleb128 0x1c
	.ascii	"p\000"
	.byte	0x1
	.2byte	0x1672
	.byte	0xc
	.4byte	0x6f5
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x1c
	.ascii	"s\000"
	.byte	0x1
	.2byte	0x1673
	.byte	0x7
	.4byte	0x8ed
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x1c
	.ascii	"rc\000"
	.byte	0x1
	.2byte	0x1674
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0xa
	.4byte	0x48
	.4byte	0x8fd
	.uleb128 0xe
	.4byte	0x41
	.byte	0x1
	.byte	0
	.uleb128 0x19
	.4byte	.LASF823
	.byte	0x1
	.2byte	0x145b
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB54
	.4byte	.LFE54-.LFB54
	.uleb128 0x1
	.byte	0x9c
	.4byte	0xb47
	.uleb128 0x20
	.4byte	.LASF824
	.byte	0x1
	.2byte	0x145c
	.byte	0xf
	.4byte	0x6df
	.uleb128 0x3
	.byte	0x91
	.sleb128 -108
	.uleb128 0x1a
	.ascii	"opt\000"
	.byte	0x1
	.2byte	0x145d
	.byte	0x7
	.4byte	0x48
	.uleb128 0x3
	.byte	0x91
	.sleb128 -109
	.uleb128 0x1a
	.ascii	"au\000"
	.byte	0x1
	.2byte	0x145e
	.byte	0x8
	.4byte	0x97
	.uleb128 0x3
	.byte	0x91
	.sleb128 -116
	.uleb128 0x20
	.4byte	.LASF825
	.byte	0x1
	.2byte	0x145f
	.byte	0x8
	.4byte	0x57a
	.uleb128 0x3
	.byte	0x91
	.sleb128 -120
	.uleb128 0x1a
	.ascii	"len\000"
	.byte	0x1
	.2byte	0x1460
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x11
	.4byte	.LASF745
	.byte	0x1
	.2byte	0x1463
	.byte	0xd
	.4byte	0x3c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -52
	.uleb128 0x11
	.4byte	.LASF748
	.byte	0x1
	.2byte	0x1464
	.byte	0xd
	.4byte	0x3c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -56
	.uleb128 0x1c
	.ascii	"cst\000"
	.byte	0x1
	.2byte	0x1465
	.byte	0x14
	.4byte	0xb57
	.uleb128 0x5
	.byte	0x3
	.4byte	cst.0
	.uleb128 0x11
	.4byte	.LASF826
	.byte	0x1
	.2byte	0x1466
	.byte	0x14
	.4byte	0xb57
	.uleb128 0x5
	.byte	0x3
	.4byte	cst32.1
	.uleb128 0x1c
	.ascii	"fmt\000"
	.byte	0x1
	.2byte	0x1467
	.byte	0x7
	.4byte	0x48
	.uleb128 0x2
	.byte	0x91
	.sleb128 -41
	.uleb128 0x1c
	.ascii	"sys\000"
	.byte	0x1
	.2byte	0x1467
	.byte	0xc
	.4byte	0x48
	.uleb128 0x2
	.byte	0x91
	.sleb128 -42
	.uleb128 0x1c
	.ascii	"buf\000"
	.byte	0x1
	.2byte	0x1467
	.byte	0x12
	.4byte	0x2f3
	.uleb128 0x3
	.byte	0x91
	.sleb128 -72
	.uleb128 0x1c
	.ascii	"pte\000"
	.byte	0x1
	.2byte	0x1467
	.byte	0x18
	.4byte	0x2f3
	.uleb128 0x3
	.byte	0x91
	.sleb128 -96
	.uleb128 0x11
	.4byte	.LASF827
	.byte	0x1
	.2byte	0x1467
	.byte	0x1d
	.4byte	0x48
	.uleb128 0x2
	.byte	0x91
	.sleb128 -61
	.uleb128 0x11
	.4byte	.LASF828
	.byte	0x1
	.2byte	0x1467
	.byte	0x23
	.4byte	0x48
	.uleb128 0x2
	.byte	0x91
	.sleb128 -62
	.uleb128 0x1c
	.ascii	"ss\000"
	.byte	0x1
	.2byte	0x1468
	.byte	0x7
	.4byte	0x67
	.uleb128 0x3
	.byte	0x91
	.sleb128 -66
	.uleb128 0x11
	.4byte	.LASF829
	.byte	0x1
	.2byte	0x1469
	.byte	0x8
	.4byte	0x97
	.uleb128 0x3
	.byte	0x91
	.sleb128 -80
	.uleb128 0x11
	.4byte	.LASF830
	.byte	0x1
	.2byte	0x1469
	.byte	0x11
	.4byte	0x97
	.uleb128 0x3
	.byte	0x91
	.sleb128 -76
	.uleb128 0x11
	.4byte	.LASF831
	.byte	0x1
	.2byte	0x1469
	.byte	0x19
	.4byte	0x97
	.uleb128 0x3
	.byte	0x91
	.sleb128 -100
	.uleb128 0x11
	.4byte	.LASF832
	.byte	0x1
	.2byte	0x1469
	.byte	0x21
	.4byte	0x97
	.uleb128 0x3
	.byte	0x91
	.sleb128 -88
	.uleb128 0x1c
	.ascii	"pau\000"
	.byte	0x1
	.2byte	0x1469
	.byte	0x29
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x11
	.4byte	.LASF770
	.byte	0x1
	.2byte	0x1469
	.byte	0x2e
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x11
	.4byte	.LASF833
	.byte	0x1
	.2byte	0x1469
	.byte	0x34
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x1c
	.ascii	"n\000"
	.byte	0x1
	.2byte	0x1469
	.byte	0x3b
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x11
	.4byte	.LASF834
	.byte	0x1
	.2byte	0x146a
	.byte	0x8
	.4byte	0x97
	.uleb128 0x3
	.byte	0x91
	.sleb128 -84
	.uleb128 0x11
	.4byte	.LASF835
	.byte	0x1
	.2byte	0x146a
	.byte	0xf
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x11
	.4byte	.LASF836
	.byte	0x1
	.2byte	0x146a
	.byte	0x16
	.4byte	0x97
	.uleb128 0x3
	.byte	0x91
	.sleb128 -92
	.uleb128 0x11
	.4byte	.LASF837
	.byte	0x1
	.2byte	0x146b
	.byte	0x8
	.4byte	0x97
	.uleb128 0x3
	.byte	0x91
	.sleb128 -104
	.uleb128 0x11
	.4byte	.LASF838
	.byte	0x1
	.2byte	0x146b
	.byte	0x10
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x11
	.4byte	.LASF839
	.byte	0x1
	.2byte	0x146b
	.byte	0x18
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x11
	.4byte	.LASF840
	.byte	0x1
	.2byte	0x146b
	.byte	0x20
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x1c
	.ascii	"i\000"
	.byte	0x1
	.2byte	0x146c
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x1c
	.ascii	"vol\000"
	.byte	0x1
	.2byte	0x146d
	.byte	0x6
	.4byte	0x29
	.uleb128 0x2
	.byte	0x91
	.sleb128 -60
	.uleb128 0x11
	.4byte	.LASF763
	.byte	0x1
	.2byte	0x146e
	.byte	0xa
	.4byte	0x49d
	.uleb128 0x2
	.byte	0x91
	.sleb128 -63
	.byte	0
	.uleb128 0xa
	.4byte	0x73
	.4byte	0xb57
	.uleb128 0xe
	.4byte	0x41
	.byte	0x6
	.byte	0
	.uleb128 0x4
	.4byte	0xb47
	.uleb128 0x19
	.4byte	.LASF841
	.byte	0x1
	.2byte	0x121e
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB53
	.4byte	.LFE53-.LFB53
	.uleb128 0x1
	.byte	0x9c
	.4byte	0xc0c
	.uleb128 0x20
	.4byte	.LASF842
	.byte	0x1
	.2byte	0x121f
	.byte	0xf
	.4byte	0x6df
	.uleb128 0x3
	.byte	0x91
	.sleb128 -148
	.uleb128 0x20
	.4byte	.LASF843
	.byte	0x1
	.2byte	0x1220
	.byte	0xf
	.4byte	0x6df
	.uleb128 0x3
	.byte	0x91
	.sleb128 -152
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0x1223
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x1c
	.ascii	"djo\000"
	.byte	0x1
	.2byte	0x1224
	.byte	0x6
	.4byte	0x379
	.uleb128 0x3
	.byte	0x91
	.sleb128 -68
	.uleb128 0x1c
	.ascii	"djn\000"
	.byte	0x1
	.2byte	0x1224
	.byte	0xb
	.4byte	0x379
	.uleb128 0x3
	.byte	0x91
	.sleb128 -116
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0x1225
	.byte	0x9
	.4byte	0x261
	.uleb128 0x3
	.byte	0x91
	.sleb128 -120
	.uleb128 0x1c
	.ascii	"buf\000"
	.byte	0x1
	.2byte	0x1226
	.byte	0x7
	.4byte	0xc0c
	.uleb128 0x3
	.byte	0x91
	.sleb128 -144
	.uleb128 0x1c
	.ascii	"dir\000"
	.byte	0x1
	.2byte	0x1226
	.byte	0x2a
	.4byte	0x2f3
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x1c
	.ascii	"dw\000"
	.byte	0x1
	.2byte	0x1227
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.uleb128 0xa
	.4byte	0x48
	.4byte	0xc1c
	.uleb128 0xe
	.4byte	0x41
	.byte	0x17
	.byte	0
	.uleb128 0x19
	.4byte	.LASF844
	.byte	0x1
	.2byte	0x11c1
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB52
	.4byte	.LFE52-.LFB52
	.uleb128 0x1
	.byte	0x9c
	.4byte	0xcd6
	.uleb128 0x20
	.4byte	.LASF824
	.byte	0x1
	.2byte	0x11c2
	.byte	0xf
	.4byte	0x6df
	.uleb128 0x3
	.byte	0x91
	.sleb128 -92
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0x11c5
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x1c
	.ascii	"dj\000"
	.byte	0x1
	.2byte	0x11c6
	.byte	0x6
	.4byte	0x379
	.uleb128 0x3
	.byte	0x91
	.sleb128 -84
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0x11c7
	.byte	0x9
	.4byte	0x261
	.uleb128 0x3
	.byte	0x91
	.sleb128 -88
	.uleb128 0x1c
	.ascii	"dir\000"
	.byte	0x1
	.2byte	0x11c8
	.byte	0x8
	.4byte	0x2f3
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x1c
	.ascii	"n\000"
	.byte	0x1
	.2byte	0x11c9
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x1c
	.ascii	"dsc\000"
	.byte	0x1
	.2byte	0x11ca
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x1c
	.ascii	"dcl\000"
	.byte	0x1
	.2byte	0x11ca
	.byte	0xd
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x1c
	.ascii	"pcl\000"
	.byte	0x1
	.2byte	0x11ca
	.byte	0x12
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x1c
	.ascii	"tm\000"
	.byte	0x1
	.2byte	0x11ca
	.byte	0x17
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.byte	0
	.uleb128 0x19
	.4byte	.LASF845
	.byte	0x1
	.2byte	0x1162
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB51
	.4byte	.LFE51-.LFB51
	.uleb128 0x1
	.byte	0x9c
	.4byte	0xd53
	.uleb128 0x20
	.4byte	.LASF824
	.byte	0x1
	.2byte	0x1163
	.byte	0xf
	.4byte	0x6df
	.uleb128 0x3
	.byte	0x91
	.sleb128 -124
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0x1166
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x1c
	.ascii	"dj\000"
	.byte	0x1
	.2byte	0x1167
	.byte	0x6
	.4byte	0x379
	.uleb128 0x2
	.byte	0x91
	.sleb128 -64
	.uleb128 0x1c
	.ascii	"sdj\000"
	.byte	0x1
	.2byte	0x1167
	.byte	0xa
	.4byte	0x379
	.uleb128 0x3
	.byte	0x91
	.sleb128 -112
	.uleb128 0x11
	.4byte	.LASF846
	.byte	0x1
	.2byte	0x1168
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0x1169
	.byte	0x9
	.4byte	0x261
	.uleb128 0x3
	.byte	0x91
	.sleb128 -116
	.byte	0
	.uleb128 0x19
	.4byte	.LASF847
	.byte	0x1
	.2byte	0x1130
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB50
	.4byte	.LFE50-.LFB50
	.uleb128 0x1
	.byte	0x9c
	.4byte	0xdad
	.uleb128 0x1a
	.ascii	"fp\000"
	.byte	0x1
	.2byte	0x1131
	.byte	0x7
	.4byte	0x5d0
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0x1134
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0x1135
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x1c
	.ascii	"ncl\000"
	.byte	0x1
	.2byte	0x1136
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.byte	0
	.uleb128 0x19
	.4byte	.LASF848
	.byte	0x1
	.2byte	0x10d9
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB49
	.4byte	.LFE49-.LFB49
	.uleb128 0x1
	.byte	0x9c
	.4byte	0xe85
	.uleb128 0x20
	.4byte	.LASF824
	.byte	0x1
	.2byte	0x10da
	.byte	0xf
	.4byte	0x6df
	.uleb128 0x2
	.byte	0x91
	.sleb128 -60
	.uleb128 0x20
	.4byte	.LASF849
	.byte	0x1
	.2byte	0x10db
	.byte	0x9
	.4byte	0xe85
	.uleb128 0x2
	.byte	0x91
	.sleb128 -64
	.uleb128 0x20
	.4byte	.LASF850
	.byte	0x1
	.2byte	0x10dc
	.byte	0xa
	.4byte	0xe8b
	.uleb128 0x3
	.byte	0x91
	.sleb128 -68
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0x10df
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0x10e0
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x11
	.4byte	.LASF851
	.byte	0x1
	.2byte	0x10e1
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x11
	.4byte	.LASF852
	.byte	0x1
	.2byte	0x10e1
	.byte	0xf
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x11
	.4byte	.LASF770
	.byte	0x1
	.2byte	0x10e1
	.byte	0x15
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x11
	.4byte	.LASF763
	.byte	0x1
	.2byte	0x10e1
	.byte	0x1b
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x1c
	.ascii	"i\000"
	.byte	0x1
	.2byte	0x10e2
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x1c
	.ascii	"p\000"
	.byte	0x1
	.2byte	0x10e3
	.byte	0x8
	.4byte	0x2f3
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x1c
	.ascii	"obj\000"
	.byte	0x1
	.2byte	0x10e4
	.byte	0x8
	.4byte	0x267
	.uleb128 0x2
	.byte	0x91
	.sleb128 -56
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.4byte	0x97
	.uleb128 0x9
	.byte	0x4
	.4byte	0x261
	.uleb128 0x19
	.4byte	.LASF853
	.byte	0x1
	.2byte	0x10b6
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB48
	.4byte	.LFE48-.LFB48
	.uleb128 0x1
	.byte	0x9c
	.4byte	0xeee
	.uleb128 0x20
	.4byte	.LASF824
	.byte	0x1
	.2byte	0x10b7
	.byte	0xf
	.4byte	0x6df
	.uleb128 0x3
	.byte	0x91
	.sleb128 -68
	.uleb128 0x1a
	.ascii	"fno\000"
	.byte	0x1
	.2byte	0x10b8
	.byte	0xb
	.4byte	0xeee
	.uleb128 0x3
	.byte	0x91
	.sleb128 -72
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0x10bb
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x1c
	.ascii	"dj\000"
	.byte	0x1
	.2byte	0x10bc
	.byte	0x6
	.4byte	0x379
	.uleb128 0x2
	.byte	0x91
	.sleb128 -60
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.4byte	0x3fe
	.uleb128 0x19
	.4byte	.LASF854
	.byte	0x1
	.2byte	0x105e
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB47
	.4byte	.LFE47-.LFB47
	.uleb128 0x1
	.byte	0x9c
	.4byte	0xf4e
	.uleb128 0x1a
	.ascii	"dp\000"
	.byte	0x1
	.2byte	0x105f
	.byte	0x7
	.4byte	0xf4e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x1a
	.ascii	"fno\000"
	.byte	0x1
	.2byte	0x1060
	.byte	0xb
	.4byte	0xeee
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0x1063
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0x1064
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.4byte	0x379
	.uleb128 0x19
	.4byte	.LASF855
	.byte	0x1
	.2byte	0x103d
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB46
	.4byte	.LFE46-.LFB46
	.uleb128 0x1
	.byte	0x9c
	.4byte	0xf9e
	.uleb128 0x1a
	.ascii	"dp\000"
	.byte	0x1
	.2byte	0x103e
	.byte	0x7
	.4byte	0xf4e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0x1041
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0x1042
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.byte	0
	.uleb128 0x19
	.4byte	.LASF856
	.byte	0x1
	.2byte	0xff7
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB45
	.4byte	.LFE45-.LFB45
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1008
	.uleb128 0x1a
	.ascii	"dp\000"
	.byte	0x1
	.2byte	0xff8
	.byte	0x7
	.4byte	0xf4e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x20
	.4byte	.LASF824
	.byte	0x1
	.2byte	0xff9
	.byte	0xf
	.4byte	0x6df
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0xffc
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0xffd
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x1c
	.ascii	"obj\000"
	.byte	0x1
	.2byte	0xffe
	.byte	0x9
	.4byte	0x1008
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.4byte	0x267
	.uleb128 0x19
	.4byte	.LASF857
	.byte	0x1
	.2byte	0xf5d
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB44
	.4byte	.LFE44-.LFB44
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x10a8
	.uleb128 0x1a
	.ascii	"fp\000"
	.byte	0x1
	.2byte	0xf5e
	.byte	0x7
	.4byte	0x5d0
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x1a
	.ascii	"ofs\000"
	.byte	0x1
	.2byte	0xf5f
	.byte	0xa
	.4byte	0xce
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0xf62
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0xf63
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x11
	.4byte	.LASF852
	.byte	0x1
	.2byte	0xf64
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x1c
	.ascii	"bcs\000"
	.byte	0x1
	.2byte	0xf64
	.byte	0xe
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x11
	.4byte	.LASF833
	.byte	0x1
	.2byte	0xf64
	.byte	0x13
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x11
	.4byte	.LASF858
	.byte	0x1
	.2byte	0xf65
	.byte	0xa
	.4byte	0xce
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x19
	.4byte	.LASF859
	.byte	0x1
	.2byte	0xed9
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB43
	.4byte	.LFE43-.LFB43
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1103
	.uleb128 0x20
	.4byte	.LASF824
	.byte	0x1
	.2byte	0xeda
	.byte	0xf
	.4byte	0x6df
	.uleb128 0x3
	.byte	0x91
	.sleb128 -68
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0xedd
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x1c
	.ascii	"dj\000"
	.byte	0x1
	.2byte	0xede
	.byte	0x6
	.4byte	0x379
	.uleb128 0x2
	.byte	0x91
	.sleb128 -60
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0xedf
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -64
	.byte	0
	.uleb128 0x19
	.4byte	.LASF860
	.byte	0x1
	.2byte	0xea1
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB42
	.4byte	.LFE42-.LFB42
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x114d
	.uleb128 0x1a
	.ascii	"fp\000"
	.byte	0x1
	.2byte	0xea2
	.byte	0x7
	.4byte	0x5d0
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0xea5
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0xea6
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.byte	0
	.uleb128 0x19
	.4byte	.LASF861
	.byte	0x1
	.2byte	0xe53
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB41
	.4byte	.LFE41-.LFB41
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x11b6
	.uleb128 0x1a
	.ascii	"fp\000"
	.byte	0x1
	.2byte	0xe54
	.byte	0x7
	.4byte	0x5d0
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0xe57
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0xe58
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x1c
	.ascii	"tm\000"
	.byte	0x1
	.2byte	0xe59
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x1c
	.ascii	"dir\000"
	.byte	0x1
	.2byte	0xe5a
	.byte	0x8
	.4byte	0x2f3
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.uleb128 0x19
	.4byte	.LASF862
	.byte	0x1
	.2byte	0xdda
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB40
	.4byte	.LFE40-.LFB40
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x128e
	.uleb128 0x1a
	.ascii	"fp\000"
	.byte	0x1
	.2byte	0xddb
	.byte	0x7
	.4byte	0x5d0
	.uleb128 0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0x20
	.4byte	.LASF822
	.byte	0x1
	.2byte	0xddc
	.byte	0xe
	.4byte	0x128e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x1a
	.ascii	"btw\000"
	.byte	0x1
	.2byte	0xddd
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -52
	.uleb128 0x1a
	.ascii	"bw\000"
	.byte	0x1
	.2byte	0xdde
	.byte	0x8
	.4byte	0x1295
	.uleb128 0x2
	.byte	0x91
	.sleb128 -56
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0xde1
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0xde2
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x11
	.4byte	.LASF852
	.byte	0x1
	.2byte	0xde3
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x11
	.4byte	.LASF770
	.byte	0x1
	.2byte	0xde3
	.byte	0xe
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x11
	.4byte	.LASF863
	.byte	0x1
	.2byte	0xde4
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x1c
	.ascii	"cc\000"
	.byte	0x1
	.2byte	0xde4
	.byte	0xd
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x11
	.4byte	.LASF864
	.byte	0x1
	.2byte	0xde4
	.byte	0x11
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x11
	.4byte	.LASF865
	.byte	0x1
	.2byte	0xde5
	.byte	0xe
	.4byte	0x129b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.4byte	0x1294
	.uleb128 0x21
	.uleb128 0x9
	.byte	0x4
	.4byte	0x30
	.uleb128 0x9
	.byte	0x4
	.4byte	0x54
	.uleb128 0x19
	.4byte	.LASF866
	.byte	0x1
	.2byte	0xd76
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB39
	.4byte	.LFE39-.LFB39
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1389
	.uleb128 0x1a
	.ascii	"fp\000"
	.byte	0x1
	.2byte	0xd77
	.byte	0x7
	.4byte	0x5d0
	.uleb128 0x2
	.byte	0x91
	.sleb128 -52
	.uleb128 0x20
	.4byte	.LASF822
	.byte	0x1
	.2byte	0xd78
	.byte	0x8
	.4byte	0x57a
	.uleb128 0x2
	.byte	0x91
	.sleb128 -56
	.uleb128 0x1a
	.ascii	"btr\000"
	.byte	0x1
	.2byte	0xd79
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -60
	.uleb128 0x1a
	.ascii	"br\000"
	.byte	0x1
	.2byte	0xd7a
	.byte	0x8
	.4byte	0x1295
	.uleb128 0x2
	.byte	0x91
	.sleb128 -64
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0xd7d
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0xd7e
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0x11
	.4byte	.LASF852
	.byte	0x1
	.2byte	0xd7f
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x11
	.4byte	.LASF770
	.byte	0x1
	.2byte	0xd7f
	.byte	0xe
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x11
	.4byte	.LASF867
	.byte	0x1
	.2byte	0xd80
	.byte	0xa
	.4byte	0xce
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x11
	.4byte	.LASF868
	.byte	0x1
	.2byte	0xd81
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x1c
	.ascii	"cc\000"
	.byte	0x1
	.2byte	0xd81
	.byte	0xd
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x11
	.4byte	.LASF864
	.byte	0x1
	.2byte	0xd81
	.byte	0x11
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x11
	.4byte	.LASF869
	.byte	0x1
	.2byte	0xd82
	.byte	0x8
	.4byte	0x2f3
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.byte	0
	.uleb128 0x19
	.4byte	.LASF870
	.byte	0x1
	.2byte	0xcad
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB38
	.4byte	.LFE38-.LFB38
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1464
	.uleb128 0x1a
	.ascii	"fp\000"
	.byte	0x1
	.2byte	0xcae
	.byte	0x7
	.4byte	0x5d0
	.uleb128 0x3
	.byte	0x91
	.sleb128 -92
	.uleb128 0x20
	.4byte	.LASF824
	.byte	0x1
	.2byte	0xcaf
	.byte	0xf
	.4byte	0x6df
	.uleb128 0x3
	.byte	0x91
	.sleb128 -96
	.uleb128 0x20
	.4byte	.LASF871
	.byte	0x1
	.2byte	0xcb0
	.byte	0x7
	.4byte	0x48
	.uleb128 0x3
	.byte	0x91
	.sleb128 -97
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0xcb3
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x1c
	.ascii	"dj\000"
	.byte	0x1
	.2byte	0xcb4
	.byte	0x6
	.4byte	0x379
	.uleb128 0x3
	.byte	0x91
	.sleb128 -84
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0xcb5
	.byte	0x9
	.4byte	0x261
	.uleb128 0x3
	.byte	0x91
	.sleb128 -88
	.uleb128 0x1c
	.ascii	"dw\000"
	.byte	0x1
	.2byte	0xcb7
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x1c
	.ascii	"cl\000"
	.byte	0x1
	.2byte	0xcb7
	.byte	0xc
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x1c
	.ascii	"bcs\000"
	.byte	0x1
	.2byte	0xcb7
	.byte	0x10
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x11
	.4byte	.LASF852
	.byte	0x1
	.2byte	0xcb7
	.byte	0x15
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x1c
	.ascii	"sc\000"
	.byte	0x1
	.2byte	0xcb7
	.byte	0x1b
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x1c
	.ascii	"ofs\000"
	.byte	0x1
	.2byte	0xcb8
	.byte	0xa
	.4byte	0xce
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.uleb128 0x19
	.4byte	.LASF872
	.byte	0x1
	.2byte	0xc7d
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB37
	.4byte	.LFE37-.LFB37
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x14ee
	.uleb128 0x1a
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0xc7e
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x20
	.4byte	.LASF824
	.byte	0x1
	.2byte	0xc7f
	.byte	0xf
	.4byte	0x6df
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x1a
	.ascii	"opt\000"
	.byte	0x1
	.2byte	0xc80
	.byte	0x7
	.4byte	0x48
	.uleb128 0x2
	.byte	0x91
	.sleb128 -33
	.uleb128 0x1c
	.ascii	"cfs\000"
	.byte	0x1
	.2byte	0xc83
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x1c
	.ascii	"vol\000"
	.byte	0x1
	.2byte	0xc84
	.byte	0x6
	.4byte	0x29
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0xc85
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -17
	.uleb128 0x1c
	.ascii	"rp\000"
	.byte	0x1
	.2byte	0xc86
	.byte	0xf
	.4byte	0x6df
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x1e
	.4byte	.LASF874
	.byte	0x1
	.2byte	0xc5b
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB36
	.4byte	.LFE36-.LFB36
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1539
	.uleb128 0x1a
	.ascii	"obj\000"
	.byte	0x1
	.2byte	0xc5c
	.byte	0x9
	.4byte	0x1008
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x1a
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0xc5d
	.byte	0xa
	.4byte	0xe8b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0xc60
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.byte	0
	.uleb128 0x1e
	.4byte	.LASF875
	.byte	0x1
	.2byte	0xb7e
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB35
	.4byte	.LFE35-.LFB35
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1664
	.uleb128 0x20
	.4byte	.LASF824
	.byte	0x1
	.2byte	0xb7f
	.byte	0x10
	.4byte	0x1664
	.uleb128 0x3
	.byte	0x91
	.sleb128 -84
	.uleb128 0x1a
	.ascii	"rfs\000"
	.byte	0x1
	.2byte	0xb80
	.byte	0xa
	.4byte	0xe8b
	.uleb128 0x3
	.byte	0x91
	.sleb128 -88
	.uleb128 0x20
	.4byte	.LASF871
	.byte	0x1
	.2byte	0xb81
	.byte	0x7
	.4byte	0x48
	.uleb128 0x3
	.byte	0x91
	.sleb128 -89
	.uleb128 0x1c
	.ascii	"fmt\000"
	.byte	0x1
	.2byte	0xb84
	.byte	0x7
	.4byte	0x48
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x1c
	.ascii	"pt\000"
	.byte	0x1
	.2byte	0xb84
	.byte	0xd
	.4byte	0x2f3
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x1c
	.ascii	"vol\000"
	.byte	0x1
	.2byte	0xb85
	.byte	0x6
	.4byte	0x29
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x11
	.4byte	.LASF763
	.byte	0x1
	.2byte	0xb86
	.byte	0xa
	.4byte	0x49d
	.uleb128 0x2
	.byte	0x91
	.sleb128 -41
	.uleb128 0x11
	.4byte	.LASF876
	.byte	0x1
	.2byte	0xb87
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x11
	.4byte	.LASF877
	.byte	0x1
	.2byte	0xb87
	.byte	0xf
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x11
	.4byte	.LASF878
	.byte	0x1
	.2byte	0xb87
	.byte	0x17
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x11
	.4byte	.LASF879
	.byte	0x1
	.2byte	0xb87
	.byte	0x1e
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -56
	.uleb128 0x11
	.4byte	.LASF849
	.byte	0x1
	.2byte	0xb87
	.byte	0x26
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -60
	.uleb128 0x11
	.4byte	.LASF880
	.byte	0x1
	.2byte	0xb87
	.byte	0x2d
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x1c
	.ascii	"br\000"
	.byte	0x1
	.2byte	0xb87
	.byte	0x35
	.4byte	0x166a
	.uleb128 0x3
	.byte	0x91
	.sleb128 -76
	.uleb128 0x11
	.4byte	.LASF881
	.byte	0x1
	.2byte	0xb88
	.byte	0x7
	.4byte	0x67
	.uleb128 0x2
	.byte	0x91
	.sleb128 -50
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0xb89
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x1c
	.ascii	"i\000"
	.byte	0x1
	.2byte	0xb8a
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.4byte	0x6df
	.uleb128 0xa
	.4byte	0x97
	.4byte	0x167a
	.uleb128 0xe
	.4byte	0x41
	.byte	0x3
	.byte	0
	.uleb128 0x1e
	.4byte	.LASF882
	.byte	0x1
	.2byte	0xb62
	.byte	0x6
	.4byte	0x48
	.4byte	.LFB34
	.4byte	.LFE34-.LFB34
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x16b5
	.uleb128 0x1a
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0xb63
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x20
	.4byte	.LASF770
	.byte	0x1
	.2byte	0xb64
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.byte	0
	.uleb128 0x22
	.4byte	.LASF883
	.byte	0x1
	.2byte	0xb25
	.byte	0x5
	.4byte	0x29
	.4byte	.LFB33
	.4byte	.LFE33-.LFB33
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x171d
	.uleb128 0x20
	.4byte	.LASF824
	.byte	0x1
	.2byte	0xb26
	.byte	0x10
	.4byte	0x1664
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x1c
	.ascii	"tp\000"
	.byte	0x1
	.2byte	0xb29
	.byte	0xf
	.4byte	0x6df
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x1c
	.ascii	"tt\000"
	.byte	0x1
	.2byte	0xb29
	.byte	0x14
	.4byte	0x6df
	.uleb128 0x2
	.byte	0x91
	.sleb128 -4
	.uleb128 0x1c
	.ascii	"i\000"
	.byte	0x1
	.2byte	0xb2a
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x1c
	.ascii	"vol\000"
	.byte	0x1
	.2byte	0xb2b
	.byte	0x6
	.4byte	0x29
	.uleb128 0x2
	.byte	0x91
	.sleb128 -8
	.byte	0
	.uleb128 0x1e
	.4byte	.LASF884
	.byte	0x1
	.2byte	0xacd
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB32
	.4byte	.LFE32-.LFB32
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1796
	.uleb128 0x1a
	.ascii	"dp\000"
	.byte	0x1
	.2byte	0xace
	.byte	0x7
	.4byte	0xf4e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x20
	.4byte	.LASF824
	.byte	0x1
	.2byte	0xacf
	.byte	0xf
	.4byte	0x6df
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0xad2
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x1c
	.ascii	"ns\000"
	.byte	0x1
	.2byte	0xad3
	.byte	0x7
	.4byte	0x48
	.uleb128 0x2
	.byte	0x91
	.sleb128 -21
	.uleb128 0x1c
	.ascii	"obj\000"
	.byte	0x1
	.2byte	0xad4
	.byte	0x9
	.4byte	0x1008
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0xad5
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.uleb128 0x1e
	.4byte	.LASF885
	.byte	0x1
	.2byte	0xa0a
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB31
	.4byte	.LFE31-.LFB31
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1855
	.uleb128 0x1a
	.ascii	"dp\000"
	.byte	0x1
	.2byte	0xa0b
	.byte	0x7
	.4byte	0xf4e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0x20
	.4byte	.LASF824
	.byte	0x1
	.2byte	0xa0c
	.byte	0x10
	.4byte	0x1664
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x1c
	.ascii	"b\000"
	.byte	0x1
	.2byte	0xa10
	.byte	0x7
	.4byte	0x48
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x1c
	.ascii	"cf\000"
	.byte	0x1
	.2byte	0xa10
	.byte	0xa
	.4byte	0x48
	.uleb128 0x2
	.byte	0x91
	.sleb128 -10
	.uleb128 0x1c
	.ascii	"w\000"
	.byte	0x1
	.2byte	0xa11
	.byte	0x8
	.4byte	0x7f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x1c
	.ascii	"lfn\000"
	.byte	0x1
	.2byte	0xa11
	.byte	0xc
	.4byte	0x1e8
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x1c
	.ascii	"i\000"
	.byte	0x1
	.2byte	0xa12
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x1c
	.ascii	"ni\000"
	.byte	0x1
	.2byte	0xa12
	.byte	0xa
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x1c
	.ascii	"si\000"
	.byte	0x1
	.2byte	0xa12
	.byte	0xe
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x1c
	.ascii	"di\000"
	.byte	0x1
	.2byte	0xa12
	.byte	0x12
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x1c
	.ascii	"p\000"
	.byte	0x1
	.2byte	0xa13
	.byte	0xf
	.4byte	0x6df
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.byte	0
	.uleb128 0x1f
	.4byte	.LASF886
	.byte	0x1
	.2byte	0x95c
	.byte	0x6
	.4byte	.LFB30
	.4byte	.LFE30-.LFB30
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x18f2
	.uleb128 0x1a
	.ascii	"dp\000"
	.byte	0x1
	.2byte	0x95d
	.byte	0x7
	.4byte	0xf4e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x1a
	.ascii	"fno\000"
	.byte	0x1
	.2byte	0x95e
	.byte	0xb
	.4byte	0xeee
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x1c
	.ascii	"i\000"
	.byte	0x1
	.2byte	0x961
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x1c
	.ascii	"j\000"
	.byte	0x1
	.2byte	0x961
	.byte	0xa
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x1c
	.ascii	"c\000"
	.byte	0x1
	.2byte	0x962
	.byte	0x8
	.4byte	0xb1
	.uleb128 0x2
	.byte	0x91
	.sleb128 -17
	.uleb128 0x1c
	.ascii	"tm\000"
	.byte	0x1
	.2byte	0x963
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x1c
	.ascii	"w\000"
	.byte	0x1
	.2byte	0x965
	.byte	0x8
	.4byte	0x7f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -26
	.uleb128 0x1c
	.ascii	"lfv\000"
	.byte	0x1
	.2byte	0x965
	.byte	0xb
	.4byte	0x7f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0x966
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x1e
	.4byte	.LASF887
	.byte	0x1
	.2byte	0x92c
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB29
	.4byte	.LFE29-.LFB29
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x194c
	.uleb128 0x1a
	.ascii	"dp\000"
	.byte	0x1
	.2byte	0x92d
	.byte	0x7
	.4byte	0xf4e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0x930
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0x931
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x11
	.4byte	.LASF888
	.byte	0x1
	.2byte	0x933
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.uleb128 0x1e
	.4byte	.LASF889
	.byte	0x1
	.2byte	0x8c5
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB28
	.4byte	.LFE28-.LFB28
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x19e3
	.uleb128 0x1a
	.ascii	"dp\000"
	.byte	0x1
	.2byte	0x8c6
	.byte	0x7
	.4byte	0xf4e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -52
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0x8c9
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0x8ca
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x1c
	.ascii	"n\000"
	.byte	0x1
	.2byte	0x8cc
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x11
	.4byte	.LASF890
	.byte	0x1
	.2byte	0x8cc
	.byte	0xa
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x11
	.4byte	.LASF891
	.byte	0x1
	.2byte	0x8cc
	.byte	0x10
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x1c
	.ascii	"sn\000"
	.byte	0x1
	.2byte	0x8cd
	.byte	0x7
	.4byte	0x369
	.uleb128 0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0x1c
	.ascii	"sum\000"
	.byte	0x1
	.2byte	0x8cd
	.byte	0xf
	.4byte	0x48
	.uleb128 0x2
	.byte	0x91
	.sleb128 -29
	.byte	0
	.uleb128 0x1e
	.4byte	.LASF892
	.byte	0x1
	.2byte	0x876
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB27
	.4byte	.LFE27-.LFB27
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1a69
	.uleb128 0x1a
	.ascii	"dp\000"
	.byte	0x1
	.2byte	0x877
	.byte	0x7
	.4byte	0xf4e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0x87a
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0x87b
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x1c
	.ascii	"c\000"
	.byte	0x1
	.2byte	0x87c
	.byte	0x7
	.4byte	0x48
	.uleb128 0x2
	.byte	0x91
	.sleb128 -10
	.uleb128 0x1c
	.ascii	"a\000"
	.byte	0x1
	.2byte	0x87e
	.byte	0x7
	.4byte	0x48
	.uleb128 0x2
	.byte	0x91
	.sleb128 -17
	.uleb128 0x1c
	.ascii	"ord\000"
	.byte	0x1
	.2byte	0x87e
	.byte	0xa
	.4byte	0x48
	.uleb128 0x2
	.byte	0x91
	.sleb128 -11
	.uleb128 0x1c
	.ascii	"sum\000"
	.byte	0x1
	.2byte	0x87e
	.byte	0xf
	.4byte	0x48
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.byte	0
	.uleb128 0x1e
	.4byte	.LASF893
	.byte	0x1
	.2byte	0x829
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB26
	.4byte	.LFE26-.LFB26
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1aff
	.uleb128 0x1a
	.ascii	"dp\000"
	.byte	0x1
	.2byte	0x82a
	.byte	0x7
	.4byte	0xf4e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x1a
	.ascii	"vol\000"
	.byte	0x1
	.2byte	0x82b
	.byte	0x6
	.4byte	0x29
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0x82e
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0x82f
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x1c
	.ascii	"a\000"
	.byte	0x1
	.2byte	0x830
	.byte	0x7
	.4byte	0x48
	.uleb128 0x2
	.byte	0x91
	.sleb128 -17
	.uleb128 0x1c
	.ascii	"c\000"
	.byte	0x1
	.2byte	0x830
	.byte	0xa
	.4byte	0x48
	.uleb128 0x2
	.byte	0x91
	.sleb128 -10
	.uleb128 0x1c
	.ascii	"ord\000"
	.byte	0x1
	.2byte	0x832
	.byte	0x7
	.4byte	0x48
	.uleb128 0x2
	.byte	0x91
	.sleb128 -11
	.uleb128 0x1c
	.ascii	"sum\000"
	.byte	0x1
	.2byte	0x832
	.byte	0x13
	.4byte	0x48
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.byte	0
	.uleb128 0x22
	.4byte	.LASF894
	.byte	0x1
	.2byte	0x70d
	.byte	0x6
	.4byte	0x48
	.4byte	.LFB25
	.4byte	.LFE25-.LFB25
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1b49
	.uleb128 0x1a
	.ascii	"dir\000"
	.byte	0x1
	.2byte	0x70e
	.byte	0xe
	.4byte	0x129b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x1c
	.ascii	"sum\000"
	.byte	0x1
	.2byte	0x711
	.byte	0x7
	.4byte	0x48
	.uleb128 0x2
	.byte	0x91
	.sleb128 -1
	.uleb128 0x1c
	.ascii	"n\000"
	.byte	0x1
	.2byte	0x712
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -8
	.byte	0
	.uleb128 0x1f
	.4byte	.LASF895
	.byte	0x1
	.2byte	0x6d2
	.byte	0x6
	.4byte	.LFB24
	.4byte	.LFE24-.LFB24
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1bf8
	.uleb128 0x1a
	.ascii	"dst\000"
	.byte	0x1
	.2byte	0x6d3
	.byte	0x8
	.4byte	0x2f3
	.uleb128 0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0x1a
	.ascii	"src\000"
	.byte	0x1
	.2byte	0x6d4
	.byte	0xe
	.4byte	0x129b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x1a
	.ascii	"lfn\000"
	.byte	0x1
	.2byte	0x6d5
	.byte	0xf
	.4byte	0x1bf8
	.uleb128 0x2
	.byte	0x91
	.sleb128 -52
	.uleb128 0x1a
	.ascii	"seq\000"
	.byte	0x1
	.2byte	0x6d6
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -56
	.uleb128 0x1c
	.ascii	"ns\000"
	.byte	0x1
	.2byte	0x6d9
	.byte	0x7
	.4byte	0x1bfe
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x1c
	.ascii	"c\000"
	.byte	0x1
	.2byte	0x6d9
	.byte	0xe
	.4byte	0x48
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x1c
	.ascii	"i\000"
	.byte	0x1
	.2byte	0x6da
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x1c
	.ascii	"j\000"
	.byte	0x1
	.2byte	0x6da
	.byte	0xa
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x1c
	.ascii	"wc\000"
	.byte	0x1
	.2byte	0x6db
	.byte	0x8
	.4byte	0x7f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -22
	.uleb128 0x1c
	.ascii	"sr\000"
	.byte	0x1
	.2byte	0x6dc
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.4byte	0x8b
	.uleb128 0xa
	.4byte	0x48
	.4byte	0x1c0e
	.uleb128 0xe
	.4byte	0x41
	.byte	0x7
	.byte	0
	.uleb128 0x1f
	.4byte	.LASF896
	.byte	0x1
	.2byte	0x6ac
	.byte	0x6
	.4byte	.LFB23
	.4byte	.LFE23-.LFB23
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1c91
	.uleb128 0x1a
	.ascii	"lfn\000"
	.byte	0x1
	.2byte	0x6ad
	.byte	0xf
	.4byte	0x1bf8
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x1a
	.ascii	"dir\000"
	.byte	0x1
	.2byte	0x6ae
	.byte	0x8
	.4byte	0x2f3
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x1a
	.ascii	"ord\000"
	.byte	0x1
	.2byte	0x6af
	.byte	0x7
	.4byte	0x48
	.uleb128 0x2
	.byte	0x91
	.sleb128 -33
	.uleb128 0x1a
	.ascii	"sum\000"
	.byte	0x1
	.2byte	0x6b0
	.byte	0x7
	.4byte	0x48
	.uleb128 0x2
	.byte	0x91
	.sleb128 -34
	.uleb128 0x1c
	.ascii	"i\000"
	.byte	0x1
	.2byte	0x6b3
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x1c
	.ascii	"s\000"
	.byte	0x1
	.2byte	0x6b3
	.byte	0xa
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x1c
	.ascii	"wc\000"
	.byte	0x1
	.2byte	0x6b4
	.byte	0x8
	.4byte	0x7f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -18
	.byte	0
	.uleb128 0x1e
	.4byte	.LASF897
	.byte	0x1
	.2byte	0x686
	.byte	0x5
	.4byte	0x29
	.4byte	.LFB22
	.4byte	.LFE22-.LFB22
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1d07
	.uleb128 0x20
	.4byte	.LASF750
	.byte	0x1
	.2byte	0x687
	.byte	0x9
	.4byte	0x1e8
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x1a
	.ascii	"dir\000"
	.byte	0x1
	.2byte	0x688
	.byte	0x8
	.4byte	0x2f3
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x1c
	.ascii	"i\000"
	.byte	0x1
	.2byte	0x68b
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x1c
	.ascii	"s\000"
	.byte	0x1
	.2byte	0x68b
	.byte	0xa
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x1c
	.ascii	"wc\000"
	.byte	0x1
	.2byte	0x68c
	.byte	0x8
	.4byte	0x7f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -18
	.uleb128 0x1c
	.ascii	"uc\000"
	.byte	0x1
	.2byte	0x68c
	.byte	0xc
	.4byte	0x7f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.uleb128 0x1e
	.4byte	.LASF898
	.byte	0x1
	.2byte	0x662
	.byte	0x5
	.4byte	0x29
	.4byte	.LFB21
	.4byte	.LFE21-.LFB21
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1d7d
	.uleb128 0x20
	.4byte	.LASF750
	.byte	0x1
	.2byte	0x663
	.byte	0xf
	.4byte	0x1bf8
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x1a
	.ascii	"dir\000"
	.byte	0x1
	.2byte	0x664
	.byte	0x8
	.4byte	0x2f3
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x1c
	.ascii	"i\000"
	.byte	0x1
	.2byte	0x667
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x1c
	.ascii	"s\000"
	.byte	0x1
	.2byte	0x667
	.byte	0xa
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x1c
	.ascii	"wc\000"
	.byte	0x1
	.2byte	0x668
	.byte	0x8
	.4byte	0x7f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -18
	.uleb128 0x1c
	.ascii	"uc\000"
	.byte	0x1
	.2byte	0x668
	.byte	0xc
	.4byte	0x7f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.uleb128 0x1f
	.4byte	.LASF899
	.byte	0x1
	.2byte	0x647
	.byte	0x6
	.4byte	.LFB20
	.4byte	.LFE20-.LFB20
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1dc3
	.uleb128 0x1a
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0x648
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x1a
	.ascii	"dir\000"
	.byte	0x1
	.2byte	0x649
	.byte	0x8
	.4byte	0x2f3
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x1a
	.ascii	"cl\000"
	.byte	0x1
	.2byte	0x64a
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.uleb128 0x1e
	.4byte	.LASF900
	.byte	0x1
	.2byte	0x635
	.byte	0x7
	.4byte	0x97
	.4byte	.LFB19
	.4byte	.LFE19-.LFB19
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1e0d
	.uleb128 0x1a
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0x636
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x1a
	.ascii	"dir\000"
	.byte	0x1
	.2byte	0x637
	.byte	0xe
	.4byte	0x129b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x1c
	.ascii	"cl\000"
	.byte	0x1
	.2byte	0x63a
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.byte	0
	.uleb128 0x1e
	.4byte	.LASF901
	.byte	0x1
	.2byte	0x60a
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB18
	.4byte	.LFE18-.LFB18
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1e75
	.uleb128 0x1a
	.ascii	"dp\000"
	.byte	0x1
	.2byte	0x60b
	.byte	0x7
	.4byte	0xf4e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x20
	.4byte	.LASF891
	.byte	0x1
	.2byte	0x60c
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0x60f
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x1c
	.ascii	"n\000"
	.byte	0x1
	.2byte	0x610
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0x611
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.uleb128 0x1e
	.4byte	.LASF902
	.byte	0x1
	.2byte	0x5c4
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB17
	.4byte	.LFE17-.LFB17
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1eed
	.uleb128 0x1a
	.ascii	"dp\000"
	.byte	0x1
	.2byte	0x5c5
	.byte	0x7
	.4byte	0xf4e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x20
	.4byte	.LASF903
	.byte	0x1
	.2byte	0x5c6
	.byte	0x6
	.4byte	0x29
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x1c
	.ascii	"ofs\000"
	.byte	0x1
	.2byte	0x5c9
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x11
	.4byte	.LASF852
	.byte	0x1
	.2byte	0x5c9
	.byte	0xd
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0x5ca
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x1c
	.ascii	"n\000"
	.byte	0x1
	.2byte	0x5cc
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.byte	0
	.uleb128 0x1e
	.4byte	.LASF904
	.byte	0x1
	.2byte	0x593
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB16
	.4byte	.LFE16-.LFB16
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1f57
	.uleb128 0x1a
	.ascii	"dp\000"
	.byte	0x1
	.2byte	0x594
	.byte	0x7
	.4byte	0xf4e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x1a
	.ascii	"ofs\000"
	.byte	0x1
	.2byte	0x595
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x1c
	.ascii	"csz\000"
	.byte	0x1
	.2byte	0x598
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x11
	.4byte	.LASF852
	.byte	0x1
	.2byte	0x598
	.byte	0xd
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0x599
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.byte	0
	.uleb128 0x1e
	.4byte	.LASF905
	.byte	0x1
	.2byte	0x520
	.byte	0x7
	.4byte	0x97
	.4byte	.LFB15
	.4byte	.LFE15-.LFB15
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1fe1
	.uleb128 0x1a
	.ascii	"obj\000"
	.byte	0x1
	.2byte	0x521
	.byte	0x9
	.4byte	0x1008
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x20
	.4byte	.LASF852
	.byte	0x1
	.2byte	0x522
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x1c
	.ascii	"cs\000"
	.byte	0x1
	.2byte	0x525
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x1c
	.ascii	"ncl\000"
	.byte	0x1
	.2byte	0x525
	.byte	0xc
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x1c
	.ascii	"scl\000"
	.byte	0x1
	.2byte	0x525
	.byte	0x11
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0x526
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -17
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0x527
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x1e
	.4byte	.LASF906
	.byte	0x1
	.2byte	0x4d0
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB14
	.4byte	.LFE14-.LFB14
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x205c
	.uleb128 0x1a
	.ascii	"obj\000"
	.byte	0x1
	.2byte	0x4d1
	.byte	0x9
	.4byte	0x1008
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x20
	.4byte	.LASF852
	.byte	0x1
	.2byte	0x4d2
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x20
	.4byte	.LASF907
	.byte	0x1
	.2byte	0x4d3
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0x4d6
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0x1c
	.ascii	"nxt\000"
	.byte	0x1
	.2byte	0x4d7
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0x4d8
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.byte	0
	.uleb128 0x1e
	.4byte	.LASF908
	.byte	0x1
	.2byte	0x426
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB13
	.4byte	.LFE13-.LFB13
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x20d4
	.uleb128 0x1a
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0x427
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x20
	.4byte	.LASF852
	.byte	0x1
	.2byte	0x428
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x1a
	.ascii	"val\000"
	.byte	0x1
	.2byte	0x429
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x1c
	.ascii	"bc\000"
	.byte	0x1
	.2byte	0x42c
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x1c
	.ascii	"p\000"
	.byte	0x1
	.2byte	0x42d
	.byte	0x8
	.4byte	0x2f3
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0x42e
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.byte	0
	.uleb128 0x1e
	.4byte	.LASF909
	.byte	0x1
	.2byte	0x3da
	.byte	0x7
	.4byte	0x97
	.4byte	.LFB12
	.4byte	.LFE12-.LFB12
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x214d
	.uleb128 0x1a
	.ascii	"obj\000"
	.byte	0x1
	.2byte	0x3db
	.byte	0x9
	.4byte	0x1008
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x20
	.4byte	.LASF852
	.byte	0x1
	.2byte	0x3dc
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x1c
	.ascii	"wc\000"
	.byte	0x1
	.2byte	0x3df
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x1c
	.ascii	"bc\000"
	.byte	0x1
	.2byte	0x3df
	.byte	0xb
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x1c
	.ascii	"val\000"
	.byte	0x1
	.2byte	0x3e0
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x1c
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0x3e1
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.byte	0
	.uleb128 0x22
	.4byte	.LASF910
	.byte	0x1
	.2byte	0x3c8
	.byte	0x7
	.4byte	0x97
	.4byte	.LFB11
	.4byte	.LFE11-.LFB11
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x2188
	.uleb128 0x1a
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0x3c9
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -4
	.uleb128 0x20
	.4byte	.LASF852
	.byte	0x1
	.2byte	0x3ca
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -8
	.byte	0
	.uleb128 0x1e
	.4byte	.LASF911
	.byte	0x1
	.2byte	0x3a1
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB10
	.4byte	.LFE10-.LFB10
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x21c3
	.uleb128 0x1a
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0x3a2
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0x3a5
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.byte	0
	.uleb128 0x1e
	.4byte	.LASF912
	.byte	0x1
	.2byte	0x381
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB9
	.4byte	.LFE9-.LFB9
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x220e
	.uleb128 0x1a
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0x382
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x20
	.4byte	.LASF913
	.byte	0x1
	.2byte	0x383
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0x386
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.byte	0
	.uleb128 0x1e
	.4byte	.LASF914
	.byte	0x1
	.2byte	0x364
	.byte	0x9
	.4byte	0x491
	.4byte	.LFB8
	.4byte	.LFE8-.LFB8
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x2268
	.uleb128 0x1a
	.ascii	"fs\000"
	.byte	0x1
	.2byte	0x365
	.byte	0x9
	.4byte	0x261
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x11
	.4byte	.LASF915
	.byte	0x1
	.2byte	0x368
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x1c
	.ascii	"nf\000"
	.byte	0x1
	.2byte	0x369
	.byte	0x7
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x1c
	.ascii	"res\000"
	.byte	0x1
	.2byte	0x36a
	.byte	0xa
	.4byte	0x491
	.uleb128 0x2
	.byte	0x91
	.sleb128 -17
	.byte	0
	.uleb128 0x22
	.4byte	.LASF916
	.byte	0x1
	.2byte	0x2ca
	.byte	0x5
	.4byte	0x29
	.4byte	.LFB7
	.4byte	.LFE7-.LFB7
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x22a4
	.uleb128 0x1a
	.ascii	"str\000"
	.byte	0x1
	.2byte	0x2ca
	.byte	0x1a
	.4byte	0x57c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -4
	.uleb128 0x1a
	.ascii	"chr\000"
	.byte	0x1
	.2byte	0x2ca
	.byte	0x23
	.4byte	0x29
	.uleb128 0x2
	.byte	0x91
	.sleb128 -8
	.byte	0
	.uleb128 0x22
	.4byte	.LASF917
	.byte	0x1
	.2byte	0x2bd
	.byte	0x5
	.4byte	0x29
	.4byte	.LFB6
	.4byte	.LFE6-.LFB6
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x231a
	.uleb128 0x1a
	.ascii	"dst\000"
	.byte	0x1
	.2byte	0x2bd
	.byte	0x1a
	.4byte	0x128e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x1a
	.ascii	"src\000"
	.byte	0x1
	.2byte	0x2bd
	.byte	0x2b
	.4byte	0x128e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x1a
	.ascii	"cnt\000"
	.byte	0x1
	.2byte	0x2bd
	.byte	0x35
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x1c
	.ascii	"d\000"
	.byte	0x1
	.2byte	0x2be
	.byte	0xe
	.4byte	0x129b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -4
	.uleb128 0x1c
	.ascii	"s\000"
	.byte	0x1
	.2byte	0x2be
	.byte	0x26
	.4byte	0x129b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -8
	.uleb128 0x1c
	.ascii	"r\000"
	.byte	0x1
	.2byte	0x2bf
	.byte	0x6
	.4byte	0x29
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.byte	0
	.uleb128 0x1d
	.4byte	.LASF918
	.byte	0x1
	.2byte	0x2b5
	.byte	0x6
	.4byte	.LFB5
	.4byte	.LFE5-.LFB5
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x2370
	.uleb128 0x1a
	.ascii	"dst\000"
	.byte	0x1
	.2byte	0x2b5
	.byte	0x15
	.4byte	0x57a
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x1a
	.ascii	"val\000"
	.byte	0x1
	.2byte	0x2b5
	.byte	0x1e
	.4byte	0x29
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x1a
	.ascii	"cnt\000"
	.byte	0x1
	.2byte	0x2b5
	.byte	0x28
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x1c
	.ascii	"d\000"
	.byte	0x1
	.2byte	0x2b6
	.byte	0x8
	.4byte	0x2f3
	.uleb128 0x2
	.byte	0x91
	.sleb128 -4
	.byte	0
	.uleb128 0x1d
	.4byte	.LASF919
	.byte	0x1
	.2byte	0x2aa
	.byte	0x6
	.4byte	.LFB4
	.4byte	.LFE4-.LFB4
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x23d4
	.uleb128 0x1a
	.ascii	"dst\000"
	.byte	0x1
	.2byte	0x2aa
	.byte	0x15
	.4byte	0x57a
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x1a
	.ascii	"src\000"
	.byte	0x1
	.2byte	0x2aa
	.byte	0x26
	.4byte	0x128e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x1a
	.ascii	"cnt\000"
	.byte	0x1
	.2byte	0x2aa
	.byte	0x30
	.4byte	0x30
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x1c
	.ascii	"d\000"
	.byte	0x1
	.2byte	0x2ab
	.byte	0x8
	.4byte	0x2f3
	.uleb128 0x2
	.byte	0x91
	.sleb128 -4
	.uleb128 0x1c
	.ascii	"s\000"
	.byte	0x1
	.2byte	0x2ac
	.byte	0xe
	.4byte	0x129b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -8
	.byte	0
	.uleb128 0x1d
	.4byte	.LASF920
	.byte	0x1
	.2byte	0x28a
	.byte	0x6
	.4byte	.LFB3
	.4byte	.LFE3-.LFB3
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x240c
	.uleb128 0x1a
	.ascii	"ptr\000"
	.byte	0x1
	.2byte	0x28a
	.byte	0x16
	.4byte	0x2f3
	.uleb128 0x2
	.byte	0x91
	.sleb128 -4
	.uleb128 0x1a
	.ascii	"val\000"
	.byte	0x1
	.2byte	0x28a
	.byte	0x21
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -8
	.byte	0
	.uleb128 0x1d
	.4byte	.LASF921
	.byte	0x1
	.2byte	0x283
	.byte	0x6
	.4byte	.LFB2
	.4byte	.LFE2-.LFB2
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x2444
	.uleb128 0x1a
	.ascii	"ptr\000"
	.byte	0x1
	.2byte	0x283
	.byte	0x15
	.4byte	0x2f3
	.uleb128 0x2
	.byte	0x91
	.sleb128 -4
	.uleb128 0x1a
	.ascii	"val\000"
	.byte	0x1
	.2byte	0x283
	.byte	0x1f
	.4byte	0x67
	.uleb128 0x2
	.byte	0x91
	.sleb128 -6
	.byte	0
	.uleb128 0x22
	.4byte	.LASF922
	.byte	0x1
	.2byte	0x264
	.byte	0x7
	.4byte	0x97
	.4byte	.LFB1
	.4byte	.LFE1-.LFB1
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x247f
	.uleb128 0x1a
	.ascii	"ptr\000"
	.byte	0x1
	.2byte	0x264
	.byte	0x1d
	.4byte	0x129b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x1c
	.ascii	"rv\000"
	.byte	0x1
	.2byte	0x266
	.byte	0x8
	.4byte	0x97
	.uleb128 0x2
	.byte	0x91
	.sleb128 -4
	.byte	0
	.uleb128 0x23
	.4byte	.LASF923
	.byte	0x1
	.2byte	0x25a
	.byte	0x6
	.4byte	0x67
	.4byte	.LFB0
	.4byte	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x1a
	.ascii	"ptr\000"
	.byte	0x1
	.2byte	0x25a
	.byte	0x1b
	.4byte	0x129b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x1c
	.ascii	"rv\000"
	.byte	0x1
	.2byte	0x25c
	.byte	0x7
	.4byte	0x67
	.uleb128 0x2
	.byte	0x91
	.sleb128 -2
	.byte	0
	.byte	0
	.section	.debug_abbrev,"",%progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x2134
	.uleb128 0x19
	.uleb128 0x55
	.uleb128 0x17
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x17
	.uleb128 0x2119
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x13
	.byte	0x1
	.uleb128 0xb
	.uleb128 0x5
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x13
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x4
	.byte	0x1
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x28
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1c
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.uleb128 0x34
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0x13
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x19
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1a
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x1b
	.uleb128 0x18
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x1c
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x1d
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1e
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1f
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x20
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x21
	.uleb128 0x26
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x22
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x23
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_pubnames,"",%progbits
	.4byte	0x53f
	.2byte	0x2
	.4byte	.Ldebug_info0
	.4byte	0x24b7
	.4byte	0x418
	.ascii	"FR_OK\000"
	.4byte	0x41e
	.ascii	"FR_DISK_ERR\000"
	.4byte	0x424
	.ascii	"FR_INT_ERR\000"
	.4byte	0x42a
	.ascii	"FR_NOT_READY\000"
	.4byte	0x430
	.ascii	"FR_NO_FILE\000"
	.4byte	0x436
	.ascii	"FR_NO_PATH\000"
	.4byte	0x43c
	.ascii	"FR_INVALID_NAME\000"
	.4byte	0x442
	.ascii	"FR_DENIED\000"
	.4byte	0x448
	.ascii	"FR_EXIST\000"
	.4byte	0x44e
	.ascii	"FR_INVALID_OBJECT\000"
	.4byte	0x454
	.ascii	"FR_WRITE_PROTECTED\000"
	.4byte	0x45a
	.ascii	"FR_INVALID_DRIVE\000"
	.4byte	0x460
	.ascii	"FR_NOT_ENABLED\000"
	.4byte	0x466
	.ascii	"FR_NO_FILESYSTEM\000"
	.4byte	0x46c
	.ascii	"FR_MKFS_ABORTED\000"
	.4byte	0x472
	.ascii	"FR_TIMEOUT\000"
	.4byte	0x478
	.ascii	"FR_LOCKED\000"
	.4byte	0x47e
	.ascii	"FR_NOT_ENOUGH_CORE\000"
	.4byte	0x484
	.ascii	"FR_TOO_MANY_OPEN_FILES\000"
	.4byte	0x48a
	.ascii	"FR_INVALID_PARAMETER\000"
	.4byte	0x4b7
	.ascii	"RES_OK\000"
	.4byte	0x4bd
	.ascii	"RES_ERROR\000"
	.4byte	0x4c3
	.ascii	"RES_WRPRT\000"
	.4byte	0x4c9
	.ascii	"RES_NOTRDY\000"
	.4byte	0x4cf
	.ascii	"RES_PARERR\000"
	.4byte	0x4e6
	.ascii	"FatFs\000"
	.4byte	0x4f9
	.ascii	"Fsid\000"
	.4byte	0x51c
	.ascii	"LfnBuf\000"
	.4byte	0x544
	.ascii	"LfnOfs\000"
	.4byte	0x4e6
	.ascii	"FatFs\000"
	.4byte	0x4f9
	.ascii	"Fsid\000"
	.4byte	0x51c
	.ascii	"LfnBuf\000"
	.4byte	0x5f3
	.ascii	"f_printf\000"
	.4byte	0x6fb
	.ascii	"f_puts\000"
	.4byte	0x748
	.ascii	"f_putc\000"
	.4byte	0x793
	.ascii	"putc_init\000"
	.4byte	0x7cf
	.ascii	"putc_flush\000"
	.4byte	0x809
	.ascii	"putc_bfd\000"
	.4byte	0x85b
	.ascii	"f_gets\000"
	.4byte	0x8fd
	.ascii	"f_mkfs\000"
	.4byte	0xb5c
	.ascii	"f_rename\000"
	.4byte	0xc1c
	.ascii	"f_mkdir\000"
	.4byte	0xcd6
	.ascii	"f_unlink\000"
	.4byte	0xd53
	.ascii	"f_truncate\000"
	.4byte	0xdad
	.ascii	"f_getfree\000"
	.4byte	0xe91
	.ascii	"f_stat\000"
	.4byte	0xef4
	.ascii	"f_readdir\000"
	.4byte	0xf54
	.ascii	"f_closedir\000"
	.4byte	0xf9e
	.ascii	"f_opendir\000"
	.4byte	0x100e
	.ascii	"f_lseek\000"
	.4byte	0x10a8
	.ascii	"f_chdir\000"
	.4byte	0x1103
	.ascii	"f_close\000"
	.4byte	0x114d
	.ascii	"f_sync\000"
	.4byte	0x11b6
	.ascii	"f_write\000"
	.4byte	0x12a1
	.ascii	"f_read\000"
	.4byte	0x1389
	.ascii	"f_open\000"
	.4byte	0x1464
	.ascii	"f_mount\000"
	.4byte	0x14ee
	.ascii	"validate\000"
	.4byte	0x1539
	.ascii	"find_volume\000"
	.4byte	0x167a
	.ascii	"check_fs\000"
	.4byte	0x16b5
	.ascii	"get_ldnumber\000"
	.4byte	0x171d
	.ascii	"follow_path\000"
	.4byte	0x1796
	.ascii	"create_name\000"
	.4byte	0x1855
	.ascii	"get_fileinfo\000"
	.4byte	0x18f2
	.ascii	"dir_remove\000"
	.4byte	0x194c
	.ascii	"dir_register\000"
	.4byte	0x19e3
	.ascii	"dir_find\000"
	.4byte	0x1a69
	.ascii	"dir_read\000"
	.4byte	0x1aff
	.ascii	"sum_sfn\000"
	.4byte	0x1b49
	.ascii	"gen_numname\000"
	.4byte	0x1c0e
	.ascii	"put_lfn\000"
	.4byte	0x1c91
	.ascii	"pick_lfn\000"
	.4byte	0x1d07
	.ascii	"cmp_lfn\000"
	.4byte	0x1d7d
	.ascii	"st_clust\000"
	.4byte	0x1dc3
	.ascii	"ld_clust\000"
	.4byte	0x1e0d
	.ascii	"dir_alloc\000"
	.4byte	0x1e75
	.ascii	"dir_next\000"
	.4byte	0x1eed
	.ascii	"dir_sdi\000"
	.4byte	0x1f57
	.ascii	"create_chain\000"
	.4byte	0x1fe1
	.ascii	"remove_chain\000"
	.4byte	0x205c
	.ascii	"put_fat\000"
	.4byte	0x20d4
	.ascii	"get_fat\000"
	.4byte	0x214d
	.ascii	"clust2sect\000"
	.4byte	0x2188
	.ascii	"sync_fs\000"
	.4byte	0x21c3
	.ascii	"move_window\000"
	.4byte	0x220e
	.ascii	"sync_window\000"
	.4byte	0x2268
	.ascii	"chk_chr\000"
	.4byte	0x22a4
	.ascii	"mem_cmp\000"
	.4byte	0x231a
	.ascii	"mem_set\000"
	.4byte	0x2370
	.ascii	"mem_cpy\000"
	.4byte	0x23d4
	.ascii	"st_dword\000"
	.4byte	0x240c
	.ascii	"st_word\000"
	.4byte	0x2444
	.ascii	"ld_dword\000"
	.4byte	0x247f
	.ascii	"ld_word\000"
	.4byte	0
	.section	.debug_pubtypes,"",%progbits
	.4byte	0x166
	.2byte	0x2
	.4byte	.Ldebug_info0
	.4byte	0x24b7
	.4byte	0x29
	.ascii	"int\000"
	.4byte	0x41
	.ascii	"unsigned int\000"
	.4byte	0x30
	.ascii	"UINT\000"
	.4byte	0x59
	.ascii	"unsigned char\000"
	.4byte	0x48
	.ascii	"BYTE\000"
	.4byte	0x60
	.ascii	"short int\000"
	.4byte	0x78
	.ascii	"short unsigned int\000"
	.4byte	0x67
	.ascii	"WORD\000"
	.4byte	0x7f
	.ascii	"WCHAR\000"
	.4byte	0x90
	.ascii	"long int\000"
	.4byte	0xa3
	.ascii	"long unsigned int\000"
	.4byte	0x97
	.ascii	"DWORD\000"
	.4byte	0xaa
	.ascii	"long long unsigned int\000"
	.4byte	0xc2
	.ascii	"char\000"
	.4byte	0xb1
	.ascii	"TCHAR\000"
	.4byte	0xce
	.ascii	"FSIZE_t\000"
	.4byte	0x1ff
	.ascii	"FATFS\000"
	.4byte	0x267
	.ascii	"_FDID\000"
	.4byte	0x2f9
	.ascii	"FIL\000"
	.4byte	0x379
	.ascii	"DIR\000"
	.4byte	0x3fe
	.ascii	"FILINFO\000"
	.4byte	0x491
	.ascii	"FRESULT\000"
	.4byte	0x49d
	.ascii	"DSTATUS\000"
	.4byte	0x563
	.ascii	"__va_list\000"
	.4byte	0x557
	.ascii	"__va_list\000"
	.4byte	0x582
	.ascii	"va_list\000"
	.4byte	0x5e6
	.ascii	"putbuff\000"
	.4byte	0
	.section	.debug_aranges,"",%progbits
	.4byte	0x204
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x4
	.byte	0
	.2byte	0
	.2byte	0
	.4byte	.LFB0
	.4byte	.LFE0-.LFB0
	.4byte	.LFB1
	.4byte	.LFE1-.LFB1
	.4byte	.LFB2
	.4byte	.LFE2-.LFB2
	.4byte	.LFB3
	.4byte	.LFE3-.LFB3
	.4byte	.LFB4
	.4byte	.LFE4-.LFB4
	.4byte	.LFB5
	.4byte	.LFE5-.LFB5
	.4byte	.LFB6
	.4byte	.LFE6-.LFB6
	.4byte	.LFB7
	.4byte	.LFE7-.LFB7
	.4byte	.LFB8
	.4byte	.LFE8-.LFB8
	.4byte	.LFB9
	.4byte	.LFE9-.LFB9
	.4byte	.LFB10
	.4byte	.LFE10-.LFB10
	.4byte	.LFB11
	.4byte	.LFE11-.LFB11
	.4byte	.LFB12
	.4byte	.LFE12-.LFB12
	.4byte	.LFB13
	.4byte	.LFE13-.LFB13
	.4byte	.LFB14
	.4byte	.LFE14-.LFB14
	.4byte	.LFB15
	.4byte	.LFE15-.LFB15
	.4byte	.LFB16
	.4byte	.LFE16-.LFB16
	.4byte	.LFB17
	.4byte	.LFE17-.LFB17
	.4byte	.LFB18
	.4byte	.LFE18-.LFB18
	.4byte	.LFB19
	.4byte	.LFE19-.LFB19
	.4byte	.LFB20
	.4byte	.LFE20-.LFB20
	.4byte	.LFB21
	.4byte	.LFE21-.LFB21
	.4byte	.LFB22
	.4byte	.LFE22-.LFB22
	.4byte	.LFB23
	.4byte	.LFE23-.LFB23
	.4byte	.LFB24
	.4byte	.LFE24-.LFB24
	.4byte	.LFB25
	.4byte	.LFE25-.LFB25
	.4byte	.LFB26
	.4byte	.LFE26-.LFB26
	.4byte	.LFB27
	.4byte	.LFE27-.LFB27
	.4byte	.LFB28
	.4byte	.LFE28-.LFB28
	.4byte	.LFB29
	.4byte	.LFE29-.LFB29
	.4byte	.LFB30
	.4byte	.LFE30-.LFB30
	.4byte	.LFB31
	.4byte	.LFE31-.LFB31
	.4byte	.LFB32
	.4byte	.LFE32-.LFB32
	.4byte	.LFB33
	.4byte	.LFE33-.LFB33
	.4byte	.LFB34
	.4byte	.LFE34-.LFB34
	.4byte	.LFB35
	.4byte	.LFE35-.LFB35
	.4byte	.LFB36
	.4byte	.LFE36-.LFB36
	.4byte	.LFB37
	.4byte	.LFE37-.LFB37
	.4byte	.LFB38
	.4byte	.LFE38-.LFB38
	.4byte	.LFB39
	.4byte	.LFE39-.LFB39
	.4byte	.LFB40
	.4byte	.LFE40-.LFB40
	.4byte	.LFB41
	.4byte	.LFE41-.LFB41
	.4byte	.LFB42
	.4byte	.LFE42-.LFB42
	.4byte	.LFB43
	.4byte	.LFE43-.LFB43
	.4byte	.LFB44
	.4byte	.LFE44-.LFB44
	.4byte	.LFB45
	.4byte	.LFE45-.LFB45
	.4byte	.LFB46
	.4byte	.LFE46-.LFB46
	.4byte	.LFB47
	.4byte	.LFE47-.LFB47
	.4byte	.LFB48
	.4byte	.LFE48-.LFB48
	.4byte	.LFB49
	.4byte	.LFE49-.LFB49
	.4byte	.LFB50
	.4byte	.LFE50-.LFB50
	.4byte	.LFB51
	.4byte	.LFE51-.LFB51
	.4byte	.LFB52
	.4byte	.LFE52-.LFB52
	.4byte	.LFB53
	.4byte	.LFE53-.LFB53
	.4byte	.LFB54
	.4byte	.LFE54-.LFB54
	.4byte	.LFB55
	.4byte	.LFE55-.LFB55
	.4byte	.LFB56
	.4byte	.LFE56-.LFB56
	.4byte	.LFB57
	.4byte	.LFE57-.LFB57
	.4byte	.LFB58
	.4byte	.LFE58-.LFB58
	.4byte	.LFB59
	.4byte	.LFE59-.LFB59
	.4byte	.LFB60
	.4byte	.LFE60-.LFB60
	.4byte	.LFB61
	.4byte	.LFE61-.LFB61
	.4byte	0
	.4byte	0
	.section	.debug_ranges,"",%progbits
.Ldebug_ranges0:
	.4byte	.LFB0
	.4byte	.LFE0
	.4byte	.LFB1
	.4byte	.LFE1
	.4byte	.LFB2
	.4byte	.LFE2
	.4byte	.LFB3
	.4byte	.LFE3
	.4byte	.LFB4
	.4byte	.LFE4
	.4byte	.LFB5
	.4byte	.LFE5
	.4byte	.LFB6
	.4byte	.LFE6
	.4byte	.LFB7
	.4byte	.LFE7
	.4byte	.LFB8
	.4byte	.LFE8
	.4byte	.LFB9
	.4byte	.LFE9
	.4byte	.LFB10
	.4byte	.LFE10
	.4byte	.LFB11
	.4byte	.LFE11
	.4byte	.LFB12
	.4byte	.LFE12
	.4byte	.LFB13
	.4byte	.LFE13
	.4byte	.LFB14
	.4byte	.LFE14
	.4byte	.LFB15
	.4byte	.LFE15
	.4byte	.LFB16
	.4byte	.LFE16
	.4byte	.LFB17
	.4byte	.LFE17
	.4byte	.LFB18
	.4byte	.LFE18
	.4byte	.LFB19
	.4byte	.LFE19
	.4byte	.LFB20
	.4byte	.LFE20
	.4byte	.LFB21
	.4byte	.LFE21
	.4byte	.LFB22
	.4byte	.LFE22
	.4byte	.LFB23
	.4byte	.LFE23
	.4byte	.LFB24
	.4byte	.LFE24
	.4byte	.LFB25
	.4byte	.LFE25
	.4byte	.LFB26
	.4byte	.LFE26
	.4byte	.LFB27
	.4byte	.LFE27
	.4byte	.LFB28
	.4byte	.LFE28
	.4byte	.LFB29
	.4byte	.LFE29
	.4byte	.LFB30
	.4byte	.LFE30
	.4byte	.LFB31
	.4byte	.LFE31
	.4byte	.LFB32
	.4byte	.LFE32
	.4byte	.LFB33
	.4byte	.LFE33
	.4byte	.LFB34
	.4byte	.LFE34
	.4byte	.LFB35
	.4byte	.LFE35
	.4byte	.LFB36
	.4byte	.LFE36
	.4byte	.LFB37
	.4byte	.LFE37
	.4byte	.LFB38
	.4byte	.LFE38
	.4byte	.LFB39
	.4byte	.LFE39
	.4byte	.LFB40
	.4byte	.LFE40
	.4byte	.LFB41
	.4byte	.LFE41
	.4byte	.LFB42
	.4byte	.LFE42
	.4byte	.LFB43
	.4byte	.LFE43
	.4byte	.LFB44
	.4byte	.LFE44
	.4byte	.LFB45
	.4byte	.LFE45
	.4byte	.LFB46
	.4byte	.LFE46
	.4byte	.LFB47
	.4byte	.LFE47
	.4byte	.LFB48
	.4byte	.LFE48
	.4byte	.LFB49
	.4byte	.LFE49
	.4byte	.LFB50
	.4byte	.LFE50
	.4byte	.LFB51
	.4byte	.LFE51
	.4byte	.LFB52
	.4byte	.LFE52
	.4byte	.LFB53
	.4byte	.LFE53
	.4byte	.LFB54
	.4byte	.LFE54
	.4byte	.LFB55
	.4byte	.LFE55
	.4byte	.LFB56
	.4byte	.LFE56
	.4byte	.LFB57
	.4byte	.LFE57
	.4byte	.LFB58
	.4byte	.LFE58
	.4byte	.LFB59
	.4byte	.LFE59
	.4byte	.LFB60
	.4byte	.LFE60
	.4byte	.LFB61
	.4byte	.LFE61
	.4byte	0
	.4byte	0
	.section	.debug_macro,"",%progbits
.Ldebug_macro0:
	.2byte	0x4
	.byte	0x2
	.4byte	.Ldebug_line0
	.byte	0x7
	.4byte	.Ldebug_macro2
	.byte	0x3
	.uleb128 0
	.uleb128 0x1
	.byte	0x3
	.uleb128 0x15
	.uleb128 0x3
	.byte	0x5
	.uleb128 0x16
	.4byte	.LASF457
	.byte	0x3
	.uleb128 0x1c
	.uleb128 0x2
	.byte	0x5
	.uleb128 0x6
	.4byte	.LASF458
	.byte	0x4
	.file 8 "C:\\Nordic\\NAATOS_fw_src\\application\\sdk\\external\\fatfs\\src\\ffconf.h"
	.byte	0x3
	.uleb128 0x1d
	.uleb128 0x8
	.byte	0x7
	.4byte	.Ldebug_macro3
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro4
	.byte	0x4
	.byte	0x3
	.uleb128 0x16
	.uleb128 0x4
	.byte	0x7
	.4byte	.Ldebug_macro5
	.byte	0x4
	.byte	0x5
	.uleb128 0x24
	.4byte	.LASF551
	.byte	0x5
	.uleb128 0x2f
	.4byte	.LASF552
	.byte	0x5
	.uleb128 0x30
	.4byte	.LASF553
	.byte	0x5
	.uleb128 0x3a
	.4byte	.LASF554
	.byte	0x5
	.uleb128 0x45
	.4byte	.LASF555
	.byte	0x5
	.uleb128 0x5d
	.4byte	.LASF556
	.byte	0x5
	.uleb128 0x5e
	.4byte	.LASF557
	.byte	0x5
	.uleb128 0x5f
	.4byte	.LASF558
	.byte	0x5
	.uleb128 0x60
	.4byte	.LASF559
	.byte	0x5
	.uleb128 0x61
	.4byte	.LASF560
	.byte	0x5
	.uleb128 0x62
	.4byte	.LASF561
	.byte	0x5
	.uleb128 0x63
	.4byte	.LASF562
	.byte	0x5
	.uleb128 0x64
	.4byte	.LASF563
	.byte	0x5
	.uleb128 0x148
	.4byte	.LASF564
	.byte	0x5
	.uleb128 0x149
	.4byte	.LASF565
	.byte	0x5
	.uleb128 0x14a
	.4byte	.LASF566
	.byte	0x5
	.uleb128 0x14f
	.4byte	.LASF567
	.byte	0x5
	.uleb128 0x157
	.4byte	.LASF568
	.byte	0x5
	.uleb128 0x163
	.4byte	.LASF569
	.byte	0x5
	.uleb128 0x164
	.4byte	.LASF570
	.byte	0x5
	.uleb128 0x165
	.4byte	.LASF571
	.byte	0x5
	.uleb128 0x169
	.4byte	.LASF572
	.byte	0x5
	.uleb128 0x16a
	.4byte	.LASF573
	.byte	0x5
	.uleb128 0x16b
	.4byte	.LASF574
	.byte	0x5
	.uleb128 0x16f
	.4byte	.LASF575
	.byte	0x5
	.uleb128 0x170
	.4byte	.LASF576
	.byte	0x5
	.uleb128 0x171
	.4byte	.LASF577
	.byte	0x5
	.uleb128 0x172
	.4byte	.LASF578
	.byte	0x5
	.uleb128 0x173
	.4byte	.LASF579
	.byte	0x5
	.uleb128 0x174
	.4byte	.LASF580
	.byte	0x5
	.uleb128 0x175
	.4byte	.LASF581
	.byte	0x5
	.uleb128 0x176
	.4byte	.LASF582
	.byte	0x5
	.uleb128 0x177
	.4byte	.LASF583
	.byte	0x5
	.uleb128 0x17b
	.4byte	.LASF584
	.byte	0x5
	.uleb128 0x17c
	.4byte	.LASF585
	.byte	0x5
	.uleb128 0x17d
	.4byte	.LASF586
	.byte	0x5
	.uleb128 0x17e
	.4byte	.LASF587
	.byte	0x5
	.uleb128 0x17f
	.4byte	.LASF588
	.byte	0x5
	.uleb128 0x180
	.4byte	.LASF589
	.byte	0x5
	.uleb128 0x187
	.4byte	.LASF590
	.byte	0x5
	.uleb128 0x188
	.4byte	.LASF591
	.byte	0x5
	.uleb128 0x189
	.4byte	.LASF592
	.byte	0x5
	.uleb128 0x18a
	.4byte	.LASF593
	.byte	0x5
	.uleb128 0x18b
	.4byte	.LASF594
	.byte	0x5
	.uleb128 0x18c
	.4byte	.LASF595
	.byte	0x5
	.uleb128 0x18d
	.4byte	.LASF596
	.byte	0x5
	.uleb128 0x18e
	.4byte	.LASF597
	.byte	0x5
	.uleb128 0x18f
	.4byte	.LASF598
	.byte	0x5
	.uleb128 0x190
	.4byte	.LASF599
	.byte	0x5
	.uleb128 0x191
	.4byte	.LASF600
	.byte	0x5
	.uleb128 0x192
	.4byte	.LASF601
	.byte	0x5
	.uleb128 0x193
	.4byte	.LASF602
	.byte	0x5
	.uleb128 0x194
	.4byte	.LASF603
	.byte	0x5
	.uleb128 0x195
	.4byte	.LASF604
	.byte	0x5
	.uleb128 0x196
	.4byte	.LASF605
	.byte	0x5
	.uleb128 0x197
	.4byte	.LASF606
	.byte	0x5
	.uleb128 0x198
	.4byte	.LASF607
	.byte	0x5
	.uleb128 0x199
	.4byte	.LASF608
	.byte	0x5
	.uleb128 0x19a
	.4byte	.LASF609
	.byte	0x5
	.uleb128 0x19b
	.4byte	.LASF610
	.byte	0x5
	.uleb128 0x19c
	.4byte	.LASF611
	.byte	0x5
	.uleb128 0x19e
	.4byte	.LASF612
	.byte	0x5
	.uleb128 0x19f
	.4byte	.LASF613
	.byte	0x5
	.uleb128 0x1a0
	.4byte	.LASF614
	.byte	0x5
	.uleb128 0x1a1
	.4byte	.LASF615
	.byte	0x5
	.uleb128 0x1a2
	.4byte	.LASF616
	.byte	0x5
	.uleb128 0x1a3
	.4byte	.LASF617
	.byte	0x5
	.uleb128 0x1a4
	.4byte	.LASF618
	.byte	0x5
	.uleb128 0x1a5
	.4byte	.LASF619
	.byte	0x5
	.uleb128 0x1a6
	.4byte	.LASF620
	.byte	0x5
	.uleb128 0x1a7
	.4byte	.LASF621
	.byte	0x5
	.uleb128 0x1a8
	.4byte	.LASF622
	.byte	0x5
	.uleb128 0x1a9
	.4byte	.LASF623
	.byte	0x5
	.uleb128 0x1aa
	.4byte	.LASF624
	.byte	0x5
	.uleb128 0x1ac
	.4byte	.LASF625
	.byte	0x5
	.uleb128 0x1ad
	.4byte	.LASF626
	.byte	0x5
	.uleb128 0x1ae
	.4byte	.LASF627
	.byte	0x5
	.uleb128 0x1af
	.4byte	.LASF628
	.byte	0x5
	.uleb128 0x1b0
	.4byte	.LASF629
	.byte	0x5
	.uleb128 0x1b1
	.4byte	.LASF630
	.byte	0x5
	.uleb128 0x1b2
	.4byte	.LASF631
	.byte	0x5
	.uleb128 0x1b3
	.4byte	.LASF632
	.byte	0x5
	.uleb128 0x1b4
	.4byte	.LASF633
	.byte	0x5
	.uleb128 0x1b5
	.4byte	.LASF634
	.byte	0x5
	.uleb128 0x1b6
	.4byte	.LASF635
	.byte	0x5
	.uleb128 0x1b7
	.4byte	.LASF636
	.byte	0x5
	.uleb128 0x1b8
	.4byte	.LASF637
	.byte	0x5
	.uleb128 0x1b9
	.4byte	.LASF638
	.byte	0x5
	.uleb128 0x1ba
	.4byte	.LASF639
	.byte	0x5
	.uleb128 0x1bb
	.4byte	.LASF640
	.byte	0x5
	.uleb128 0x1bc
	.4byte	.LASF641
	.byte	0x5
	.uleb128 0x1bd
	.4byte	.LASF642
	.byte	0x5
	.uleb128 0x1be
	.4byte	.LASF643
	.byte	0x5
	.uleb128 0x1c0
	.4byte	.LASF644
	.byte	0x5
	.uleb128 0x1c1
	.4byte	.LASF645
	.byte	0x5
	.uleb128 0x1c2
	.4byte	.LASF646
	.byte	0x5
	.uleb128 0x1c3
	.4byte	.LASF647
	.byte	0x5
	.uleb128 0x1c5
	.4byte	.LASF648
	.byte	0x5
	.uleb128 0x1c6
	.4byte	.LASF649
	.byte	0x5
	.uleb128 0x1c7
	.4byte	.LASF650
	.byte	0x5
	.uleb128 0x1c8
	.4byte	.LASF651
	.byte	0x5
	.uleb128 0x1c9
	.4byte	.LASF652
	.byte	0x5
	.uleb128 0x1ca
	.4byte	.LASF653
	.byte	0x5
	.uleb128 0x1cb
	.4byte	.LASF654
	.byte	0x5
	.uleb128 0x1cc
	.4byte	.LASF655
	.byte	0x5
	.uleb128 0x1cd
	.4byte	.LASF656
	.byte	0x5
	.uleb128 0x1ce
	.4byte	.LASF657
	.byte	0x5
	.uleb128 0x1cf
	.4byte	.LASF658
	.byte	0x5
	.uleb128 0x1d0
	.4byte	.LASF659
	.byte	0x5
	.uleb128 0x1d2
	.4byte	.LASF660
	.byte	0x5
	.uleb128 0x1d3
	.4byte	.LASF661
	.byte	0x5
	.uleb128 0x1d4
	.4byte	.LASF662
	.byte	0x5
	.uleb128 0x1d5
	.4byte	.LASF663
	.byte	0x5
	.uleb128 0x1d6
	.4byte	.LASF664
	.byte	0x5
	.uleb128 0x1d7
	.4byte	.LASF665
	.byte	0x5
	.uleb128 0x1d8
	.4byte	.LASF666
	.byte	0x5
	.uleb128 0x1d9
	.4byte	.LASF667
	.byte	0x5
	.uleb128 0x1da
	.4byte	.LASF668
	.byte	0x5
	.uleb128 0x1db
	.4byte	.LASF669
	.byte	0x5
	.uleb128 0x1dc
	.4byte	.LASF670
	.byte	0x5
	.uleb128 0x1dd
	.4byte	.LASF671
	.byte	0x5
	.uleb128 0x1de
	.4byte	.LASF672
	.byte	0x5
	.uleb128 0x1df
	.4byte	.LASF673
	.byte	0x5
	.uleb128 0x1e0
	.4byte	.LASF674
	.byte	0x5
	.uleb128 0x1e1
	.4byte	.LASF675
	.byte	0x5
	.uleb128 0x1e2
	.4byte	.LASF676
	.byte	0x5
	.uleb128 0x1e3
	.4byte	.LASF677
	.byte	0x5
	.uleb128 0x1e4
	.4byte	.LASF678
	.byte	0x5
	.uleb128 0x1e5
	.4byte	.LASF679
	.byte	0x5
	.uleb128 0x1e6
	.4byte	.LASF680
	.byte	0x5
	.uleb128 0x1e7
	.4byte	.LASF681
	.byte	0x5
	.uleb128 0x1e8
	.4byte	.LASF682
	.byte	0x5
	.uleb128 0x1e9
	.4byte	.LASF683
	.byte	0x5
	.uleb128 0x1ea
	.4byte	.LASF684
	.byte	0x5
	.uleb128 0x1eb
	.4byte	.LASF685
	.byte	0x5
	.uleb128 0x1ec
	.4byte	.LASF686
	.byte	0x5
	.uleb128 0x1ed
	.4byte	.LASF687
	.byte	0x5
	.uleb128 0x1ee
	.4byte	.LASF688
	.byte	0x5
	.uleb128 0x1ef
	.4byte	.LASF689
	.byte	0x5
	.uleb128 0x1f0
	.4byte	.LASF690
	.byte	0x5
	.uleb128 0x1f1
	.4byte	.LASF691
	.byte	0x5
	.uleb128 0x1f2
	.4byte	.LASF692
	.byte	0x5
	.uleb128 0x1f3
	.4byte	.LASF693
	.byte	0x5
	.uleb128 0x1f4
	.4byte	.LASF694
	.byte	0x5
	.uleb128 0x1f5
	.4byte	.LASF695
	.byte	0x5
	.uleb128 0x1f7
	.4byte	.LASF696
	.byte	0x5
	.uleb128 0x1f8
	.4byte	.LASF697
	.byte	0x5
	.uleb128 0x1f9
	.4byte	.LASF698
	.byte	0x5
	.uleb128 0x1fa
	.4byte	.LASF699
	.byte	0x5
	.uleb128 0x226
	.4byte	.LASF700
	.byte	0x5
	.uleb128 0x227
	.4byte	.LASF701
	.byte	0x5
	.uleb128 0x228
	.4byte	.LASF702
	.byte	0x3
	.uleb128 0x16b5
	.uleb128 0x6
	.byte	0x5
	.uleb128 0x27
	.4byte	.LASF703
	.byte	0x3
	.uleb128 0x2a
	.uleb128 0x5
	.byte	0x7
	.4byte	.Ldebug_macro6
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro7
	.byte	0x4
	.byte	0x4
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.0.74bb9c9306d8c261a1059c51ea087e49,comdat
.Ldebug_macro2:
	.2byte	0x4
	.byte	0
	.byte	0x5
	.uleb128 0
	.4byte	.LASF0
	.byte	0x5
	.uleb128 0
	.4byte	.LASF1
	.byte	0x5
	.uleb128 0
	.4byte	.LASF2
	.byte	0x5
	.uleb128 0
	.4byte	.LASF3
	.byte	0x5
	.uleb128 0
	.4byte	.LASF4
	.byte	0x5
	.uleb128 0
	.4byte	.LASF5
	.byte	0x5
	.uleb128 0
	.4byte	.LASF6
	.byte	0x5
	.uleb128 0
	.4byte	.LASF7
	.byte	0x5
	.uleb128 0
	.4byte	.LASF8
	.byte	0x5
	.uleb128 0
	.4byte	.LASF9
	.byte	0x5
	.uleb128 0
	.4byte	.LASF10
	.byte	0x5
	.uleb128 0
	.4byte	.LASF11
	.byte	0x5
	.uleb128 0
	.4byte	.LASF12
	.byte	0x5
	.uleb128 0
	.4byte	.LASF13
	.byte	0x5
	.uleb128 0
	.4byte	.LASF14
	.byte	0x5
	.uleb128 0
	.4byte	.LASF15
	.byte	0x5
	.uleb128 0
	.4byte	.LASF16
	.byte	0x5
	.uleb128 0
	.4byte	.LASF17
	.byte	0x5
	.uleb128 0
	.4byte	.LASF18
	.byte	0x5
	.uleb128 0
	.4byte	.LASF19
	.byte	0x5
	.uleb128 0
	.4byte	.LASF20
	.byte	0x5
	.uleb128 0
	.4byte	.LASF21
	.byte	0x5
	.uleb128 0
	.4byte	.LASF22
	.byte	0x5
	.uleb128 0
	.4byte	.LASF23
	.byte	0x5
	.uleb128 0
	.4byte	.LASF24
	.byte	0x5
	.uleb128 0
	.4byte	.LASF25
	.byte	0x5
	.uleb128 0
	.4byte	.LASF26
	.byte	0x5
	.uleb128 0
	.4byte	.LASF27
	.byte	0x5
	.uleb128 0
	.4byte	.LASF28
	.byte	0x5
	.uleb128 0
	.4byte	.LASF29
	.byte	0x5
	.uleb128 0
	.4byte	.LASF30
	.byte	0x5
	.uleb128 0
	.4byte	.LASF31
	.byte	0x5
	.uleb128 0
	.4byte	.LASF32
	.byte	0x5
	.uleb128 0
	.4byte	.LASF33
	.byte	0x5
	.uleb128 0
	.4byte	.LASF34
	.byte	0x5
	.uleb128 0
	.4byte	.LASF35
	.byte	0x5
	.uleb128 0
	.4byte	.LASF36
	.byte	0x5
	.uleb128 0
	.4byte	.LASF37
	.byte	0x5
	.uleb128 0
	.4byte	.LASF38
	.byte	0x5
	.uleb128 0
	.4byte	.LASF39
	.byte	0x5
	.uleb128 0
	.4byte	.LASF40
	.byte	0x5
	.uleb128 0
	.4byte	.LASF41
	.byte	0x5
	.uleb128 0
	.4byte	.LASF42
	.byte	0x5
	.uleb128 0
	.4byte	.LASF43
	.byte	0x5
	.uleb128 0
	.4byte	.LASF44
	.byte	0x5
	.uleb128 0
	.4byte	.LASF45
	.byte	0x5
	.uleb128 0
	.4byte	.LASF46
	.byte	0x5
	.uleb128 0
	.4byte	.LASF47
	.byte	0x5
	.uleb128 0
	.4byte	.LASF48
	.byte	0x5
	.uleb128 0
	.4byte	.LASF49
	.byte	0x5
	.uleb128 0
	.4byte	.LASF50
	.byte	0x5
	.uleb128 0
	.4byte	.LASF51
	.byte	0x5
	.uleb128 0
	.4byte	.LASF52
	.byte	0x5
	.uleb128 0
	.4byte	.LASF53
	.byte	0x5
	.uleb128 0
	.4byte	.LASF54
	.byte	0x5
	.uleb128 0
	.4byte	.LASF55
	.byte	0x5
	.uleb128 0
	.4byte	.LASF56
	.byte	0x5
	.uleb128 0
	.4byte	.LASF57
	.byte	0x5
	.uleb128 0
	.4byte	.LASF58
	.byte	0x5
	.uleb128 0
	.4byte	.LASF59
	.byte	0x5
	.uleb128 0
	.4byte	.LASF60
	.byte	0x5
	.uleb128 0
	.4byte	.LASF61
	.byte	0x5
	.uleb128 0
	.4byte	.LASF62
	.byte	0x5
	.uleb128 0
	.4byte	.LASF63
	.byte	0x5
	.uleb128 0
	.4byte	.LASF64
	.byte	0x5
	.uleb128 0
	.4byte	.LASF65
	.byte	0x5
	.uleb128 0
	.4byte	.LASF66
	.byte	0x5
	.uleb128 0
	.4byte	.LASF67
	.byte	0x5
	.uleb128 0
	.4byte	.LASF68
	.byte	0x5
	.uleb128 0
	.4byte	.LASF69
	.byte	0x5
	.uleb128 0
	.4byte	.LASF70
	.byte	0x5
	.uleb128 0
	.4byte	.LASF71
	.byte	0x5
	.uleb128 0
	.4byte	.LASF72
	.byte	0x5
	.uleb128 0
	.4byte	.LASF73
	.byte	0x5
	.uleb128 0
	.4byte	.LASF74
	.byte	0x5
	.uleb128 0
	.4byte	.LASF75
	.byte	0x5
	.uleb128 0
	.4byte	.LASF76
	.byte	0x5
	.uleb128 0
	.4byte	.LASF77
	.byte	0x5
	.uleb128 0
	.4byte	.LASF78
	.byte	0x5
	.uleb128 0
	.4byte	.LASF79
	.byte	0x5
	.uleb128 0
	.4byte	.LASF80
	.byte	0x5
	.uleb128 0
	.4byte	.LASF81
	.byte	0x5
	.uleb128 0
	.4byte	.LASF82
	.byte	0x5
	.uleb128 0
	.4byte	.LASF83
	.byte	0x5
	.uleb128 0
	.4byte	.LASF84
	.byte	0x5
	.uleb128 0
	.4byte	.LASF85
	.byte	0x5
	.uleb128 0
	.4byte	.LASF86
	.byte	0x5
	.uleb128 0
	.4byte	.LASF87
	.byte	0x5
	.uleb128 0
	.4byte	.LASF88
	.byte	0x5
	.uleb128 0
	.4byte	.LASF89
	.byte	0x5
	.uleb128 0
	.4byte	.LASF90
	.byte	0x5
	.uleb128 0
	.4byte	.LASF91
	.byte	0x5
	.uleb128 0
	.4byte	.LASF92
	.byte	0x5
	.uleb128 0
	.4byte	.LASF93
	.byte	0x5
	.uleb128 0
	.4byte	.LASF94
	.byte	0x5
	.uleb128 0
	.4byte	.LASF95
	.byte	0x5
	.uleb128 0
	.4byte	.LASF96
	.byte	0x5
	.uleb128 0
	.4byte	.LASF97
	.byte	0x5
	.uleb128 0
	.4byte	.LASF98
	.byte	0x5
	.uleb128 0
	.4byte	.LASF99
	.byte	0x5
	.uleb128 0
	.4byte	.LASF100
	.byte	0x5
	.uleb128 0
	.4byte	.LASF101
	.byte	0x5
	.uleb128 0
	.4byte	.LASF102
	.byte	0x5
	.uleb128 0
	.4byte	.LASF103
	.byte	0x5
	.uleb128 0
	.4byte	.LASF104
	.byte	0x5
	.uleb128 0
	.4byte	.LASF105
	.byte	0x5
	.uleb128 0
	.4byte	.LASF106
	.byte	0x5
	.uleb128 0
	.4byte	.LASF107
	.byte	0x5
	.uleb128 0
	.4byte	.LASF108
	.byte	0x5
	.uleb128 0
	.4byte	.LASF109
	.byte	0x5
	.uleb128 0
	.4byte	.LASF110
	.byte	0x5
	.uleb128 0
	.4byte	.LASF111
	.byte	0x5
	.uleb128 0
	.4byte	.LASF112
	.byte	0x5
	.uleb128 0
	.4byte	.LASF113
	.byte	0x5
	.uleb128 0
	.4byte	.LASF114
	.byte	0x5
	.uleb128 0
	.4byte	.LASF115
	.byte	0x5
	.uleb128 0
	.4byte	.LASF116
	.byte	0x5
	.uleb128 0
	.4byte	.LASF117
	.byte	0x5
	.uleb128 0
	.4byte	.LASF118
	.byte	0x5
	.uleb128 0
	.4byte	.LASF119
	.byte	0x5
	.uleb128 0
	.4byte	.LASF120
	.byte	0x5
	.uleb128 0
	.4byte	.LASF121
	.byte	0x5
	.uleb128 0
	.4byte	.LASF122
	.byte	0x5
	.uleb128 0
	.4byte	.LASF123
	.byte	0x5
	.uleb128 0
	.4byte	.LASF124
	.byte	0x5
	.uleb128 0
	.4byte	.LASF125
	.byte	0x5
	.uleb128 0
	.4byte	.LASF126
	.byte	0x5
	.uleb128 0
	.4byte	.LASF127
	.byte	0x5
	.uleb128 0
	.4byte	.LASF128
	.byte	0x5
	.uleb128 0
	.4byte	.LASF129
	.byte	0x5
	.uleb128 0
	.4byte	.LASF130
	.byte	0x5
	.uleb128 0
	.4byte	.LASF131
	.byte	0x5
	.uleb128 0
	.4byte	.LASF132
	.byte	0x5
	.uleb128 0
	.4byte	.LASF133
	.byte	0x5
	.uleb128 0
	.4byte	.LASF134
	.byte	0x5
	.uleb128 0
	.4byte	.LASF135
	.byte	0x5
	.uleb128 0
	.4byte	.LASF136
	.byte	0x5
	.uleb128 0
	.4byte	.LASF137
	.byte	0x5
	.uleb128 0
	.4byte	.LASF138
	.byte	0x5
	.uleb128 0
	.4byte	.LASF139
	.byte	0x5
	.uleb128 0
	.4byte	.LASF140
	.byte	0x5
	.uleb128 0
	.4byte	.LASF141
	.byte	0x5
	.uleb128 0
	.4byte	.LASF142
	.byte	0x5
	.uleb128 0
	.4byte	.LASF143
	.byte	0x5
	.uleb128 0
	.4byte	.LASF144
	.byte	0x5
	.uleb128 0
	.4byte	.LASF145
	.byte	0x5
	.uleb128 0
	.4byte	.LASF146
	.byte	0x5
	.uleb128 0
	.4byte	.LASF147
	.byte	0x5
	.uleb128 0
	.4byte	.LASF148
	.byte	0x5
	.uleb128 0
	.4byte	.LASF149
	.byte	0x5
	.uleb128 0
	.4byte	.LASF150
	.byte	0x5
	.uleb128 0
	.4byte	.LASF151
	.byte	0x5
	.uleb128 0
	.4byte	.LASF152
	.byte	0x5
	.uleb128 0
	.4byte	.LASF153
	.byte	0x5
	.uleb128 0
	.4byte	.LASF154
	.byte	0x5
	.uleb128 0
	.4byte	.LASF155
	.byte	0x5
	.uleb128 0
	.4byte	.LASF156
	.byte	0x5
	.uleb128 0
	.4byte	.LASF157
	.byte	0x5
	.uleb128 0
	.4byte	.LASF158
	.byte	0x5
	.uleb128 0
	.4byte	.LASF159
	.byte	0x5
	.uleb128 0
	.4byte	.LASF160
	.byte	0x5
	.uleb128 0
	.4byte	.LASF161
	.byte	0x5
	.uleb128 0
	.4byte	.LASF162
	.byte	0x5
	.uleb128 0
	.4byte	.LASF163
	.byte	0x5
	.uleb128 0
	.4byte	.LASF164
	.byte	0x5
	.uleb128 0
	.4byte	.LASF165
	.byte	0x5
	.uleb128 0
	.4byte	.LASF166
	.byte	0x5
	.uleb128 0
	.4byte	.LASF167
	.byte	0x5
	.uleb128 0
	.4byte	.LASF168
	.byte	0x5
	.uleb128 0
	.4byte	.LASF169
	.byte	0x5
	.uleb128 0
	.4byte	.LASF170
	.byte	0x5
	.uleb128 0
	.4byte	.LASF171
	.byte	0x5
	.uleb128 0
	.4byte	.LASF172
	.byte	0x5
	.uleb128 0
	.4byte	.LASF173
	.byte	0x5
	.uleb128 0
	.4byte	.LASF174
	.byte	0x5
	.uleb128 0
	.4byte	.LASF175
	.byte	0x5
	.uleb128 0
	.4byte	.LASF176
	.byte	0x5
	.uleb128 0
	.4byte	.LASF177
	.byte	0x5
	.uleb128 0
	.4byte	.LASF178
	.byte	0x5
	.uleb128 0
	.4byte	.LASF179
	.byte	0x5
	.uleb128 0
	.4byte	.LASF180
	.byte	0x5
	.uleb128 0
	.4byte	.LASF181
	.byte	0x5
	.uleb128 0
	.4byte	.LASF182
	.byte	0x5
	.uleb128 0
	.4byte	.LASF183
	.byte	0x5
	.uleb128 0
	.4byte	.LASF184
	.byte	0x5
	.uleb128 0
	.4byte	.LASF185
	.byte	0x5
	.uleb128 0
	.4byte	.LASF186
	.byte	0x5
	.uleb128 0
	.4byte	.LASF187
	.byte	0x5
	.uleb128 0
	.4byte	.LASF188
	.byte	0x5
	.uleb128 0
	.4byte	.LASF189
	.byte	0x5
	.uleb128 0
	.4byte	.LASF190
	.byte	0x5
	.uleb128 0
	.4byte	.LASF191
	.byte	0x5
	.uleb128 0
	.4byte	.LASF192
	.byte	0x5
	.uleb128 0
	.4byte	.LASF193
	.byte	0x5
	.uleb128 0
	.4byte	.LASF194
	.byte	0x5
	.uleb128 0
	.4byte	.LASF195
	.byte	0x5
	.uleb128 0
	.4byte	.LASF196
	.byte	0x5
	.uleb128 0
	.4byte	.LASF197
	.byte	0x5
	.uleb128 0
	.4byte	.LASF198
	.byte	0x5
	.uleb128 0
	.4byte	.LASF199
	.byte	0x5
	.uleb128 0
	.4byte	.LASF200
	.byte	0x5
	.uleb128 0
	.4byte	.LASF201
	.byte	0x5
	.uleb128 0
	.4byte	.LASF202
	.byte	0x5
	.uleb128 0
	.4byte	.LASF203
	.byte	0x5
	.uleb128 0
	.4byte	.LASF204
	.byte	0x5
	.uleb128 0
	.4byte	.LASF205
	.byte	0x5
	.uleb128 0
	.4byte	.LASF206
	.byte	0x5
	.uleb128 0
	.4byte	.LASF207
	.byte	0x5
	.uleb128 0
	.4byte	.LASF208
	.byte	0x5
	.uleb128 0
	.4byte	.LASF209
	.byte	0x5
	.uleb128 0
	.4byte	.LASF210
	.byte	0x5
	.uleb128 0
	.4byte	.LASF211
	.byte	0x5
	.uleb128 0
	.4byte	.LASF212
	.byte	0x5
	.uleb128 0
	.4byte	.LASF213
	.byte	0x5
	.uleb128 0
	.4byte	.LASF214
	.byte	0x5
	.uleb128 0
	.4byte	.LASF215
	.byte	0x5
	.uleb128 0
	.4byte	.LASF216
	.byte	0x5
	.uleb128 0
	.4byte	.LASF217
	.byte	0x5
	.uleb128 0
	.4byte	.LASF218
	.byte	0x5
	.uleb128 0
	.4byte	.LASF219
	.byte	0x5
	.uleb128 0
	.4byte	.LASF220
	.byte	0x5
	.uleb128 0
	.4byte	.LASF221
	.byte	0x5
	.uleb128 0
	.4byte	.LASF222
	.byte	0x5
	.uleb128 0
	.4byte	.LASF223
	.byte	0x5
	.uleb128 0
	.4byte	.LASF224
	.byte	0x5
	.uleb128 0
	.4byte	.LASF225
	.byte	0x5
	.uleb128 0
	.4byte	.LASF226
	.byte	0x5
	.uleb128 0
	.4byte	.LASF227
	.byte	0x5
	.uleb128 0
	.4byte	.LASF228
	.byte	0x5
	.uleb128 0
	.4byte	.LASF229
	.byte	0x5
	.uleb128 0
	.4byte	.LASF230
	.byte	0x5
	.uleb128 0
	.4byte	.LASF231
	.byte	0x5
	.uleb128 0
	.4byte	.LASF232
	.byte	0x5
	.uleb128 0
	.4byte	.LASF233
	.byte	0x5
	.uleb128 0
	.4byte	.LASF234
	.byte	0x5
	.uleb128 0
	.4byte	.LASF235
	.byte	0x5
	.uleb128 0
	.4byte	.LASF236
	.byte	0x5
	.uleb128 0
	.4byte	.LASF237
	.byte	0x5
	.uleb128 0
	.4byte	.LASF238
	.byte	0x5
	.uleb128 0
	.4byte	.LASF239
	.byte	0x5
	.uleb128 0
	.4byte	.LASF240
	.byte	0x5
	.uleb128 0
	.4byte	.LASF241
	.byte	0x5
	.uleb128 0
	.4byte	.LASF242
	.byte	0x5
	.uleb128 0
	.4byte	.LASF243
	.byte	0x5
	.uleb128 0
	.4byte	.LASF244
	.byte	0x5
	.uleb128 0
	.4byte	.LASF245
	.byte	0x5
	.uleb128 0
	.4byte	.LASF246
	.byte	0x5
	.uleb128 0
	.4byte	.LASF247
	.byte	0x5
	.uleb128 0
	.4byte	.LASF248
	.byte	0x5
	.uleb128 0
	.4byte	.LASF249
	.byte	0x5
	.uleb128 0
	.4byte	.LASF250
	.byte	0x5
	.uleb128 0
	.4byte	.LASF251
	.byte	0x5
	.uleb128 0
	.4byte	.LASF252
	.byte	0x5
	.uleb128 0
	.4byte	.LASF253
	.byte	0x5
	.uleb128 0
	.4byte	.LASF254
	.byte	0x5
	.uleb128 0
	.4byte	.LASF255
	.byte	0x5
	.uleb128 0
	.4byte	.LASF256
	.byte	0x5
	.uleb128 0
	.4byte	.LASF257
	.byte	0x5
	.uleb128 0
	.4byte	.LASF258
	.byte	0x5
	.uleb128 0
	.4byte	.LASF259
	.byte	0x5
	.uleb128 0
	.4byte	.LASF260
	.byte	0x5
	.uleb128 0
	.4byte	.LASF261
	.byte	0x5
	.uleb128 0
	.4byte	.LASF262
	.byte	0x5
	.uleb128 0
	.4byte	.LASF263
	.byte	0x5
	.uleb128 0
	.4byte	.LASF264
	.byte	0x5
	.uleb128 0
	.4byte	.LASF265
	.byte	0x5
	.uleb128 0
	.4byte	.LASF266
	.byte	0x5
	.uleb128 0
	.4byte	.LASF267
	.byte	0x5
	.uleb128 0
	.4byte	.LASF268
	.byte	0x5
	.uleb128 0
	.4byte	.LASF269
	.byte	0x5
	.uleb128 0
	.4byte	.LASF270
	.byte	0x5
	.uleb128 0
	.4byte	.LASF271
	.byte	0x5
	.uleb128 0
	.4byte	.LASF272
	.byte	0x5
	.uleb128 0
	.4byte	.LASF273
	.byte	0x5
	.uleb128 0
	.4byte	.LASF274
	.byte	0x5
	.uleb128 0
	.4byte	.LASF275
	.byte	0x5
	.uleb128 0
	.4byte	.LASF276
	.byte	0x5
	.uleb128 0
	.4byte	.LASF277
	.byte	0x5
	.uleb128 0
	.4byte	.LASF278
	.byte	0x5
	.uleb128 0
	.4byte	.LASF279
	.byte	0x5
	.uleb128 0
	.4byte	.LASF280
	.byte	0x5
	.uleb128 0
	.4byte	.LASF281
	.byte	0x5
	.uleb128 0
	.4byte	.LASF282
	.byte	0x5
	.uleb128 0
	.4byte	.LASF283
	.byte	0x5
	.uleb128 0
	.4byte	.LASF284
	.byte	0x5
	.uleb128 0
	.4byte	.LASF285
	.byte	0x5
	.uleb128 0
	.4byte	.LASF286
	.byte	0x5
	.uleb128 0
	.4byte	.LASF287
	.byte	0x5
	.uleb128 0
	.4byte	.LASF288
	.byte	0x5
	.uleb128 0
	.4byte	.LASF289
	.byte	0x5
	.uleb128 0
	.4byte	.LASF290
	.byte	0x5
	.uleb128 0
	.4byte	.LASF291
	.byte	0x5
	.uleb128 0
	.4byte	.LASF292
	.byte	0x5
	.uleb128 0
	.4byte	.LASF293
	.byte	0x5
	.uleb128 0
	.4byte	.LASF294
	.byte	0x5
	.uleb128 0
	.4byte	.LASF295
	.byte	0x5
	.uleb128 0
	.4byte	.LASF296
	.byte	0x5
	.uleb128 0
	.4byte	.LASF297
	.byte	0x5
	.uleb128 0
	.4byte	.LASF298
	.byte	0x5
	.uleb128 0
	.4byte	.LASF299
	.byte	0x5
	.uleb128 0
	.4byte	.LASF300
	.byte	0x5
	.uleb128 0
	.4byte	.LASF301
	.byte	0x5
	.uleb128 0
	.4byte	.LASF302
	.byte	0x5
	.uleb128 0
	.4byte	.LASF303
	.byte	0x5
	.uleb128 0
	.4byte	.LASF304
	.byte	0x5
	.uleb128 0
	.4byte	.LASF305
	.byte	0x5
	.uleb128 0
	.4byte	.LASF306
	.byte	0x5
	.uleb128 0
	.4byte	.LASF307
	.byte	0x5
	.uleb128 0
	.4byte	.LASF308
	.byte	0x5
	.uleb128 0
	.4byte	.LASF309
	.byte	0x5
	.uleb128 0
	.4byte	.LASF310
	.byte	0x5
	.uleb128 0
	.4byte	.LASF311
	.byte	0x5
	.uleb128 0
	.4byte	.LASF312
	.byte	0x5
	.uleb128 0
	.4byte	.LASF313
	.byte	0x5
	.uleb128 0
	.4byte	.LASF314
	.byte	0x5
	.uleb128 0
	.4byte	.LASF315
	.byte	0x5
	.uleb128 0
	.4byte	.LASF316
	.byte	0x5
	.uleb128 0
	.4byte	.LASF317
	.byte	0x5
	.uleb128 0
	.4byte	.LASF318
	.byte	0x5
	.uleb128 0
	.4byte	.LASF319
	.byte	0x5
	.uleb128 0
	.4byte	.LASF320
	.byte	0x5
	.uleb128 0
	.4byte	.LASF321
	.byte	0x5
	.uleb128 0
	.4byte	.LASF322
	.byte	0x5
	.uleb128 0
	.4byte	.LASF323
	.byte	0x5
	.uleb128 0
	.4byte	.LASF324
	.byte	0x5
	.uleb128 0
	.4byte	.LASF325
	.byte	0x5
	.uleb128 0
	.4byte	.LASF326
	.byte	0x5
	.uleb128 0
	.4byte	.LASF327
	.byte	0x5
	.uleb128 0
	.4byte	.LASF328
	.byte	0x5
	.uleb128 0
	.4byte	.LASF329
	.byte	0x5
	.uleb128 0
	.4byte	.LASF330
	.byte	0x5
	.uleb128 0
	.4byte	.LASF331
	.byte	0x5
	.uleb128 0
	.4byte	.LASF332
	.byte	0x5
	.uleb128 0
	.4byte	.LASF333
	.byte	0x5
	.uleb128 0
	.4byte	.LASF334
	.byte	0x5
	.uleb128 0
	.4byte	.LASF335
	.byte	0x5
	.uleb128 0
	.4byte	.LASF336
	.byte	0x5
	.uleb128 0
	.4byte	.LASF337
	.byte	0x5
	.uleb128 0
	.4byte	.LASF338
	.byte	0x5
	.uleb128 0
	.4byte	.LASF339
	.byte	0x5
	.uleb128 0
	.4byte	.LASF340
	.byte	0x5
	.uleb128 0
	.4byte	.LASF341
	.byte	0x5
	.uleb128 0
	.4byte	.LASF342
	.byte	0x5
	.uleb128 0
	.4byte	.LASF343
	.byte	0x5
	.uleb128 0
	.4byte	.LASF344
	.byte	0x5
	.uleb128 0
	.4byte	.LASF345
	.byte	0x5
	.uleb128 0
	.4byte	.LASF346
	.byte	0x5
	.uleb128 0
	.4byte	.LASF347
	.byte	0x5
	.uleb128 0
	.4byte	.LASF348
	.byte	0x5
	.uleb128 0
	.4byte	.LASF349
	.byte	0x5
	.uleb128 0
	.4byte	.LASF350
	.byte	0x5
	.uleb128 0
	.4byte	.LASF351
	.byte	0x5
	.uleb128 0
	.4byte	.LASF352
	.byte	0x5
	.uleb128 0
	.4byte	.LASF353
	.byte	0x5
	.uleb128 0
	.4byte	.LASF354
	.byte	0x5
	.uleb128 0
	.4byte	.LASF355
	.byte	0x5
	.uleb128 0
	.4byte	.LASF356
	.byte	0x5
	.uleb128 0
	.4byte	.LASF357
	.byte	0x5
	.uleb128 0
	.4byte	.LASF358
	.byte	0x5
	.uleb128 0
	.4byte	.LASF359
	.byte	0x5
	.uleb128 0
	.4byte	.LASF360
	.byte	0x5
	.uleb128 0
	.4byte	.LASF361
	.byte	0x5
	.uleb128 0
	.4byte	.LASF362
	.byte	0x5
	.uleb128 0
	.4byte	.LASF363
	.byte	0x5
	.uleb128 0
	.4byte	.LASF364
	.byte	0x5
	.uleb128 0
	.4byte	.LASF365
	.byte	0x5
	.uleb128 0
	.4byte	.LASF366
	.byte	0x5
	.uleb128 0
	.4byte	.LASF367
	.byte	0x5
	.uleb128 0
	.4byte	.LASF368
	.byte	0x5
	.uleb128 0
	.4byte	.LASF369
	.byte	0x5
	.uleb128 0
	.4byte	.LASF370
	.byte	0x5
	.uleb128 0
	.4byte	.LASF371
	.byte	0x5
	.uleb128 0
	.4byte	.LASF372
	.byte	0x5
	.uleb128 0
	.4byte	.LASF373
	.byte	0x5
	.uleb128 0
	.4byte	.LASF374
	.byte	0x5
	.uleb128 0
	.4byte	.LASF375
	.byte	0x5
	.uleb128 0
	.4byte	.LASF376
	.byte	0x5
	.uleb128 0
	.4byte	.LASF377
	.byte	0x5
	.uleb128 0
	.4byte	.LASF378
	.byte	0x5
	.uleb128 0
	.4byte	.LASF379
	.byte	0x5
	.uleb128 0
	.4byte	.LASF380
	.byte	0x6
	.uleb128 0
	.4byte	.LASF381
	.byte	0x5
	.uleb128 0
	.4byte	.LASF382
	.byte	0x6
	.uleb128 0
	.4byte	.LASF383
	.byte	0x6
	.uleb128 0
	.4byte	.LASF384
	.byte	0x6
	.uleb128 0
	.4byte	.LASF385
	.byte	0x6
	.uleb128 0
	.4byte	.LASF386
	.byte	0x5
	.uleb128 0
	.4byte	.LASF387
	.byte	0x6
	.uleb128 0
	.4byte	.LASF388
	.byte	0x6
	.uleb128 0
	.4byte	.LASF389
	.byte	0x6
	.uleb128 0
	.4byte	.LASF390
	.byte	0x5
	.uleb128 0
	.4byte	.LASF391
	.byte	0x5
	.uleb128 0
	.4byte	.LASF392
	.byte	0x6
	.uleb128 0
	.4byte	.LASF393
	.byte	0x5
	.uleb128 0
	.4byte	.LASF394
	.byte	0x5
	.uleb128 0
	.4byte	.LASF395
	.byte	0x5
	.uleb128 0
	.4byte	.LASF396
	.byte	0x6
	.uleb128 0
	.4byte	.LASF397
	.byte	0x5
	.uleb128 0
	.4byte	.LASF398
	.byte	0x5
	.uleb128 0
	.4byte	.LASF399
	.byte	0x6
	.uleb128 0
	.4byte	.LASF400
	.byte	0x5
	.uleb128 0
	.4byte	.LASF401
	.byte	0x5
	.uleb128 0
	.4byte	.LASF402
	.byte	0x5
	.uleb128 0
	.4byte	.LASF403
	.byte	0x5
	.uleb128 0
	.4byte	.LASF404
	.byte	0x5
	.uleb128 0
	.4byte	.LASF405
	.byte	0x5
	.uleb128 0
	.4byte	.LASF406
	.byte	0x6
	.uleb128 0
	.4byte	.LASF407
	.byte	0x5
	.uleb128 0
	.4byte	.LASF408
	.byte	0x5
	.uleb128 0
	.4byte	.LASF409
	.byte	0x5
	.uleb128 0
	.4byte	.LASF410
	.byte	0x6
	.uleb128 0
	.4byte	.LASF411
	.byte	0x5
	.uleb128 0
	.4byte	.LASF412
	.byte	0x6
	.uleb128 0
	.4byte	.LASF413
	.byte	0x6
	.uleb128 0
	.4byte	.LASF414
	.byte	0x6
	.uleb128 0
	.4byte	.LASF415
	.byte	0x6
	.uleb128 0
	.4byte	.LASF416
	.byte	0x6
	.uleb128 0
	.4byte	.LASF417
	.byte	0x6
	.uleb128 0
	.4byte	.LASF418
	.byte	0x5
	.uleb128 0
	.4byte	.LASF419
	.byte	0x6
	.uleb128 0
	.4byte	.LASF420
	.byte	0x6
	.uleb128 0
	.4byte	.LASF421
	.byte	0x6
	.uleb128 0
	.4byte	.LASF422
	.byte	0x5
	.uleb128 0
	.4byte	.LASF423
	.byte	0x5
	.uleb128 0
	.4byte	.LASF424
	.byte	0x5
	.uleb128 0
	.4byte	.LASF425
	.byte	0x5
	.uleb128 0
	.4byte	.LASF426
	.byte	0x6
	.uleb128 0
	.4byte	.LASF427
	.byte	0x5
	.uleb128 0
	.4byte	.LASF428
	.byte	0x5
	.uleb128 0
	.4byte	.LASF429
	.byte	0x5
	.uleb128 0
	.4byte	.LASF430
	.byte	0x6
	.uleb128 0
	.4byte	.LASF431
	.byte	0x5
	.uleb128 0
	.4byte	.LASF432
	.byte	0x6
	.uleb128 0
	.4byte	.LASF433
	.byte	0x6
	.uleb128 0
	.4byte	.LASF434
	.byte	0x6
	.uleb128 0
	.4byte	.LASF435
	.byte	0x6
	.uleb128 0
	.4byte	.LASF436
	.byte	0x6
	.uleb128 0
	.4byte	.LASF437
	.byte	0x6
	.uleb128 0
	.4byte	.LASF438
	.byte	0x5
	.uleb128 0
	.4byte	.LASF439
	.byte	0x5
	.uleb128 0
	.4byte	.LASF440
	.byte	0x5
	.uleb128 0
	.4byte	.LASF441
	.byte	0x5
	.uleb128 0
	.4byte	.LASF424
	.byte	0x5
	.uleb128 0
	.4byte	.LASF442
	.byte	0x5
	.uleb128 0
	.4byte	.LASF443
	.byte	0x5
	.uleb128 0
	.4byte	.LASF444
	.byte	0x5
	.uleb128 0
	.4byte	.LASF445
	.byte	0x5
	.uleb128 0
	.4byte	.LASF446
	.byte	0x5
	.uleb128 0
	.4byte	.LASF447
	.byte	0x5
	.uleb128 0
	.4byte	.LASF448
	.byte	0x5
	.uleb128 0
	.4byte	.LASF449
	.byte	0x5
	.uleb128 0
	.4byte	.LASF450
	.byte	0x5
	.uleb128 0
	.4byte	.LASF451
	.byte	0x5
	.uleb128 0
	.4byte	.LASF452
	.byte	0x5
	.uleb128 0
	.4byte	.LASF447
	.byte	0x5
	.uleb128 0
	.4byte	.LASF448
	.byte	0x5
	.uleb128 0
	.4byte	.LASF453
	.byte	0x5
	.uleb128 0
	.4byte	.LASF454
	.byte	0x5
	.uleb128 0
	.4byte	.LASF455
	.byte	0x5
	.uleb128 0
	.4byte	.LASF456
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.ffconf.h.5.b4839ce64205d45063d760af69148a1c,comdat
.Ldebug_macro3:
	.2byte	0x4
	.byte	0
	.byte	0x5
	.uleb128 0x5
	.4byte	.LASF459
	.byte	0x5
	.uleb128 0xb
	.4byte	.LASF460
	.byte	0x5
	.uleb128 0x12
	.4byte	.LASF461
	.byte	0x5
	.uleb128 0x1c
	.4byte	.LASF462
	.byte	0x5
	.uleb128 0x25
	.4byte	.LASF463
	.byte	0x5
	.uleb128 0x2a
	.4byte	.LASF464
	.byte	0x5
	.uleb128 0x2e
	.4byte	.LASF465
	.byte	0x5
	.uleb128 0x32
	.4byte	.LASF466
	.byte	0x5
	.uleb128 0x36
	.4byte	.LASF467
	.byte	0x5
	.uleb128 0x3b
	.4byte	.LASF468
	.byte	0x5
	.uleb128 0x40
	.4byte	.LASF469
	.byte	0x5
	.uleb128 0x48
	.4byte	.LASF470
	.byte	0x5
	.uleb128 0x65
	.4byte	.LASF471
	.byte	0x5
	.uleb128 0x66
	.4byte	.LASF472
	.byte	0x5
	.uleb128 0x77
	.4byte	.LASF473
	.byte	0x5
	.uleb128 0x7d
	.4byte	.LASF474
	.byte	0x5
	.uleb128 0x89
	.4byte	.LASF475
	.byte	0x5
	.uleb128 0x96
	.4byte	.LASF476
	.byte	0x5
	.uleb128 0x9a
	.4byte	.LASF477
	.byte	0x5
	.uleb128 0x9b
	.4byte	.LASF478
	.byte	0x5
	.uleb128 0xa3
	.4byte	.LASF479
	.byte	0x5
	.uleb128 0xac
	.4byte	.LASF480
	.byte	0x5
	.uleb128 0xad
	.4byte	.LASF481
	.byte	0x5
	.uleb128 0xb6
	.4byte	.LASF482
	.byte	0x5
	.uleb128 0xbc
	.4byte	.LASF483
	.byte	0x5
	.uleb128 0xcd
	.4byte	.LASF484
	.byte	0x5
	.uleb128 0xd4
	.4byte	.LASF485
	.byte	0x5
	.uleb128 0xda
	.4byte	.LASF486
	.byte	0x5
	.uleb128 0xdb
	.4byte	.LASF487
	.byte	0x5
	.uleb128 0xdc
	.4byte	.LASF488
	.byte	0x5
	.uleb128 0xdd
	.4byte	.LASF489
	.byte	0x5
	.uleb128 0xe8
	.4byte	.LASF490
	.byte	0x5
	.uleb128 0xf4
	.4byte	.LASF491
	.byte	0x5
	.uleb128 0xf5
	.4byte	.LASF492
	.byte	0x5
	.uleb128 0xf6
	.4byte	.LASF493
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.ff.h.49.302f9883cba4050a8f4125907f144cd6,comdat
.Ldebug_macro4:
	.2byte	0x4
	.byte	0
	.byte	0x5
	.uleb128 0x31
	.4byte	.LASF494
	.byte	0x5
	.uleb128 0x32
	.4byte	.LASF495
	.byte	0x5
	.uleb128 0x46
	.4byte	.LASF496
	.byte	0x5
	.uleb128 0x47
	.4byte	.LASF497
	.byte	0x5
	.uleb128 0x11a
	.4byte	.LASF498
	.byte	0x5
	.uleb128 0x11b
	.4byte	.LASF499
	.byte	0x5
	.uleb128 0x11c
	.4byte	.LASF500
	.byte	0x5
	.uleb128 0x11d
	.4byte	.LASF501
	.byte	0x5
	.uleb128 0x11e
	.4byte	.LASF502
	.byte	0x5
	.uleb128 0x11f
	.4byte	.LASF503
	.byte	0x5
	.uleb128 0x122
	.4byte	.LASF504
	.byte	0x5
	.uleb128 0x14a
	.4byte	.LASF505
	.byte	0x5
	.uleb128 0x14b
	.4byte	.LASF506
	.byte	0x5
	.uleb128 0x14c
	.4byte	.LASF507
	.byte	0x5
	.uleb128 0x14d
	.4byte	.LASF508
	.byte	0x5
	.uleb128 0x14e
	.4byte	.LASF509
	.byte	0x5
	.uleb128 0x14f
	.4byte	.LASF510
	.byte	0x5
	.uleb128 0x150
	.4byte	.LASF511
	.byte	0x5
	.uleb128 0x153
	.4byte	.LASF512
	.byte	0x5
	.uleb128 0x156
	.4byte	.LASF513
	.byte	0x5
	.uleb128 0x157
	.4byte	.LASF514
	.byte	0x5
	.uleb128 0x158
	.4byte	.LASF515
	.byte	0x5
	.uleb128 0x159
	.4byte	.LASF516
	.byte	0x5
	.uleb128 0x15a
	.4byte	.LASF517
	.byte	0x5
	.uleb128 0x15d
	.4byte	.LASF518
	.byte	0x5
	.uleb128 0x15e
	.4byte	.LASF519
	.byte	0x5
	.uleb128 0x15f
	.4byte	.LASF520
	.byte	0x5
	.uleb128 0x160
	.4byte	.LASF521
	.byte	0x5
	.uleb128 0x163
	.4byte	.LASF522
	.byte	0x5
	.uleb128 0x164
	.4byte	.LASF523
	.byte	0x5
	.uleb128 0x165
	.4byte	.LASF524
	.byte	0x5
	.uleb128 0x166
	.4byte	.LASF525
	.byte	0x5
	.uleb128 0x167
	.4byte	.LASF526
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.diskio.h.6.0589a180d40e129a797d61e5322f03c1,comdat
.Ldebug_macro5:
	.2byte	0x4
	.byte	0
	.byte	0x5
	.uleb128 0x6
	.4byte	.LASF527
	.byte	0x5
	.uleb128 0x29
	.4byte	.LASF528
	.byte	0x5
	.uleb128 0x2a
	.4byte	.LASF529
	.byte	0x5
	.uleb128 0x2b
	.4byte	.LASF530
	.byte	0x5
	.uleb128 0x31
	.4byte	.LASF531
	.byte	0x5
	.uleb128 0x32
	.4byte	.LASF532
	.byte	0x5
	.uleb128 0x33
	.4byte	.LASF533
	.byte	0x5
	.uleb128 0x34
	.4byte	.LASF534
	.byte	0x5
	.uleb128 0x35
	.4byte	.LASF535
	.byte	0x5
	.uleb128 0x38
	.4byte	.LASF536
	.byte	0x5
	.uleb128 0x39
	.4byte	.LASF537
	.byte	0x5
	.uleb128 0x3a
	.4byte	.LASF538
	.byte	0x5
	.uleb128 0x3b
	.4byte	.LASF539
	.byte	0x5
	.uleb128 0x3e
	.4byte	.LASF540
	.byte	0x5
	.uleb128 0x3f
	.4byte	.LASF541
	.byte	0x5
	.uleb128 0x40
	.4byte	.LASF542
	.byte	0x5
	.uleb128 0x41
	.4byte	.LASF543
	.byte	0x5
	.uleb128 0x42
	.4byte	.LASF544
	.byte	0x5
	.uleb128 0x43
	.4byte	.LASF545
	.byte	0x5
	.uleb128 0x44
	.4byte	.LASF546
	.byte	0x5
	.uleb128 0x45
	.4byte	.LASF547
	.byte	0x5
	.uleb128 0x48
	.4byte	.LASF548
	.byte	0x5
	.uleb128 0x49
	.4byte	.LASF549
	.byte	0x5
	.uleb128 0x4a
	.4byte	.LASF550
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.__crossworks.h.39.ff21eb83ebfc80fb95245a821dd1e413,comdat
.Ldebug_macro6:
	.2byte	0x4
	.byte	0
	.byte	0x5
	.uleb128 0x27
	.4byte	.LASF704
	.byte	0x5
	.uleb128 0x3b
	.4byte	.LASF705
	.byte	0x6
	.uleb128 0x3d
	.4byte	.LASF706
	.byte	0x5
	.uleb128 0x3f
	.4byte	.LASF707
	.byte	0x5
	.uleb128 0x43
	.4byte	.LASF708
	.byte	0x5
	.uleb128 0x45
	.4byte	.LASF709
	.byte	0x5
	.uleb128 0x56
	.4byte	.LASF710
	.byte	0x5
	.uleb128 0x5d
	.4byte	.LASF705
	.byte	0x5
	.uleb128 0x63
	.4byte	.LASF711
	.byte	0x5
	.uleb128 0x64
	.4byte	.LASF712
	.byte	0x5
	.uleb128 0x65
	.4byte	.LASF713
	.byte	0x5
	.uleb128 0x66
	.4byte	.LASF714
	.byte	0x5
	.uleb128 0x67
	.4byte	.LASF715
	.byte	0x5
	.uleb128 0x68
	.4byte	.LASF716
	.byte	0x5
	.uleb128 0x69
	.4byte	.LASF717
	.byte	0x5
	.uleb128 0x6a
	.4byte	.LASF718
	.byte	0x5
	.uleb128 0x6d
	.4byte	.LASF719
	.byte	0x5
	.uleb128 0x6e
	.4byte	.LASF720
	.byte	0x5
	.uleb128 0x6f
	.4byte	.LASF721
	.byte	0x5
	.uleb128 0x70
	.4byte	.LASF722
	.byte	0x5
	.uleb128 0x73
	.4byte	.LASF723
	.byte	0x5
	.uleb128 0xd8
	.4byte	.LASF724
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.stdarg.h.65.197a2f7844805c40e37e0a7534170ffa,comdat
.Ldebug_macro7:
	.2byte	0x4
	.byte	0
	.byte	0x5
	.uleb128 0x41
	.4byte	.LASF725
	.byte	0x5
	.uleb128 0x44
	.4byte	.LASF726
	.byte	0x5
	.uleb128 0x47
	.4byte	.LASF727
	.byte	0x5
	.uleb128 0x4a
	.4byte	.LASF728
	.byte	0
	.section	.debug_line,"",%progbits
.Ldebug_line0:
	.section	.debug_str,"MS",%progbits,1
.LASF346:
	.ascii	"__UHA_FBIT__ 8\000"
.LASF839:
	.ascii	"sz_fat\000"
.LASF703:
	.ascii	"__stdarg_H \000"
.LASF698:
	.ascii	"DDEM 0xE5\000"
.LASF773:
	.ascii	"dptr\000"
.LASF358:
	.ascii	"__CHAR_UNSIGNED__ 1\000"
.LASF477:
	.ascii	"_STR_VOLUME_ID 0\000"
.LASF674:
	.ascii	"LDIR_FstClusLO 26\000"
.LASF895:
	.ascii	"gen_numname\000"
.LASF221:
	.ascii	"__FLT64_HAS_INFINITY__ 1\000"
.LASF838:
	.ascii	"sz_rsv\000"
.LASF705:
	.ascii	"__THREAD __thread\000"
.LASF310:
	.ascii	"__LLACCUM_MIN__ (-0X1P31LLK-0X1P31LLK)\000"
.LASF6:
	.ascii	"__GNUC_MINOR__ 2\000"
.LASF900:
	.ascii	"ld_clust\000"
.LASF302:
	.ascii	"__LACCUM_EPSILON__ 0x1P-31LK\000"
.LASF77:
	.ascii	"__PTRDIFF_MAX__ 0x7fffffff\000"
.LASF768:
	.ascii	"fptr\000"
.LASF605:
	.ascii	"BS_NTres 37\000"
.LASF590:
	.ascii	"BS_JmpBoot 0\000"
.LASF89:
	.ascii	"__INTMAX_C(c) c ## LL\000"
.LASF217:
	.ascii	"__FLT64_MIN__ 1.1\000"
.LASF662:
	.ascii	"DIR_NTres 12\000"
.LASF88:
	.ascii	"__INTMAX_MAX__ 0x7fffffffffffffffLL\000"
.LASF327:
	.ascii	"__TQ_IBIT__ 0\000"
.LASF579:
	.ascii	"NS_BODY 0x08\000"
.LASF225:
	.ascii	"__FLT32X_MIN_EXP__ (-1021)\000"
.LASF677:
	.ascii	"XDIR_Label 2\000"
.LASF507:
	.ascii	"FA_OPEN_EXISTING 0x00\000"
.LASF464:
	.ascii	"_USE_MKFS 1\000"
.LASF14:
	.ascii	"__ATOMIC_CONSUME 1\000"
.LASF303:
	.ascii	"__ULACCUM_FBIT__ 32\000"
.LASF73:
	.ascii	"__WCHAR_MAX__ 0xffffffffU\000"
.LASF633:
	.ascii	"BPB_VolIDEx 100\000"
.LASF445:
	.ascii	"__SES_VERSION 53400\000"
.LASF655:
	.ascii	"PTE_EdHead 5\000"
.LASF145:
	.ascii	"__FLT_MANT_DIG__ 24\000"
.LASF449:
	.ascii	"APP_TIMER_V2 1\000"
.LASF584:
	.ascii	"MAX_FAT12 0xFF5\000"
.LASF18:
	.ascii	"__SIZEOF_LONG_LONG__ 8\000"
.LASF615:
	.ascii	"BPB_RootClus32 44\000"
.LASF166:
	.ascii	"__DBL_MAX_10_EXP__ 308\000"
.LASF250:
	.ascii	"__FRACT_MIN__ (-0.5R-0.5R)\000"
.LASF316:
	.ascii	"__ULLACCUM_MAX__ 0XFFFFFFFFFFFFFFFFP-32ULLK\000"
.LASF630:
	.ascii	"BPB_DataOfsEx 88\000"
.LASF286:
	.ascii	"__USACCUM_MAX__ 0XFFFFP-8UHK\000"
.LASF238:
	.ascii	"__SFRACT_FBIT__ 7\000"
.LASF824:
	.ascii	"path\000"
.LASF519:
	.ascii	"FS_FAT16 2\000"
.LASF436:
	.ascii	"__ARM_FEATURE_BF16_SCALAR_ARITHMETIC\000"
.LASF323:
	.ascii	"__SQ_IBIT__ 0\000"
.LASF28:
	.ascii	"__ORDER_PDP_ENDIAN__ 3412\000"
.LASF805:
	.ascii	"RES_WRPRT\000"
.LASF814:
	.ascii	"nchr\000"
.LASF647:
	.ascii	"FSI_Nxt_Free 492\000"
.LASF157:
	.ascii	"__FLT_HAS_DENORM__ 1\000"
.LASF41:
	.ascii	"__INT8_TYPE__ signed char\000"
.LASF398:
	.ascii	"__ARM_ARCH_PROFILE 77\000"
.LASF458:
	.ascii	"_FF_INTEGER \000"
.LASF285:
	.ascii	"__USACCUM_MIN__ 0.0UHK\000"
.LASF883:
	.ascii	"get_ldnumber\000"
.LASF198:
	.ascii	"__FLT32_DECIMAL_DIG__ 9\000"
.LASF182:
	.ascii	"__DECIMAL_DIG__ 17\000"
.LASF178:
	.ascii	"__LDBL_MIN_EXP__ (-1021)\000"
.LASF176:
	.ascii	"__LDBL_MANT_DIG__ 53\000"
.LASF517:
	.ascii	"FM_SFD 0x08\000"
.LASF117:
	.ascii	"__UINT8_C(c) c\000"
.LASF525:
	.ascii	"AM_DIR 0x10\000"
.LASF42:
	.ascii	"__INT16_TYPE__ short int\000"
.LASF586:
	.ascii	"MAX_FAT32 0xFFFFFF5\000"
.LASF537:
	.ascii	"CTRL_LOCK 6\000"
.LASF536:
	.ascii	"CTRL_POWER 5\000"
.LASF359:
	.ascii	"__GCC_HAVE_SYNC_COMPARE_AND_SWAP_1 1\000"
.LASF920:
	.ascii	"st_dword\000"
.LASF725:
	.ascii	"va_start(v,l) __builtin_va_start((v),l)\000"
.LASF542:
	.ascii	"MMC_GET_CID 12\000"
.LASF4:
	.ascii	"__STDC_HOSTED__ 1\000"
.LASF875:
	.ascii	"find_volume\000"
.LASF755:
	.ascii	"fsize\000"
.LASF857:
	.ascii	"f_lseek\000"
.LASF60:
	.ascii	"__INT_FAST64_TYPE__ long long int\000"
.LASF35:
	.ascii	"__WINT_TYPE__ unsigned int\000"
.LASF884:
	.ascii	"follow_path\000"
.LASF911:
	.ascii	"sync_fs\000"
.LASF197:
	.ascii	"__FLT32_MAX_10_EXP__ 38\000"
.LASF741:
	.ascii	"TCHAR\000"
.LASF468:
	.ascii	"_USE_LABEL 0\000"
.LASF613:
	.ascii	"BPB_ExtFlags32 40\000"
.LASF246:
	.ascii	"__USFRACT_MAX__ 0XFFP-8UHR\000"
.LASF207:
	.ascii	"__FP_FAST_FMAF32 1\000"
.LASF138:
	.ascii	"__UINTPTR_MAX__ 0xffffffffU\000"
.LASF194:
	.ascii	"__FLT32_MIN_EXP__ (-125)\000"
.LASF569:
	.ascii	"AM_VOL 0x08\000"
.LASF631:
	.ascii	"BPB_NumClusEx 92\000"
.LASF656:
	.ascii	"PTE_EdSec 6\000"
.LASF763:
	.ascii	"stat\000"
.LASF809:
	.ascii	"Fsid\000"
.LASF509:
	.ascii	"FA_CREATE_ALWAYS 0x08\000"
.LASF800:
	.ascii	"FR_INVALID_PARAMETER\000"
.LASF211:
	.ascii	"__FLT64_MIN_10_EXP__ (-307)\000"
.LASF491:
	.ascii	"_FS_REENTRANT 0\000"
.LASF131:
	.ascii	"__INT_FAST64_WIDTH__ 64\000"
.LASF1:
	.ascii	"__STDC_VERSION__ 199901L\000"
.LASF877:
	.ascii	"fasize\000"
.LASF242:
	.ascii	"__SFRACT_EPSILON__ 0x1P-7HR\000"
.LASF625:
	.ascii	"BPB_ZeroedEx 11\000"
.LASF912:
	.ascii	"move_window\000"
.LASF111:
	.ascii	"__INT32_C(c) c ## L\000"
.LASF27:
	.ascii	"__ORDER_BIG_ENDIAN__ 4321\000"
.LASF423:
	.ascii	"__THUMB_INTERWORK__ 1\000"
.LASF322:
	.ascii	"__SQ_FBIT__ 31\000"
.LASF215:
	.ascii	"__FLT64_MAX__ 1.1\000"
.LASF352:
	.ascii	"__UTA_FBIT__ 64\000"
.LASF863:
	.ascii	"wcnt\000"
.LASF489:
	.ascii	"_NORTC_YEAR 2016\000"
.LASF330:
	.ascii	"__UHQ_FBIT__ 16\000"
.LASF751:
	.ascii	"last_clst\000"
.LASF210:
	.ascii	"__FLT64_MIN_EXP__ (-1021)\000"
.LASF451:
	.ascii	"BOARD_PCA10056 1\000"
.LASF870:
	.ascii	"f_open\000"
.LASF818:
	.ascii	"f_putc\000"
.LASF233:
	.ascii	"__FLT32X_EPSILON__ 1.1\000"
.LASF132:
	.ascii	"__UINT_FAST8_MAX__ 0xffffffffU\000"
.LASF447:
	.ascii	"DEBUG 1\000"
.LASF152:
	.ascii	"__FLT_MAX__ 1.1\000"
.LASF299:
	.ascii	"__LACCUM_IBIT__ 32\000"
.LASF704:
	.ascii	"__crossworks_H \000"
.LASF817:
	.ascii	"f_puts\000"
.LASF879:
	.ascii	"sysect\000"
.LASF787:
	.ascii	"FR_INVALID_NAME\000"
.LASF410:
	.ascii	"__VFP_FP__ 1\000"
.LASF577:
	.ascii	"NS_LFN 0x02\000"
.LASF270:
	.ascii	"__LLFRACT_MIN__ (-0.5LLR-0.5LLR)\000"
.LASF136:
	.ascii	"__INTPTR_MAX__ 0x7fffffff\000"
.LASF520:
	.ascii	"FS_FAT32 3\000"
.LASF480:
	.ascii	"_MIN_SS 512\000"
.LASF133:
	.ascii	"__UINT_FAST16_MAX__ 0xffffffffU\000"
.LASF849:
	.ascii	"nclst\000"
.LASF823:
	.ascii	"f_mkfs\000"
.LASF581:
	.ascii	"NS_DOT 0x20\000"
.LASF815:
	.ascii	"putbuff\000"
.LASF492:
	.ascii	"_FS_TIMEOUT 1000\000"
.LASF202:
	.ascii	"__FLT32_EPSILON__ 1.1\000"
.LASF876:
	.ascii	"bsect\000"
.LASF255:
	.ascii	"__UFRACT_MIN__ 0.0UR\000"
.LASF127:
	.ascii	"__INT_FAST16_WIDTH__ 32\000"
.LASF455:
	.ascii	"NO_VTOR_CONFIG 1\000"
.LASF804:
	.ascii	"RES_ERROR\000"
.LASF434:
	.ascii	"__ARM_FEATURE_CDE_COPROC\000"
.LASF94:
	.ascii	"__SIG_ATOMIC_MIN__ (-__SIG_ATOMIC_MAX__ - 1)\000"
.LASF236:
	.ascii	"__FLT32X_HAS_INFINITY__ 1\000"
.LASF610:
	.ascii	"BS_BootCode 62\000"
.LASF882:
	.ascii	"check_fs\000"
.LASF551:
	.ascii	"ABORT(fs,res) { fp->err = (BYTE)(res); LEAVE_FF(fs,"
	.ascii	" res); }\000"
.LASF508:
	.ascii	"FA_CREATE_NEW 0x04\000"
.LASF53:
	.ascii	"__UINT_LEAST8_TYPE__ unsigned char\000"
.LASF691:
	.ascii	"XDIR_NumName 35\000"
.LASF845:
	.ascii	"f_unlink\000"
.LASF288:
	.ascii	"__ACCUM_FBIT__ 15\000"
.LASF294:
	.ascii	"__UACCUM_IBIT__ 16\000"
.LASF737:
	.ascii	"long int\000"
.LASF776:
	.ascii	"ftime\000"
.LASF232:
	.ascii	"__FLT32X_MIN__ 1.1\000"
.LASF130:
	.ascii	"__INT_FAST64_MAX__ 0x7fffffffffffffffLL\000"
.LASF229:
	.ascii	"__FLT32X_DECIMAL_DIG__ 17\000"
.LASF710:
	.ascii	"__CODE \000"
.LASF237:
	.ascii	"__FLT32X_HAS_QUIET_NAN__ 1\000"
.LASF664:
	.ascii	"DIR_CrtTime 14\000"
.LASF552:
	.ascii	"ENTER_FF(fs) \000"
.LASF746:
	.ascii	"wflag\000"
.LASF57:
	.ascii	"__INT_FAST8_TYPE__ int\000"
.LASF350:
	.ascii	"__UDA_FBIT__ 32\000"
.LASF469:
	.ascii	"_USE_FORWARD 0\000"
.LASF213:
	.ascii	"__FLT64_MAX_10_EXP__ 308\000"
.LASF287:
	.ascii	"__USACCUM_EPSILON__ 0x1P-8UHK\000"
.LASF91:
	.ascii	"__UINTMAX_C(c) c ## ULL\000"
.LASF31:
	.ascii	"__SIZEOF_POINTER__ 4\000"
.LASF49:
	.ascii	"__INT_LEAST8_TYPE__ signed char\000"
.LASF362:
	.ascii	"__GCC_ATOMIC_BOOL_LOCK_FREE 2\000"
.LASF420:
	.ascii	"__ARM_NEON__\000"
.LASF196:
	.ascii	"__FLT32_MAX_EXP__ 128\000"
.LASF669:
	.ascii	"DIR_FileSize 28\000"
.LASF922:
	.ascii	"ld_dword\000"
.LASF276:
	.ascii	"__ULLFRACT_MAX__ 0XFFFFFFFFFFFFFFFFP-64ULLR\000"
.LASF214:
	.ascii	"__FLT64_DECIMAL_DIG__ 17\000"
.LASF791:
	.ascii	"FR_WRITE_PROTECTED\000"
.LASF226:
	.ascii	"__FLT32X_MIN_10_EXP__ (-307)\000"
.LASF39:
	.ascii	"__CHAR32_TYPE__ long unsigned int\000"
.LASF810:
	.ascii	"LfnBuf\000"
.LASF134:
	.ascii	"__UINT_FAST32_MAX__ 0xffffffffU\000"
.LASF149:
	.ascii	"__FLT_MAX_EXP__ 128\000"
.LASF17:
	.ascii	"__SIZEOF_LONG__ 4\000"
.LASF533:
	.ascii	"GET_SECTOR_SIZE 2\000"
.LASF642:
	.ascii	"BPB_RsvdEx 113\000"
.LASF789:
	.ascii	"FR_EXIST\000"
.LASF21:
	.ascii	"__SIZEOF_DOUBLE__ 8\000"
.LASF112:
	.ascii	"__INT_LEAST32_WIDTH__ 32\000"
.LASF812:
	.ascii	"__va_list\000"
.LASF783:
	.ascii	"FR_INT_ERR\000"
.LASF153:
	.ascii	"__FLT_NORM_MAX__ 1.1\000"
.LASF665:
	.ascii	"DIR_LstAccDate 18\000"
.LASF620:
	.ascii	"BS_BootSig32 66\000"
.LASF874:
	.ascii	"validate\000"
.LASF148:
	.ascii	"__FLT_MIN_10_EXP__ (-37)\000"
.LASF151:
	.ascii	"__FLT_DECIMAL_DIG__ 9\000"
.LASF404:
	.ascii	"__thumb__ 1\000"
.LASF915:
	.ascii	"wsect\000"
.LASF686:
	.ascii	"XDIR_ModTime10 21\000"
.LASF48:
	.ascii	"__UINT64_TYPE__ long long unsigned int\000"
.LASF692:
	.ascii	"XDIR_NameHash 36\000"
.LASF419:
	.ascii	"__ARM_FEATURE_FMA 1\000"
.LASF187:
	.ascii	"__LDBL_EPSILON__ 1.1\000"
.LASF356:
	.ascii	"__GNUC_STDC_INLINE__ 1\000"
.LASF628:
	.ascii	"BPB_FatOfsEx 80\000"
.LASF248:
	.ascii	"__FRACT_FBIT__ 15\000"
.LASF7:
	.ascii	"__GNUC_PATCHLEVEL__ 1\000"
.LASF679:
	.ascii	"XDIR_NumSec 1\000"
.LASF118:
	.ascii	"__UINT_LEAST16_MAX__ 0xffff\000"
.LASF548:
	.ascii	"ATA_GET_REV 20\000"
.LASF298:
	.ascii	"__LACCUM_FBIT__ 31\000"
.LASF629:
	.ascii	"BPB_FatSzEx 84\000"
.LASF771:
	.ascii	"dir_sect\000"
.LASF807:
	.ascii	"RES_PARERR\000"
.LASF890:
	.ascii	"nlen\000"
.LASF146:
	.ascii	"__FLT_DIG__ 6\000"
.LASF799:
	.ascii	"FR_TOO_MANY_OPEN_FILES\000"
.LASF63:
	.ascii	"__UINT_FAST32_TYPE__ unsigned int\000"
.LASF732:
	.ascii	"unsigned char\000"
.LASF3:
	.ascii	"__STDC_UTF_32__ 1\000"
.LASF20:
	.ascii	"__SIZEOF_FLOAT__ 4\000"
.LASF279:
	.ascii	"__SACCUM_IBIT__ 8\000"
.LASF150:
	.ascii	"__FLT_MAX_10_EXP__ 38\000"
.LASF251:
	.ascii	"__FRACT_MAX__ 0X7FFFP-15R\000"
.LASF139:
	.ascii	"__GCC_IEC_559 0\000"
.LASF164:
	.ascii	"__DBL_MIN_10_EXP__ (-307)\000"
.LASF624:
	.ascii	"BS_BootCode32 90\000"
.LASF660:
	.ascii	"DIR_Name 0\000"
.LASF836:
	.ascii	"b_data\000"
.LASF10:
	.ascii	"__ATOMIC_SEQ_CST 5\000"
.LASF518:
	.ascii	"FS_FAT12 1\000"
.LASF685:
	.ascii	"XDIR_CrtTime10 20\000"
.LASF885:
	.ascii	"create_name\000"
.LASF545:
	.ascii	"ISDIO_READ 55\000"
.LASF744:
	.ascii	"fs_type\000"
.LASF861:
	.ascii	"f_sync\000"
.LASF530:
	.ascii	"STA_PROTECT 0x04\000"
.LASF101:
	.ascii	"__UINT16_MAX__ 0xffff\000"
.LASF784:
	.ascii	"FR_NOT_READY\000"
.LASF644:
	.ascii	"FSI_LeadSig 0\000"
.LASF326:
	.ascii	"__TQ_FBIT__ 127\000"
.LASF172:
	.ascii	"__DBL_DENORM_MIN__ ((double)1.1)\000"
.LASF405:
	.ascii	"__thumb2__ 1\000"
.LASF313:
	.ascii	"__ULLACCUM_FBIT__ 32\000"
.LASF622:
	.ascii	"BS_VolLab32 71\000"
.LASF498:
	.ascii	"f_eof(fp) ((int)((fp)->fptr == (fp)->obj.objsize))\000"
.LASF858:
	.ascii	"ifptr\000"
.LASF593:
	.ascii	"BPB_SecPerClus 13\000"
.LASF580:
	.ascii	"NS_EXT 0x10\000"
.LASF167:
	.ascii	"__DBL_DECIMAL_DIG__ 17\000"
.LASF22:
	.ascii	"__SIZEOF_LONG_DOUBLE__ 8\000"
.LASF617:
	.ascii	"BPB_BkBootSec32 50\000"
.LASF374:
	.ascii	"__PRAGMA_REDEFINE_EXTNAME 1\000"
.LASF34:
	.ascii	"__WCHAR_TYPE__ unsigned int\000"
.LASF742:
	.ascii	"char\000"
.LASF193:
	.ascii	"__FLT32_DIG__ 6\000"
.LASF360:
	.ascii	"__GCC_HAVE_SYNC_COMPARE_AND_SWAP_2 1\000"
.LASF905:
	.ascii	"create_chain\000"
.LASF684:
	.ascii	"XDIR_AccTime 16\000"
.LASF640:
	.ascii	"BPB_DrvNumEx 111\000"
.LASF559:
	.ascii	"_DF2E 0xFC\000"
.LASF64:
	.ascii	"__UINT_FAST64_TYPE__ long long unsigned int\000"
.LASF653:
	.ascii	"PTE_StCyl 3\000"
.LASF833:
	.ascii	"nsect\000"
.LASF774:
	.ascii	"blk_ofs\000"
.LASF866:
	.ascii	"f_read\000"
.LASF607:
	.ascii	"BS_VolID 39\000"
.LASF427:
	.ascii	"__FDPIC__\000"
.LASF754:
	.ascii	"n_fatent\000"
.LASF234:
	.ascii	"__FLT32X_DENORM_MIN__ 1.1\000"
.LASF693:
	.ascii	"XDIR_ValidFileSize 40\000"
.LASF832:
	.ascii	"n_clst\000"
.LASF244:
	.ascii	"__USFRACT_IBIT__ 0\000"
.LASF567:
	.ascii	"IsDBCS1(c) (((BYTE)(c) >= _DF1S && (BYTE)(c) <= _DF"
	.ascii	"1E) || ((BYTE)(c) >= _DF2S && (BYTE)(c) <= _DF2E))\000"
.LASF245:
	.ascii	"__USFRACT_MIN__ 0.0UHR\000"
.LASF720:
	.ascii	"__CTYPE_ALNUM (__CTYPE_UPPER | __CTYPE_LOWER | __CT"
	.ascii	"YPE_DIGIT)\000"
.LASF441:
	.ascii	"__SIZEOF_WCHAR_T 4\000"
.LASF421:
	.ascii	"__ARM_NEON\000"
.LASF100:
	.ascii	"__UINT8_MAX__ 0xff\000"
.LASF180:
	.ascii	"__LDBL_MAX_EXP__ 1024\000"
.LASF484:
	.ascii	"_FS_TINY 0\000"
.LASF788:
	.ascii	"FR_DENIED\000"
.LASF903:
	.ascii	"stretch\000"
.LASF553:
	.ascii	"LEAVE_FF(fs,res) return res\000"
.LASF173:
	.ascii	"__DBL_HAS_DENORM__ 1\000"
.LASF568:
	.ascii	"IsDBCS2(c) (((BYTE)(c) >= _DS1S && (BYTE)(c) <= _DS"
	.ascii	"1E) || ((BYTE)(c) >= _DS2S && (BYTE)(c) <= _DS2E))\000"
.LASF743:
	.ascii	"FSIZE_t\000"
.LASF503:
	.ascii	"f_rewinddir(dp) f_readdir((dp), 0)\000"
.LASF514:
	.ascii	"FM_FAT32 0x02\000"
.LASF342:
	.ascii	"__DA_FBIT__ 31\000"
.LASF67:
	.ascii	"__GXX_ABI_VERSION 1014\000"
.LASF456:
	.ascii	"NRF52840_XXAA 1\000"
.LASF107:
	.ascii	"__INT_LEAST16_MAX__ 0x7fff\000"
.LASF811:
	.ascii	"LfnOfs\000"
.LASF602:
	.ascii	"BPB_HiddSec 28\000"
.LASF688:
	.ascii	"XDIR_ModTZ 23\000"
.LASF121:
	.ascii	"__UINT32_C(c) c ## UL\000"
.LASF295:
	.ascii	"__UACCUM_MIN__ 0.0UK\000"
.LASF583:
	.ascii	"NS_NONAME 0x80\000"
.LASF513:
	.ascii	"FM_FAT 0x01\000"
.LASF216:
	.ascii	"__FLT64_NORM_MAX__ 1.1\000"
.LASF467:
	.ascii	"_USE_CHMOD 0\000"
.LASF367:
	.ascii	"__GCC_ATOMIC_SHORT_LOCK_FREE 2\000"
.LASF769:
	.ascii	"clust\000"
.LASF33:
	.ascii	"__PTRDIFF_TYPE__ int\000"
.LASF452:
	.ascii	"CONFIG_GPIO_AS_PINRESET 1\000"
.LASF407:
	.ascii	"__ARM_ARCH_ISA_THUMB\000"
.LASF561:
	.ascii	"_DS1E 0x7E\000"
.LASF297:
	.ascii	"__UACCUM_EPSILON__ 0x1P-16UK\000"
.LASF435:
	.ascii	"__ARM_FEATURE_MATMUL_INT8\000"
.LASF785:
	.ascii	"FR_NO_FILE\000"
.LASF611:
	.ascii	"BS_55AA 510\000"
.LASF847:
	.ascii	"f_truncate\000"
.LASF918:
	.ascii	"mem_set\000"
.LASF283:
	.ascii	"__USACCUM_FBIT__ 8\000"
.LASF371:
	.ascii	"__GCC_ATOMIC_TEST_AND_SET_TRUEVAL 1\000"
.LASF475:
	.ascii	"_FS_RPATH 1\000"
.LASF493:
	.ascii	"_SYNC_t HANDLE\000"
.LASF440:
	.ascii	"__ELF__ 1\000"
.LASF479:
	.ascii	"_MULTI_PARTITION 0\000"
.LASF766:
	.ascii	"_FDID\000"
.LASF549:
	.ascii	"ATA_GET_MODEL 21\000"
.LASF671:
	.ascii	"LDIR_Attr 11\000"
.LASF206:
	.ascii	"__FLT32_HAS_QUIET_NAN__ 1\000"
.LASF722:
	.ascii	"__CTYPE_PRINT (__CTYPE_BLANK | __CTYPE_PUNCT | __CT"
	.ascii	"YPE_UPPER | __CTYPE_LOWER | __CTYPE_DIGIT)\000"
.LASF190:
	.ascii	"__LDBL_HAS_INFINITY__ 1\000"
.LASF344:
	.ascii	"__TA_FBIT__ 63\000"
.LASF228:
	.ascii	"__FLT32X_MAX_10_EXP__ 308\000"
.LASF786:
	.ascii	"FR_NO_PATH\000"
.LASF821:
	.ascii	"f_gets\000"
.LASF906:
	.ascii	"remove_chain\000"
.LASF702:
	.ascii	"FREE_NAMBUF() \000"
.LASF183:
	.ascii	"__LDBL_DECIMAL_DIG__ 17\000"
.LASF116:
	.ascii	"__UINT_LEAST8_MAX__ 0xff\000"
.LASF582:
	.ascii	"NS_NOLFN 0x40\000"
.LASF708:
	.ascii	"__RAL_SIZE_MAX 4294967295UL\000"
.LASF614:
	.ascii	"BPB_FSVer32 42\000"
.LASF160:
	.ascii	"__FP_FAST_FMAF 1\000"
.LASF927:
	.ascii	"__ap\000"
.LASF9:
	.ascii	"__ATOMIC_RELAXED 0\000"
.LASF465:
	.ascii	"_USE_FASTSEEK 0\000"
.LASF917:
	.ascii	"mem_cmp\000"
.LASF109:
	.ascii	"__INT_LEAST16_WIDTH__ 16\000"
.LASF263:
	.ascii	"__ULFRACT_FBIT__ 32\000"
.LASF792:
	.ascii	"FR_INVALID_DRIVE\000"
.LASF143:
	.ascii	"__DEC_EVAL_METHOD__ 2\000"
.LASF724:
	.ascii	"__MAX_CATEGORY 5\000"
.LASF418:
	.ascii	"__ARM_FEATURE_FP16_FML\000"
.LASF636:
	.ascii	"BPB_ActFatEx 107\000"
.LASF171:
	.ascii	"__DBL_EPSILON__ ((double)1.1)\000"
.LASF558:
	.ascii	"_DF2S 0xE0\000"
.LASF470:
	.ascii	"_CODE_PAGE 932\000"
.LASF752:
	.ascii	"free_clst\000"
.LASF247:
	.ascii	"__USFRACT_EPSILON__ 0x1P-8UHR\000"
.LASF627:
	.ascii	"BPB_TotSecEx 72\000"
.LASF243:
	.ascii	"__USFRACT_FBIT__ 8\000"
.LASF609:
	.ascii	"BS_FilSysType 54\000"
.LASF557:
	.ascii	"_DF1E 0x9F\000"
.LASF902:
	.ascii	"dir_next\000"
.LASF826:
	.ascii	"cst32\000"
.LASF103:
	.ascii	"__UINT64_MAX__ 0xffffffffffffffffULL\000"
.LASF668:
	.ascii	"DIR_FstClusLO 26\000"
.LASF851:
	.ascii	"nfree\000"
.LASF681:
	.ascii	"XDIR_Attr 4\000"
.LASF529:
	.ascii	"STA_NODISK 0x02\000"
.LASF471:
	.ascii	"_USE_LFN 1\000"
.LASF104:
	.ascii	"__INT_LEAST8_MAX__ 0x7f\000"
.LASF372:
	.ascii	"__GCC_ATOMIC_POINTER_LOCK_FREE 2\000"
.LASF921:
	.ascii	"st_word\000"
.LASF392:
	.ascii	"__ARM_FEATURE_CLZ 1\000"
.LASF11:
	.ascii	"__ATOMIC_ACQUIRE 2\000"
.LASF386:
	.ascii	"__ARM_FEATURE_COMPLEX\000"
.LASF843:
	.ascii	"path_new\000"
.LASF396:
	.ascii	"__ARM_SIZEOF_WCHAR_T 4\000"
.LASF227:
	.ascii	"__FLT32X_MAX_EXP__ 1024\000"
.LASF598:
	.ascii	"BPB_Media 21\000"
.LASF120:
	.ascii	"__UINT_LEAST32_MAX__ 0xffffffffUL\000"
.LASF860:
	.ascii	"f_close\000"
.LASF260:
	.ascii	"__LFRACT_MIN__ (-0.5LR-0.5LR)\000"
.LASF442:
	.ascii	"__SES_ARM 1\000"
.LASF115:
	.ascii	"__INT_LEAST64_WIDTH__ 64\000"
.LASF158:
	.ascii	"__FLT_HAS_INFINITY__ 1\000"
.LASF462:
	.ascii	"_USE_STRFUNC 1\000"
.LASF576:
	.ascii	"NS_LOSS 0x01\000"
.LASF96:
	.ascii	"__INT8_MAX__ 0x7f\000"
.LASF463:
	.ascii	"_USE_FIND 0\000"
.LASF240:
	.ascii	"__SFRACT_MIN__ (-0.5HR-0.5HR)\000"
.LASF739:
	.ascii	"long unsigned int\000"
.LASF341:
	.ascii	"__SA_IBIT__ 16\000"
.LASF140:
	.ascii	"__GCC_IEC_559_COMPLEX 0\000"
.LASF388:
	.ascii	"__ARM_FEATURE_MVE\000"
.LASF413:
	.ascii	"__ARM_FP16_FORMAT_IEEE\000"
.LASF46:
	.ascii	"__UINT16_TYPE__ short unsigned int\000"
.LASF820:
	.ascii	"putc_bfd\000"
.LASF128:
	.ascii	"__INT_FAST32_MAX__ 0x7fffffff\000"
.LASF185:
	.ascii	"__LDBL_NORM_MAX__ 1.1\000"
.LASF102:
	.ascii	"__UINT32_MAX__ 0xffffffffUL\000"
.LASF872:
	.ascii	"f_mount\000"
.LASF142:
	.ascii	"__FLT_EVAL_METHOD_TS_18661_3__ 0\000"
.LASF79:
	.ascii	"__SCHAR_WIDTH__ 8\000"
.LASF61:
	.ascii	"__UINT_FAST8_TYPE__ unsigned int\000"
.LASF309:
	.ascii	"__LLACCUM_IBIT__ 32\000"
.LASF252:
	.ascii	"__FRACT_EPSILON__ 0x1P-15R\000"
.LASF889:
	.ascii	"dir_register\000"
.LASF641:
	.ascii	"BPB_PercInUseEx 112\000"
.LASF336:
	.ascii	"__UTQ_FBIT__ 128\000"
.LASF98:
	.ascii	"__INT32_MAX__ 0x7fffffffL\000"
.LASF646:
	.ascii	"FSI_Free_Count 488\000"
.LASF621:
	.ascii	"BS_VolID32 67\000"
.LASF113:
	.ascii	"__INT_LEAST64_MAX__ 0x7fffffffffffffffLL\000"
.LASF437:
	.ascii	"__ARM_FEATURE_BF16_VECTOR_ARITHMETIC\000"
.LASF571:
	.ascii	"AM_MASK 0x3F\000"
.LASF544:
	.ascii	"MMC_GET_SDSTAT 14\000"
.LASF203:
	.ascii	"__FLT32_DENORM_MIN__ 1.1\000"
.LASF819:
	.ascii	"putc_init\000"
.LASF192:
	.ascii	"__FLT32_MANT_DIG__ 24\000"
.LASF801:
	.ascii	"FRESULT\000"
.LASF123:
	.ascii	"__UINT64_C(c) c ## ULL\000"
.LASF56:
	.ascii	"__UINT_LEAST64_TYPE__ long long unsigned int\000"
.LASF638:
	.ascii	"BPB_SecPerClusEx 109\000"
.LASF364:
	.ascii	"__GCC_ATOMIC_CHAR16_T_LOCK_FREE 2\000"
.LASF757:
	.ascii	"fatbase\000"
.LASF541:
	.ascii	"MMC_GET_CSD 11\000"
.LASF165:
	.ascii	"__DBL_MAX_EXP__ 1024\000"
.LASF808:
	.ascii	"FatFs\000"
.LASF65:
	.ascii	"__INTPTR_TYPE__ int\000"
.LASF894:
	.ascii	"sum_sfn\000"
.LASF235:
	.ascii	"__FLT32X_HAS_DENORM__ 1\000"
.LASF910:
	.ascii	"clust2sect\000"
.LASF335:
	.ascii	"__UDQ_IBIT__ 0\000"
.LASF459:
	.ascii	"_FFCONF 68020\000"
.LASF914:
	.ascii	"sync_window\000"
.LASF368:
	.ascii	"__GCC_ATOMIC_INT_LOCK_FREE 2\000"
.LASF667:
	.ascii	"DIR_ModTime 22\000"
.LASF485:
	.ascii	"_FS_EXFAT 0\000"
.LASF802:
	.ascii	"DSTATUS\000"
.LASF727:
	.ascii	"va_copy(d,s) __builtin_va_copy((d),(s))\000"
.LASF99:
	.ascii	"__INT64_MAX__ 0x7fffffffffffffffLL\000"
.LASF380:
	.ascii	"__ARM_FEATURE_SAT 1\000"
.LASF599:
	.ascii	"BPB_FATSz16 22\000"
.LASF274:
	.ascii	"__ULLFRACT_IBIT__ 0\000"
.LASF919:
	.ascii	"mem_cpy\000"
.LASF450:
	.ascii	"APP_TIMER_V2_RTC0_ ENABLED 1\000"
.LASF92:
	.ascii	"__INTMAX_WIDTH__ 64\000"
.LASF496:
	.ascii	"_T(x) x\000"
.LASF5:
	.ascii	"__GNUC__ 10\000"
.LASF373:
	.ascii	"__HAVE_SPECULATION_SAFE_VALUE 1\000"
.LASF219:
	.ascii	"__FLT64_DENORM_MIN__ 1.1\000"
.LASF446:
	.ascii	"__GNU_LINKER 1\000"
.LASF82:
	.ascii	"__LONG_WIDTH__ 32\000"
.LASF293:
	.ascii	"__UACCUM_FBIT__ 16\000"
.LASF257:
	.ascii	"__UFRACT_EPSILON__ 0x1P-16UR\000"
.LASF74:
	.ascii	"__WCHAR_MIN__ 0U\000"
.LASF657:
	.ascii	"PTE_EdCyl 7\000"
.LASF188:
	.ascii	"__LDBL_DENORM_MIN__ 1.1\000"
.LASF329:
	.ascii	"__UQQ_IBIT__ 0\000"
.LASF201:
	.ascii	"__FLT32_MIN__ 1.1\000"
.LASF369:
	.ascii	"__GCC_ATOMIC_LONG_LOCK_FREE 2\000"
.LASF305:
	.ascii	"__ULACCUM_MIN__ 0.0ULK\000"
.LASF701:
	.ascii	"INIT_NAMBUF(fs) \000"
.LASF401:
	.ascii	"__ARM_ARCH 7\000"
.LASF595:
	.ascii	"BPB_NumFATs 16\000"
.LASF690:
	.ascii	"XDIR_GenFlags 33\000"
.LASF144:
	.ascii	"__FLT_RADIX__ 2\000"
.LASF488:
	.ascii	"_NORTC_MDAY 1\000"
.LASF389:
	.ascii	"__ARM_FEATURE_CMSE\000"
.LASF813:
	.ascii	"va_list\000"
.LASF626:
	.ascii	"BPB_VolOfsEx 64\000"
.LASF654:
	.ascii	"PTE_System 4\000"
.LASF759:
	.ascii	"database\000"
.LASF643:
	.ascii	"BS_BootCodeEx 120\000"
.LASF191:
	.ascii	"__LDBL_HAS_QUIET_NAN__ 1\000"
.LASF83:
	.ascii	"__LONG_LONG_WIDTH__ 64\000"
.LASF779:
	.ascii	"fname\000"
.LASF135:
	.ascii	"__UINT_FAST64_MAX__ 0xffffffffffffffffULL\000"
.LASF411:
	.ascii	"__ARM_FP\000"
.LASF339:
	.ascii	"__HA_IBIT__ 8\000"
.LASF659:
	.ascii	"PTE_SizLba 12\000"
.LASF370:
	.ascii	"__GCC_ATOMIC_LLONG_LOCK_FREE 1\000"
.LASF678:
	.ascii	"XDIR_CaseSum 4\000"
.LASF124:
	.ascii	"__INT_FAST8_MAX__ 0x7fffffff\000"
.LASF212:
	.ascii	"__FLT64_MAX_EXP__ 1024\000"
.LASF168:
	.ascii	"__DBL_MAX__ ((double)1.1)\000"
.LASF880:
	.ascii	"szbfat\000"
.LASF85:
	.ascii	"__WINT_WIDTH__ 32\000"
.LASF44:
	.ascii	"__INT64_TYPE__ long long int\000"
.LASF186:
	.ascii	"__LDBL_MIN__ 1.1\000"
.LASF24:
	.ascii	"__CHAR_BIT__ 8\000"
.LASF887:
	.ascii	"dir_remove\000"
.LASF632:
	.ascii	"BPB_RootClusEx 96\000"
.LASF482:
	.ascii	"_USE_TRIM 0\000"
.LASF231:
	.ascii	"__FLT32X_NORM_MAX__ 1.1\000"
.LASF770:
	.ascii	"sect\000"
.LASF256:
	.ascii	"__UFRACT_MAX__ 0XFFFFP-16UR\000"
.LASF591:
	.ascii	"BS_OEMName 3\000"
.LASF487:
	.ascii	"_NORTC_MON 1\000"
.LASF515:
	.ascii	"FM_EXFAT 0x04\000"
.LASF648:
	.ascii	"MBR_Table 446\000"
.LASF749:
	.ascii	"csize\000"
.LASF110:
	.ascii	"__INT_LEAST32_MAX__ 0x7fffffffL\000"
.LASF239:
	.ascii	"__SFRACT_IBIT__ 0\000"
.LASF731:
	.ascii	"BYTE\000"
.LASF29:
	.ascii	"__BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__\000"
.LASF822:
	.ascii	"buff\000"
.LASF422:
	.ascii	"__ARM_NEON_FP\000"
.LASF908:
	.ascii	"put_fat\000"
.LASF850:
	.ascii	"fatfs\000"
.LASF504:
	.ascii	"EOF (-1)\000"
.LASF355:
	.ascii	"__USER_LABEL_PREFIX__ \000"
.LASF253:
	.ascii	"__UFRACT_FBIT__ 16\000"
.LASF334:
	.ascii	"__UDQ_FBIT__ 64\000"
.LASF156:
	.ascii	"__FLT_DENORM_MIN__ 1.1\000"
.LASF645:
	.ascii	"FSI_StrucSig 484\000"
.LASF181:
	.ascii	"__LDBL_MAX_10_EXP__ 308\000"
.LASF432:
	.ascii	"__ARM_FEATURE_COPROC 15\000"
.LASF594:
	.ascii	"BPB_RsvdSecCnt 14\000"
.LASF831:
	.ascii	"sz_blk\000"
.LASF760:
	.ascii	"winsect\000"
.LASF497:
	.ascii	"_TEXT(x) x\000"
.LASF59:
	.ascii	"__INT_FAST32_TYPE__ int\000"
.LASF230:
	.ascii	"__FLT32X_MAX__ 1.1\000"
.LASF478:
	.ascii	"_VOLUME_STRS \"RAM\",\"NAND\",\"CF\",\"SD\",\"SD2\""
	.ascii	",\"USB\",\"USB2\",\"USB3\"\000"
.LASF729:
	.ascii	"unsigned int\000"
.LASF913:
	.ascii	"sector\000"
.LASF555:
	.ascii	"GET_FATTIME() ((DWORD)(_NORTC_YEAR - 1980) << 25 | "
	.ascii	"(DWORD)_NORTC_MON << 21 | (DWORD)_NORTC_MDAY << 16)"
	.ascii	"\000"
.LASF898:
	.ascii	"cmp_lfn\000"
.LASF311:
	.ascii	"__LLACCUM_MAX__ 0X7FFFFFFFFFFFFFFFP-31LLK\000"
.LASF147:
	.ascii	"__FLT_MIN_EXP__ (-125)\000"
.LASF222:
	.ascii	"__FLT64_HAS_QUIET_NAN__ 1\000"
.LASF798:
	.ascii	"FR_NOT_ENOUGH_CORE\000"
.LASF909:
	.ascii	"get_fat\000"
.LASF284:
	.ascii	"__USACCUM_IBIT__ 8\000"
.LASF750:
	.ascii	"lfnbuf\000"
.LASF424:
	.ascii	"__ARM_ARCH_7EM__ 1\000"
.LASF349:
	.ascii	"__USA_IBIT__ 16\000"
.LASF314:
	.ascii	"__ULLACCUM_IBIT__ 32\000"
.LASF37:
	.ascii	"__UINTMAX_TYPE__ long long unsigned int\000"
.LASF666:
	.ascii	"DIR_FstClusHI 20\000"
.LASF353:
	.ascii	"__UTA_IBIT__ 64\000"
.LASF923:
	.ascii	"ld_word\000"
.LASF141:
	.ascii	"__FLT_EVAL_METHOD__ 0\000"
.LASF747:
	.ascii	"fsi_flag\000"
.LASF762:
	.ascii	"attr\000"
.LASF68:
	.ascii	"__SCHAR_MAX__ 0x7f\000"
.LASF803:
	.ascii	"RES_OK\000"
.LASF125:
	.ascii	"__INT_FAST8_WIDTH__ 32\000"
.LASF2:
	.ascii	"__STDC_UTF_16__ 1\000"
.LASF505:
	.ascii	"FA_READ 0x01\000"
.LASF390:
	.ascii	"__ARM_FEATURE_LDREX\000"
.LASF328:
	.ascii	"__UQQ_FBIT__ 8\000"
.LASF881:
	.ascii	"nrsv\000"
.LASF778:
	.ascii	"altname\000"
.LASF767:
	.ascii	"flag\000"
.LASF415:
	.ascii	"__ARM_FP16_ARGS\000"
.LASF296:
	.ascii	"__UACCUM_MAX__ 0XFFFFFFFFP-16UK\000"
.LASF868:
	.ascii	"rcnt\000"
.LASF261:
	.ascii	"__LFRACT_MAX__ 0X7FFFFFFFP-31LR\000"
.LASF377:
	.ascii	"__SIZEOF_PTRDIFF_T__ 4\000"
.LASF827:
	.ascii	"pdrv\000"
.LASF0:
	.ascii	"__STDC__ 1\000"
.LASF443:
	.ascii	"__ARM_ARCH_FPV4_SP_D16__ 1\000"
.LASF429:
	.ascii	"__ARM_FEATURE_IDIV 1\000"
.LASF30:
	.ascii	"__FLOAT_WORD_ORDER__ __ORDER_LITTLE_ENDIAN__\000"
.LASF45:
	.ascii	"__UINT8_TYPE__ unsigned char\000"
.LASF170:
	.ascii	"__DBL_MIN__ ((double)1.1)\000"
.LASF40:
	.ascii	"__SIG_ATOMIC_TYPE__ int\000"
.LASF672:
	.ascii	"LDIR_Type 12\000"
.LASF80:
	.ascii	"__SHRT_WIDTH__ 16\000"
.LASF397:
	.ascii	"__ARM_ARCH_PROFILE\000"
.LASF220:
	.ascii	"__FLT64_HAS_DENORM__ 1\000"
.LASF348:
	.ascii	"__USA_FBIT__ 16\000"
.LASF466:
	.ascii	"_USE_EXPAND 0\000"
.LASF179:
	.ascii	"__LDBL_MIN_10_EXP__ (-307)\000"
.LASF159:
	.ascii	"__FLT_HAS_QUIET_NAN__ 1\000"
.LASF734:
	.ascii	"WORD\000"
.LASF363:
	.ascii	"__GCC_ATOMIC_CHAR_LOCK_FREE 2\000"
.LASF901:
	.ascii	"dir_alloc\000"
.LASF262:
	.ascii	"__LFRACT_EPSILON__ 0x1P-31LR\000"
.LASF675:
	.ascii	"XDIR_Type 0\000"
.LASF461:
	.ascii	"_FS_MINIMIZE 0\000"
.LASF736:
	.ascii	"WCHAR\000"
.LASF169:
	.ascii	"__DBL_NORM_MAX__ ((double)1.1)\000"
.LASF745:
	.ascii	"n_fats\000"
.LASF395:
	.ascii	"__ARM_SIZEOF_MINIMAL_ENUM 1\000"
.LASF399:
	.ascii	"__arm__ 1\000"
.LASF730:
	.ascii	"UINT\000"
.LASF707:
	.ascii	"__RAL_SIZE_T unsigned\000"
.LASF195:
	.ascii	"__FLT32_MIN_10_EXP__ (-37)\000"
.LASF538:
	.ascii	"CTRL_EJECT 7\000"
.LASF738:
	.ascii	"DWORD\000"
.LASF829:
	.ascii	"szb_buf\000"
.LASF414:
	.ascii	"__ARM_FP16_FORMAT_ALTERNATIVE\000"
.LASF457:
	.ascii	"_FATFS 68020\000"
.LASF713:
	.ascii	"__CTYPE_DIGIT 0x04\000"
.LASF639:
	.ascii	"BPB_NumFATsEx 110\000"
.LASF793:
	.ascii	"FR_NOT_ENABLED\000"
.LASF25:
	.ascii	"__BIGGEST_ALIGNMENT__ 8\000"
.LASF795:
	.ascii	"FR_MKFS_ABORTED\000"
.LASF345:
	.ascii	"__TA_IBIT__ 64\000"
.LASF476:
	.ascii	"_VOLUMES 1\000"
.LASF712:
	.ascii	"__CTYPE_LOWER 0x02\000"
.LASF683:
	.ascii	"XDIR_ModTime 12\000"
.LASF383:
	.ascii	"__ARM_FEATURE_QRDMX\000"
.LASF777:
	.ascii	"fattrib\000"
.LASF474:
	.ascii	"_STRF_ENCODE 3\000"
.LASF893:
	.ascii	"dir_read\000"
.LASF72:
	.ascii	"__LONG_LONG_MAX__ 0x7fffffffffffffffLL\000"
.LASF699:
	.ascii	"RDDEM 0x05\000"
.LASF254:
	.ascii	"__UFRACT_IBIT__ 0\000"
.LASF387:
	.ascii	"__ARM_32BIT_STATE 1\000"
.LASF499:
	.ascii	"f_error(fp) ((fp)->err)\000"
.LASF105:
	.ascii	"__INT8_C(c) c\000"
.LASF259:
	.ascii	"__LFRACT_IBIT__ 0\000"
.LASF453:
	.ascii	"FLOAT_ABI_HARD 1\000"
.LASF163:
	.ascii	"__DBL_MIN_EXP__ (-1021)\000"
.LASF825:
	.ascii	"work\000"
.LASF726:
	.ascii	"va_arg __builtin_va_arg\000"
.LASF365:
	.ascii	"__GCC_ATOMIC_CHAR32_T_LOCK_FREE 2\000"
.LASF790:
	.ascii	"FR_INVALID_OBJECT\000"
.LASF603:
	.ascii	"BPB_TotSec32 32\000"
.LASF852:
	.ascii	"clst\000"
.LASF119:
	.ascii	"__UINT16_C(c) c\000"
.LASF878:
	.ascii	"tsect\000"
.LASF572:
	.ascii	"FA_SEEKEND 0x20\000"
.LASF556:
	.ascii	"_DF1S 0x81\000"
.LASF8:
	.ascii	"__VERSION__ \"10.2.1 20201103 (release)\"\000"
.LASF189:
	.ascii	"__LDBL_HAS_DENORM__ 1\000"
.LASF748:
	.ascii	"n_rootdir\000"
.LASF290:
	.ascii	"__ACCUM_MIN__ (-0X1P15K-0X1P15K)\000"
.LASF381:
	.ascii	"__ARM_FEATURE_CRYPTO\000"
.LASF51:
	.ascii	"__INT_LEAST32_TYPE__ long int\000"
.LASF846:
	.ascii	"dclst\000"
.LASF122:
	.ascii	"__UINT_LEAST64_MAX__ 0xffffffffffffffffULL\000"
.LASF249:
	.ascii	"__FRACT_IBIT__ 0\000"
.LASF775:
	.ascii	"fdate\000"
.LASF837:
	.ascii	"sz_vol\000"
.LASF26:
	.ascii	"__ORDER_LITTLE_ENDIAN__ 1234\000"
.LASF543:
	.ascii	"MMC_GET_OCR 13\000"
.LASF925:
	.ascii	"C:\\Nordic\\NAATOS_fw_src\\application\\sdk\\extern"
	.ascii	"al\\fatfs\\src\\ff.c\000"
.LASF604:
	.ascii	"BS_DrvNum 36\000"
.LASF740:
	.ascii	"long long unsigned int\000"
.LASF528:
	.ascii	"STA_NOINIT 0x01\000"
.LASF304:
	.ascii	"__ULACCUM_IBIT__ 32\000"
.LASF69:
	.ascii	"__SHRT_MAX__ 0x7fff\000"
.LASF317:
	.ascii	"__ULLACCUM_EPSILON__ 0x1P-32ULLK\000"
.LASF402:
	.ascii	"__APCS_32__ 1\000"
.LASF75:
	.ascii	"__WINT_MAX__ 0xffffffffU\000"
.LASF324:
	.ascii	"__DQ_FBIT__ 63\000"
.LASF486:
	.ascii	"_FS_NORTC 1\000"
.LASF717:
	.ascii	"__CTYPE_BLANK 0x40\000"
.LASF331:
	.ascii	"__UHQ_IBIT__ 0\000"
.LASF924:
	.ascii	"GNU C99 10.2.1 20201103 (release) -fmessage-length="
	.ascii	"0 -mcpu=cortex-m4 -mlittle-endian -mfloat-abi=hard "
	.ascii	"-mfpu=fpv4-sp-d16 -mthumb -mtp=soft -munaligned-acc"
	.ascii	"ess -std=gnu99 -g3 -gpubnames -fomit-frame-pointer "
	.ascii	"-fno-dwarf2-cfi-asm -fno-builtin -ffunction-section"
	.ascii	"s -fdata-sections -fshort-enums -fno-common\000"
.LASF361:
	.ascii	"__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4 1\000"
.LASF58:
	.ascii	"__INT_FAST16_TYPE__ int\000"
.LASF280:
	.ascii	"__SACCUM_MIN__ (-0X1P7HK-0X1P7HK)\000"
.LASF266:
	.ascii	"__ULFRACT_MAX__ 0XFFFFFFFFP-32ULR\000"
.LASF54:
	.ascii	"__UINT_LEAST16_TYPE__ short unsigned int\000"
.LASF155:
	.ascii	"__FLT_EPSILON__ 1.1\000"
.LASF301:
	.ascii	"__LACCUM_MAX__ 0X7FFFFFFFFFFFFFFFP-31LK\000"
.LASF38:
	.ascii	"__CHAR16_TYPE__ short unsigned int\000"
.LASF224:
	.ascii	"__FLT32X_DIG__ 15\000"
.LASF855:
	.ascii	"f_closedir\000"
.LASF511:
	.ascii	"FA_OPEN_APPEND 0x30\000"
.LASF154:
	.ascii	"__FLT_MIN__ 1.1\000"
.LASF835:
	.ascii	"b_fat\000"
.LASF312:
	.ascii	"__LLACCUM_EPSILON__ 0x1P-31LLK\000"
.LASF15:
	.ascii	"__FINITE_MATH_ONLY__ 0\000"
.LASF126:
	.ascii	"__INT_FAST16_MAX__ 0x7fffffff\000"
.LASF565:
	.ascii	"IsLower(c) (((c)>='a')&&((c)<='z'))\000"
.LASF694:
	.ascii	"XDIR_FstClus 52\000"
.LASF521:
	.ascii	"FS_EXFAT 4\000"
.LASF19:
	.ascii	"__SIZEOF_SHORT__ 2\000"
.LASF753:
	.ascii	"cdir\000"
.LASF796:
	.ascii	"FR_TIMEOUT\000"
.LASF306:
	.ascii	"__ULACCUM_MAX__ 0XFFFFFFFFFFFFFFFFP-32ULK\000"
.LASF756:
	.ascii	"volbase\000"
.LASF265:
	.ascii	"__ULFRACT_MIN__ 0.0ULR\000"
.LASF531:
	.ascii	"CTRL_SYNC 0\000"
.LASF841:
	.ascii	"f_rename\000"
.LASF325:
	.ascii	"__DQ_IBIT__ 0\000"
.LASF438:
	.ascii	"__ARM_BF16_FORMAT_ALTERNATIVE\000"
.LASF43:
	.ascii	"__INT32_TYPE__ long int\000"
.LASF564:
	.ascii	"IsUpper(c) (((c)>='A')&&((c)<='Z'))\000"
.LASF635:
	.ascii	"BPB_VolFlagEx 106\000"
.LASF12:
	.ascii	"__ATOMIC_RELEASE 3\000"
.LASF523:
	.ascii	"AM_HID 0x02\000"
.LASF848:
	.ascii	"f_getfree\000"
.LASF534:
	.ascii	"GET_BLOCK_SIZE 3\000"
.LASF448:
	.ascii	"DEBUG_NRF 1\000"
.LASF570:
	.ascii	"AM_LFN 0x0F\000"
.LASF596:
	.ascii	"BPB_RootEntCnt 17\000"
.LASF532:
	.ascii	"GET_SECTOR_COUNT 1\000"
.LASF658:
	.ascii	"PTE_StLba 8\000"
.LASF506:
	.ascii	"FA_WRITE 0x02\000"
.LASF337:
	.ascii	"__UTQ_IBIT__ 0\000"
.LASF764:
	.ascii	"sclust\000"
.LASF340:
	.ascii	"__SA_FBIT__ 15\000"
.LASF272:
	.ascii	"__LLFRACT_EPSILON__ 0x1P-63LLR\000"
.LASF687:
	.ascii	"XDIR_CrtTZ 22\000"
.LASF612:
	.ascii	"BPB_FATSz32 36\000"
.LASF426:
	.ascii	"__ARM_EABI__ 1\000"
.LASF406:
	.ascii	"__THUMBEL__ 1\000"
.LASF616:
	.ascii	"BPB_FSInfo32 48\000"
.LASF378:
	.ascii	"__ARM_FEATURE_DSP 1\000"
.LASF862:
	.ascii	"f_write\000"
.LASF319:
	.ascii	"__QQ_IBIT__ 0\000"
.LASF527:
	.ascii	"_DISKIO_DEFINED \000"
.LASF588:
	.ascii	"MAX_DIR 0x200000\000"
.LASF689:
	.ascii	"XDIR_AccTZ 24\000"
.LASF723:
	.ascii	"__RAL_WCHAR_T __WCHAR_TYPE__\000"
.LASF308:
	.ascii	"__LLACCUM_FBIT__ 31\000"
.LASF550:
	.ascii	"ATA_GET_SN 22\000"
.LASF719:
	.ascii	"__CTYPE_ALPHA (__CTYPE_UPPER | __CTYPE_LOWER)\000"
.LASF333:
	.ascii	"__USQ_IBIT__ 0\000"
.LASF55:
	.ascii	"__UINT_LEAST32_TYPE__ long unsigned int\000"
.LASF716:
	.ascii	"__CTYPE_CNTRL 0x20\000"
.LASF393:
	.ascii	"__ARM_FEATURE_NUMERIC_MAXMIN\000"
.LASF36:
	.ascii	"__INTMAX_TYPE__ long long int\000"
.LASF510:
	.ascii	"FA_OPEN_ALWAYS 0x10\000"
.LASF408:
	.ascii	"__ARM_ARCH_ISA_THUMB 2\000"
.LASF526:
	.ascii	"AM_ARC 0x20\000"
.LASF454:
	.ascii	"INITIALIZE_USER_SECTIONS 1\000"
.LASF589:
	.ascii	"MAX_DIR_EX 0x10000000\000"
.LASF379:
	.ascii	"__ARM_FEATURE_QBIT 1\000"
.LASF772:
	.ascii	"dir_ptr\000"
.LASF175:
	.ascii	"__DBL_HAS_QUIET_NAN__ 1\000"
.LASF566:
	.ascii	"IsDigit(c) (((c)>='0')&&((c)<='9'))\000"
.LASF271:
	.ascii	"__LLFRACT_MAX__ 0X7FFFFFFFFFFFFFFFP-63LLR\000"
.LASF608:
	.ascii	"BS_VolLab 43\000"
.LASF490:
	.ascii	"_FS_LOCK 0\000"
.LASF281:
	.ascii	"__SACCUM_MAX__ 0X7FFFP-7HK\000"
.LASF66:
	.ascii	"__UINTPTR_TYPE__ unsigned int\000"
.LASF512:
	.ascii	"CREATE_LINKMAP ((FSIZE_t)0 - 1)\000"
.LASF578:
	.ascii	"NS_LAST 0x04\000"
.LASF623:
	.ascii	"BS_FilSysType32 82\000"
.LASF354:
	.ascii	"__REGISTER_PREFIX__ \000"
.LASF162:
	.ascii	"__DBL_DIG__ 15\000"
.LASF267:
	.ascii	"__ULFRACT_EPSILON__ 0x1P-32ULR\000"
.LASF560:
	.ascii	"_DS1S 0x40\000"
.LASF797:
	.ascii	"FR_LOCKED\000"
.LASF23:
	.ascii	"__SIZEOF_SIZE_T__ 4\000"
.LASF651:
	.ascii	"PTE_StHead 1\000"
.LASF711:
	.ascii	"__CTYPE_UPPER 0x01\000"
.LASF114:
	.ascii	"__INT64_C(c) c ## LL\000"
.LASF916:
	.ascii	"chk_chr\000"
.LASF896:
	.ascii	"put_lfn\000"
.LASF816:
	.ascii	"f_printf\000"
.LASF433:
	.ascii	"__ARM_FEATURE_CDE\000"
.LASF289:
	.ascii	"__ACCUM_IBIT__ 16\000"
.LASF844:
	.ascii	"f_mkdir\000"
.LASF926:
	.ascii	"C:\\Nordic\\NAATOS_fw_src\\application\\solution\000"
.LASF700:
	.ascii	"DEF_NAMBUF \000"
.LASF460:
	.ascii	"_FS_READONLY 0\000"
.LASF892:
	.ascii	"dir_find\000"
.LASF853:
	.ascii	"f_stat\000"
.LASF733:
	.ascii	"short int\000"
.LASF652:
	.ascii	"PTE_StSec 2\000"
.LASF765:
	.ascii	"objsize\000"
.LASF842:
	.ascii	"path_old\000"
.LASF416:
	.ascii	"__ARM_FEATURE_FP16_SCALAR_ARITHMETIC\000"
.LASF501:
	.ascii	"f_size(fp) ((fp)->obj.objsize)\000"
.LASF907:
	.ascii	"pclst\000"
.LASF351:
	.ascii	"__UDA_IBIT__ 32\000"
.LASF871:
	.ascii	"mode\000"
.LASF761:
	.ascii	"FATFS\000"
.LASF357:
	.ascii	"__NO_INLINE__ 1\000"
.LASF695:
	.ascii	"XDIR_FileSize 56\000"
.LASF676:
	.ascii	"XDIR_NumLabel 1\000"
.LASF431:
	.ascii	"__ARM_FEATURE_COPROC\000"
.LASF174:
	.ascii	"__DBL_HAS_INFINITY__ 1\000"
.LASF806:
	.ascii	"RES_NOTRDY\000"
.LASF93:
	.ascii	"__SIG_ATOMIC_MAX__ 0x7fffffff\000"
.LASF208:
	.ascii	"__FLT64_MANT_DIG__ 53\000"
.LASF886:
	.ascii	"get_fileinfo\000"
.LASF758:
	.ascii	"dirbase\000"
.LASF522:
	.ascii	"AM_RDO 0x01\000"
.LASF428:
	.ascii	"__ARM_ARCH_EXT_IDIV__ 1\000"
.LASF601:
	.ascii	"BPB_NumHeads 26\000"
.LASF547:
	.ascii	"ISDIO_MRITE 57\000"
.LASF859:
	.ascii	"f_chdir\000"
.LASF500:
	.ascii	"f_tell(fp) ((fp)->fptr)\000"
.LASF830:
	.ascii	"sz_buf\000"
.LASF97:
	.ascii	"__INT16_MAX__ 0x7fff\000"
.LASF834:
	.ascii	"b_vol\000"
.LASF81:
	.ascii	"__INT_WIDTH__ 32\000"
.LASF394:
	.ascii	"__ARM_FEATURE_SIMD32 1\000"
.LASF199:
	.ascii	"__FLT32_MAX__ 1.1\000"
.LASF714:
	.ascii	"__CTYPE_SPACE 0x08\000"
.LASF618:
	.ascii	"BS_DrvNum32 64\000"
.LASF318:
	.ascii	"__QQ_FBIT__ 7\000"
.LASF473:
	.ascii	"_LFN_UNICODE 0\000"
.LASF904:
	.ascii	"dir_sdi\000"
.LASF575:
	.ascii	"NSFLAG 11\000"
.LASF95:
	.ascii	"__SIG_ATOMIC_WIDTH__ 32\000"
.LASF502:
	.ascii	"f_rewind(fp) f_lseek((fp), 0)\000"
.LASF670:
	.ascii	"LDIR_Ord 0\000"
.LASF840:
	.ascii	"sz_dir\000"
.LASF854:
	.ascii	"f_readdir\000"
.LASF494:
	.ascii	"LD2PD(vol) (BYTE)(vol)\000"
.LASF384:
	.ascii	"__ARM_FEATURE_CRC32\000"
.LASF258:
	.ascii	"__LFRACT_FBIT__ 31\000"
.LASF277:
	.ascii	"__ULLFRACT_EPSILON__ 0x1P-64ULLR\000"
.LASF376:
	.ascii	"__SIZEOF_WINT_T__ 4\000"
.LASF563:
	.ascii	"_DS2E 0xFC\000"
.LASF382:
	.ascii	"__ARM_FEATURE_UNALIGNED 1\000"
.LASF439:
	.ascii	"__GXX_TYPEINFO_EQUALITY_INLINE 0\000"
.LASF897:
	.ascii	"pick_lfn\000"
.LASF606:
	.ascii	"BS_BootSig 38\000"
.LASF300:
	.ascii	"__LACCUM_MIN__ (-0X1P31LK-0X1P31LK)\000"
.LASF137:
	.ascii	"__INTPTR_WIDTH__ 32\000"
.LASF204:
	.ascii	"__FLT32_HAS_DENORM__ 1\000"
.LASF315:
	.ascii	"__ULLACCUM_MIN__ 0.0ULLK\000"
.LASF129:
	.ascii	"__INT_FAST32_WIDTH__ 32\000"
.LASF366:
	.ascii	"__GCC_ATOMIC_WCHAR_T_LOCK_FREE 2\000"
.LASF718:
	.ascii	"__CTYPE_XDIGIT 0x80\000"
.LASF619:
	.ascii	"BS_NTres32 65\000"
.LASF430:
	.ascii	"__ARM_ASM_SYNTAX_UNIFIED__ 1\000"
.LASF597:
	.ascii	"BPB_TotSec16 19\000"
.LASF715:
	.ascii	"__CTYPE_PUNCT 0x10\000"
.LASF47:
	.ascii	"__UINT32_TYPE__ long unsigned int\000"
.LASF385:
	.ascii	"__ARM_FEATURE_DOTPROD\000"
.LASF856:
	.ascii	"f_opendir\000"
.LASF539:
	.ascii	"CTRL_FORMAT 8\000"
.LASF696:
	.ascii	"SZDIRE 32\000"
.LASF425:
	.ascii	"__ARM_PCS_VFP 1\000"
.LASF782:
	.ascii	"FR_DISK_ERR\000"
.LASF546:
	.ascii	"ISDIO_WRITE 56\000"
.LASF275:
	.ascii	"__ULLFRACT_MIN__ 0.0ULLR\000"
.LASF16:
	.ascii	"__SIZEOF_INT__ 4\000"
.LASF697:
	.ascii	"LLEF 0x40\000"
.LASF269:
	.ascii	"__LLFRACT_IBIT__ 0\000"
.LASF706:
	.ascii	"__RAL_SIZE_T\000"
.LASF282:
	.ascii	"__SACCUM_EPSILON__ 0x1P-7HK\000"
.LASF403:
	.ascii	"__GCC_ASM_FLAG_OUTPUTS__ 1\000"
.LASF375:
	.ascii	"__SIZEOF_WCHAR_T__ 4\000"
.LASF728:
	.ascii	"va_end(ap) __builtin_va_end(ap)\000"
.LASF412:
	.ascii	"__ARM_FP 4\000"
.LASF661:
	.ascii	"DIR_Attr 11\000"
.LASF62:
	.ascii	"__UINT_FAST16_TYPE__ unsigned int\000"
.LASF347:
	.ascii	"__UHA_IBIT__ 8\000"
.LASF585:
	.ascii	"MAX_FAT16 0xFFF5\000"
.LASF516:
	.ascii	"FM_ANY 0x07\000"
.LASF292:
	.ascii	"__ACCUM_EPSILON__ 0x1P-15K\000"
.LASF307:
	.ascii	"__ULACCUM_EPSILON__ 0x1P-32ULK\000"
.LASF338:
	.ascii	"__HA_FBIT__ 7\000"
.LASF177:
	.ascii	"__LDBL_DIG__ 15\000"
.LASF899:
	.ascii	"st_clust\000"
.LASF673:
	.ascii	"LDIR_Chksum 13\000"
.LASF87:
	.ascii	"__SIZE_WIDTH__ 32\000"
.LASF781:
	.ascii	"FR_OK\000"
.LASF76:
	.ascii	"__WINT_MIN__ 0U\000"
.LASF540:
	.ascii	"MMC_GET_TYPE 10\000"
.LASF209:
	.ascii	"__FLT64_DIG__ 15\000"
.LASF535:
	.ascii	"CTRL_TRIM 4\000"
.LASF106:
	.ascii	"__INT_LEAST8_WIDTH__ 8\000"
.LASF50:
	.ascii	"__INT_LEAST16_TYPE__ short int\000"
.LASF663:
	.ascii	"DIR_CrtTime10 13\000"
.LASF184:
	.ascii	"__LDBL_MAX__ 1.1\000"
.LASF735:
	.ascii	"short unsigned int\000"
.LASF268:
	.ascii	"__LLFRACT_FBIT__ 63\000"
.LASF205:
	.ascii	"__FLT32_HAS_INFINITY__ 1\000"
.LASF865:
	.ascii	"wbuff\000"
.LASF483:
	.ascii	"_FS_NOFSINFO 0\000"
.LASF721:
	.ascii	"__CTYPE_GRAPH (__CTYPE_PUNCT | __CTYPE_UPPER | __CT"
	.ascii	"YPE_LOWER | __CTYPE_DIGIT)\000"
.LASF409:
	.ascii	"__ARMEL__ 1\000"
.LASF650:
	.ascii	"PTE_Boot 0\000"
.LASF320:
	.ascii	"__HQ_FBIT__ 15\000"
.LASF291:
	.ascii	"__ACCUM_MAX__ 0X7FFFFFFFP-15K\000"
.LASF680:
	.ascii	"XDIR_SetSum 2\000"
.LASF574:
	.ascii	"FA_DIRTY 0x80\000"
.LASF78:
	.ascii	"__SIZE_MAX__ 0xffffffffU\000"
.LASF592:
	.ascii	"BPB_BytsPerSec 11\000"
.LASF634:
	.ascii	"BPB_FSVerEx 104\000"
.LASF400:
	.ascii	"__ARM_ARCH\000"
.LASF200:
	.ascii	"__FLT32_NORM_MAX__ 1.1\000"
.LASF71:
	.ascii	"__LONG_MAX__ 0x7fffffffL\000"
.LASF444:
	.ascii	"__HEAP_SIZE__ 8192\000"
.LASF794:
	.ascii	"FR_NO_FILESYSTEM\000"
.LASF869:
	.ascii	"rbuff\000"
.LASF417:
	.ascii	"__ARM_FEATURE_FP16_VECTOR_ARITHMETIC\000"
.LASF524:
	.ascii	"AM_SYS 0x04\000"
.LASF391:
	.ascii	"__ARM_FEATURE_LDREX 7\000"
.LASF32:
	.ascii	"__SIZE_TYPE__ unsigned int\000"
.LASF332:
	.ascii	"__USQ_FBIT__ 32\000"
.LASF600:
	.ascii	"BPB_SecPerTrk 24\000"
.LASF241:
	.ascii	"__SFRACT_MAX__ 0X7FP-7HR\000"
.LASF223:
	.ascii	"__FLT32X_MANT_DIG__ 53\000"
.LASF495:
	.ascii	"LD2PT(vol) 0\000"
.LASF84:
	.ascii	"__WCHAR_WIDTH__ 32\000"
.LASF573:
	.ascii	"FA_MODIFIED 0x40\000"
.LASF867:
	.ascii	"remain\000"
.LASF108:
	.ascii	"__INT16_C(c) c\000"
.LASF481:
	.ascii	"_MAX_SS 512\000"
.LASF343:
	.ascii	"__DA_IBIT__ 32\000"
.LASF86:
	.ascii	"__PTRDIFF_WIDTH__ 32\000"
.LASF649:
	.ascii	"SZ_PTE 16\000"
.LASF13:
	.ascii	"__ATOMIC_ACQ_REL 4\000"
.LASF321:
	.ascii	"__HQ_IBIT__ 0\000"
.LASF873:
	.ascii	"putc_flush\000"
.LASF273:
	.ascii	"__ULLFRACT_FBIT__ 64\000"
.LASF864:
	.ascii	"csect\000"
.LASF891:
	.ascii	"nent\000"
.LASF828:
	.ascii	"part\000"
.LASF587:
	.ascii	"MAX_EXFAT 0x7FFFFFFD\000"
.LASF554:
	.ascii	"SS(fs) ((UINT)_MAX_SS)\000"
.LASF562:
	.ascii	"_DS2S 0x80\000"
.LASF218:
	.ascii	"__FLT64_EPSILON__ 1.1\000"
.LASF90:
	.ascii	"__UINTMAX_MAX__ 0xffffffffffffffffULL\000"
.LASF709:
	.ascii	"__RAL_PTRDIFF_T int\000"
.LASF161:
	.ascii	"__DBL_MANT_DIG__ 53\000"
.LASF264:
	.ascii	"__ULFRACT_IBIT__ 0\000"
.LASF780:
	.ascii	"FILINFO\000"
.LASF472:
	.ascii	"_MAX_LFN 255\000"
.LASF70:
	.ascii	"__INT_MAX__ 0x7fffffff\000"
.LASF52:
	.ascii	"__INT_LEAST64_TYPE__ long long int\000"
.LASF888:
	.ascii	"last\000"
.LASF682:
	.ascii	"XDIR_CrtTime 8\000"
.LASF278:
	.ascii	"__SACCUM_FBIT__ 7\000"
.LASF637:
	.ascii	"BPB_BytsPerSecEx 108\000"
	.ident	"GCC: (GNU) 10.2.1 20201103 (release)"

# This file is part of Asea OS.
# Copyright (C) 2018 Ivan Kmeťo
#
# Asea OS is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option)
# any later version.
#
# Asea OS is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License along with
# Asea OS.  If not, see <http://www.gnu.org/licenses/>.

.set IRQ_BASE, 0x20

.section .text

.extern _ZN4asea5hwcom16InterruptManager15handleInterruptEhj

.macro HandleException num
.global _ZN4asea5hwcom16InterruptManager16HandleException\num\()Ev
_ZN4asea5hwcom16InterruptManager16HandleException\num\()Ev:
	movb $\num, (interruptnumber)
	jmp int_bottom
.endm

.macro HandleInterruptRequest num
.global _ZN4asea5hwcom16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN4asea5hwcom16InterruptManager26HandleInterruptRequest\num\()Ev:
	movb $\num + IRQ_BASE, (interruptnumber)
	jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01
HandleInterruptRequest 0x0C

int_bottom:
	pusha
	pushl %ds
	pushl %es
	pushl %fs
	pushl %gs
	
	pushl %esp
	push (interruptnumber)
	call _ZN4asea5hwcom16InterruptManager15handleInterruptEhj
	# addl $5, %esp
	movl %eax, %esp

	popl %gs
	popl %fs
	popl %es
	popl %ds
	popa

.global _ZN4asea5hwcom16InterruptManager22IgnoreInterruptRequestEv
_ZN4asea5hwcom16InterruptManager22IgnoreInterruptRequestEv:

	iret

.data
	interruptnumber: .byte 0
    

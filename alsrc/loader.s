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

.set MAGIC, 0x1badb002
.set FLAGS, (1<<0 | 1<<1)
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
	.long MAGIC
	.long FLAGS
	.long CHECKSUM

.section .text
.extern kernelMain
.extern callConstructors
.global loader

loader:
	mov $kernel_stack, %esp

	call callConstructors

	push %eax
	push %ebx
	call kernelMain

_stop:
	cli
	hlt
	jmp _stop

.section .bss
.space 2*1024*1024; # 2 MiB
kernel_stack:
    

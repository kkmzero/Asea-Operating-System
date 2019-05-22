/*
 * This file is part of Asea OS.
 * Copyright (C) 2018 - 2019 Ivan Kmeťo
 *
 * Asea OS is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Asea OS is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Asea OS.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __ASEA__SYSTEM__CORE_H
#define __ASEA__SYSTEM__CORE_H

#include <common/types.h>
#include <System/lib/asl.h>

using namespace asea::System::lib;

void printf(char*);

namespace asea
{
	namespace System
	{
		namespace core
		{
			static void reboot() {
				#define KBRD_INTRFC 0x64
 
				/* keyboard interface bits */
				#define KBRD_BIT_KDATA 0 /* keyboard data is in buffer (output buffer is empty) (bit 0) */
				#define KBRD_BIT_UDATA 1 /* user data is in buffer (command buffer is empty) (bit 1) */
 
				#define KBRD_IO 0x60 /* keyboard IO port */
				#define KBRD_RESET 0xFE /* reset CPU command */
 
				#define bit(n) (1<<(n)) /* Set bit n to 1 */
 
				/* Check if bit n in flags is set */
				#define check_flag(flags, n) ((flags) & bit(n))
				
				asea::common::uint8_t temp;
 
				asm volatile ("cli");
 
				/* Clear all keyboard buffers (output and command buffers) */
				do {
					temp = asl::io::inb(KBRD_INTRFC); /* empty user data */
					if (check_flag(temp, KBRD_BIT_KDATA) != 0)
						asl::io::inb(KBRD_IO); /* empty keyboard data */
				} while (check_flag(temp, KBRD_BIT_UDATA) != 0);
 
				asl::io::outb(KBRD_INTRFC, KBRD_RESET); /* pulse CPU reset line */
				loop:
					asm volatile ("hlt"); /* if that didn't work, halt the CPU */
				goto loop; /* if a NMI is received, halt again */
			}

			static void kernelPanic(char* message) {
				asm("cli");
				printf(message);
				while(1);
			}

		}
	}
}

#endif

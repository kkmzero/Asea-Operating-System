/*
 * This file is part of Asea OS.
 * Copyright (C) 2018 - 2020 Ivan Kmeťo
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

#ifndef __ASEA__SYSTEM__SYSCALLS_H
#define __ASEA__SYSTEM__SYSCALLS_H

#include <common/types.h>
#include <astd>

namespace asea
{
    namespace system
    {
        static void _reboot() {
            #define KBRD_INTRFC 0x64
 
            /* keyboard interface bits */
            #define KBRD_BIT_KDATA 0 /* keyboard data is in buffer (output buffer is empty) (bit 0) */
            #define KBRD_BIT_UDATA 1 /* user data is in buffer (command buffer is empty) (bit 1) */

            #define KBRD_RESET 0xFE /* reset CPU command */

            #define bit(n) (1<<(n)) /* Set bit n to 1 */

            /* Check if bit n in flags is set */
            #define check_flag(flags, n) ((flags) & bit(n))

            asea::common::uint8_t temp;

            asm volatile ("cli");

            /* Clear all keyboard buffers (output and command buffers) */
            do {
                temp = inb(KBRD_INTRFC); //empty user data
                if (check_flag(temp, KBRD_BIT_KDATA) != 0)
                    inb(0x60); //empty keyboard data
            } while (check_flag(temp, KBRD_BIT_UDATA) != 0);

            outb(KBRD_INTRFC, KBRD_RESET); //pulse CPU reset line
            loop:
                asm volatile ("hlt"); //if that didn't work, halt the CPU
            goto loop; //if a NMI is received, halt again
        }

        static void _kernelPanic(char* message) {
            printf(message);
            asm("cli");
            while(1);
        }

        static void _sleep(asea::common::uint32_t sleepTime) {
            for(int i = 0; i < sleepTime; i++) {
                asm("nop");
            }
        }

    }
}


#endif

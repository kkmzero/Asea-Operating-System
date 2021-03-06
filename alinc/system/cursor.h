/*
 * This file is part of Asea OS.
 * Copyright (C) 2019 Ivan Kmeťo
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

#ifndef __ASEA__SYSTEM__CURSOR_H
#define __ASEA__SYSTEM__CURSOR_H

#include <common/types.h>
#include <astd>

namespace asea
{
    namespace system
    {
        static void cursor_enable(asea::common::uint8_t cursor_start, asea::common::uint8_t cursor_end) {
            outb(0x3D4, 0x0A);
            outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
            outb(0x3D4, 0x0B);
            outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
        }

        static void cursor_disable() {
            outb(0x3D4, 0x0A);
            outb(0x3D5, 0x20);
        }

        static void cursor_update(asea::common::int32_t x, asea::common::int32_t y) {
            asea::common::uint16_t pos = y * 80 + x;

            outb(0x3D4, 0x0F);
            outb(0x3D5, (asea::common::uint8_t) (pos & 0xFF));
            outb(0x3D4, 0x0E);
            outb(0x3D5, (asea::common::uint8_t) ((pos >> 8) & 0xFF));
        }

    }
}


#endif

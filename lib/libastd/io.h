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

#ifndef __ASEA__LIB__ASTD__IO_H
#define __ASEA__LIB__ASTD__IO_H

#include <common/types.h>


//
static void outb(asea::common::uint16_t port, asea::common::uint8_t val)
{
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port) );
}

//
static asea::common::uint8_t inb(asea::common::uint16_t port)
{
    asea::common::uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port) );

    return ret;
}


#endif

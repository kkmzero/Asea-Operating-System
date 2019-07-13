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

#ifndef __ASEA__LIB__ASTD__PRINTFHEX_H
#define __ASEA__LIB__ASTD__PRINTFHEX_H

#include <common/types.h>

void printf(char*);


//
void printfhex(asea::common::uint8_t key)
{
    char* pfx = "00";
    char* hex = "0123456789ABCDEF";
    pfx[0] = hex[(key >> 4) & 0xF];
    pfx[1] = hex[key & 0xF];
    printf(pfx);
}


#endif

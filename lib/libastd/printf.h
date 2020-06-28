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

#ifndef __ASEA__LIB__ASTD__PRINTF_H
#define __ASEA__LIB__ASTD__PRINTF_H

#include <common/types.h>
#include <system/lists/tmodecolor.list>

void _sysprintf(char*, asea::common::uint8_t, asea::common::uint8_t);


//Text Mode Print
static void printf(char* str)
{
    _sysprintf(str, 0x00, COLOR_WHITE);
}

//Text Mode Print, Set Text Color
static void printf(char* str, asea::common::uint8_t forecolor)
{
    _sysprintf(str, 0x00, forecolor);
}

//Text Mode Print, Set Text and Background Color
static void printf(char* str, asea::common::uint8_t bgcolor, asea::common::uint8_t forecolor)
{
    _sysprintf(str, bgcolor, forecolor);
}


#endif

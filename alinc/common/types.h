/*
 * This file is part of Asea OS.
 * Copyright (C) 2018 Ivan Kmeťo
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

#ifndef __ASEA__COMMON__TYPES_H
#define __ASEA__COMMON__TYPES_H

namespace asea
{
	namespace common
	{
		typedef char                     int8_t;
		typedef unsigned char           uint8_t;
		typedef short                   int16_t;
		typedef unsigned short         uint16_t;
		typedef int                     int32_t;
		typedef unsigned int           uint32_t;
		typedef long long int           int64_t;
		typedef unsigned long long int uint64_t;

		typedef const char*              string;
		typedef uint32_t                 size_t;

	}
}
    
#endif

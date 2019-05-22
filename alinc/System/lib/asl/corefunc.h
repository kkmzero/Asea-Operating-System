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

#ifndef __ASEA__SYSTEM__LIB__ASL__COREFUNC_H
#define __ASEA__SYSTEM__LIB__ASL__COREFUNC_H

#include <common/types.h>

void printf(char*);

namespace asea
{
	namespace System
	{
		namespace lib
		{
			namespace asl
			{
			void putChar(char c);
			void sleep(asea::common::uint32_t sleepTime);
			void printfHex(asea::common::uint8_t key);

			char* itoa(asea::common::int32_t value, char* result, asea::common::int32_t base);
			}
		}
	}
}

#endif

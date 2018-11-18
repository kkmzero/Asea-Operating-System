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

#ifndef __ASEA__SYSTEM__HEADERS__ASYSLIB_H
#define __ASEA__SYSTEM__HEADERS__ASYSLIB_H

#include <common/types.h>

void printf(char*);

namespace asea
{
	namespace System
	{
		namespace headers
		{
			namespace asl
			{
			void putChar(char c);
			void sleep(asea::common::uint32_t sleepTime);
			void printfHex(asea::common::uint8_t key);

			char* itoa(asea::common::int32_t value, char* result, asea::common::int32_t base);

				namespace math
				{
					double abs(double number);
					double pow(double base, double exponent);
				}

				namespace io
				{
					void outb(asea::common::uint16_t port, asea::common::uint8_t val);
					asea::common::uint8_t inb(asea::common::uint16_t port);
				}

			}
		}
	}
}

#endif

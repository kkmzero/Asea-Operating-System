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

#ifndef __ASEA__HWCOM__PORT_H
#define __ASEA__HWCOM__PORT_H

#include <common/types.h>

namespace asea
{
	namespace hwcom
	{
		class Port
		{
		protected:
			asea::common::uint16_t portnumber;
			Port(asea::common::uint16_t portnumber);
			~Port();
		};

		class Port8Bit : public Port
		{
		public:
			Port8Bit(asea::common::uint16_t portnumber);
			~Port8Bit();
			virtual void Write(asea::common::uint8_t data);
			virtual asea::common::uint8_t Read();
		};

		class Port8BitSlow : public Port8Bit
		{
		public:
			Port8BitSlow(asea::common::uint16_t portnumber);
			~Port8BitSlow();
			virtual void Write(asea::common::uint8_t data);
		};

		class Port16Bit : public Port
		{
		public:
			Port16Bit(asea::common::uint16_t portnumber);
			~Port16Bit();
			virtual void Write(asea::common::uint16_t data);
			virtual asea::common::uint16_t Read();
		};

		class Port32Bit : public Port
		{
		public:
			Port32Bit(asea::common::uint16_t portnumber);
			~Port32Bit();
			virtual void Write(asea::common::uint32_t data);
			virtual asea::common::uint32_t Read();
		};

	}
}

#endif

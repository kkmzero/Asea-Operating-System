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

#ifndef __ASEA__GDT_H
#define __ASEA__GDT_H

#include <common/types.h>

namespace asea
{
	class GlobalDescriptorTable
	{
	public:
		class SegmentDescriptor
		{
		private:
			asea::common::uint16_t limit_lo;
			asea::common::uint16_t base_lo;
			asea::common::uint8_t base_hi;
			asea::common::uint8_t type;
			asea::common::uint8_t flags_limit_hi;
			asea::common::uint8_t base_vhi;
		public:
			SegmentDescriptor(asea::common::uint32_t base, asea::common::uint32_t limits, asea::common::uint8_t type);
			asea::common::uint32_t Base();
			asea::common::uint32_t Limit();
		} __attribute__((packed));

		SegmentDescriptor nullSegmentSelector;
		SegmentDescriptor unusedSegmentSelector;
		SegmentDescriptor codeSegmentSelector;
		SegmentDescriptor dataSegmentSelector;

	public:
		GlobalDescriptorTable();
		~GlobalDescriptorTable();

		asea::common::uint16_t CodeSegmentSelector();
		asea::common::uint16_t DataSegmentSelector();
	};

}

#endif

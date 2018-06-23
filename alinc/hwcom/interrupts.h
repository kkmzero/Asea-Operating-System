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

#ifndef __ASEA_HWCOM__INTERRUPTS_H
#define __ASEA_HWCOM__INTERRUPTS_H

#include <gdt.h>
#include <common/types.h>
#include <hwcom/port.h>

namespace asea
{
	namespace hwcom
	{
		class InterruptManager;

		class InterruptHandler
		{
		protected:
			asea::common::uint8_t interruptNumber;
			InterruptManager* interruptManager;

			InterruptHandler(asea::common::uint8_t interruptNumber, InterruptManager* interruptManager);
			~InterruptHandler();

		public:
			virtual asea::common::uint32_t HandleInterrupt(asea::common::uint32_t esp);
		};

		class InterruptManager
		{
			friend class InterruptHandler;

		protected:
			static InterruptManager* ActiveInterruptManager;

			InterruptHandler* handlers[256];

			struct GateDescriptor
			{
				asea::common::uint16_t handlerAddressLowBits;
				asea::common::uint16_t gdt_codeSegmentSelector;
				asea::common::uint8_t reserved;
				asea::common::uint8_t access;
				asea::common::uint16_t handlerAddressHighBits;
			} __attribute__((packed));

			static GateDescriptor interruptDescriptorTable[256];

			struct InterruptDescriptorTablePointer
			{
				asea::common::uint16_t size;
				asea::common::uint32_t base;
			} __attribute__((packed));

			static void SetInterruptDescriptorTableEntry(
				asea::common::uint8_t interruptNumber,
				asea::common::uint16_t codeSegmentSelectorOffset,
				void (*handler)(),
				asea::common::uint8_t DescriptorPrivilegeLevel,
				asea::common::uint8_t DescriptorType
			);

			Port8BitSlow picMasterCommand;
			Port8BitSlow picMasterData;
			Port8BitSlow picSlaveCommand;
			Port8BitSlow picSlaveData;

		public:
			InterruptManager(asea::GlobalDescriptorTable* gdt);
			~InterruptManager();

			void Activate();
			void Deactivate();

			static asea::common::uint32_t handleInterrupt(asea::common::uint8_t interruptNumber, asea::common::uint32_t esp);

			asea::common::uint32_t DoHandleInterrupt(asea::common::uint8_t interruptNumber, asea::common::uint32_t esp);

			static void IgnoreInterruptRequest();
			static void HandleInterruptRequest0x00();
			static void HandleInterruptRequest0x01();
			static void HandleInterruptRequest0x0C();
		};

	}
}

#endif

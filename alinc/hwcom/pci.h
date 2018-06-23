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

#ifndef __ASEA__HWCOM__PCI_H
#define __ASEA__HWCOM__PCI_H

#include <hwcom/port.h>
#include <drivers/driver.h>
#include <common/types.h>
#include <hwcom/interrupts.h>

namespace asea
{
	namespace hwcom
	{
		enum BaseAddressRegisterType
		{
			MemoryMapping = 0,
			InputOutput = 1
		};

		class BaseAddressRegister
		{
		public:
			bool prefetchable;
			asea::common::uint8_t* address;
			asea::common::uint32_t size;
			BaseAddressRegisterType type;
		};

		class PCInterconnectDeviceDescriptor
		{
		public:
			asea::common::uint32_t portBase;
			asea::common::uint32_t interrupt;

			asea::common::uint16_t bus;
			asea::common::uint16_t device;
			asea::common::uint16_t function;

			asea::common::uint16_t vendor_id;
			asea::common::uint16_t device_id;

			asea::common::uint8_t class_id;
			asea::common::uint8_t subclass_id;
			asea::common::uint8_t interface_id;

			asea::common::uint8_t revision;

			PCInterconnectDeviceDescriptor();
			~PCInterconnectDeviceDescriptor();
		};

		class PCInterconnectController
		{
			Port32Bit dataPort;
			Port32Bit commandPort;

		public:
			PCInterconnectController();
			~PCInterconnectController();

			asea::common::uint32_t Read(asea::common::uint16_t bus, 
				asea::common::uint16_t device, 
				asea::common::uint16_t function, 
				asea::common::uint32_t registeroffset);

			void Write(asea::common::uint16_t bus, 
				asea::common::uint16_t device, 
				asea::common::uint16_t function, 
				asea::common::uint32_t registeroffset,
				asea::common::uint32_t value);

			bool DeviceHasFunctions(asea::common::uint16_t bus, 
				asea::common::uint16_t device);

			void SelectDrivers(asea::drivers::DriverManager* driverManager, asea::hwcom::InterruptManager* interrupts);

			asea::drivers::Driver* GetDriver(PCInterconnectDeviceDescriptor dev, asea::hwcom::InterruptManager* interrupts);

			PCInterconnectDeviceDescriptor GetDeviceDescriptor(asea::common::uint16_t bus, 
				asea::common::uint16_t device, 
				asea::common::uint16_t function);

			BaseAddressRegister GetBaseAddressRegister(asea::common::uint16_t bus, 
				asea::common::uint16_t device, 
				asea::common::uint16_t function,
				asea::common::uint16_t bar);
		};

	}
}

#endif

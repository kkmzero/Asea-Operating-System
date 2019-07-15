/*
 * This file is part of Asea OS.
 * Copyright (C) 2018 - 2019 Ivan Kmeťo
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

#include <hwcom/pci.h>
#include <astd>

using namespace asea::common;
using namespace asea::drivers;
using namespace asea::hwcom;

PCInterconnectDeviceDescriptor::PCInterconnectDeviceDescriptor() {
}

PCInterconnectDeviceDescriptor::~PCInterconnectDeviceDescriptor() {
}

PCInterconnectController::PCInterconnectController()
: dataPort(0xCFC),
commandPort(0xCF8) {
}

PCInterconnectController::~PCInterconnectController() {
}

uint32_t PCInterconnectController::Read(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset) {
	uint32_t id =
	0x1 << 31
	| ((bus & 0xFF) << 16)
	| ((device & 0x1F) << 11)
	| ((function & 0x07) << 8)
	| (registeroffset & 0xFC);

	commandPort.Write(id);
	uint32_t result = dataPort.Read();

	return result >> (8* (registeroffset % 4));
}

void PCInterconnectController::Write(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t value) {
	uint32_t id =
	0x1 << 31
	| ((bus & 0xFF) << 16)
	| ((device & 0x1F) << 11)
	| ((function & 0x07) << 8)
	| (registeroffset & 0xFC);

	commandPort.Write(id);
	dataPort.Write(value);
}

bool PCInterconnectController::DeviceHasFunctions(uint16_t bus, uint16_t device){
	return Read(bus, device, 0, 0x0E) & (1<<7);
}

void PCInterconnectController::SelectDrivers(DriverManager* driverManager, InterruptManager* interrupts) {
	for(int bus = 0; bus < 8; bus++)
	{
		for(int device = 0; device < 32; device++)
		{
			int numFunctions = DeviceHasFunctions(bus, device) ? 8 : 1;
			for(int function =0; function < numFunctions; function++)
			{
				PCInterconnectDeviceDescriptor dev = GetDeviceDescriptor(bus, device, function);

				if(dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF)
					continue;

				for(int barNum = 0; barNum < 6; barNum++)
				{
					BaseAddressRegister bar = GetBaseAddressRegister(bus, device, function, barNum);
					if(bar.address && (bar.type == InputOutput))
						dev.portBase = (uint32_t)bar.address;

					Driver* driver = GetDriver(dev, interrupts);
					if(driver != 0)
						driverManager->AddDriver(driver);
				}

				printf("PCI BUS ");
				printfhex(bus & 0xFF);

				printf(", DEVICE ");
				printfhex(device & 0xFF);

				printf(" = VENDOR ");
				printfhex((dev.vendor_id & 0xFF00) >> 8);
				printfhex(dev.vendor_id & 0xFF);

				printf(", DEVICE ");
				printfhex((dev.device_id & 0xFF00) >> 8);
				printfhex(dev.device_id & 0xFF);
				printf("\n");
			}
		}
	}
}

BaseAddressRegister PCInterconnectController::GetBaseAddressRegister(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar) {
	BaseAddressRegister result;

	uint32_t headertype = Read(bus, device, function, 0x0E) & 0x7F;
	int maxBARs = 6 - (4*headertype);
	if(bar >= maxBARs)
		return result;

	uint32_t bar_value = Read(bus, device, function, 0x10 + 4*bar);
	result.type = (bar_value & 0x1) ? InputOutput : MemoryMapping;
	uint32_t temp;

	if(result.type == MemoryMapping)
	{
		switch((bar_value >> 1) & 0x3)
		{
			case 0: //32Bit Mode
			case 1: //20Bit Mode
			case 2: //64Bit Mode
				break;
		}
		//result.prefetchable = ((bar_value >> 3) & 0x1) == 0x1;
	}
	else //InputOutput
	{
		result.address = (uint8_t*)(bar_value & ~0x3);
		result.prefetchable = false;
	}

	return result;
}

Driver* PCInterconnectController::GetDriver(PCInterconnectDeviceDescriptor dev, InterruptManager* interrupts) {
	switch(dev.vendor_id)
	{
		case 0x1022: //Advanced Micro Devices (AMD)
			switch(dev.device_id) {
				case 0x2000: //am79c973 (driver?)
					break;
			}
			break;

		case 0x8086: //Intel Corporation
			break;

		case 0x80EE: //InnoTek Systemberatung GmbH
			switch(dev.device_id) {
				case 0xBEEF: //VirtualBox Graphics Adapter
					break;
				case 0xCAFE: //VirtualBox Guest Service
					break;
			}
			break;
	}

	//Class Codes
	switch(dev.class_id)
	{
		case 0x00: //Unclassified
			switch(dev.subclass_id) {
				case 0x00: //Non-VGA Compatible devices
					break;

				case 0x01: //VGA Compatible Device
					break;
			}
			break;

		case 0x01: //Mass Storage Controller
			switch(dev.subclass_id) {
				case 0x00: //SCSI Bus Controller
					break;

				case 0x01: //IDE Controller
					break;

				case 0x02: //Floppy Disk Controller
					break;

				case 0x03: //IPI Bus Controller
					break;

				case 0x04: //RAID Controller
					break;

				case 0x05: //ATA Controller
					break;

				case 0x06: //Serial ATA
					break;

				case 0x07: //Serial Attached SCSI
					break;

				case 0x08: //Non-Volatile Memory Controller
					break;

				case 0x80: //Other
					break;
			}
			break;

		case 0x02: //Network Controller
			switch(dev.subclass_id) {
				case 0x00: //Ethernet Controller
					break;

				case 0x01: //Token Ring Controller
					break;

				case 0x02: //FDDI Controller
					break;

				case 0x03: //ATM Controller
					break;

				case 0x04: //ISDN Controller
					break;

				case 0x05: //WorldFip Controller
					break;

				case 0x06: //PICMG 2.14 Multi Computing
					break;

				case 0x07: //Infiniband Controller
					break;

				case 0x08: //Fabric Controller
					break;

				case 0x80: //Other
					break;
			}
			break;

		case 0x03: //Display Controller
			switch(dev.subclass_id) {
				case 0x00: //VGA Compatible Controller
					break;

				case 0x01: //XGA Controller
					break;

				case 0x02: //3D Controller (Not VGA-Compatible)
					break;

				case 0x80: //Other
					break;
			}
			break;

		case 0x04: //Multimedia Controller
			switch(dev.subclass_id) {
				case 0x00: //Multimedia Video Controller
					break;

				case 0x01: //Multimedia Audio Controller
					break;

				case 0x02: //Computer Telephony Device
					break;

				case 0x03: //Audio Device
					break;

				case 0x80: //Other
					break;
			}
			break;

		case 0x05: //Memory Controller
			switch(dev.subclass_id) {
				case 0x00: //RAM Controller
					break;

				case 0x01: //Flash Controller
					break;

				case 0x80: //Other
					break;
			}
			break;

		case 0x06: //Bridge Device
			switch(dev.subclass_id) {
				case 0x00: //Host Bridge
					break;

				case 0x01: //ISA Bridge
					break;

				case 0x02: //EISA Bridge
					break;

				case 0x03: //MCA Bridge
					break;

				case 0x04: //PCI-to-PCI Bridge
					break;

				case 0x05: //PCMCIA Bridge
					break;

				case 0x06: //NuBus Bridge
					break;

				case 0x07: //CardBus Bridge
					break;

				case 0x08: //RACEway Bridge
					break;

				case 0x09: //PCI-to-PCI Bridge
					break;

				case 0x0A: //InfiniBand-to-PCI Host Bridge
					break;

				case 0x80: //Other
					break;
			}
			break;

		case 0x07: //Simple Communication Controller
			switch(dev.subclass_id) {
				case 0x00: //Serial Controller
					break;

				case 0x01: //Parallel Controller
					break;

				case 0x02: //Multiport Serial Controller
					break;

				case 0x03: //Modem
					break;

				case 0x04: //IEEE 488.1/2 (GPIB) Controller
					break;

				case 0x05: //Smart Card
					break;

				case 0x80: //Other
					break;
			}
			break;

		case 0x08: //Base System Peripheral
			switch(dev.subclass_id) {
				case 0x00: //PIC
					break;

				case 0x01: //DMA Controller
					break;

				case 0x02: //Timer
					break;

				case 0x03: //RTC Controller
					break;

				case 0x04: //PCI Hot-Plug Controller
					break;

				case 0x05: //SD Host Controller
					break;

				case 0x06: //IOMMU
					break;

				case 0x80: //Other
					break;
			}
			break;

		case 0x09: //Input Device Controller
			switch(dev.subclass_id) {
				case 0x00: //Keyboard Controller
					break;

				case 0x01: //Digitizer Pen
					break;

				case 0x02: //Mouse Controller
					break;

				case 0x03: //Scanner Controller
					break;

				case 0x04: //Gameport Controller
					break;

				case 0x80: //Other
					break;
			}
			break;

		case 0x0A: //Docking Station
			switch(dev.subclass_id) {
				case 0x00: //Generic
					break;

				case 0x80: //Other
					break;
			}
			break;

		case 0x0B: //Processor
			switch(dev.subclass_id) {
				case 0x00: //386
					break;

				case 0x01: //486
					break;

				case 0x02: //Pentium
					break;

				case 0x10: //Alpha
					break;

				case 0x20: //PowerPC
					break;

				case 0x30: //MIPS
					break;

				case 0x40: //Co-Processor
					break;
			}
			break;

		case 0x0C: //Serial Bus Controller
			switch(dev.subclass_id) {
				case 0x00: //FireWire (IEEE 1394) Controller
					break;

				case 0x01: //ACCESS Bus
					break;

				case 0x02: //SSA
					break;

				case 0x03: //USB Controller
					break;

				case 0x04: //Fibre Channel
					break;

				case 0x05: //SMBus
					break;

				case 0x06: //InfiniBand
					break;

				case 0x07: //IPMI Interface
					break;

				case 0x08: //SERCOS Interface (IEC 61491)
					break;

				case 0x09: //CANbus
					break;
			}
			break;

		case 0x0D: //Wireless Controller
			switch(dev.subclass_id) {
				case 0x00: //iRDA Compatible Controller
					break;

				case 0x01: //Consumer IR Controller
					break;

				case 0x10: //RF Controller
					break;

				case 0x11: //Bluetooth Controller
					break;

				case 0x12: //Broadband Controller
					break;

				case 0x20: //Ethernet Controller (802.1a)
					break;

				case 0x21: //Ethernet Controller (802.1b)
					break;

				case 0x80: //Other
					break;
			}
			break;

		case 0x0E: //Intelligent Controller
			switch(dev.subclass_id) {
				case 0x00: //I20
					break;
			}
			break;

		case 0x0F: //Satellite Communication Controller
			switch(dev.subclass_id) {
				case 0x01: //Satellite TV Controller
					break;

				case 0x02: //Satellite Audio Controller
					break;

				case 0x03: //Satellite Voice Controller
					break;

				case 0x04: //Satellite Data Controller
					break;
			}
			break;

		case 0x10: //Encryption Controller
			switch(dev.subclass_id) {
				case 0x00: //Network and Computing Encrpytion/Decryption
					break;

				case 0x10: //Entertainment Encryption/Decryption
					break;

				case 0x80: //Other Encryption/Decryption
					break;
			}
			break;

		case 0x11: //Signal Processing Controller
			switch(dev.subclass_id) {
				case 0x00: //DPIO Modules
					break;

				case 0x01: //Performance Counters
					break;

				case 0x10: //Communication Synchronizer
					break;

				case 0x20: //Signal Processing Management
					break;

				case 0x80: //Other
					break;
			}
			break;

		case 0x12: //Processing Accelerator
			break;

		case 0x13: //Non-Essential Instrumentation
			break;

		case 0x14: //0x3F (Reserved)
			break;

		case 0x40: //Co-Processor
			break;

		case 0x41: //0xFE (Reserved)
			break;

		case 0xFF: //Unassigned Class (Vendor specific)
			break;
	}
	//END Class Codes

	return 0;
}

PCInterconnectDeviceDescriptor PCInterconnectController::GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function) {
	PCInterconnectDeviceDescriptor result;

	result.bus = bus;
	result.device = device;
	result.function = function;

	result.vendor_id = Read(bus, device, function, 0x00);
	result.device_id = Read(bus, device, function, 0x02);

	result.class_id = Read(bus, device, function, 0x0b);
	result.subclass_id = Read(bus, device, function, 0x0a);
	result.interface_id = Read(bus, device, function, 0x09);

	result.revision = Read(bus, device, function, 0x08);
	result.interrupt = Read(bus, device, function, 0x3c);

	return result;
}

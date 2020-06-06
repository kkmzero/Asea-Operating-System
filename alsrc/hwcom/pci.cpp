/*
 * This file is part of Asea OS.
 * Copyright (C) 2018 - 2020 Ivan Kmeťo
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
#include <system/lists/idspci.list>

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

bool PCInterconnectController::DeviceHasFunctions(uint16_t bus, uint16_t device) {
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
    //Vendor IDs and Devices
    switch(dev.vendor_id)
    {
        case PCI_ID_VENDOR_AMD:
            switch(dev.device_id) {
                case 0x2000: //am79c973 (driver?)
                    break;
            }
            break;

        case PCI_ID_VENDOR_APPLE: break;

        case PCI_ID_VENDOR_REDHAT_VIRTIO: break;

        case PCI_ID_VENDOR_INTEL: break;

        case PCI_ID_VENDOR_INNOTEK:
            switch(dev.device_id) {
                case PCI_ID_DEVICE_VBOXGFX: break;

                case PCI_ID_DEVICE_VBOXGUEST: break;
            }
            break;

        case PCI_ID_VENDOR_REALTEK: break;
    }

    //Class Codes
    switch(dev.class_id)
    {
        case PCI_ID_CLASS_UNCLASSIFIED:
            switch(dev.subclass_id) {
                case PCI_ID_SUBCLASS_NONVGA: break;

                case PCI_ID_SUBCLASS_VGACMP: break;
            }
            break;

        case PCI_ID_CLASS_MSC:
            switch(dev.subclass_id) {
                case PCI_ID_SUBCLASS_SCSIBC: break;

                case PCI_ID_SUBCLASS_IDEC: break;

                case PCI_ID_SUBCLASS_FDISKC: break;

                case PCI_ID_SUBCLASS_IPIBC: break;

                case PCI_ID_SUBCLASS_RAIDC: break;

                case PCI_ID_SUBCLASS_ATAC: break;

                case PCI_ID_SUBCLASS_SERIALATA: break;

                case PCI_ID_SUBCLASS_SASCSI: break;

                case PCI_ID_SUBCLASS_NVMEMC: break;

                case PCI_ID_SUBCLASS_OTHER: break;
            }
            break;

        case PCI_ID_CLASS_NETC:
            switch(dev.subclass_id) {
                case PCI_ID_SUBCLASS_ETHERNETC: break;

                case PCI_ID_SUBCLASS_TOKENRINGC: break;

                case PCI_ID_SUBCLASS_FDDIC: break;

                case PCI_ID_SUBCLASS_ATMC: break;

                case PCI_ID_SUBCLASS_ISDNC: break;

                case PCI_ID_SUBCLASS_WFIPC: break;

                case PCI_ID_SUBCLASS_PICMGMC: break;

                case PCI_ID_SUBCLASS_INFINIBANDC: break;

                case PCI_ID_SUBCLASS_FABRICC: break;

                case PCI_ID_SUBCLASS_OTHER: break;
            }
            break;

        case PCI_ID_CLASS_DISPLAYC:
            switch(dev.subclass_id) {
                case PCI_ID_SUBCLASS_VGADCC: break;

                case PCI_ID_SUBCLASS_XGAC: break;

                case PCI_ID_SUBCLASS_3DC: break;

                case PCI_ID_SUBCLASS_OTHER: break;
            }
            break;

        case PCI_ID_CLASS_MMEDIAC:
            switch(dev.subclass_id) {
                case PCI_ID_SUBCLASS_MMEDIAVIDC: break;

                case PCI_ID_SUBCLASS_MMEDIAAUDC: break;

                case PCI_ID_SUBCLASS_CTDEVICE: break;

                case PCI_ID_SUBCLASS_AUDIODEVICE: break;

                case PCI_ID_SUBCLASS_OTHER: break;
            }
            break;

        case PCI_ID_CLASS_MEMORYC:
            switch(dev.subclass_id) {
                case PCI_ID_SUBCLASS_RAMC: break;

                case PCI_ID_SUBCLASS_FLASHC: break;

                case PCI_ID_SUBCLASS_OTHER: break;
            }
            break;

        case PCI_ID_CLASS_BRIDGEDEVICE:
            switch(dev.subclass_id) {
                case PCI_ID_SUBCLASS_HOSTBRIDGE: break;

                case PCI_ID_SUBCLASS_ISABRIDGE: break;

                case PCI_ID_SUBCLASS_EISABRIDGE: break;

                case PCI_ID_SUBCLASS_MCABRIDGE: break;

                case PCI_ID_SUBCLASS_PCI2PCIB1: break;

                case PCI_ID_SUBCLASS_PCMCIABRIDGE: break;

                case PCI_ID_SUBCLASS_NUBUSBRIDGE: break;

                case PCI_ID_SUBCLASS_CARDBUSBRIDGE: break;

                case PCI_ID_SUBCLASS_RACEWAYBRIDGE: break;

                case PCI_ID_SUBCLASS_PCI2PCIB2: break;

                case PCI_ID_SUBCLASS_INIFINIB2PCI: break;

                case PCI_ID_SUBCLASS_OTHER: break;
            }
            break;

        case PCI_ID_CLASS_SIMPLECOMC:
            switch(dev.subclass_id) {
                case PCI_ID_SUBCLASS_SERIALC: break;

                case PCI_ID_SUBCLASS_PARALLELC: break;

                case PCI_ID_SUBCLASS_MPORTSERIALC: break;

                case PCI_ID_SUBCLASS_MODEM: break;

                case PCI_ID_SUBCLASS_IEEEGPIBC: break;

                case PCI_ID_SUBCLASS_SMARTCARD: break;

                case PCI_ID_SUBCLASS_OTHER: break;
            }
            break;

        case PCI_ID_CLASS_BSP:
            switch(dev.subclass_id) {
                case PCI_ID_SUBCLASS_PIC: break;

                case PCI_ID_SUBCLASS_DMAC: break;

                case PCI_ID_SUBCLASS_TIMER: break;

                case PCI_ID_SUBCLASS_RTCC: break;

                case PCI_ID_SUBCLASS_PCIHOTPLUGC: break;

                case PCI_ID_SUBCLASS_SDHOSTC: break;

                case PCI_ID_SUBCLASS_IOMMU: break;

                case PCI_ID_SUBCLASS_OTHER: break;
            }
            break;

        case PCI_ID_CLASS_IDEVC:
            switch(dev.subclass_id) {
                case PCI_ID_SUBCLASS_KBDC: break;

                case PCI_ID_SUBCLASS_DIGITIZERPEN: break;

                case PCI_ID_SUBCLASS_MOUSEC: break;

                case PCI_ID_SUBCLASS_SCANNERC: break;

                case PCI_ID_SUBCLASS_GAMEPORTC: break;

                case PCI_ID_SUBCLASS_OTHER: break;
            }
            break;

        case PCI_ID_CLASS_DOCKINGSTATION:
            switch(dev.subclass_id) {
                case PCI_ID_SUBCLASS_DSTATGENERIC: break;

                case PCI_ID_SUBCLASS_OTHER: break;
            }
            break;

        case PCI_ID_CLASS_CPU:
            switch(dev.subclass_id) {
                case PCI_ID_SUBCLASS_ARCH386: break;

                case PCI_ID_SUBCLASS_ARCH486: break;

                case PCI_ID_SUBCLASS_ARCHPENTIUM: break;

                case PCI_ID_SUBCLASS_ARCHPENTIUMPRO: break;

                case PCI_ID_SUBCLASS_ARCHALPHA: break;

                case PCI_ID_SUBCLASS_ARCHPOWERPC: break;

                case PCI_ID_SUBCLASS_ARCHMIPS: break;

                case PCI_ID_SUBCLASS_COPROCESSOR: break;

				case PCI_ID_SUBCLASS_OTHER: break;
            }
            break;

        case PCI_ID_CLASS_SERIALBUSC:
            switch(dev.subclass_id) {
                case PCI_ID_SUBCLASS_FWIEEE1394C: break;

                case PCI_ID_SUBCLASS_ACCESSBUS: break;

                case PCI_ID_SUBCLASS_SSA: break;

                case PCI_ID_SUBCLASS_USBCONTROLLER: break;

                case PCI_ID_SUBCLASS_FIBRECHANNEL: break;

                case PCI_ID_SUBCLASS_SMBUS: break;

                case PCI_ID_SUBCLASS_SBINFINIBAND: break;

                case PCI_ID_SUBCLASS_IPMII: break;

                case PCI_ID_SUBCLASS_SERCOSI: break;

                case PCI_ID_SUBCLASS_CANBUS: break;

                case PCI_ID_SUBCLASS_OTHER: break;
            }
            break;

        case PCI_ID_CLASS_WIRELESSC:
            switch(dev.subclass_id) {
                case PCI_ID_SUBCLASS_IRDACC: break;

                case PCI_ID_SUBCLASS_CIRC: break;

                case PCI_ID_SUBCLASS_RFC: break;

                case PCI_ID_SUBCLASS_BLUETOOTHC: break;

                case PCI_ID_SUBCLASS_BROADBANDC: break;

                case PCI_ID_SUBCLASS_ETHERNETC8021A: break;

                case PCI_ID_SUBCLASS_ETHERNETC8021B: break;

                case PCI_ID_SUBCLASS_OTHER: break;
            }
            break;

        case PCI_ID_CLASS_INTELLIGENTC:
            switch(dev.subclass_id) {
                case PCI_ID_SUBCLASS_ICI20: break;
            }
            break;

        case PCI_ID_CLASS_SATELLITECC:
            switch(dev.subclass_id) {
                case PCI_ID_SUBCLASS_SATTVC: break;

                case PCI_ID_SUBCLASS_SATAUDIOC: break;

                case PCI_ID_SUBCLASS_SATVOICEC: break;

                case PCI_ID_SUBCLASS_SATDATAC: break;
            }
            break;

        case PCI_ID_CLASS_ENCRYPTIONC:
            switch(dev.subclass_id) {
                case PCI_ID_SUBCLASS_NETCEDC: break;

                case PCI_ID_SUBCLASS_EEDC: break;

                case PCI_ID_SUBCLASS_OTHER: break;
            }
            break;

        case PCI_ID_CLASS_SIGNALPROCC:
            switch(dev.subclass_id) {
                case PCI_ID_SUBCLASS_DPIOMOD: break;

                case PCI_ID_SUBCLASS_PERFCOUNTER: break;

                case PCI_ID_SUBCLASS_COMSYNC: break;

                case PCI_ID_SUBCLASS_SPMAN: break;

                case PCI_ID_SUBCLASS_OTHER: break;
            }
            break;

        case PCI_ID_CLASS_PROCACCELERATOR: break;

        case PCI_ID_CLASS_NONEINSTR: break;

        case 0x14: //0x3F (Reserved)
            break;

        case PCI_ID_CLASS_COPROCESSOR: break;

        case 0x41: //0xFE (Reserved)
            break;

        case PCI_ID_CLASS_UNASSIGNED: break;
    }

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

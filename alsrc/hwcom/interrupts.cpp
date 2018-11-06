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

#include <hwcom/interrupts.h>
#include <System/headers/asyslib.h>

using namespace asea::common;
using namespace asea::hwcom;
using namespace asea::System::headers::asl;

void printf(char* str);

InterruptHandler::InterruptHandler(uint8_t interruptNumber, InterruptManager* interruptManager) {
	this->interruptNumber = interruptNumber;
	this->interruptManager = interruptManager;
	interruptManager->handlers[interruptNumber] = this;
}

InterruptHandler::~InterruptHandler() {
	if(interruptManager->handlers[interruptNumber] == this)
		interruptManager->handlers[interruptNumber] = 0;
}

uint32_t InterruptHandler::HandleInterrupt(uint32_t esp) {
	return esp;
}


InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

InterruptManager* InterruptManager::ActiveInterruptManager = 0;


void InterruptManager::SetInterruptDescriptorTableEntry(
		uint8_t interruptNumber,
		uint16_t codeSegmentSelectorOffset,
		void (*handler)(),
		uint8_t DescriptorPrivilegeLevel,
		uint8_t DescriptorType)
{
	const uint8_t IDT_DESC_PRESENT = 0x80;

	interruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF;
	interruptDescriptorTable[interruptNumber].handlerAddressHighBits = (((uint32_t)handler) >> 16) & 0xFFFF;
	interruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = codeSegmentSelectorOffset;
	interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | DescriptorType | ((DescriptorPrivilegeLevel&3) << 5);
	interruptDescriptorTable[interruptNumber].reserved = 0;
}

InterruptManager::InterruptManager(GlobalDescriptorTable* gdt)
: picMasterCommand(0x20),
  picMasterData(0x21),
  picSlaveCommand(0xA0),
  picSlaveData(0xA1)
{
	uint16_t CodeSegment = gdt->CodeSegmentSelector();
	const uint8_t IDT_INTERRUPT_GATE = 0xE;

	for(uint16_t i = 0; i < 256; i++)
	{
		handlers[i] = 0;
		SetInterruptDescriptorTableEntry(i, CodeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
	}

	SetInterruptDescriptorTableEntry(0x20, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
	SetInterruptDescriptorTableEntry(0x21, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
	SetInterruptDescriptorTableEntry(0x2C, CodeSegment, &HandleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);


	picMasterCommand.Write(0x11);
	picSlaveCommand.Write(0x11);
	
	picMasterData.Write(0x20);
	picSlaveData.Write(0x28);

	picMasterData.Write(0x04);
	picSlaveData.Write(0x02);

	picMasterData.Write(0x01);
	picSlaveData.Write(0x01);

	picMasterData.Write(0x00);
	picSlaveData.Write(0x00);


	InterruptDescriptorTablePointer idt;
	idt.size = 256 * sizeof(GateDescriptor) - 1;
	idt.base = (uint32_t)interruptDescriptorTable;
	asm volatile("lidt %0" : : "m" (idt));
}

InterruptManager::~InterruptManager() {
}

void InterruptManager::Activate() {
	if(ActiveInterruptManager != 0)
		ActiveInterruptManager->Deactivate();
	ActiveInterruptManager = this;
	asm("sti");
}

void InterruptManager::Deactivate() {
	if(ActiveInterruptManager = this)
	{
		ActiveInterruptManager = 0;
		asm("cli");
	}
}

uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp) {
	if(ActiveInterruptManager != 0)
		return ActiveInterruptManager->DoHandleInterrupt(interruptNumber, esp);

	return esp;
}

uint32_t InterruptManager::DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp) {
	if(handlers[interruptNumber] != 0)
	{
		esp = handlers[interruptNumber]->HandleInterrupt(esp);
	}
	else if(interruptNumber != 0x20)
	{
		printf("UNHANDLED INTERRUPT 0x");
		printfHex(interruptNumber);
	}

	if(0x20 <= interruptNumber && interruptNumber < 0x30)
	{
		picMasterCommand.Write(0x20);

		if(0x28 <= interruptNumber)
			picSlaveCommand.Write(0x20);
	}

	return esp;
}

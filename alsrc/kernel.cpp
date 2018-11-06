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

#include <common/types.h>
#include <gdt.h>
#include <hwcom/interrupts.h>
#include <hwcom/pci.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <System/headers/sysnfo.h>
#include <System/headers/sysmsgs.h>

using namespace asea;
using namespace asea::common;
using namespace asea::drivers;
using namespace asea::hwcom;
using namespace asea::System::headers;

void printf(char* str) {
	static uint16_t* VideoMemory = (uint16_t*)0xb8000;
	static uint8_t x = 0, y = 0;

	for(int i = 0; str[i] != '\0'; ++i)
	{
		switch(str[i])
		{
			case '\n':
				y++;
				x = 0;
				break;

			case '\b':
				if (x <= 0)
				{
					x = 0;
				}
				else {
					x -= 1;
				}
				VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
				break;

			default:
				VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
				x++;
				break;
		}

		if(x >= 80)
		{
			y++;
			x = 0;
		}

		if (y >= 25)
		{
			for(y = 0; y < 25; y++)
			for (x =0; x < 80; x++)
			VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';

			x = 0;
			y = 0;
		}
	}
}

class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
	public:
		void OnKeyDown(char c)
		{
			char* foo = " ";
			foo[0] = c;
			printf(foo);
		}
};

class MouseToConsole : public MouseEventHandler
{
		int8_t x, y;
	public:
		MouseToConsole()
		{
			static uint16_t* VideoMemory = (uint16_t*)0xb8000;
			x = 40;
			y = 12;
			VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4)
								| ((VideoMemory[80*y+x] & 0x0F00) << 4)
								| ((VideoMemory[80*y+x] & 0x00FF));			
		}

	void OnMouseMove(int xoffset, int yoffset)
	{
		static uint16_t* VideoMemory = (uint16_t*)0xb8000;
		VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4)
							| ((VideoMemory[80*y+x] & 0x0F00) << 4)
							| ((VideoMemory[80*y+x] & 0x00FF));

		x += xoffset;
		if(x >= 80) x = 79;
		if(x < 0) x = 0;

		y -= yoffset;
		if(y >= 25) y = 24;
		if(y < 0) y = 0;

		VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4)
							| ((VideoMemory[80*y+x] & 0x0F00) << 4)
							| ((VideoMemory[80*y+x] & 0x00FF));
	}
};

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors() {
	for(constructor* i = &start_ctors; i != &end_ctors; i++)
		(*i)();
}

extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*multiboot_magic*/)
{
	AseaSystemInfo sysInfo;
	AseaSystemMessages sysMsgs;

	sysInfo.AS_PrintSysInfoMsg(0x001);

	GlobalDescriptorTable gdt;
	InterruptManager interrupts(&gdt);

	sysMsgs.AS_StatusMsgInf(0x01);
	DriverManager drvManager;

	PrintfKeyboardEventHandler kbhandler;
	KeyboardDriver keyboard(&interrupts, &kbhandler);
	drvManager.AddDriver(&keyboard);

	MouseToConsole mousehandler;
	MouseDriver mouse(&interrupts, &mousehandler);
	drvManager.AddDriver(&mouse);

	PCInterconnectController PCIController;
	PCIController.SelectDrivers(&drvManager, &interrupts);

	drvManager.ActivateAll();
	sysMsgs.AS_StatusMsg(0x01, "Hardware Initialization\n");

	interrupts.Activate();
	sysMsgs.AS_StatusMsg(0x01, "Hardware Interrupts\n\n");

	while(1);
}

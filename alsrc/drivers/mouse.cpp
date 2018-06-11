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

#include <drivers/mouse.h>

using namespace asea::common;
using namespace asea::drivers;
using namespace asea::hwcom;

MouseEventHandler::MouseEventHandler() {
}

void MouseEventHandler::OnActivate() {
}

void MouseEventHandler::OnMouseDown(uint8_t button) {
	// Instructions for what to do when mouse button is pressed 
}

void MouseEventHandler::OnMouseUp(uint8_t button) {
	// Instructions for what to do when mouse is released 
}

void MouseEventHandler::OnMouseMove(int x, int y) {
}

MouseDriver::MouseDriver(InterruptManager * manager, MouseEventHandler* handler)
: InterruptHandler(0x2C, manager),
dataport(0x60),
commandport(0x64)
{
	this->handler = handler;
}

MouseDriver::~MouseDriver() {
}

void MouseDriver::Activate() {
	offset = 0;
	buttons = 0;

	commandport.Write(0xA8); //activate interrupts
	commandport.Write(0x20); //get current state
	uint8_t status = dataport.Read() | 2;
	commandport.Write(0x60); //set state
	dataport.Write(status);

	commandport.Write(0xD4);
	dataport.Write(0xF4);
	dataport.Read();
}

uint32_t MouseDriver::HandleInterrupt(uint32_t esp) {
	uint8_t status = commandport.Read();
	if((!(status & 0x20)) || handler == 0)
		return esp;

	buffer[offset] = dataport.Read();
	offset = (offset + 1) % 3;

	if(offset == 0)
	{
		if(buffer[1] != 0 || buffer[2] != 0)
		{
			handler->OnMouseMove((int8_t)buffer[1], ((int8_t)buffer[2]));
		}


		for(uint8_t i = 0; i < 3; i++)
		{
			if((buffer[0] & (0x01 << i)) != (buttons & (0x01<<i)))
			{
				if(buttons & (0x1<<i))
					handler->OnMouseUp(i+1);
				else
					handler->OnMouseDown(i+1);
			}	
		}

		buttons = buffer[0];
	}

	return esp;
}

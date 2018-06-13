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

#include <drivers/keyboard.h>

using namespace asea::common;
using namespace asea::drivers;
using namespace asea::hwcom;

KeyboardEventHandler::KeyboardEventHandler() {
}

void KeyboardEventHandler::OnKeyDown(char) {
}

void KeyboardEventHandler::OnKeyUp(char) {
}

KeyboardDriver::KeyboardDriver(InterruptManager * manager, KeyboardEventHandler* handler)
: InterruptHandler(0x21, manager),
dataport(0x60),
commandport(0x64)
{
	this->handler = handler;
}

KeyboardDriver::~KeyboardDriver() {
}

void printf(char*);
void printfHex(uint8_t);

void KeyboardDriver::Activate() {
	while(commandport.Read() & 0x1)
		dataport.Read();
	commandport.Write(0xAE); //activate interrupts
	commandport.Write(0x20); //get current state
	uint8_t status = (dataport.Read() | 1) & ~0x10;
	commandport.Write(0x60); //set state
	dataport.Write(status);

	dataport.Write(0xF4);
}

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp) {
	uint8_t key = dataport.Read();

	static bool Uppercase = false;

	if(handler == 0)
		return esp;

		switch(key)
		{
			case 0xFA: break;

			//---KEYMAPPING---
			//case 0x01: if(Uppercase) printf("A"); else printf("a"); break; //ESCAPE

			case 0x02: if(Uppercase) handler->OnKeyDown('!'); else handler->OnKeyDown('1'); break;
			case 0x03: if(Uppercase) handler->OnKeyDown('@'); else handler->OnKeyDown('2'); break;
			case 0x04: if(Uppercase) handler->OnKeyDown('#'); else handler->OnKeyDown('3'); break;
			case 0x05: if(Uppercase) handler->OnKeyDown('$'); else handler->OnKeyDown('4'); break;
			case 0x06: if(Uppercase) handler->OnKeyDown('%'); else handler->OnKeyDown('5'); break;
			case 0x07: if(Uppercase) handler->OnKeyDown('^'); else handler->OnKeyDown('6'); break;
			case 0x08: if(Uppercase) handler->OnKeyDown('&'); else handler->OnKeyDown('7'); break;
			case 0x09: if(Uppercase) handler->OnKeyDown('*'); else handler->OnKeyDown('8'); break;
			case 0x0A: if(Uppercase) handler->OnKeyDown('('); else handler->OnKeyDown('9'); break;
			case 0x0B: if(Uppercase) handler->OnKeyDown(')'); else handler->OnKeyDown('0'); break;
			case 0x0C: if(Uppercase) handler->OnKeyDown('_'); else handler->OnKeyDown('-'); break;
			case 0x0D: if(Uppercase) handler->OnKeyDown('+'); else handler->OnKeyDown('='); break;

			//case 0x0E: handler->OnKeyDown('\b'); break; //BACKSPACE

			case 0x0F: printf("    "); break; //TAB

			case 0x10: if(Uppercase) handler->OnKeyDown('Q'); else handler->OnKeyDown('q'); break;
			case 0x11: if(Uppercase) handler->OnKeyDown('W'); else handler->OnKeyDown('w'); break;
			case 0x12: if(Uppercase) handler->OnKeyDown('E'); else handler->OnKeyDown('e'); break;
			case 0x13: if(Uppercase) handler->OnKeyDown('R'); else handler->OnKeyDown('r'); break;
			case 0x14: if(Uppercase) handler->OnKeyDown('T'); else handler->OnKeyDown('t'); break;
			case 0x15: if(Uppercase) handler->OnKeyDown('Y'); else handler->OnKeyDown('y'); break;
			case 0x16: if(Uppercase) handler->OnKeyDown('U'); else handler->OnKeyDown('u'); break;
			case 0x17: if(Uppercase) handler->OnKeyDown('I'); else handler->OnKeyDown('i'); break;
			case 0x18: if(Uppercase) handler->OnKeyDown('O'); else handler->OnKeyDown('o'); break;
			case 0x19: if(Uppercase) handler->OnKeyDown('P'); else handler->OnKeyDown('p'); break;
			case 0x1A: if(Uppercase) handler->OnKeyDown('{'); else handler->OnKeyDown('['); break;
			case 0x1B: if(Uppercase) handler->OnKeyDown('}'); else handler->OnKeyDown(']'); break;
			case 0x1C: handler->OnKeyDown('\n'); break; //ENTER

			//case 0x1D: printf("\n"); break; //LEFT CONTROL

			case 0x1E: if(Uppercase) handler->OnKeyDown('A'); else handler->OnKeyDown('a'); break;
			case 0x1F: if(Uppercase) handler->OnKeyDown('S'); else handler->OnKeyDown('s'); break;
			case 0x20: if(Uppercase) handler->OnKeyDown('D'); else handler->OnKeyDown('d'); break;
			case 0x21: if(Uppercase) handler->OnKeyDown('F'); else handler->OnKeyDown('f'); break;
			case 0x22: if(Uppercase) handler->OnKeyDown('G'); else handler->OnKeyDown('g'); break;
			case 0x23: if(Uppercase) handler->OnKeyDown('H'); else handler->OnKeyDown('h'); break;
			case 0x24: if(Uppercase) handler->OnKeyDown('J'); else handler->OnKeyDown('j'); break;
			case 0x25: if(Uppercase) handler->OnKeyDown('K'); else handler->OnKeyDown('k'); break;
			case 0x26: if(Uppercase) handler->OnKeyDown('L'); else handler->OnKeyDown('l'); break;
			case 0x27: if(Uppercase) handler->OnKeyDown(':'); else handler->OnKeyDown(';'); break;
			case 0x28: if(Uppercase) handler->OnKeyDown('\"'); else handler->OnKeyDown('\''); break;
			case 0x29: if(Uppercase) handler->OnKeyDown('~'); else handler->OnKeyDown('`'); break;
			case 0x2B: if(Uppercase) handler->OnKeyDown('|'); else handler->OnKeyDown('\\'); break;
			case 0x2C: if(Uppercase) handler->OnKeyDown('Z'); else handler->OnKeyDown('z'); break;
			case 0x2D: if(Uppercase) handler->OnKeyDown('X'); else handler->OnKeyDown('x'); break;
			case 0x2E: if(Uppercase) handler->OnKeyDown('C'); else handler->OnKeyDown('c'); break;
			case 0x2F: if(Uppercase) handler->OnKeyDown('V'); else handler->OnKeyDown('v'); break;
			case 0x30: if(Uppercase) handler->OnKeyDown('B'); else handler->OnKeyDown('b'); break;
			case 0x31: if(Uppercase) handler->OnKeyDown('N'); else handler->OnKeyDown('n'); break;
			case 0x32: if(Uppercase) handler->OnKeyDown('M'); else handler->OnKeyDown('m'); break;
			case 0x33: if(Uppercase) handler->OnKeyDown('<'); else handler->OnKeyDown(','); break;
			case 0x34: if(Uppercase) handler->OnKeyDown('>'); else handler->OnKeyDown('.'); break;
			case 0x35: if(Uppercase) handler->OnKeyDown('?'); else handler->OnKeyDown('/'); break;

			case 0x37: break; //Numpad: *
			case 0x38: break; //LEFT ALT

			case 0x39: handler->OnKeyDown(' '); break; //SPACE

			//SHIFT
			case 0x2A: case 0x36: Uppercase = !Uppercase; break; //LEFT || RIGHT SHIFT HOLD
			case 0xAA: case 0xB6: Uppercase = !Uppercase; break; //LEFT || RIGHT RELEASE
			//END SHIFT

			case 0x3A: Uppercase = !Uppercase; //CAPSLOCK

			case 0x3B: break; //F1
			case 0x3C: break; //F2
			case 0x3D: break; //F3
			case 0x3E: break; //F4
			case 0x3F: break; //F5
			case 0x40: break; //F6
			case 0x41: break; //F7
			case 0x42: break; //F8
			case 0x43: break; //F9
			case 0x44: break; //F10
			case 0x57: break; //F11
			case 0x58: break; //F12
            
			case 0x45: break; //NumLock
			case 0x46: break; //ScrollLock
            
			case 0x47: break; //Numpad: 7
			case 0x48: break; //Numpad: 8
			case 0x49: break; //Numpad: 9
			case 0x4A: break; //Numpad: -
			case 0x4B: break; //Numpad: 4
			case 0x4C: break; //Numpad: 5
			case 0x4D: break; //Numpad: 6
			case 0x4E: break; //Numpad: +
			case 0x4F: break; //Numpad: 1
			case 0x50: break; //Numpad: 2
			case 0x51: break; //Numpad: 3
			case 0x52: break; //Numpad: 0
			case 0x53: break; //Numpad: .

			//---END KEYMAPPING---

            case 0xC5: break;


		default:
		if(key < 0x80)
		{
			{
					printf("UNHANDLED INTERRUPT KEYBOARD0x");
					printfHex(key);
				break;
			}
		}

	}

	return esp;
}

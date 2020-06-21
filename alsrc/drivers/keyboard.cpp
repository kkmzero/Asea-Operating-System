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

#include <drivers/keyboard.h>
#include <astd>

using namespace asea::common;
using namespace asea::drivers;
using namespace asea::hwcom;

static bool Uppercase = false;
static bool NumLock = false;
static bool media_key = false;


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

void kbd_ack() {
    while(!(inb(0x60) == 0xFA));
}

void kbd_handle_led(uint8_t status) {
    outb(0x60, 0xED);
    kbd_ack();
    outb(0x60, status);
}

void key_toggle() { //[!] TESTING
    outb(0x60, 0xF5);
    kbd_ack();
    outb(0x60, 0xF4);
}

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp) {
    uint8_t key = dataport.Read();

    if(handler == 0)
        return esp;

        switch(key)
        {
            //Response Byte
            case 0x00: printf("KDError",0x0C); break; //Key Detection Error or internal buffer overrun
            //case 0xAA: printf(" STP ",0x0C); break; //Self-Test Passed
            case 0xEE: printf("ECHO",0x0C); break;    //Response to Echo command
            case 0xFA: printf("ACK",0x0C); break;     //Command Acknowledged (ACK)
            case 0xFC: printf("FAIL",0x0C); break;    //Self-Test Failed
            case 0xFD: printf("FAIL",0x0C); break;    //Self-Test Failed
            case 0xFE: printf("RESEND",0x0C); break;  //Repeat Last command send
            case 0xFF: printf("KDError",0x0C); break; //Key Detection Error or internal buffer overrun

            //---KEYMAPPING---
            case 0xE0: media_key = !media_key; printf(" [0xE0] ",0x0B); break; //Media Key Flag
            case 0x01: break; //ESCAPE pressed

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

            case 0x0E: handler->OnKeyDown('\b'); break; //BACKSPACE pressed
            case 0x0F: printf("    "); break; //TAB pressed

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
            case 0x1C: key_toggle(); if(media_key) { if(NumLock) handler->OnKeyDown('\n'); } else { handler->OnKeyDown('\n'); } break; //ENTER pressed, 0xE0: Numpad: ENTER pressed

            case 0x1D: break; //LEFT CONTROL pressed

            case 0x1E: if(Uppercase) handler->OnKeyDown('A'); else handler->OnKeyDown('a'); break;
            case 0x1F: if(Uppercase) handler->OnKeyDown('S'); else handler->OnKeyDown('s'); break;
            case 0x20: if(Uppercase) handler->OnKeyDown('D'); else handler->OnKeyDown('d'); break;
            case 0x21: if(media_key) { /*dohandlemediakey*/ } else { if(Uppercase) handler->OnKeyDown('F'); else handler->OnKeyDown('f'); } break; //0xE0: Calculator pressed
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
            case 0x32: if(media_key) { /*dohandlemediakey*/ } else { if(Uppercase) handler->OnKeyDown('M'); else handler->OnKeyDown('m'); } break; //0xE0: WWW home pressed
            case 0x33: if(Uppercase) handler->OnKeyDown('<'); else handler->OnKeyDown(','); break;
            case 0x34: if(Uppercase) handler->OnKeyDown('>'); else handler->OnKeyDown('.'); break;
            case 0x35: if(media_key) { if(NumLock) handler->OnKeyDown('/'); } else { if(Uppercase) handler->OnKeyDown('?'); else handler->OnKeyDown('/'); } break; // "/" pressed, 0xEO: Numpad: "/" pressed

            case 0x37: if(NumLock) handler->OnKeyDown('*'); break; //Numpad: * pressed
            case 0x38: break; //LEFT ALT pressed

            case 0x39: handler->OnKeyDown(' '); break; //SPACE pressed

            //SHIFT
            case 0x2A: if(media_key) { /*dohandlemediakey*/ } else { key_toggle(); Uppercase = !Uppercase; break; } //LEFT SHIFT pressed, 0xE0 Printscreen pressed
            case 0x36: key_toggle(); Uppercase = !Uppercase; break; //RIGHT SHIFT pressed
            case 0xAA: key_toggle(); Uppercase = !Uppercase; break; //LEFT SHIFT released
            case 0xB6: key_toggle(); Uppercase = !Uppercase; break; //RIGHT SHIFT released
            //END SHIFT

            case 0x3A: key_toggle(); Uppercase = !Uppercase; /*kbd_handle_led(0x02);*/ break; //CAPSLOCK pressed

            case 0x3B: break; //F1 pressed
            case 0x3C: break; //F2 pressed
            case 0x3D: break; //F3 pressed
            case 0x3E: break; //F4 pressed
            case 0x3F: break; //F5 pressed
            case 0x40: break; //F6 pressed
            case 0x41: break; //F7 pressed
            case 0x42: break; //F8 pressed
            case 0x43: break; //F9 pressed
            case 0x44: break; //F10 pressed
            case 0x57: break; //F11 pressed
            case 0x58: break; //F12 pressed

            case 0x45: key_toggle(); NumLock = !NumLock; break; //NumLock pressed
            case 0x46: break; //ScrollLock pressed

            case 0x47: if(media_key) { /*dohandlemediakey*/ } else { if(NumLock) handler->OnKeyDown('7'); } break; //Numpad: 7 pressed, 0xE0: Home pressed
            case 0x48: if(media_key) { /*dohandlemediakey*/ } else { if(NumLock) handler->OnKeyDown('8'); } break; //Numpad: 8 pressed, 0xE0: ArrowUp pressed
            case 0x49: if(media_key) { /*dohandlemediakey*/ } else { if(NumLock) handler->OnKeyDown('9'); } break; //Numpad: 9 pressed, 0xE0: PageUp pressed
            case 0x4A: if(NumLock) handler->OnKeyDown('-'); break; //Numpad: - pressed
            case 0x4B: if(media_key) { /*dohandlemediakey*/ } else { if(NumLock) handler->OnKeyDown('4'); } break; //Numpad: 4 pressed, 0xE0: ArrowLeft pressed
            case 0x4C: if(NumLock) handler->OnKeyDown('5'); break; //Numpad: 5 pressed
            case 0x4D: if(media_key) { /*dohandlemediakey*/ } else { if(NumLock) handler->OnKeyDown('6'); } break; //Numpad: 6 pressed, 0xE0: ArrowRight pressed
            case 0x4E: if(NumLock) handler->OnKeyDown('+'); break; //Numpad: + pressed
            case 0x4F: if(media_key) { /*dohandlemediakey*/ } else { if(NumLock) handler->OnKeyDown('1'); } break; //Numpad: 1 pressed, 0xE0: End pressed
            case 0x50: if(media_key) { /*dohandlemediakey*/ } else { if(NumLock) handler->OnKeyDown('2'); } break; //Numpad: 2 pressed, 0xE0: ArrowDown pressed
            case 0x51: if(media_key) { /*dohandlemediakey*/ } else { if(NumLock) handler->OnKeyDown('3'); } break; //Numpad: 3 pressed, 0xE0: PageDown pressed
            case 0x52: if(NumLock) handler->OnKeyDown('0'); break; //Numpad: 0 pressed
            case 0x53: if(media_key) { /*dohandlemediakey*/ } else { if(NumLock) handler->OnKeyDown('.'); } break; //Numpad: . pressed, 0xE0: Delete pressed

            case 0x5B: break; //Left Mod (Win) key pressed
            case 0xDB: break; //Left Mod (Win) key released
            case 0x5C: break; //Right Mod (Win) key pressed
            case 0xDC: break; //Right Mod (Win) key released
            case 0x5D: break; //"Apps" key pressed
            case 0xDD: break; //"Apps" key released

            case 0x81: break; //ESCAPE released
            case 0x82: break; //1 released
            case 0x83: break; //2 released
            case 0x84: break; //3 released
            case 0x85: break; //4 released
            case 0x86: break; //5 released
            case 0x87: break; //6 released
            case 0x88: break; //7 released
            case 0x89: break; //8 released
            case 0x8A: break; //9 released
            case 0x8B: break; //0 released
            case 0x8C: break; //- released
            case 0x8D: break; //= released
            case 0x8E: break; //BACKSPACE released
            case 0x8F: break; //TAB released
            case 0x90: break; //Q released
            case 0x91: break; //W released
            case 0x92: break; //E released
            case 0x93: break; //R released
            case 0x94: break; //T released
            case 0x95: break; //Y released
            case 0x96: break; //U released
            case 0x97: break; //I released
            case 0x98: break; //O released
            case 0x99: break; //P released
            case 0x9A: break; //[ released
            case 0x9B: break; //] released
            case 0x9C: if(media_key) { /*dohandlemediakey*/ } else { } break; //ENTER released, 0xE0: Numpad: ENTER released
            case 0x9D: break; //LEFT CONTROL released
            case 0x9E: break; //A released
            case 0x9F: break; //S released
            case 0xA0: break; //D released
            case 0xA1: break; //F released
            case 0xA2: break; //G released
            case 0xA3: break; //H released
            case 0xA4: break; //J released
            case 0xA5: break; //K released
            case 0xA6: break; //L released
            case 0xA7: break; //; released
            case 0xA8: break; //' released
            case 0xA9: break; //(back tick) released
            case 0xAB: break; //\ released
            case 0xAC: break; //Z released
            case 0xAD: break; //X released
            case 0xAE: break; //C released
            case 0xAF: break; //V released
            case 0xB0: break; //B released
            case 0xB1: break; //N released
            case 0xB2: break; //M released
            case 0xB3: break; //, released
            case 0xB4: break; //. released
            case 0xB5: break; //(/) released
            case 0xB7: break; //Numpad: * released
            case 0xB8: break; //LEFT ALT released
            case 0xB9: break; //SPACE RELEASED
            case 0xBA: break; //CAPSLOCK released
            case 0xBB: break; //F1 released
            case 0xBC: break; //F2 released
            case 0xBD: break; //F3 released
            case 0xBE: break; //F4 released
            case 0xBF: break; //F5 released
            case 0xC0: break; //F6 released
            case 0xC1: break; //F7 released
            case 0xC2: break; //F8 released
            case 0xC3: break; //F9 released
            case 0xC4: break; //F10 released
            case 0xC5: break; //NumLock released
            case 0xC6: break; //ScrollLock released
            case 0xC7: break; //Numpad: 7 released
            case 0xC8: break; //Numpad: 8 released
            case 0xC9: break; //Numpad: 9 released
            case 0xCA: break; //Numpad: - released
            case 0xCB: break; //Numpad: 4 released
            case 0xCC: break; //Numpad: 5 released
            case 0xCD: break; //Numpad: 6 released
            case 0xCE: break; //Numpad: + released
            case 0xCF: break; //Numpad: 1 released
            case 0xD0: break; //Numpad: 2 released
            case 0xD1: break; //Numpad: 3 released
            case 0xD2: break; //Numpad: 0 released
            case 0xD3: /*dohandlemediakey*/ break; //Numpad: . released, 0xE0: Delete released
            case 0xD6: break; //AltBackslash released
            case 0xD7: break; //F11 released
            case 0xD8: break; //F12 released
            
            case 0x6C: if(media_key) { /*dohandlemediakey*/ } break; //0xE0: mail pressed
            case 0xE1: break; //pause pressed
            case 0xEC: if(media_key) { /*dohandlemediakey*/ } break; //0xE0: mail released

            //TODO: CIV
            case 0x56: if(Uppercase) handler->OnKeyDown('|'); else handler->OnKeyDown('\\'); break;

            //---END KEYMAPPING---

            default:
                printf("UNHANDLED INTERRUPT KEYBOARD 0x");
                printfhex(key);
                break;
            }


    return esp;
}

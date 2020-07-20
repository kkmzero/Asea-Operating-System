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
#include <asea.h>
#include <system/lists/keymap.list>

using namespace asea::common;
using namespace asea::drivers;
using namespace asea::hwcom;

static bool Uppercase = false;
static bool NumLock = false;
static bool MediaKey = false;


KeyboardEventHandler::KeyboardEventHandler() {
    //AS_StatusMsg(STATUSMSG_INIT, "Driver: PS/2 Keyboard");
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

void key_toggle(uint8_t key_release) { //[!] TESTING
    while(!(inb(0x60) == key_release));
}

void shift_force_toggle() { //[!] TESTING
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
            //Response Bytes (For Debugging)
            case 0x00: /*printf(" [0x00] KDError ",COLOR_DKGRAY);*/ break; //Key Detection Error or internal buffer overrun
            //case 0xAA: printf(" [0xAA] STP ",COLOR_DKGRAY); break; //Self test passed (sent after "0xFF (reset)" command or keyboard power up), WARNING: Duplicate Case Value
            case 0xEE: /*printf(" [0xEE] ECHO ",COLOR_DKGRAY);*/ break;    //Response to Echo command
            case 0xFA: /*printf(" [0xFA] ACK ",COLOR_DKGRAY);*/ break;     //Command Acknowledged (ACK)
            case 0xFC: /*printf(" [0xFC] FAIL ",COLOR_DKGRAY);*/ break;    //Self test failed (sent after "0xFF (reset)" command or keyboard power up)
            case 0xFD: /*printf(" [0xFD] FAIL ",COLOR_DKGRAY);*/ break;    //Self test failed (sent after "0xFF (reset)" command or keyboard power up)
            case 0xFE: /*printf(" [0xFE] RESEND ",COLOR_DKGRAY);*/ break;  //Repeat Last command send
            case 0xFF: /*printf(" [0xFF] KDError ",COLOR_DKGRAY);*/ break; //Key Detection Error or internal buffer overrun

            //Media Key Flag/Escape Sequence
            case 0xE0: MediaKey = !MediaKey; /*printf(" [0xE0] ",COLOR_DKGRAY);*/ break;

            //---KEYMAPPING---
            case KP_ESC: break; //ESCAPE pressed

            case KP_1: if(Uppercase) handler->OnKeyDown('!'); else handler->OnKeyDown('1'); break;
            case KP_2: if(Uppercase) handler->OnKeyDown('@'); else handler->OnKeyDown('2'); break;
            case KP_3: if(Uppercase) handler->OnKeyDown('#'); else handler->OnKeyDown('3'); break;
            case KP_4: if(Uppercase) handler->OnKeyDown('$'); else handler->OnKeyDown('4'); break;
            case KP_5: if(Uppercase) handler->OnKeyDown('%'); else handler->OnKeyDown('5'); break;
            case KP_6: if(Uppercase) handler->OnKeyDown('^'); else handler->OnKeyDown('6'); break;
            case KP_7: if(Uppercase) handler->OnKeyDown('&'); else handler->OnKeyDown('7'); break;
            case KP_8: if(Uppercase) handler->OnKeyDown('*'); else handler->OnKeyDown('8'); break;
            case KP_9: if(Uppercase) handler->OnKeyDown('('); else handler->OnKeyDown('9'); break;
            case KP_0: if(Uppercase) handler->OnKeyDown(')'); else handler->OnKeyDown('0'); break;
            case KP_DASH: if(Uppercase) handler->OnKeyDown('_'); else handler->OnKeyDown('-'); break;
            case KP_EQUAL: if(Uppercase) handler->OnKeyDown('+'); else handler->OnKeyDown('='); break;

            case KP_BACKSPACE: handler->OnKeyDown('\b'); break;
            case KP_TAB: printf("    "); break;

            case KP_Q: if(MediaKey) { /*dohandlemediakey*/ } else { if(Uppercase) handler->OnKeyDown('Q'); else handler->OnKeyDown('q'); } break; //0xE0: (multimedia) previous track pressed
            case KP_W: if(Uppercase) handler->OnKeyDown('W'); else handler->OnKeyDown('w'); break;
            case KP_E: if(Uppercase) handler->OnKeyDown('E'); else handler->OnKeyDown('e'); break;
            case KP_R: if(Uppercase) handler->OnKeyDown('R'); else handler->OnKeyDown('r'); break;
            case KP_T: if(Uppercase) handler->OnKeyDown('T'); else handler->OnKeyDown('t'); break;
            case KP_Y: if(Uppercase) handler->OnKeyDown('Y'); else handler->OnKeyDown('y'); break;
            case KP_U: if(Uppercase) handler->OnKeyDown('U'); else handler->OnKeyDown('u'); break;
            case KP_I: if(Uppercase) handler->OnKeyDown('I'); else handler->OnKeyDown('i'); break;
            case KP_O: if(Uppercase) handler->OnKeyDown('O'); else handler->OnKeyDown('o'); break;
            case KP_P: if(MediaKey) { /*dohandlemediakey*/ } else { if(Uppercase) handler->OnKeyDown('P'); else handler->OnKeyDown('p'); } break; //0xE0: (multimedia) next track pressed
            case 0x1A: if(Uppercase) handler->OnKeyDown('{'); else handler->OnKeyDown('['); break;
            case 0x1B: if(Uppercase) handler->OnKeyDown('}'); else handler->OnKeyDown(']'); break;
            case KP_ENTER: handler->OnKeyDown('\n'); break; //0xE0: Numpad: ENTER pressed

            case KP_LCTRL: if(MediaKey) { /*rctrl*/ } else { /*lctrl*/ } break; //0xE0: Right Control Pressed

            case KP_A: if(Uppercase) handler->OnKeyDown('A'); else handler->OnKeyDown('a'); break;
            case KP_S: if(Uppercase) handler->OnKeyDown('S'); else handler->OnKeyDown('s'); break;
            case KP_D: if(MediaKey) { /*dohandlemediakey*/ } else { if(Uppercase) handler->OnKeyDown('D'); else handler->OnKeyDown('d'); } break; //0xE0: (multimedia) mute pressed
            case KP_F: if(MediaKey) { /*dohandlemediakey*/ } else { if(Uppercase) handler->OnKeyDown('F'); else handler->OnKeyDown('f'); } break; //0xE0: Calculator pressed
            case KP_G: if(MediaKey) { /*dohandlemediakey*/ } else { if(Uppercase) handler->OnKeyDown('G'); else handler->OnKeyDown('g'); } break; //0xE0: (multimedia) play pressed
            case KP_H: if(Uppercase) handler->OnKeyDown('H'); else handler->OnKeyDown('h'); break;
            case KP_J: if(MediaKey) { /*dohandlemediakey*/ } else { if(Uppercase) handler->OnKeyDown('J'); else handler->OnKeyDown('j'); } break; //0xE0: (multimedia) stop pressed
            case KP_K: if(Uppercase) handler->OnKeyDown('K'); else handler->OnKeyDown('k'); break;
            case KP_L: if(Uppercase) handler->OnKeyDown('L'); else handler->OnKeyDown('l'); break;
            case 0x27: if(Uppercase) handler->OnKeyDown(':'); else handler->OnKeyDown(';'); break;
            case 0x28: if(Uppercase) handler->OnKeyDown('\"'); else handler->OnKeyDown('\''); break;
            case 0x29: if(Uppercase) handler->OnKeyDown('~'); else handler->OnKeyDown('`'); break;
            case 0x2B: if(Uppercase) handler->OnKeyDown('|'); else handler->OnKeyDown('\\'); break;
            case KP_Z: if(Uppercase) handler->OnKeyDown('Z'); else handler->OnKeyDown('z'); break;
            case KP_X: if(Uppercase) handler->OnKeyDown('X'); else handler->OnKeyDown('x'); break;
            case KP_C: if(MediaKey) { /*dohandlemediakey*/ } else { if(Uppercase) handler->OnKeyDown('C'); else handler->OnKeyDown('c'); } break; //0xE0: (multimedia) volume down pressed
            case KP_V: if(Uppercase) handler->OnKeyDown('V'); else handler->OnKeyDown('v'); break;
            case KP_B: if(MediaKey) { /*dohandlemediakey*/ } else { if(Uppercase) handler->OnKeyDown('B'); else handler->OnKeyDown('b'); } break; //0xE0: (multimedia) volume up pressed
            case KP_N: if(Uppercase) handler->OnKeyDown('N'); else handler->OnKeyDown('n'); break;
            case KP_M: if(MediaKey) { /*dohandlemediakey*/ } else { if(Uppercase) handler->OnKeyDown('M'); else handler->OnKeyDown('m'); } break; //0xE0: WWW home pressed
            case KP_COMMA: if(Uppercase) handler->OnKeyDown('<'); else handler->OnKeyDown(','); break;
            case KP_DOT: if(Uppercase) handler->OnKeyDown('>'); else handler->OnKeyDown('.'); break;
            case 0x35: if(MediaKey) { if(NumLock) handler->OnKeyDown('/'); } else { if(Uppercase) handler->OnKeyDown('?'); else handler->OnKeyDown('/'); } break; // "/" pressed, 0xEO: Numpad: "/" pressed

            case KP_NUM_MUL: if(NumLock) handler->OnKeyDown('*'); break; //Numpad: * pressed
            case KP_LALT: if(MediaKey) { /*ralt*/ } else { /*lalt*/ } break; //0xE0: Right Alt or AltGr pressed
            case KP_SPACE: handler->OnKeyDown(' '); break;

            //SHIFT
            case KP_LSHIFT: if(MediaKey) { /*dohandlemediakey*/ } else { shift_force_toggle(); Uppercase = !Uppercase; break; } //0xE0 Printscreen pressed; WARNING: PRTSCR NOT SET UP PROPERLY
            case KP_RSHIFT: shift_force_toggle(); Uppercase = !Uppercase; break;
            case KR_LSHIFT: Uppercase = !Uppercase; break;
            case KR_RSHIFT: Uppercase = !Uppercase; break;
            //END SHIFT

            case KP_CAPSLOCK: key_toggle(KR_CAPSLOCK); Uppercase = !Uppercase; kbd_handle_led(0x02); break;

            case KP_F1: break;
            case KP_F2: break;
            case KP_F3: break;
            case KP_F4: break;
            case KP_F5: break;
            case KP_F6: break;
            case KP_F7: break;
            case KP_F8: break;
            case KP_F9: break;
            case KP_F10: break;
            case KP_F11: break;
            case KP_F12: break;

            case KP_NUMLOCK: key_toggle(KR_NUMLOCK); NumLock = !NumLock; kbd_handle_led(0x01); break;
            case KP_SCRLLOCK: break;

            case KP_NUM_7: if(MediaKey) { /*dohandlemediakey*/ } else { if(NumLock) handler->OnKeyDown('7'); } break; //0xE0: Home pressed
            case KP_NUM_8: if(MediaKey) { /*dohandlemediakey*/ } else { if(NumLock) handler->OnKeyDown('8'); } break; //0xE0: ArrowUp pressed
            case KP_NUM_9: if(MediaKey) { /*dohandlemediakey*/ } else { if(NumLock) handler->OnKeyDown('9'); } break; //0xE0: PageUp pressed
            case KP_NUM_MINUS: if(NumLock) handler->OnKeyDown('-'); break;
            case KP_NUM_4: if(MediaKey) { /*dohandlemediakey*/ } else { if(NumLock) handler->OnKeyDown('4'); } break; //0xE0: ArrowLeft pressed
            case KP_NUM_5: if(NumLock) handler->OnKeyDown('5'); break;
            case KP_NUM_6: if(MediaKey) { /*dohandlemediakey*/ } else { if(NumLock) handler->OnKeyDown('6'); } break; //0xE0: ArrowRight pressed
            case KP_NUM_PLUS: if(NumLock) handler->OnKeyDown('+'); break;
            case KP_NUM_1: if(MediaKey) { /*dohandlemediakey*/ } else { if(NumLock) handler->OnKeyDown('1'); } break; //0xE0: End pressed
            case KP_NUM_2: if(MediaKey) { /*dohandlemediakey*/ } else { if(NumLock) handler->OnKeyDown('2'); } break; //0xE0: ArrowDown pressed
            case KP_NUM_3: if(MediaKey) { /*dohandlemediakey*/ } else { if(NumLock) handler->OnKeyDown('3'); } break; //0xE0: PageDown pressed
            case KP_NUM_0: if(MediaKey) { /*dohandlemediakey*/ } else { if(NumLock) handler->OnKeyDown('0'); } break; //0xE0: Insert pressed
            case KP_NUM_DOT: if(MediaKey) { /*dohandlemediakey*/ } else { if(NumLock) handler->OnKeyDown('.'); } break; //0xE0: Delete pressed

            case 0x5B: if(MediaKey) { /*dohandlemediakey*/ } break; //0xE0: Left Mod (Win) key pressed
            case 0xDB: break; //Left Mod (Win) key released
            case 0x5C: if(MediaKey) { /*dohandlemediakey*/ } break; //0xE0: Right Mod (Win) key pressed
            case 0xDC: break; //Right Mod (Win) key released
            case 0x5D: if(MediaKey) { /*dohandlemediakey*/ } break; //0xE0: "Apps" key pressed
            case 0xDD: break; //"Apps" key released
            case 0x5E: if(MediaKey) { /*dohandlemediakey*/ } break; //0xE0: (ACPI) power pressed
            case 0x5F: if(MediaKey) { /*dohandlemediakey*/ } break; //0xE0: (ACPI) sleep pressed
            case 0x63: if(MediaKey) { /*dohandlemediakey*/ } break; //0xE0: (ACPI) wake pressed
            case 0x65: if(MediaKey) { /*dohandlemediakey*/ } break; //0xE0: (multimedia) WWW search pressed
            case 0x66: if(MediaKey) { /*dohandlemediakey*/ } break; //0xE0: (multimedia) WWW favorites pressed
            case 0x67: if(MediaKey) { /*dohandlemediakey*/ } break; //0xE0: (multimedia) WWW refresh pressed
            case 0x68: if(MediaKey) { /*dohandlemediakey*/ } break; //0xE0: (multimedia) WWW stop pressed
            case 0x69: if(MediaKey) { /*dohandlemediakey*/ } break; //0xE0: (multimedia) WWW forward pressed
            case 0x6A: if(MediaKey) { /*dohandlemediakey*/ } break; //0xE0: (multimedia) WWW back pressed
            case 0x6B: if(MediaKey) { /*dohandlemediakey*/ } break; //0xE0: (multimedia) my computer pressed
            case 0x6C: if(MediaKey) { /*dohandlemediakey*/ } break; //0xE0: (multimedia) email pressed
            case 0x6D: if(MediaKey) { /*dohandlemediakey*/ } break; //0xE0: (multimedia) media select pressed

            case KR_ESC: break;
            case KR_1: break;
            case KR_2: break;
            case KR_3: break;
            case KR_4: break;
            case KR_5: break;
            case KR_6: break;
            case KR_7: break;
            case KR_8: break;
            case KR_9: break;
            case KR_0: break;
            case KR_DASH: break;
            case KR_EQUAL: break;
            case KR_BACKSPACE: break;
            case KR_TAB: break;
            case KR_Q: break;
            case KR_W: break;
            case KR_E: break;
            case KR_R: break;
            case KR_T: break;
            case KR_Y: break;
            case KR_U: break;
            case KR_I: break;
            case KR_O: break;
            case KR_P: break;
            case 0x9A: break; //[ released
            case 0x9B: break; //] released
            case KR_ENTER: break; //0xE0: Numpad: ENTER released
            case KR_LCTRL: break;
            case KR_A: break;
            case KR_S: break;
            case KR_D: break;
            case KR_F: break;
            case KR_G: break;
            case KR_H: break;
            case KR_J: break;
            case KR_K: break;
            case KR_L: break;
            case 0xA7: break; //; released
            case 0xA8: break; //' released
            case 0xA9: break; //(back tick) released
            case 0xAB: break; //\ released
            case KR_Z: break;
            case KR_X: break;
            case KR_C: break;
            case KR_V: break;
            case KR_B: break;
            case KR_N: break;
            case KR_M: break;
            case KR_COMMA: break;
            case KR_DOT: break;
            case 0xB5: break; //(/) released
            case KR_NUM_MUL: break; //Numpad: * released
            case KR_LALT: break;
            case KR_SPACE: break;
            case KR_CAPSLOCK: break;
            case KR_F1: break;
            case KR_F2: break;
            case KR_F3: break;
            case KR_F4: break;
            case KR_F5: break;
            case KR_F6: break;
            case KR_F7: break;
            case KR_F8: break;
            case KR_F9: break;
            case KR_F10: break;
            case KR_NUMLOCK: break;
            case KR_SCRLLOCK: break;
            case KR_NUM_7: break;
            case KR_NUM_8: break;
            case KR_NUM_9: break;
            case KR_NUM_MINUS: break;
            case KR_NUM_4: break;
            case KR_NUM_5: break;
            case KR_NUM_6: break;
            case KR_NUM_PLUS: break;
            case KR_NUM_1: break;
            case KR_NUM_2: break;
            case KR_NUM_3: break;
            case KR_NUM_0: break;
            case KR_NUM_DOT: if(MediaKey) { /*dohandlemediakey*/ } break; //0xE0: Delete released
            case 0xD6: break; //AltBackslash released
            case KR_F11: break;
            case KR_F12: break;

            case 0xE1: break; //pause pressed
            case 0xEC: if(MediaKey) { /*dohandlemediakey*/ } break; //0xE0: mail released

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

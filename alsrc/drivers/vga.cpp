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

#include <drivers/vga.h>

using namespace asea::common;
using namespace asea::drivers;


VideoGraphicsArray::VideoGraphicsArray() :
	miscPort(0x3c2),
	crtcIndexPort(0x3d4),
	crtcDataPort(0x3d5),
	sequencerIndexPort(0x3c4),
	sequencerDataPort(0x3c5),
	graphicsControllerIndexPort(0x3ce),
	graphicsControllerDataPort(0x3cf),
	attributeControllerIndexPort(0x3c1),
	attributeControllerReadPort(0x3c0),
	attributeControllerWritePort(0x3c0),
	attributeControllerResetPort(0x3da)
{
}

VideoGraphicsArray::~VideoGraphicsArray()
{
}

void VideoGraphicsArray::WriteRegisters(uint8_t* registers)
{
	//misc
	miscPort.Write(*(registers++));

	//seqencer
	for(uint8_t i = 0; i < 5; i++) {
		sequencerIndexPort.Write(i);
		sequencerDataPort.Write(*(registers++));
	}

	//crtc
	crtcIndexPort.Write(0x03);
	crtcDataPort.Write(crtcDataPort.Read() | 0x80);
	crtcIndexPort.Write(0x11);
	crtcDataPort.Write(crtcDataPort.Read() & ~0x80);

	registers[0x03] = registers[0x03] | 0x80;
	registers[0x11] = registers[0x11] & ~0x80;

	for(uint8_t i = 0; i < 25; i++) {
		crtcIndexPort.Write(i);
		crtcDataPort.Write(*(registers++));
	}

	//gc
	for(uint8_t i = 0; i < 9; i++) {
		graphicsControllerIndexPort.Write(i);
		graphicsControllerDataPort.Write(*(registers++));
	}

	//attribc
	for(uint8_t i = 0; i < 21; i++) {
		attributeControllerResetPort.Read();
		attributeControllerIndexPort.Write(i);
		attributeControllerWritePort.Write(*(registers++));
	}

	attributeControllerResetPort.Read();
	attributeControllerIndexPort.Write(0x20);
}

bool VideoGraphicsArray::SupportsMode(uint32_t width, uint32_t height, uint32_t colordepth)
{
	return width == 320 && height == 200 && colordepth == 8;
}

bool VideoGraphicsArray::SetMode(uint32_t width, uint32_t height, uint32_t colordepth)
{
	if(!SupportsMode(width, height, colordepth))
		return false;

	unsigned char g_320x200x256[] =
	{
	/* MISC */
		0x63,
	/* SEQ */
		0x03, 0x01, 0x0F, 0x00, 0x0E,
	/* CRTC */
		0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
		0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
		0xFF,
	/* GC */
		0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
		0xFF,
	/* AC */
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
		0x41, 0x00, 0x0F, 0x00,	0x00
	};

	WriteRegisters(g_320x200x256);
	return true;
}

uint8_t* VideoGraphicsArray::GetFrameBufferSegment()
{
	graphicsControllerIndexPort.Write(0x06);
	uint8_t segmentNumber = ((graphicsControllerDataPort.Read() >> 2) & 0x03);
	switch(segmentNumber) {
		default: 
		case 0: return (uint8_t*)0x00000;
		case 1: return (uint8_t*)0xA0000;
		case 2: return (uint8_t*)0xB0000;
		case 3: return (uint8_t*)0xB8000;
	}
}

void VideoGraphicsArray::PutPixel( uint32_t x, uint32_t y, uint8_t colorindex)
{
	uint8_t* pixelAddress = GetFrameBufferSegment() + 320*y + x;
	*pixelAddress = colorindex;
}

uint8_t VideoGraphicsArray::GetColorIndex(uint8_t r, uint8_t g, uint8_t b)
{
	if(r == 0x00 && g == 0x00 && b == 0x00) return 0x00; //black
	if(r == 0x00 && g == 0x00 && b == 0xAA) return 0x1;  //blue
	if(r == 0x00 && g == 0xAA && b == 0x00) return 0x2;  //green
	if(r == 0x00 && g == 0xAA && b == 0xAA) return 0x3;  //cyan
	if(r == 0xAA && g == 0x00 && b == 0x00) return 0x4;  //red
	if(r == 0xAA && g == 0x00 && b == 0xAA) return 0x5;  //magenta
	if(r == 0xAA && g == 0x55 && b == 0x00) return 0x14; //brown
	if(r == 0xAA && g == 0xAA && b == 0xAA) return 0x7;  //gray
	if(r == 0x55 && g == 0x55 && b == 0x55) return 0x38; //dark gray
	if(r == 0x55 && g == 0x55 && b == 0xFF) return 0x39; //bright blue
	if(r == 0x55 && g == 0xFF && b == 0x55) return 0x3A; //bright green
	if(r == 0x55 && g == 0xFF && b == 0xFF) return 0x3B; //bright cyan
	if(r == 0xFF && g == 0x55 && b == 0x55) return 0x3C; //bright red
	if(r == 0xFF && g == 0x55 && b == 0xFF) return 0x3D; //bright magenta
	if(r == 0xFF && g == 0xFF && b == 0x55) return 0x3E; //yellow
	if(r == 0xFF && g == 0xFF && b == 0xFF) return 0x3F; //white

	return 0x00;
}

void VideoGraphicsArray::PutPixel(uint32_t x,  uint32_t y, uint8_t r, uint8_t g, uint8_t b)
{
	PutPixel(x, y, GetColorIndex(r, g, b));
}

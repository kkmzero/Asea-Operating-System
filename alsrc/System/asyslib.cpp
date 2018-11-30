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

#include <System/headers/asyslib.h>

using namespace asea::common;
using namespace asea::System::headers;

void asl::putChar(char c) {
	char* f = " ";
	f[0] = c;
	printf(f);
}

void asl::sleep(uint32_t sleepTime) {
	for(int i = 0; i < sleepTime; i++) {
		asm("nop");
	}
}


void asl::printfHex(uint8_t key) {
	char* pfx = "00";
	char* hex = "0123456789ABCDEF";
	pfx[0] = hex[(key >> 4) & 0xF];
	pfx[1] = hex[key & 0xF];
	printf(pfx);
}

char* asl::itoa(int32_t value, char* result, int32_t base) {
	if (base < 2 || base > 36) { *result = '\0'; return result; }

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while (value);

	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}

	return result;
}




double asl::math::abs(double num) {
	return num < 0 ? -num : num;
}

double asl::math::pow(double base, double exponent) {
	double result = 0;
	double tmp_result = base;

	for(double d = 0; d < (asl::math::abs((exponent-1))); d++) {
		result = tmp_result * base;
		tmp_result = result;
	}

	return result;
}

int32_t asl::math::fact(int32_t num) {
	int32_t factorial = 1;

	for(int i = 1; i <= num; ++i) {
		factorial *= i;
	}

	return factorial;
}




void asl::io::outb(uint16_t port, uint8_t val) {
	asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port) );
}

uint8_t asl::io::inb(uint16_t port) {
	uint8_t ret;
	asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port) );

	return ret;
}

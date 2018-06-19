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

#include <System/headers/sysnfo.h>
#include <System/headers/verdef.h>

using namespace asea::common;
using namespace asea::System::headers;

AseaSystemInfo::AseaSystemInfo() {
}

AseaSystemInfo::~AseaSystemInfo() {
}

void printf(char* str);

void AseaSystemInfo::AS_PrintSysInfoMsg(uint16_t sysinfomsg_ID) {
	switch(sysinfomsg_ID) {
		case 0x000:
			printf("Copyright (c) 2018, Asea OS. All Rights Reserved.\n");
			printf("AL " ASD_AL_VERSION " " ASD_AL_BUILDVR "\n\n");
			break;

		default:
			printf("[!] Undefined sysinfomsg_ID\n");
			break;
	}
	
	return;
}

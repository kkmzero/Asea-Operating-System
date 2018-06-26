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

#include <System/headers/sysmsgs.h>

using namespace asea::common;
using namespace asea::System::headers;

AseaSystemMessages::AseaSystemMessages() {
}

AseaSystemMessages::~AseaSystemMessages() {
}

void printf(char* str);

void AseaSystemMessages::AS_StatusMsg(uint8_t statusmsg_ID, char* description) {
	switch(statusmsg_ID) {
		case 0x00:
			printf("[OK] ");
			printf(description);
			break;

		case 0x01:
			printf("[FAILED] ");
			printf(description);
			break;

		case 0x02:
			printf("[ABORT] ");
			printf(description);
			break;

		case 0x03:
			printf("[WARNING] ");
			printf(description);
			break;
	}

	return;
}

void AseaSystemMessages::AS_StatusMsgInf(uint8_t statusmsginf_ID) {
	switch(statusmsginf_ID) {
		case 0x00:
			printf("Initializing...\n");
			break;
	}

	return;
}

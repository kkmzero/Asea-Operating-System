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

void AseaSystemMessages::AS_StatusMsg(STATUS_MSG_ID statusmsg_id, char* description) {
	switch(statusmsg_id) {
		case STATUSMSG_NONE:
			printf(description);
			break;

		case STATUSMSG_OK:
			printf("[OK] ");
			printf(description);
			break;

		case STATUSMSG_FAILED:
			printf("[FAILED] ");
			printf(description);
			break;

		case STATUSMSG_ABORT:
			printf("[ABORT] ");
			printf(description);
			break;

		case STATUSMSG_WARNING:
			printf("[WARNING] ");
			printf(description);
			break;

		case STATUSMSG_ERROR:
			printf("[ERROR] ");
			printf(description);
			break;
	}

	return;
}

void AseaSystemMessages::AS_StatusMsgInf(STATUS_MSG_INF_ID statusmsginf_id) {
	switch(statusmsginf_id) {
		case STATUSMSG_INF_NONE:
			break;

		case STATUSMSG_INF_INITIALIZING:
			printf("Initializing...\n");
			break;
	}

	return;
}

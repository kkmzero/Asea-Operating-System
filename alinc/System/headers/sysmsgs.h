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

#ifndef __ASEA__SYSTEM__HEADERS__SYSMSGS_H
#define __ASEA__SYSTEM__HEADERS__SYSMSGS_H

#include <common/types.h>

namespace asea
{
	namespace System
	{
		namespace headers
		{
			enum STATUS_MSG_ID : asea::common::uint8_t {
				STATUSMSG_NONE = 0x00,
				STATUSMSG_OK = 0x01,
				STATUSMSG_FAILED = 0x02,
				STATUSMSG_ABORT = 0x03,
				STATUSMSG_WARNING = 0x04,
				STATUSMSG_ERROR = 0x05
			};

			enum STATUS_MSG_INF_ID : asea::common::uint8_t {
				STATUSMSG_INF_NONE = 0x00,
				STATUSMSG_INF_INITIALIZING = 0x01
			};

			class AseaSystemMessages
			{
			public:
				AseaSystemMessages();
				~AseaSystemMessages();
				
				void AS_StatusMsg(STATUS_MSG_ID statusmsg_id, char* description);
				void AS_StatusMsgInf(STATUS_MSG_INF_ID statumsginf_id);
			};

		}
	}
}

#endif

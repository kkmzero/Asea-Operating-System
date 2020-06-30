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

#ifndef __ASEA__SYSTEM__HEADERS__SYSMSGS_H
#define __ASEA__SYSTEM__HEADERS__SYSMSGS_H

#include <common/types.h>
#include <astd>

namespace asea
{
    namespace system
    {
        namespace headers
        {
            enum STATUS_MSG_ID : asea::common::uint8_t {
                STATUSMSG_NONE = 0x00,
                STATUSMSG_OK = 0x01,
                STATUSMSG_FAILED = 0x02,
                STATUSMSG_ABORT = 0x03,
                STATUSMSG_WARNING = 0x04,
                STATUSMSG_ERROR = 0x05,
                STATUSMSG_INIT = 0x06
            };

            enum STATUS_MSG_INF_ID : asea::common::uint8_t {
                STATUSMSG_INF_NONE = 0x00,
                STATUSMSG_INF_INITIALIZING = 0x01
            };


            static void AS_StatusMsg(STATUS_MSG_ID statusmsg_id, char* description) {
                printf("[");
                switch(statusmsg_id) {
                    case STATUSMSG_NONE: printf("*", COLOR_LTGRAY); break;
                    case STATUSMSG_OK: printf("OK", COLOR_LTGREEN); break;
                    case STATUSMSG_FAILED: printf("FAILED", COLOR_LTRED); break;
                    case STATUSMSG_ABORT: printf("ABORT", COLOR_LTBROWN); break;
                    case STATUSMSG_WARNING: printf("WARNING", COLOR_LTMAGENTA); break;
                    case STATUSMSG_ERROR: printf("ERROR", COLOR_LTRED); break;
                    case STATUSMSG_INIT: printf("INIT", COLOR_LTGRAY); break;
                }
                printf("] ");
                printf(description);
                printf("\n");
            }

            static void AS_StatusMsgInf(STATUS_MSG_INF_ID statusmsginf_id) {
                switch(statusmsginf_id) {
                    case STATUSMSG_INF_NONE:
                        break;

                    case STATUSMSG_INF_INITIALIZING:
                        printf("Initializing...\n");
                        break;
                }
            }

        }
    }
}

#endif

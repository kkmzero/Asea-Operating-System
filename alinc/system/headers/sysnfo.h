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

#ifndef __ASEA__SYSTEM__HEADERS__SYSNFO_H
#define __ASEA__SYSTEM__HEADERS__SYSNFO_H

#include <common/types.h>
#include <system/headers/verdef.h>

namespace asea
{
    namespace system
    {
        namespace headers
        {
            enum SYSINFO_MSG_ID : asea::common::uint16_t {
                SYSINFOMSG_NONE = 0x000,
                SYSINFOMSG_COPYRIGHTVR = 0x001
            };


            static void AS_PrintSysInfoMsg(SYSINFO_MSG_ID sysinfomsg_id) {
                switch(sysinfomsg_id) {
                    case SYSINFOMSG_NONE:
                        break;

                    case SYSINFOMSG_COPYRIGHTVR:
                        printf("Copyright (c) 2018 - 2020, Asea OS. All Rights Reserved.\n");
                        printf("AL " ASD_AL_VERSION " " ASD_AL_BUILDVR "\n\n");
                        break;

                    default:
                        printf("[!] Undefined sysinfomsg_id\n");
                        break;
                }
            }

        }
    }
}

#endif

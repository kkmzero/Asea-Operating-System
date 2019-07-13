/*
 * This file is part of Asea OS.
 * Copyright (C) 2019 Ivan Kmeťo
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

#ifndef __ASEA__MEMMGR_H
#define __ASEA__MEMMGR_H

#include <common/types.h>

namespace asea
{
    struct MemoryChunk
    {
        MemoryChunk *next;
        MemoryChunk *prev;
        bool allocated;
        common::size_t size;
    };

    class MemMgr
    {
    protected:
        MemoryChunk* first;
    public:
        static MemMgr *activeMemMgr;
        MemMgr(common::size_t first, common::size_t size);
        ~MemMgr();

        void* malloc(common::size_t size);
        void free(void* ptr);
    };

}

#endif

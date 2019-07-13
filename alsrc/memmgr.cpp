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

#include <memmgr.h>

using namespace asea;
using namespace asea::common;

MemMgr* MemMgr::activeMemMgr = 0;

MemMgr::MemMgr(size_t start, size_t size)
{
    activeMemMgr = this;

    if(size < sizeof(MemoryChunk)) {
        first = 0;
    }
    else {
        first = (MemoryChunk*)start;

        first -> allocated = false;
        first -> prev = 0;
        first -> next = 0;
        first -> size = size - sizeof(MemoryChunk);
    }
}

MemMgr::~MemMgr()
{
    if(activeMemMgr == this)
        activeMemMgr = 0;
}

void* MemMgr::malloc(size_t size)
{
    MemoryChunk *result = 0;

    for(MemoryChunk* chunk = first; chunk != 0 && result == 0; chunk = chunk->next)
        if(chunk->size > size && !chunk->allocated)
            result = chunk;

    if(result == 0)
        return 0;

    if(result->size >= size + sizeof(MemoryChunk) + 1) {
        MemoryChunk* temp = (MemoryChunk*)((size_t)result + sizeof(MemoryChunk) + size);

        temp->allocated = false;
        temp->size = result->size - size - sizeof(MemoryChunk);
        temp->prev = result;
        temp->next = result->next;
        if(temp->next != 0)
            temp->next->prev = temp;

        result->size = size;
        result->next = temp;
    }

    result->allocated = true;
    return (void*)(((size_t)result) + sizeof(MemoryChunk));
}

void MemMgr::free(void* ptr)
{
    MemoryChunk* chunk = (MemoryChunk*)((size_t)ptr - sizeof(MemoryChunk));

    chunk -> allocated = false;

    if(chunk->prev != 0 && chunk->prev->allocated) {
        chunk->prev->next = chunk->next;
        chunk->prev->size += chunk->size + sizeof(MemoryChunk);

        if(chunk->next != 0)
            chunk->next->prev = chunk->prev;

        chunk = chunk->prev;
    }

    if(chunk->next != 0 && !chunk->next->allocated) {
        chunk->size += chunk->next->size + sizeof(MemoryChunk);
        chunk->next = chunk->next->next;

        if(chunk->next != 0)
            chunk->next->prev = chunk;
    }
}

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

#ifndef __ASEA__DRIVERS__VGA_H
#define __ASEA__DRIVERS__VGA_H

#include <common/types.h>
#include <drivers/driver.h>
#include <hwcom/port.h>

namespace asea
{
    namespace drivers
    {
        class VideoGraphicsArray
        {
        protected:
            hwcom::Port8Bit miscPort;
            hwcom::Port8Bit crtcIndexPort;
            hwcom::Port8Bit crtcDataPort;
            hwcom::Port8Bit sequencerIndexPort;
            hwcom::Port8Bit sequencerDataPort;
            hwcom::Port8Bit graphicsControllerIndexPort;
            hwcom::Port8Bit graphicsControllerDataPort;
            hwcom::Port8Bit attributeControllerIndexPort;
            hwcom::Port8Bit attributeControllerReadPort;
            hwcom::Port8Bit attributeControllerWritePort;
            hwcom::Port8Bit attributeControllerResetPort;

            void WriteRegisters(asea::common::uint8_t* registers);
            asea::common::uint8_t* GetFrameBufferSegment();

            virtual asea::common::uint8_t GetColorIndex (asea::common::uint8_t r, asea::common::uint8_t g, asea::common::uint8_t b);

        public:
            VideoGraphicsArray();
            ~VideoGraphicsArray();

            virtual bool SupportsMode(asea::common::uint32_t width, asea::common::uint32_t height, asea::common::uint32_t colordepth);
            virtual bool SetMode(asea::common::uint32_t width, asea::common::uint32_t height, asea::common::uint32_t colordepth);
            virtual void PutPixel(asea::common::uint32_t x, asea::common::uint32_t y, asea::common::uint8_t r, asea::common::uint8_t g, asea::common::uint8_t b);

            virtual void PutPixel(asea::common::uint32_t x, asea::common::uint32_t y, asea::common::uint8_t colorindex);

        };

    }
}

#endif

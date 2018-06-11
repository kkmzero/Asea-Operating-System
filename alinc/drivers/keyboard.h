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

#ifndef __ASEA__DRIVERS__KEYBOARD_H
#define __ASEA__DRIVERS__KEYBOARD_H

#include <common/types.h>
#include <hwcom/interrupts.h>
#include <drivers/driver.h>
#include <hwcom/port.h>

namespace asea
{
	namespace drivers
	{
		class KeyboardEventHandler
		{
			public:
				KeyboardEventHandler();

				virtual void OnKeyDown(char);
				virtual void OnKeyUp(char);
		};

		class KeyboardDriver : public asea::hwcom::InterruptHandler, public Driver
		{
			asea::hwcom::Port8Bit dataport;
			asea::hwcom::Port8Bit commandport;

			KeyboardEventHandler* handler;

			public:
				KeyboardDriver(asea::hwcom::InterruptManager * manager, KeyboardEventHandler* handler);
				~KeyboardDriver();
				virtual asea::common::uint32_t HandleInterrupt(asea::common::uint32_t esp);
				virtual void Activate();
		};

	}
}

#endif

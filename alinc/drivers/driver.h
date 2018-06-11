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

#ifndef __ASEA__DRIVERS__DRIVER_H
#define __ASEA__DRIVERS__DRIVER_H

namespace asea
{
	namespace drivers
	{
		class Driver
		{
			public:
				Driver();
				~Driver();

				virtual void Activate();
				virtual int Reset();
				virtual void Deactivate();
		};

		class DriverManager
		{
			private:
				Driver* drivers[265];
				int numDrivers;

			public:
				DriverManager();
				void AddDriver(Driver*);

				void ActivateAll();
		};

	}
}

#endif

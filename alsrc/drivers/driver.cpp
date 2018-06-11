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

#include <drivers/driver.h>

using namespace asea::drivers;

Driver::Driver() {
}

Driver::~Driver() {
}

void Driver::Activate() {
}

int Driver::Reset() {
	return 0;
}

void Driver::Deactivate() {
}

DriverManager::DriverManager() {
	numDrivers = 0;
}

void DriverManager::AddDriver(Driver* drv) {
	drivers[numDrivers] = drv;
	numDrivers++;
}

void DriverManager::ActivateAll() {
	for(int i = 0; i < numDrivers; i++)
		drivers[i]->Activate();
}

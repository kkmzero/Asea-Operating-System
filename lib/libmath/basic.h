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

#ifndef __ASEA__LIB__MATH__BASIC_H
#define __ASEA__LIB__MATH__BASIC_H


//
template<typename T>
T min(T a, T b){
    return (b < a) ? b : a;
}

//
template<typename T>
T max(T a, T b){
    return (a < b) ? b : a;
}

//
template<typename T>
T abs(T value){
    return (value < 0) ? -value : value;
}


#endif

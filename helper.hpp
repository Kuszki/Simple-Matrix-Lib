/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Simple matrix implementation for simulation purposes                   *
 *  Copyright (C) 2022  Łukasz "Kuszki" Dróżdż  lukasz.kuszki@gmail.com    *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the  Free Software Foundation, either  version 3 of the  License, or   *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This  program  is  distributed  in the hope  that it will be useful,   *
 *  but WITHOUT ANY  WARRANTY;  without  even  the  implied  warranty of   *
 *  MERCHANTABILITY  or  FITNESS  FOR  A  PARTICULAR  PURPOSE.  See  the   *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have  received a copy  of the  GNU General Public License   *
 *  along with this program. If not, see http://www.gnu.org/licenses/.     *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef HELPER_HPP
#define HELPER_HPP

#include <iostream>
#include <fstream>
#include <random>
#include <chrono>

#include "matrix.hpp"

template<typename data>
void print_matrix(const matrix<data>& m);

template<typename data>
requires std::is_floating_point_v<data>
void randomize_matrix(matrix<data>& m, data min, data max);

template<typename data>
requires std::is_integral_v<data>
void randomize_matrix(matrix<data>& m, data min, data max);

template<typename data>
double test_speed(const size_t size, const size_t iters);

template<typename data, typename base = long double>
matrix<base> test_diff(const matrix<base>& mat,
				   const size_t iters = 1e5,
				   const base min = -1.0l,
				   const base max = 1.0l);

#ifndef HELPER_CPP
#include "helper.cpp"
#endif

#endif // HELPER_HPP

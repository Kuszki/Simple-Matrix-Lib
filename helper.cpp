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

#ifndef HELPER_CPP
#define HELPER_CPP

#ifndef HELPER_HPP
#include "helper.hpp"
#endif

template<typename data>
void print_matrix(const matrix<data>& m)
{
	for (int i = 0; i < m.rows(); ++i)
	{
		for (int j = 0; j < m.cols(); ++j)
		{
			std::cout << m(i, j) << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

template<>
void print_matrix(const matrix<__float128>& m)
{
	for (int i = 0; i < m.rows(); ++i)
	{
		for (int j = 0; j < m.cols(); ++j)
		{
			std::cout << (long double) m(i, j) << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

template<typename data>
requires std::is_floating_point_v<data>
void randomize_matrix(matrix<data>& m, data min, data max)
{
	static thread_local std::uniform_real_distribution<data> dis(min, max);
	static thread_local std::random_device rd;
	static thread_local std::mt19937 gen(rd());

	for (int i = 0; i < m.rows(); ++i)
	{
		for (int j = 0; j < m.cols(); ++j)
		{
			m(i, j) = dis(gen);
		}
	}
}

template<typename data>
requires std::is_integral_v<data>
void randomize_matrix(matrix<data>& m, data min, data max)
{
	static thread_local std::uniform_int_distribution<data> dis(min, max);
	static thread_local std::random_device rd;
	static thread_local std::mt19937 gen(rd());

	for (int i = 0; i < m.rows(); ++i)
	{
		for (int j = 0; j < m.cols(); ++j)
		{
			m(i, j) = dis(gen);
		}
	}
}

template<>
void randomize_matrix(matrix<__float128>& m, __float128 min, __float128 max)
{
	std::uniform_real_distribution<long double> dis(min, max);
	std::random_device rd;
	std::mt19937 gen(rd());

	for (int i = 0; i < m.rows(); ++i)
	{
		for (int j = 0; j < m.cols(); ++j)
		{
			m(i, j) = (__float128) dis(gen);
		}
	}
}

#endif

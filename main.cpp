/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  {description}                                                          *
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

#include <iostream>
#include <fstream>
#include <random>

#include "matrix.hpp"

// -mlong-double-128 -lquadmath

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
void print_matrix(const matrix<long double>& m)
{
	for (int i = 0; i < m.rows(); ++i)
	{
		for (int j = 0; j < m.cols(); ++j)
		{
			std::cout << static_cast<double>(m(i, j)) << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

template<typename data>
requires std::is_floating_point_v<data>
void randomize_matrix(matrix<data>& m, data min, data max)
{
	std::uniform_real_distribution<data> dis(min, max);
	std::random_device rd;
	std::mt19937 gen(rd());

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
	std::uniform_int_distribution<data> dis(min, max);
	std::random_device rd;
	std::mt19937 gen(rd());

	for (int i = 0; i < m.rows(); ++i)
	{
		for (int j = 0; j < m.cols(); ++j)
		{
			m(i, j) = dis(gen);
		}
	}
}

int main(int argc, char* args[])
{
	std::cout.precision(3);

	matrix<long double> m1(5, 5);

	randomize_matrix(m1, -10.0l, 10.0l);
	print_matrix(m1);

	matrix<long double> diff = m1 - matrix<int>(m1);

	print_matrix(diff);

	std::cout << sizeof(__float128) << std::endl;
	std::cout << sizeof(long double) << std::endl;
	std::cout << sizeof(double) << std::endl;
	std::cout << sizeof(float) << std::endl;
	std::cout << "======\n";
	std::cout << sizeof(char) << std::endl;
	std::cout << sizeof(short) << std::endl;
	std::cout << sizeof(int) << std::endl;
	std::cout << sizeof(long) << std::endl;
	std::cout << sizeof(long long) << std::endl;

	return 0;
}

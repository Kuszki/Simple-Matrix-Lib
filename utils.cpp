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

#include "utils.hpp"

std::vector<std::pair<size_t, size_t>> get_fwt_levels(size_t count, size_t dec)
{
	std::vector<std::pair<size_t, size_t>> out(dec + 1);
	size_t last = count;

	if (count / std::pow(2, dec) < 1) return out;
	else for (size_t i = dec; i > 0; --i)
	{
		count /= 2;
		out[i] = { count - 1, last - 1 };
		last = count;
	}

	out[0] = {0, last - 1};

	return out;
}

void print_finfo(void)
{
	#if __GNUC__ && !__clang__
	std::cout << sizeof(__float128) << " " << sizeof(long double) << " "
			<< std::is_same<long double, __float128>::value << std::endl;

	std::cout << sizeof(__float80) << " " << sizeof(long double) << " "
			<< std::is_same<long double, __float80>::value << std::endl;
	#endif

	std::cout << sizeof(_Float64) << " " << sizeof(double) << " "
			<< std::is_same<double, _Float64>::value << std::endl;

	std::cout << sizeof(_Float32) << " " << sizeof(float) << " "
			<< std::is_same<float, _Float32>::value << std::endl;
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

template<>
void randomize_matrix(matrix<__float128>& m, __float128 min, __float128 max)
{
	static thread_local std::uniform_real_distribution<long double> dis(min, max);
	static thread_local std::random_device rd;
	static thread_local std::mt19937 gen(rd());

	for (int i = 0; i < m.rows(); ++i)
	{
		for (int j = 0; j < m.cols(); ++j)
		{
			m(i, j) = (__float128) dis(gen);
		}
	}
}

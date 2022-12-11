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

#include <chrono>

#include "matrix.hpp"
#include "helper.hpp"

// -mlong-double-128 -lquadmath -m128bit-long-double

//template<>
//void print_matrix(const matrix<long double>& m)
//{
//	for (int i = 0; i < m.rows(); ++i)
//	{
//		for (int j = 0; j < m.cols(); ++j)
//		{
//			std::cout << static_cast<double>(m(i, j)) << "\t";
//		}
//		std::cout << std::endl;
//	}
//	std::cout << std::endl;
//}

template<typename data>
double test_speed(const size_t size, const size_t iters)
{
	using namespace std::chrono;

	matrix<data> a(size, size), b(size, size), c;

	randomize_matrix(a, data(-1.0), data(1.0));
	randomize_matrix(b, data(-1.0), data(1.0));

	auto start = system_clock::now();
	for (size_t i = 0; i < iters; ++i) c = a * b;
	auto stop = system_clock::now();

	return duration_cast<milliseconds>(stop - start).count() / 1000.0;
}

template<typename data>
double test_diff(const size_t size, const size_t iters)
{
	matrix<data> mS1, mS2, mS3;
	matrix<long double>
	          mL1(size, size),
	          mL2(size, size),
	          mL3(size, size);

	randomize_matrix(mL1, -1.0l, 1.0l);

	long double diff = 0.0; mS1 = mL1;

	for (int i = 0; i < iters; ++i)
	{
		randomize_matrix(mL2, -1.0l, 1.0l);

		mS2 = mL2;

		mL3 = mL1 * mL2;
		mS3 = mS1 * mS2;

		mL3 = data(mS3) - mL3;

		diff += mL3.var();
	}

	return diff / iters;
}

void print_finfo(void)
{
	std::cout << sizeof(__float128) << " " << sizeof(long double) << " "
	          << std::is_same<long double, __float128>::value << std::endl;

	std::cout << sizeof(_Float64) << " " << sizeof(double) << " "
	          << std::is_same<double, _Float64>::value << std::endl;

	std::cout << sizeof(_Float32) << " " << sizeof(float) << " "
	          << std::is_same<float, _Float32>::value << std::endl;
}

int main(int argc, char* args[])
{
	std::cout.precision(10);

	for (size_t i = 16; i <= 2048; i *= 2)
	{
		std::cout << i << "\t" << test_speed<float>(i, 500) << std::endl;
	}

	return 0;
}

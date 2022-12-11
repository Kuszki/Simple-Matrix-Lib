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

double test(const size_t size, const size_t iters)
{
	matrix<double> a(size, size), b(size, size), c;

	randomize_matrix(a, -1.0, 1.0);
	randomize_matrix(b, -1.0, 1.0);

	auto start = std::chrono::system_clock::now();
	for (size_t i = 0; i < iters; ++i) a * b;
	auto stop = std::chrono::system_clock::now();

	return std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()/1000.0;
}

int main(int argc, char* args[])
{
//	const size_t size = 128;
	std::cout.precision(10);
	//void* tst = malloc(2048*2048*8);

//	matrix<float> mS1, mS2, mS3;
//	matrix<long double>
//	          mL1(size, size),
//	          mL2(size, size),
//	          mL3(size, size);

//	randomize_matrix(mL1, -1.0l, 1.0l);

//	long double diff = 0.0; mS1 = mL1;

//	for (int i = 0; i < 5; ++i)
//	{
//		randomize_matrix(mL2, -1.0l, 1.0l);

//		mS2 = mL2;

//		mL3 = mL1 * mL2;
//		mS3 = mS1 * mS2;

//		mL3 = decltype(mL3)(mS3) - mL3;

//		diff += mL3.var();
//	}

	for (size_t i = 16; i <= 2048; i += 16)
	{
//		std::cout << i << " : " << (test(i, 2048/i) / (2048/i)) << std::endl;
		std::cout << i << " : " << (test(i, 1) / 1.0) << std::endl;
	}

	//free(tst);

//	std::cout << "var: " << double(diff) << std::endl;

//	std::cout << sizeof(__float128) << " " << sizeof(long double) << " "
//	          << std::is_same<long double, __float128>::value << std::endl;
//	std::cout << sizeof(_Float64) << " " << sizeof(double) << " "
//	          << std::is_same<double, _Float64>::value << std::endl;
//	std::cout << sizeof(_Float32) << " " << sizeof(float) << " "
//	          << std::is_same<float, _Float32>::value << std::endl;

	return 0;
}

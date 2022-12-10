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

#include "matrix.hpp"
#include "helper.hpp"

// -mlong-double-128 -lquadmath -m128bit-long-double

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

int main(int argc, char* args[])
{
	const size_t size = 128;
	std::cout.precision(3);

	matrix<float> mS1, mS2, mS3;
	matrix<long double>
	          mL1(size, size),
	          mL2(size, size),
	          mL3(size, size);


	randomize_matrix(mL1, -1.0l, 1.0l);
	print_matrix(mS1 = mL1);

	long double diff = 0.0;

	for (int i = 0; i < 10; ++i)
	{
		randomize_matrix(mL2, -1.0l, 1.0l);

		mS2 = mL2;

		mL3 = mL1 * mL2;
		mS3 = mS1 * mS2;

		mL3 = decltype(mL3)(mS3) - mL3;

		diff += mL3.var();
	}

	std::cout << "var: " << double(diff) << std::endl;

	std::cout << sizeof(__float128) << " " << sizeof(long double) << " "
	          << std::is_same<long double, __float128>::value << std::endl;
	std::cout << sizeof(_Float64) << " " << sizeof(double) << " "
	          << std::is_same<double, _Float64>::value << std::endl;
	std::cout << sizeof(_Float32) << " " << sizeof(float) << " "
	          << std::is_same<float, _Float32>::value << std::endl;

	return 0;
}

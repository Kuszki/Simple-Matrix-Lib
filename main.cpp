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

	print_matrix(a);

	return duration_cast<milliseconds>(stop - start).count() / 1000.0;
}

template<typename data, typename base = long double>
long double test_diff(const matrix<long double>& mat,
                      const size_t iters = 1e5,
                      const base min = -1.0l,
                      const base max = 1.0l)
{
	const size_t osize = mat.size();

	matrix<base> vmat(1, osize * iters);
	matrix<base> imat = mat.transpose();
	matrix<base> omat;

	const matrix<data> s_mat = mat;
	matrix<data>s_imat, s_omat;

     #pragma omp parallel for default(none) \
	     shared(mat, vmat, s_mat, iters, osize, min, max) \
	     firstprivate(imat, omat, s_imat, s_omat)
	for (size_t i = 0; i < iters; ++i)
	{
		randomize_matrix(imat, min, max);

		size_t index = i * osize;
		s_imat = matrix<data>(imat);
		s_omat = s_mat * s_imat;

		omat = mat * imat;
		omat -= s_omat;

		for (size_t k = 0; k < omat.rows(); ++k)
			for (size_t l = 0; l < omat.cols(); ++l)
			{
				vmat(0, index++) = omat(k, l);
			}
	}

	return vmat.var() / iters;
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

int main(int argc, char* args[])
{
	std::cout.precision(10);

//	for (size_t i = 16; i <= 2048; i *= 2)
//	{
//		std::cout << i << "\t" << test_speed<long double>(i, 50) << std::endl;
//	}

	matrix<long double> mat("db2_2_16.txt");

	for (size_t i = 0; i < mat.rows(); ++i)
	{
		std::cout << (i) << "\t" << test_diff<float>(mat.get_row(i)) << std::endl;
	}

	return 0;
}

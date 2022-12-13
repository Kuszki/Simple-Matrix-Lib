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

	return duration_cast<milliseconds>(stop - start).count() / 1000.0;
}

template<typename data, typename base = long double>
matrix<base> test_diff(const matrix<base>& mat,
				   const size_t iters = 1e5,
				   const base min = -1.0l,
				   const base max = 1.0l)
{
	const size_t osize = mat.size();

	matrix<base> vmat(1, iters);
	matrix<base> imat = mat.transpose();

	const matrix<data> s_mat = mat;

	#pragma omp parallel for default(shared) firstprivate(imat)
	for (size_t i = 0; i < iters; ++i)
	{
		randomize_matrix(imat, min, max);

		matrix<data> s_imat = imat;

		vmat(0, i) =
				(s_mat * s_imat)(0, 0) -
				(mat * imat)(0, 0);
	}

	return vmat;
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
	std::cout.precision(2);

	const size_t iters = 1e6;

//	for (size_t i = 16; i <= 2048; i *= 2)
//	{
//		std::cout << i << "\t" << test_speed<long double>(i, 50) << std::endl;
//	}

//	const std::string path = "db2_2_16.txt";
//	const matrix<long double> mat(path);

//	test_diff<float, long double>(mat.get_row(1), iters).save("test.txt", 3);

//	std::cout << "out\tfloat16\tfloat32\tfloat64\n";

//	for (size_t i = 0; i < mat.rows(); ++i)
//	{
//		const auto row = mat.get_row(i);

//		std::cout << std::fixed << (i) << "\t" << std::scientific
//		          << test_diff<_Float16>(row, iters).var() / iters << "\t"
//		          << test_diff<_Float32>(row, iters).var() / iters << "\t"
//		          << test_diff<_Float64>(row, iters).var() / iters << "\n";
//	}

//	matrix<long double> mat(1, 32);
//	std::ofstream file("test.txt");

//	for (size_t i = 0; i < 1000; ++i)
//	{
//		randomize_matrix(mat, -1.0l, 1.0l);
//		mat.save(file);
//	}

	return 0;
}

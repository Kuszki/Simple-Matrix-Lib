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

	const size_t iters = 1e5;

//	for (size_t i = 16; i <= 2048; i *= 2)
//	{
//		std::cout << i << "\t" << test_speed<long double>(i, 50) << std::endl;
//	}

//	const std::string path = "vec_coif5/coif5_5_1024.txt";
//	const matrix<long double> mat(path);

//	test_diff<_Float16>(mat, iters).save("coif5_f16.txt", 100);
//	test_diff<_Float32>(mat, iters).save("coif5_f32.txt", 100);

//	for (size_t i = 0; i < mat.rows(); ++i)
//	{
//		const auto row = mat.get_row(i);

//		std::cout << std::fixed << (i+1) << "\t" << std::scientific
//				<< test_diff<_Float32>(row, iters, 3.0l, 9.0l).var() << "\n";
//	}

//	const std::vector<int> ndec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
//	const std::vector<int> nsam = { 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096 };

//	std::cout << "nsam";
//	for (const auto& j : ndec) std::cout << std::fixed << "\t" << j;
//	std::cout << std::endl;

//	for (const auto& i : nsam)
//	{
//		std::cout << std::fixed << i;

//		for (const auto& j : ndec)
//		{
//			if (double(i) / std::pow(2.0, j) >= 1)
//			{
//				const std::string path =
//						std::string("vec_coif5/coif5_") +
//						std::to_string(j) + std::string("_") +
//						std::to_string(i) + std::string(".txt");

//				const matrix<long double> mat(path);

//				std::cout << "\t" << std::scientific
//						<< test_diff<_Float32>(mat, iters).var();
//			}
//		}

//		std::cout << std::fixed << std::endl;
//	}

//	std::cout << std::endl;

	return 0;
}

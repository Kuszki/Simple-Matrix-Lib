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

template<typename data, typename base>
matrix<base> test_diff(const matrix<base>& mat,
				   const size_t iters,
				   const base min,
				   const base max)
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

#endif

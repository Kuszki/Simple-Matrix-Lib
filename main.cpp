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

template<typename data>
void speedtest(const size_t start = 16,
			const size_t stop = 4096,
			const size_t iters = 50);

template<typename data, typename base = long double>
void vartestall(const std::string& wname,
			 const size_t ndec,
			 const size_t count,
			 const size_t iters = 1e5,
			 const base min = base(-1),
			 const base max = base(1));

template<typename data, typename base = long double>
void vartestranges(const std::string& wname,
			    const size_t ndec,
			    const size_t count,
			    const size_t iters = 1e5,
			    const base min = base(-1),
			    const base max = base(1));

template<typename data, typename base = long double>
void vartestsingle(const std::vector<int> ndec,
			    const std::vector<int> nsam,
			    const size_t iters = 1e5,
			    const base min = base(-1),
			    const base max = base(1));

int main(int argc, char* args[])
{
//	const std::vector<int> ndec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
//	const std::vector<int> nsam = { 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096 };

	std::cout.precision(2);

	vartestranges<_Float32>("db2", 5, 1024);

	return 0;
}

template<typename data>
void speedtest(const size_t start,
			const size_t stop,
			const size_t iters)
{
	for (size_t i = start; i <= stop; i *= 2)
	{
		std::cout << i << "\t" << test_speed<data>(i, iters) << std::endl;
	}
}

template<typename data, typename base>
void vartestall(const std::string& wname,
			 const size_t ndec,
			 const size_t count,
			 const size_t iters,
			 const base min,
			 const base max)
{
	const std::string path =
			std::string("mat_") +
			wname + std::string("/") +
			wname + std::string("_") +
			std::to_string(ndec) + std::string("_") +
			std::to_string(count) + std::string(".txt");

	const matrix<base> mat(path);

	for (size_t i = 0; i < mat.rows(); ++i)
	{
		const auto var = test_diff<data, base>(mat.get_row(i), iters, min, max).var();
		std::cout << std::fixed << (i+1) << '\t' << std::scientific << var << std::endl;
	}
}

template<typename data, typename base>
void vartestranges(const std::string& wname,
			    const size_t ndec,
			    const size_t count,
			    const size_t iters,
			    const base min,
			    const base max)
{
	const std::string path =
			std::string("mat_") +
			wname + std::string("/") +
			wname + std::string("_") +
			std::to_string(ndec) + std::string("_") +
			std::to_string(count) + std::string(".txt");

	const auto lvls = get_fwt_levels(count, ndec);
	const matrix<base> mat(path);

	for (const auto& [start, stop] : lvls)
	{
		double var = 0.0;

		for (size_t i = start; i <= stop; ++i)
		{
			var += test_diff<data, base>(mat.get_row(i), iters, min, max).var();
		}

		var /= (stop - start + 1);

		std::cout << std::fixed << start << ':' << stop
				<< '\t' << std::scientific << var
				<< std::endl;
	}
}

template<typename data, typename base>
void vartestsingle(const std::string& wname,
			    const std::vector<int> ndec,
			    const std::vector<int> nsam,
			    const size_t iters,
			    const base min,
			    const base max)
{
	std::cout << "nsam";
	for (const auto& j : ndec) std::cout << std::fixed << "\t" << j;
	std::cout << std::endl;

	for (const auto& i : nsam)
	{
		std::cout << std::fixed << i << std::scientific;

		for (const auto& j : ndec)
		{
			if (double(i) / std::pow(2.0, j) >= 1)
			{
				const std::string path =
						std::string("vec_") +
						wname + std::string("/") +
						wname + std::string("_") +
						std::to_string(j) + std::string("_") +
						std::to_string(i) + std::string(".txt");

				const matrix<base> mat(path);

				std::cout << "\t" << test_diff<data, base>(mat, iters, min, max).var();
			}
		}

		std::cout << std::fixed << std::endl;
	}

	std::cout << std::endl;

	//	test_diff<_Float16>(mat, iters).save("coif5_f16.txt", 100);
	//	test_diff<_Float32>(mat, iters).save("coif5_f32.txt", 100);
}

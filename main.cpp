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

#include "matrix.cpp"

template<typename data>
void print_matrix(const matrix<data>& m)
{
	for (int i = 0; i < m.rows(); ++i)
	{
		for (int j = 0; j < m.cols(); ++j)
		{
			std::cout << m.get(i, j) << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

int main(int argc, char* args[])
{
	matrix<double> a(4, 2, { 1, 2, 3, 4, 5, 6, 7, 8 });
	matrix<double> b(2, 3, { 1, 2, 3, 4, 5, 6});

	auto c = a * b;

	print_matrix(b);
	print_matrix(a);
	print_matrix(c);
	print_matrix(c + c);

	const matrix<double> g(3, 5, { 2, 2, 3, 4, 5, 6, 7, 8, 10, 11, 11, 11, 12, 17, 18 });
	print_matrix(g);

	std::cout << g.var(0, decltype(g)::mode::rows) << std::endl;
	std::cout << g.var(1, decltype(g)::mode::rows) << std::endl;
	std::cout << g.var(2, decltype(g)::mode::rows) << std::endl;

	return 0;
}

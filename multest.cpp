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

#define debugmsg std::cout << "Test " << __FILE__ << " failed at line " << __LINE__ << std::endl
#define endtest(num, ok) { debugmsg; ++num; } else { ++num; ++ok; }

#include <iostream>

#include "matrix.hpp"

int main(int argc, char* args[])
{
	int n = 0, ok = 0;

	const matrix<int> a(4, 2, { 1, 2, 3, 4, 5, 6, 7, 8 });
	const matrix<int> b(2, 3, { 1, 2, 3, 4, 5, 6});

	const matrix<int> r1(4, 3, { 9, 12, 15, 19, 26, 33, 29, 40, 51, 39, 54, 69 });
	const matrix<int> r2(4, 2, { 2, 4, 6, 8, 10, 12, 14, 16 });

	const auto c = a * b;
	auto d = a; d *= b;

	if (r1 != c || r1 != d) endtest(n, ok);

	const auto e = a * 2;
	auto f = a; f *= 2;

	if (r2 != e || r2 != f) endtest(n, ok);

	if (a * -1 != -a) endtest(n, ok);
	if (b * -1 != -b) endtest(n, ok);

	return !(n == ok);
}

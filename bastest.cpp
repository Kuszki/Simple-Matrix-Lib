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

	const matrix<int> a(3, 3, { 1, 2, 3, 4, 5, 6, 7, 8, 9 });
	const matrix<int> b(2, 4, { 1, 2, 3, 4, 5, 6, 7, 8 });
	const matrix<int> c(2, 4, { 0, 0, 0, 0, 0, 0, 0, 0 });
	const matrix<int> d(1, 5, { 1, 2, 3, 4, 5 });
	const matrix<int> e = { 1, 2, 3, 4, 5 };
	const matrix<int> f = {{ 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }};

	if (c != matrix<int>(2, 4, 0)) endtest(n, ok);
	if (d != e) endtest(n, ok);
	if (f != a) endtest(n, ok);

	if (a.is_square() != true) endtest(n, ok);
	if (b.is_square() != false) endtest(n, ok);

	if (a.rows() != 3 || a.cols() != 3) endtest(n, ok);
	if (b.rows() != 2 || b.cols() != 4) endtest(n, ok);

	if (a.get_val(0, 0) != 1 || a.get_val(1, 2) != 6 ||
	    a.get_val(2, 1) != 8 || a.get_val(2, 2) != 9) endtest(n, ok);

	if (b.get_val(0, 0) != 1 || b.get_val(1, 3) != 8 ||
	    b.get_val(1, 1) != 6 || b.get_val(1, 2) != 7) endtest(n, ok);

	if (a.get_val(3, 1, -1) != -1 || b.get_val(1, 4, -2) != -2) endtest(n, ok);

	matrix<int> g = a; if (g != a || g(1, 1) != a(1, 1)) endtest(n, ok);

	if (!g.set_val(1, 1, 666) || g.get_val(1, 1) != 666) endtest(n, ok);

	return !(n == ok);
}

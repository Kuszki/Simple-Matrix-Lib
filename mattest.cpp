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

	const matrix<int> a(2, 2, { 1, 2, 3, 4 });
	const matrix<int> b(1, 1, { 5 });
	const matrix<int> c(2, 1, { 1, 2 });
	const matrix<int> d(3, 3, { 1, 2, 3, 4, 5, 6, 7, 8, 10 });
	const matrix<int> e(4, 4, { 9, 3, 3, 4, 5, 6, 7, 8, 9, 11, 11, 12, 13, 14, 15, 16 });

	if (a.det() != -2) endtest(n, ok);
	if (b.det() != 5) endtest(n, ok);
	if (c.det() != 0) endtest(n, ok);
	if (d.det() != -3) endtest(n, ok);
	if (e.det() != 64) endtest(n, ok);

	return !(n == ok);
}

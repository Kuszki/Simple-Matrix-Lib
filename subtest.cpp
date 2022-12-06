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

#include "matrix.cpp"

int main(int, char*[])
{
	const matrix<int> a(4, 2, { 1, 2, 3, 4, 5, 6, 7, 8 });
	const matrix<int> b(4, 2, { 1, 0, 1, 2, 3, 4, 5, 6 });

	const matrix<int> r1(4, 2, { 0, 2, 2, 2, 2, 2, 2, 2 });
	const matrix<int> r2(4, 2, { -9, -8, -7, -6, -5, -4, -3, -2 });

	const auto c = a - b;
	auto d = a; d -= b;

	if (r1 != c || r1 != d) return -1;

	const auto e = a - 10;
	auto f = a; f -= 10;

	if (r2 != e || r2 != f) return -2;

	return 0;
}

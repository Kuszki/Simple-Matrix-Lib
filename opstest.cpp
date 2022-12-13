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

#define debugmsg std::cout << "Test " << __FILE__ << " failed at line " << __LINE__ << std::endl
#define endtest(num, ok) { debugmsg; ++num; } else { ++num; ++ok; }

#include <iostream>

#include "matrix.hpp"

int main(int argc, char* args[])
{
	int n = 0, ok = 0;

	matrix<int> a(3, 3, { 1, 2, 3, 4, 5, 6, 7, 8, 9 });

	const matrix<int> r1(2, 2, { 5, 6, 8, 9 });
	const matrix<int> r2(2, 2, { 1, 3, 7, 9 });
	const matrix<int> r3(2, 2, { 1, 2, 4, 5 });
	const matrix<int> r4(2, 2, { 4, 5, 7, 8 });
	const matrix<int> r5(2, 2, { 1, 2, 7, 8 });
	const matrix<int> r6(1, 3, { 1, 2, 3 });
	const matrix<int> r7(1, 3, { 7, 8, 9 });
	const matrix<int> r8(3, 1, { 2, 5, 8 });
	const matrix<int> r9(3, 1, { 3, 6, 9 });

	const matrix<int> r10(3, 3, { 1, 4, 7, 2, 5, 8, 3, 6, 9 });
	const matrix<int> r11(1, 3, { 1, 5, 9 });

	if (r1 != a.submatrix(0, 0)) endtest(n, ok);
	if (r2 != a.submatrix(1, 1)) endtest(n, ok);
	if (r3 != a.submatrix(2, 2)) endtest(n, ok);
	if (r4 != a.submatrix(0, 2)) endtest(n, ok);
	if (r5 != a.submatrix(1, 2)) endtest(n, ok);

	if (a != a.submatrix(4, 1)) endtest(n, ok);
	if (a != a.submatrix(0, 5)) endtest(n, ok);

	if (r6 != a.get_row(0)) endtest(n, ok);
	if (r7 != a.get_row(2)) endtest(n, ok);

	if (r8 != a.get_col(1)) endtest(n, ok);
	if (r9 != a.get_col(2)) endtest(n, ok);

	if (r9 != a.get_col(2)) endtest(n, ok);

	if (r6.transpose() != a.get_row(0).transpose()) endtest(n, ok);
	if (r7.transpose() != a.get_row(2).transpose()) endtest(n, ok);

	if (r8.transpose() != a.get_col(1).transpose()) endtest(n, ok);
	if (r9.transpose() != a.get_col(2).transpose()) endtest(n, ok);

	if (r9.transpose() != a.get_col(2).transpose()) endtest(n, ok);

	if (r10 != a.transpose()) endtest(n, ok);
	if (r11 != a.diagonal()) endtest(n, ok);

	if (!a.set_row(0, r7) || a.get_row(0) != r7) endtest(n, ok);
	if (!a.set_row(2, r6) || a.get_row(2) != r6) endtest(n, ok);

	if (!a.set_col(0, r8) || a.get_col(0) != r8) endtest(n, ok);
	if (!a.set_col(1, r9) || a.get_col(1) != r9) endtest(n, ok);

	if (!a.set_row(1, r8) || a.get_row(1) != r8.transpose()) endtest(n, ok);
	if (!a.set_col(2, r6) || a.get_col(2) != r6.transpose()) endtest(n, ok);

	return !(n == ok);
}

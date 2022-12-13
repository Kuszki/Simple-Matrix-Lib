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

bool comp(double a, double b, double max_diff = 0.01)
{
	return std::abs(a - b) < max_diff;
}

int main(int argc, char* args[])
{
	int n = 0, ok = 0;

	const matrix<double> a(3, 5, { 2, 2, 3, 4, 5, 6, 7, 8, 10, 11, 11, 11, 12, 17, 18 });

	if (!comp(a.mean(), 8.467)) endtest(n, ok);
	if (!comp(a.var(), 25.124)) endtest(n, ok);
	if (!comp(a.std(), 5.0124)) endtest(n, ok);

	if (!comp(a.mean(0, decltype(a)::mode::rows), 3.20)) endtest(n, ok);
	if (!comp(a.mean(1, decltype(a)::mode::rows), 8.40)) endtest(n, ok);
	if (!comp(a.mean(2, decltype(a)::mode::rows), 13.8)) endtest(n, ok);

	if (!comp(a.std(0, decltype(a)::mode::rows), 1.30384)) endtest(n, ok);
	if (!comp(a.std(1, decltype(a)::mode::rows), 2.07364)) endtest(n, ok);
	if (!comp(a.std(2, decltype(a)::mode::rows), 3.42053)) endtest(n, ok);

	if (!comp(a.var(0, decltype(a)::mode::rows), 1.70)) endtest(n, ok);
	if (!comp(a.var(1, decltype(a)::mode::rows), 4.30)) endtest(n, ok);
	if (!comp(a.var(2, decltype(a)::mode::rows), 11.7)) endtest(n, ok);

	if (!comp(a.mean(0, decltype(a)::mode::cols), 6.3333)) endtest(n, ok);
	if (!comp(a.mean(2, decltype(a)::mode::cols), 7.6667)) endtest(n, ok);
	if (!comp(a.mean(4, decltype(a)::mode::cols), 11.333)) endtest(n, ok);

	if (!comp(a.std(0, decltype(a)::mode::cols), 4.5092)) endtest(n, ok);
	if (!comp(a.std(2, decltype(a)::mode::cols), 4.5092)) endtest(n, ok);
	if (!comp(a.std(4, decltype(a)::mode::cols), 6.5064)) endtest(n, ok);

	if (!comp(a.var(0, decltype(a)::mode::cols), 20.333)) endtest(n, ok);
	if (!comp(a.var(2, decltype(a)::mode::cols), 20.333)) endtest(n, ok);
	if (!comp(a.var(4, decltype(a)::mode::cols), 42.333)) endtest(n, ok);

	return !(n == ok);
}

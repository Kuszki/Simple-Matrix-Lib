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

bool comp(double a, double b, double max_diff = 0.01)
{
	return std::abs(a - b) < max_diff;
}

int main(int argc, char* args[])
{
	const matrix<double> a(3, 5, { 2, 2, 3, 4, 5, 6, 7, 8, 10, 11, 11, 11, 12, 17, 18 });

	if (!comp(a.mean(), 8.467)) return 1;
	if (!comp(a.var(), 25.124)) return 2;
	if (!comp(a.std(), 5.0124)) return 3;

	if (!comp(a.mean(0, decltype(a)::mode::rows), 3.20)) return 4;
	if (!comp(a.mean(1, decltype(a)::mode::rows), 8.40)) return 5;
	if (!comp(a.mean(2, decltype(a)::mode::rows), 13.8)) return 6;

	if (!comp(a.std(0, decltype(a)::mode::rows), 1.30384)) return 7;
	if (!comp(a.std(1, decltype(a)::mode::rows), 2.07364)) return 8;
	if (!comp(a.std(2, decltype(a)::mode::rows), 3.42053)) return 9;

	if (!comp(a.var(0, decltype(a)::mode::rows), 1.70)) return 10;
	if (!comp(a.var(1, decltype(a)::mode::rows), 4.30)) return 11;
	if (!comp(a.var(2, decltype(a)::mode::rows), 11.7)) return 12;

	if (!comp(a.mean(0, decltype(a)::mode::cols), 6.3333)) return 13;
	if (!comp(a.mean(2, decltype(a)::mode::cols), 7.6667)) return 14;
	if (!comp(a.mean(4, decltype(a)::mode::cols), 11.333)) return 15;

	if (!comp(a.std(0, decltype(a)::mode::cols), 4.5092)) return 16;
	if (!comp(a.std(2, decltype(a)::mode::cols), 4.5092)) return 17;
	if (!comp(a.std(4, decltype(a)::mode::cols), 6.5064)) return 18;

	if (!comp(a.var(0, decltype(a)::mode::cols), 20.333)) return 19;
	if (!comp(a.var(2, decltype(a)::mode::cols), 20.333)) return 20;
	if (!comp(a.var(4, decltype(a)::mode::cols), 42.333)) return 21;

	return 0;
}
